#ifndef POINTERANALYSIS__HPP__
#define POINTERANALYSIS__HPP__

#include "RndLearnerV3.hpp"
#include "ae/ExprSimplBv.hpp"

using namespace std;
using namespace boost;
namespace ufo
{
  class PointerAnalysis : public RndLearnerV3
  {
    public:

    PointerAnalysis (ExprFactory &m_efac, EZ3 &z3, CHCs& r, int debug, int to,
      map<int, string>& v) :
      RndLearnerV3 (m_efac, z3, r, to, false, false, 0, 1, false, 0, false,
        false, true, false, 0, false, false, to, debug), varIds(v){}

    map<Expr, int> allocMap, offMap, sizMap, memMap;
    map<Expr, ExprSet> allocVals;
    map<Expr, map<Expr, ExprSet>> aliases, aliasesRev;
    map<Expr, ExprMap> arrVars, arrVarsPr;
    Expr memTy, allocTy, zeralloc;
    map<int, string>& varIds;
    Expr lastDecl = NULL;
    map<int, set<pair<int, int>>> stableVars;

    virtual tribool wrapSMT(HornRuleExt& hr, ExprSet& exprs, ExprSet& negged)
    {
      if (negged.empty()) return false;
      ExprMap repl;
      for (auto & t : ruleManager.transit[hr.id])
        repl[t.second] = t.first;

      ExprSet negged2;
      for (auto n : negged) negged2.insert(replaceAll(n, repl));
      exprs.insert(disjoin(negged2, m_efac));
      return u.isSat(exprs);
    }

    bool multiHoudini (vector<HornRuleExt*> worklist, bool recur = true,
      bool fastExit = false)
    {
      if (printLog >= 6) outs () << "MultiHoudini\n";
      if (printLog >= 6) printCands();

      bool res1 = true;
      for (auto &hr: worklist)
      {
        if (printLog >= 6)
          outs () << "  Doing CHC check (" << hr->srcRelation << " -> "
                                   << hr->dstRelation << ")\n";
        if (hr->isQuery) continue;
        tribool b = checkCHC(*hr, candidates);
        if (b || indeterminate(b))
        {
          if (printLog >= 6) outs () << "    CHC check failed\n";
          if (fastExit)
          {
            assert(!indeterminate(b) && "unable to extract CTI");
            lastDecl = hr->dstRelation;
            return false;
          }
          int invNum = getVarIndex(hr->dstRelation, decls);
          SamplFactory& sf = sfs[invNum].back();

          map<Expr, tribool> vals;
          for (auto & cand : candidates[invNum])
          {
            Expr repl = cand;
            for (auto & v : invarVars[invNum])
              repl = replaceAll(repl, v.second, hr->dstVars[v.first]);
            vals[cand] = u.eval(repl);
            if (printLog > 6)
            {
              if (indeterminate(vals[cand]))
                outs () << "     >>  indet: " << cand << "\n";
              else if (!vals[cand])
                outs () << "        >> fail: " << cand << "\n";
            }
          }

          // first try to remove candidates immediately by their models
          // then, invalidate (one-by-one) all candidates
          //            for which Z3 failed to find a model

          for (int i = 0; i < 3 /*weakeningPriorities.size() */; i++)
          {
            if (weaken (invNum, candidates[invNum], vals, hr, sf,
                (*weakeningPriorities[i]), i > 0))
              break;
          }
          if (recur)
          {
            res1 = false;
            break;
          }
        }
        else if (printLog >= 6) outs () << "    CHC check succeeded\n";
      }
      if (fastExit) return true;
      if (!recur) return false;
      if (res1) return anyProgress(worklist);
      else return multiHoudini(worklist);
    }

    void computeStableVars()
    {
      ruleManager.prepTransitVars();
      for (int i = 0; i < ruleManager.chcs.size(); i++)
      {
        auto & r = ruleManager.chcs[i];

        // currently, only alloc
        if (r.isFact || r.isQuery) continue;

        auto s = allocMap[r.srcRelation], d = allocMap[r.dstRelation];
        auto a = r.srcVars[s], b = r.dstVars[d];

        for (auto & t : ruleManager.transit[r.id])
          if (t.first == a && t.second == b)
            stableVars[i].insert({d, s});
      }
    }

    // beginning of the preprocessing:
    // determine the alloca and mem vars
    bool initAlloca()
    {
      memTy = NULL;
      for (auto & d : ruleManager.decls)
      {
        Expr decl = d->left();
        memMap[decl] = -1;
        allocMap[decl] = -1;
        offMap[decl] = -1;
        sizMap[decl] = -1;
      }

      for (auto & d : ruleManager.decls)
      {
        Expr decl = d->left();
        if (printLog >= 3) outs () << "initAlloca, decl: " << decl << "\n";
        auto & vs = ruleManager.invVars[decl];
        for (int i = 0; i < vs.size(); i++)
        {
          auto & v = vs[i];
          if (isArray(v))
          {
            Expr ty = typeOf(v), rty = ty->right(), lty = ty->left();
            if (isOpX<BVSORT>(lty) && isOpX<BVSORT>(rty))
            {
              assignVarInd("_alloc", v, allocMap[decl], i);
              assignVarInd("_off", v, offMap[decl], i);
              assignVarInd("_siz", v, sizMap[decl], i);

            }
            else if (isOpX<BVSORT>(lty) && isOpX<ARRAY_TY>(rty))
              assignVarInd("_mem", v, memMap[decl], i);
          }
        }
        if (allocMap[decl] == -1)
          return false;
        if(memMap[decl] == -1)
        {
          errs() << "Memory is never accessed\n";
          return false;
        }
        Expr ty = typeOf(ruleManager.invVars[decl][memMap[decl]]);
        assert(ty->right() == typeOf(ruleManager.invVars[decl][allocMap[decl]]));
        assignType(ty, memTy);
        assignType(ty->right(), allocTy);
      }
      assert(memTy != NULL);
      zeralloc = bvnum(mkMPZ(0, m_efac), allocTy->left());
      return true;
    }

    void assignVarInd(string name, Expr v, int & pos, int i)
    {
      if (lexical_cast<string>(v) != name) return;
      assert(pos == -1);
      pos = i;
      if (printLog >= 3) outs () << "  " << name << " arg # "  << i << ": "
                                 << v << "   " << typeOf(v) << "\n";
    }

    void assignType(Expr ty, Expr& varTy)
    {
      if (varTy == NULL) varTy = ty;
      else assert (varTy == ty);
    }

    // get a CHC r = body -> rel(..), look for its alloca storage (key -> val)
    // construct \phi as `body(r) /\ alloc[key] = value`
    // while \phi is sat,
    //       1) get m, s.t. m \models \phi, and
    //       2) update \phi to \phi /\ m(val) != val
    //       3) insert m(val) to alloca_vals [rel]
    //       4) insert m(key) to aliases [rel] [m(val)], and
    //       5) insert m(val) to aliases^-1 [rel] [m(key)]

    void extractPtrInfo(HornRuleExt & r, Expr init, Expr ptr)
    {
      auto & v = r.dstVars[allocMap[r.dstRelation]];

      if (printLog >= 6) outs () << "extractPtrInfo: "
          << r.srcRelation << " -> " << r.dstRelation << "\n";

      Expr key, val;
      if (ptr != NULL) key = ptr;
      else key = mkConst(mkTerm<string> ("key", m_efac), allocTy->left());
      val = mkConst(mkTerm<string> ("val", m_efac), allocTy->right());
      ExprVector cnjs = {init, r.body, mk<EQ>(mk<SELECT>(v, key), val)};

      bool first = true;
      while (true == u.isSat(cnjs, first))
      {
        first = false;
        auto m = u.getModel(val);
        cnjs = {mk<NEQ>(val, m)};
        if (lexical_cast<string>(toMpz(m)) != "0")
        {
          if (ptr == NULL)
            allocVals[r.dstRelation].insert(m);
          else
          {
            aliases[r.dstRelation][ptr].insert(m);
            aliasesRev[r.dstRelation][m].insert(ptr);
            if (printLog >= 6) outs () << "    model:  " << toMpz(m) <<
                      " <- " << toMpz(ptr) << "\n";
          }
        }
      }
    }

    void getInitAllocaVals()
    {
      // get init vals, from fact
      for (auto & r : ruleManager.chcs)
        if (r.isFact)
          extractPtrInfo(r, mk<TRUE>(m_efac), NULL);
    }

    Expr getAllocCand(Expr decl)
    {
      auto & v = ruleManager.invVars[decl][allocMap[decl]];
      Expr key = mkConst(mkTerm<string> ("key", m_efac), allocTy->left());
      allocVals[decl].insert(zeralloc);

      ExprSet dsjs;
      for (auto & val : allocVals[decl])
        dsjs.insert(mk<EQ>(mk<SELECT>(v, key), val));

      return mknary<FORALL>(ExprVector({key->left(), disjoin(dsjs, m_efac)}));
    }

    Expr getAliasCand(Expr decl, Expr ptr)
    {
      auto & v = ruleManager.invVars[decl][allocMap[decl]];
      auto key = mkConst(mkTerm<string> ("key", m_efac), allocTy->left());
      auto eq = mk<EQ>(mk<SELECT>(v, key), ptr);
      ExprSet dsjs;
      for (auto & k : aliases[decl][ptr]) dsjs.insert(mk<EQ>(key, k));
      return mknary<FORALL>(
            ExprVector({key->left(), mk<IMPL>(eq, disjoin(dsjs, m_efac))}));
    }

    void propagateAllocVals()
    {
      bool found = false;
      for (int i = 0; i < ruleManager.chcs.size(); i++)
      {
        auto & r = ruleManager.chcs[i];
        if (r.isFact || r.isQuery) continue;

        int sz = allocVals[r.dstRelation].size();
        if (stableVars[i].empty())
        {
          extractPtrInfo(r, getAllocCand(r.srcRelation), NULL);
          if (allocVals[r.dstRelation].size() > sz)
          {
            found = true;
            break;
          }
        }
        else
        {
          // by construction, it is only alloc vars:
          for (auto & a : allocVals[r.srcRelation])
            allocVals[r.dstRelation].insert(a);
        }
        found |= allocVals[r.dstRelation].size() > sz;
      }
      if (found) propagateAllocVals();
    }

    void propagateAliases(Expr ptr)
    {
      bool found = false;
      for (int i = 0; i < ruleManager.chcs.size(); i++)
      {
        auto & r = ruleManager.chcs[i];
        if (r.isFact || r.isQuery) continue;

        int sz = aliases[r.dstRelation][ptr].size();
        if (stableVars[i].empty())
        {
          ExprVector cnjs;
          Expr v = r.srcVars[allocMap[r.srcRelation]];
          for (auto & a : aliases[r.srcRelation][ptr])
            cnjs.push_back(mk<EQ>(mk<SELECT>(v, ptr), a));
          extractPtrInfo(r, disjoin(cnjs, m_efac), ptr);

          if (aliases[r.dstRelation][ptr].size() > sz)
          {
            found = true;
            break;
          }
        }
        else
        {
          // by construction, it is only alloc vars:
          for (auto & a : aliases[r.srcRelation][ptr])
          {
            aliases[r.dstRelation][ptr].insert(a);
            aliasesRev[r.dstRelation][a].insert(ptr);
          }
        }
        found |= aliases[r.dstRelation][ptr].size() > sz;
      }
      if (found) propagateAliases(ptr);
    }

    // Houdini-based method to find all alloca values
    // for each relation: create a candidate \forall key, alloc[key] = 0
    // check inductive: for each cti: extract new model
    //    and update cand to \forall key, (alloc[key] = 0 \/ alloc[key] = model)
    void getAllocaVals()
    {
      propagateAllocVals();

      // get init cands, from vals
      for (int ind = 0; ind < decls.size(); ind++)
      {
        Expr decl = decls[ind];
        if (printLog >= 6) outs () << "getAllocaVals, decl " << decl
                                   << ", ind = "<< ind << "\n";
        candidates[ind] = {getAllocCand(decl)};
        if (printLog >= 6)
        {
          outs () << "\ngetAllocaVals, decl: " << decl << ":\n";
          pprint(candidates[ind], 4);
        }
      }

      if (multiHoudini(ruleManager.allCHCs, false, true))
      {
        if (printLog >= 3) outs () << "getAllocaVals: ALL found\n";
      }
      else
      {
        if (printLog >= 6) outs () << "fail for " << lastDecl << "\n";
        Expr mdl = u.getModel(
                  ruleManager.invVarsPrime[lastDecl][allocMap[lastDecl]]);
        ExprVector st;
        filter (mdl, IsStore (), inserter(st, st.begin()));
        if (printLog >= 6) outs () << "  mdl:  " << mdl << "\n";
        if (st.empty() && isOpX<CONST_ARRAY>(mdl))
          st = {mk<STORE>(mdl, mdl->last(), mdl->last())};    // hack for now;
        for (auto & m : st)
          if (is_bvnum(m->last()))
          {
            if (printLog >= 6) outs () << "  adding val " << m->last() << "\n";
            allocVals[lastDecl].insert(m->last());
          }
        getAllocaVals();
      }
    }

    // Houdini-based method to find all aliases of ptr
    // for each relation: create a candidate:
    //     \forall key, alloc[key] = ptr => false
    // check inductive: for each cti: extract new model and update cand to:
    //     \forall key, alloc[key] = ptr => key = cti(alloca)[ptr] \/ ...
    void getAliases(Expr ptr)
    {
      propagateAliases(ptr);
      for (int ind = 0; ind < decls.size(); ind++)
      {
        Expr decl = decls[ind];
        if (printLog >= 6) outs () << "\ngetAliases, decl " << decl
                                   << ", ind = "<< ind << "\n";

        candidates[ind] = {getAliasCand(decl, ptr)};

        if (printLog >= 6)
        {
          outs () << "\ngetAliases, decl: " << decl << " vs " << ptr << ":\n";
          pprint(candidates[ind], 4);
        }
      }
      if (multiHoudini(ruleManager.allCHCs, false, true))
      {
        if (printLog >= 2)
          outs () << "getAliases: ALL found\n";
      }
      else
      {
        if (printLog >= 6) outs () << "fail for " << lastDecl << "\n";
        Expr mdl = u.getModel(
               ruleManager.invVarsPrime[lastDecl][allocMap[lastDecl]]);
        if (printLog >= 6) outs () << "  mdl:  " << mdl << "\n";
        ExprVector st;
        filter (mdl, IsStore (), inserter(st, st.begin()));
        for (auto & m : st)
          if (is_bvnum(m->right()) && m->last() == ptr)
          {
            if (printLog >= 6) outs () << "  adding val " << m->right() << "\n";
            aliases[lastDecl][ptr].insert(m->right());
            aliasesRev[lastDecl][m->right()].insert(ptr);
          }
        getAliases(ptr);
      }
    }

    void getAliases()
    {
      ExprSet all;
      for (auto & d : decls)
        for (auto & val : allocVals[d])
          if (lexical_cast<string>(toMpz(val)) != "0")
            all.insert(val);
      for (auto & a : all)
      {
        if (printLog >= 2) outs () << "\naliases for " << a << "\n";
        getAliases(a);
      }
    }

    void printAliases()
    {
      if (printLog < 2) return;
      for (auto & d : decls)
      {
        outs () << "  >> decl >>  " << d << ":\n";
        for (auto & al : aliases[d])
          for (auto & a : al.second)
            outs () << "    " << a << "  ->  " << al.first << "\n";
      }
    }

    string getNewArrName(Expr bv)
    {
      int num = lexical_cast<int>(toMpz(bv));
      if (varIds[num] == "")
        return "_new_ar_" + to_string(num);
      return varIds[num];
    }

    ExprSet allArrVars, allArrVarsPr;

    void addAliasVars()
    {
      if (printLog)
      {
        ExprSet aa, ab;
        for (auto & d : decls)
        {
          for (auto & al : aliases[d])
          {
            aa.insert(al.first);

            for (auto & a : al.second)
            {
              ab.insert(a);
            }
          }
        }
        outs () << "Detected memory regions: " << aa.size() <<
                   ", aliases: " << ab.size() << ",\n";
        outs () << "memory regions:\n";
        pprint(aa, 2);
        outs () << "aliases:\n";
        pprint(ab, 2);
      }

      for (int ind = 0; ind < decls.size(); ind++)
      {
        Expr d = decls[ind];
        for (auto & al : aliases[d])
        {
          auto name = getNewArrName(al.first);
          auto a = mkConst(mkTerm<string> (name, m_efac), memTy->right());
          auto b = mkConst(mkTerm<string> (name + "'", m_efac), memTy->right());
          allArrVars.insert(a);
          allArrVarsPr.insert(b);
          arrVars[d][al.first] = a;
          arrVarsPr[d][al.first] = b;
          invarVars[ind][
            ruleManager.invVars[d].size() - 1 +
            arrVars[d].size()] = a;
        }
      }
    }

    Expr getUpdLiteral(Expr body, Expr ty, string name)
    {
      ExprSet tmp;
      getConj(body, tmp);
      Expr upd = NULL;
      for (auto c : tmp)
      {
        if (isOpX<EQ>(c) && typeOf(c->left()) == ty &&
            (lexical_cast<string>(c->left()) == name ||
             lexical_cast<string>(c->right()) == name))
        {
          assert(upd == NULL && "cannot have several updates of this type");
          upd = c;
        }
      }
      return upd;
    }

    void getAliasCombs (Expr body, Expr rel, Expr allocVar,
        vector<ExprMap> & aliasCombs)
    {
      ExprSet se;
      filter (body, IsSelect (), inserter(se, se.begin()));
      for (auto & s : se)
      {
        if (s->left() != allocVar) continue;

        ExprSet regions;
        if (is_bvnum(s->last()))
          regions = aliasesRev[rel][s->last()];
        else
          for (auto & val : allocVals[rel])
            if (lexical_cast<string>(toMpz(val)) != "0")
              regions.insert(val);

        if (regions.empty()) continue;

        if (aliasCombs.empty())
          for (auto & a : regions)
            aliasCombs.push_back({{s, a}});
        else
        {
          vector<ExprMap> aliasCombsTmp = aliasCombs;
          aliasCombs.clear();
          for (auto & a : regions)
          {
            for (auto & m : aliasCombsTmp)
            {
              assert (m[s] == NULL);
              aliasCombs.push_back(m);
              aliasCombs.back()[s] = a;
            }
          }
        }
      }
    }

    void extendBody(Expr body, HornRuleExt& r, ExprVector& b, Expr ty, Expr esc)
    {
      // binding src/dst vars that are not affected by the array update:
      for (auto & a : arrVars[r.dstRelation])
        if (a.first != esc)
          b.push_back(mk<EQ>(arrVars[r.dstRelation][a.first],
                             arrVarsPr[r.dstRelation][a.first]));

      Expr memUpd = getUpdLiteral(body, ty, "_mem'");
      if (memUpd != NULL) body = replaceAll(body, memUpd, mk<TRUE>(m_efac));
      b.push_back(body);
    }

    // assumes small-block encoding:
    //  - fact has no memory manipulation, and actually is empty
    //  - every CHC has at most 1 operation (either for mem, alloca, or non-mem)
    //  - only unprimed mem/alloca are used in the RHS of assignments
    // so the algorithm goes for each CHC:
    //  - search for select(_alloc, X) terms in the body
    //  - figure out where X can point to (e.g., Y)
    //  - create a set of combinations X -> Y for each X used in the body
    //  - for each combination:
    //    + replace the corresponding select(_alloc, X) by Y
    //    + introduce a fresh var for each select(_mem, Y)
    //    + search for store(_mem, Y, store (_new_ar_Y, ..))
    //    + introduce a constraint _new_ar_Y' = store (_new_ar_Y, ..)
    //    + remove _mem' = store(_mem, ...)
    //    + add missing _new_ar_X' = _new_ar_X for all unaffected
    //    + use the replaced body under assumption that X -> Y (i.e., comb)
    //  - conjoin all "conditional" bodies after such replacements
    Expr rewriteMemToArr (HornRuleExt& r, ExprMap& m, Expr body)
    {
      outs () << "rewriteMemToArr: "
              << r.srcRelation << " -> " << r.dstRelation << "\n";
      // this aux method assumes `r` is not a fact
      ExprVector newConstrs;
      Expr rel = r.srcRelation, memVar = r.srcVars[memMap[rel]];
      body = replaceAll(body, m);

      for (auto & al : aliases[rel])
      {
        Expr e = al.first, mem = mk<SELECT>(memVar, e), arr = arrVars[rel][e];
        if (printLog >= 2)
          outs () << "rewriting " << mem << " with " << arr << "\n";
        body = replaceAll(body, mem, arr);
      }

      body = fixpointRewrite(body,
        [](Expr e){return simplifyBool(simplifyArr(simplifyBV(e)));});
      ExprVector st;
      filter (body, IsStore (), inserter(st, st.begin()));

      if (printLog >= 2)
      {
        outs () << "  body after replacement:\n";
        pprint(body);
        outs() << "\n";
      }

      Expr num = NULL, newDef = NULL;
      for (auto s : st)   // find at most one new array update
      {
        if (s->left() != memVar) continue;
        num = s->right();
        assert(newDef == NULL && "possibly, not a small-step encoding");
        if (!is_bvnum(num)) continue;
        newDef = s;
        newConstrs = {mk<EQ>(arrVarsPr[rel][num], s->last())};
      }
      if (newDef == NULL)
      {
        if (printLog >= 2) outs () << "  no store found\n";
      }
      else
      {
        if (printLog >= 2)
          outs () << "  found store:\n    " << newDef << "\n"
                  << "  replaced by:\n    " << newConstrs[0] << "\n";

        body = replaceAll(body, newDef, memVar);
        body = simplifyArr(simplifyBV(body));
      }

      extendBody(body, r, newConstrs, memTy, num);
      return conjoin(newConstrs, m_efac);
    }

    void rewriteMemCHC(HornRuleExt& r)
    {
      if (printLog >= 2)
        outs () << "-----------   processing CHC:   "
                << r.srcRelation << " -> " << r.dstRelation <<
                (r.isQuery ? " [Q]": (r.isInductive ? " [TR]": "")) << "\n";
      Expr body = r.body;
      if (!r.isQuery)
      {
        ExprVector toElim = {r.dstVars[allocMap[r.dstRelation]]};
        ExprSet dsjsSet, ndsjsSet;
        getDisj(disjunctify(body), dsjsSet);
        for (auto d : dsjsSet)
        {
          Expr allocUpd = getUpdLiteral(d, allocTy, "_alloc'");
          d = simpleQE(d, toElim, false);
          assert (allocUpd != NULL && "ALLOC UPDATE NOT FOUND");
          ndsjsSet.insert(mk<AND>(allocUpd, d));
        }
        body = distribDisjoin(ndsjsSet, m_efac);
      }
      if (r.isFact)
      {
        assert(!contains(body, memTy));
        return;
      }
      body = fixpointRewrite(body,
        [](Expr e){return simplifyBool(simplifyArr(simplifyBV(e)));});

      if (printLog >= 8) pprint(body, 3);

      // find alloc-selects, replace them by respective vals
      //                      (need to be an invar)
      Expr rel = r.srcRelation, memVar = r.srcVars[memMap[rel]];
      vector<ExprMap> aliasCombs;
      getAliasCombs(body, rel, r.srcVars[allocMap[rel]], aliasCombs);
      if (aliasCombs.empty())
      {
        // no memory accesses, so binding src/dst vars
        if (printLog >= 2) outs () << "  aliasCombs empty\n";
        ExprVector bodyExt;
        extendBody(body, r, bodyExt, memTy, NULL);
        r.body = conjoin(bodyExt, m_efac);
        for (auto & a : arrVars[r.dstRelation])
          assert(!contains(r.body, mk<SELECT>(memVar, a.first)));
      }
      else
      {
        ExprVector allCombEqs, newBodies;
        for (auto & m : aliasCombs)
        {
          allCombEqs.push_back(conjoin(m, m_efac));
          if (printLog >= 2)
          {
            outs () << "  combination #" << newBodies.size() <<"\n";
            pprint(allCombEqs.back(), 4);
          }
          auto rewr = rewriteMemToArr(r, m, body);
          newBodies.push_back(mk<IMPL>(allCombEqs.back(), rewr));
        }

        if (allCombEqs.size() == 1) // optimization (modus ponens)
        {
          assert (newBodies.size() == 1);
          newBodies = {newBodies[0]->left(), newBodies[0]->right()};
        }
        else
          newBodies.push_back(distribDisjoin(allCombEqs, m_efac));

        r.body = conjoin(newBodies, m_efac);
        assert(!contains(r.body, memTy)); // sanity check after repl
      }

      r.body = simplifyBool(simplifyArr(simplifyBV(r.body)));
      if (printLog >= 8) pprint(r.body, 4);
    }

    void rewriteMem()
    {
      for (auto &r : ruleManager.chcs)
      {
        rewriteMemCHC(r);
        if (!r.isFact)
          for (auto & v : arrVars[r.srcRelation])
            r.srcVars.push_back(v.second);
        if (!r.isQuery)
          for (auto & v : arrVarsPr[r.dstRelation])
            r.dstVars.push_back(v.second);
      }

      ExprSet updDecls;
      for (auto & d : ruleManager.decls)
      {
        ExprVector args;
        args.insert (args.end (), d->args_begin (), d->args_end ()-1);
        for (auto & v : arrVars[d->left()]) args.push_back(typeOf(v.second));
        args.push_back(d->last());
        updDecls.insert(mknary<FDECL>(args));
      }
      ruleManager.decls = updDecls;
      for (auto & d : ruleManager.decls)
      {
        for (auto & v : arrVars[d->left()]){
          ruleManager.invVars[d->left()].push_back(v.second);
        }
        for (auto & v : arrVarsPr[d->left()]){
          ruleManager.invVarsPrime[d->left()].push_back(v.second);
        }
      }
    }

    void simplifyCHCstruct()
    {
      ruleManager.prepTransitVars();
      vector<int> simplCHCs;
      for (int j = 0; j < ruleManager.chcs.size(); j++)
      {
        auto & c = ruleManager.chcs[j];
        if (c.isFact) simplCHCs.push_back(j);
        if (c.isFact || c.isQuery ||
           memMap[c.srcRelation] == -1 || memMap[c.dstRelation] == -1) continue;
        Expr a = c.srcVars[memMap[c.srcRelation]];
        Expr b = c.dstVars[memMap[c.dstRelation]];
        bool broke = false;
        for (auto & t : ruleManager.transit[c.id])
        {
          if (t.first == a && t.second == b)
          {
            broke = true;
            break;
          }
        }
        if (!broke) simplCHCs.push_back(j);
      }
      ruleManager.doElim(false, simplCHCs);
    }

    int getStat()
    {
      return u.getStat();
    }
  };
}

#endif
