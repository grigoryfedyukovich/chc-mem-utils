#ifndef MEMPROCESSOR__HPP__
#define MEMPROCESSOR__HPP__

#include "RndLearnerV3.hpp"
#include "ae/ExprSimplBv.hpp"

using namespace std;
using namespace boost;
namespace ufo
{
  class MemProcessor : public RndLearnerV3
  {
    public:

    MemProcessor (ExprFactory &m_efac, EZ3 &z3, CHCs& r,
                                      map<int, string>& _v, int debug, int to) :
      RndLearnerV3 (m_efac, z3, r, to, false, false, 0, 1, false, 0, false,
                false, true, false, 0, false, false, to, debug),
                varIds(_v) {}

    map<Expr, int> allocMap, offMap, sizMap, memMap;
    map<Expr, ExprSet> allocVals;
    map<Expr, map<Expr, ExprSet>> aliases, aliasesRev;
    map<Expr, ExprMap> arrVars, arrVarsPr;
    Expr memTy, allocTy, defAlloc;
    map<int, string>& varIds;
    Expr lastDecl = NULL;

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
              if (indeterminate(vals[cand])) outs () << "     >>  indet: " << cand << "\n";
              else if (!vals[cand]) outs () << "        >> fail: " << cand << "\n";
            }
          }

          // first try to remove candidates immediately by their models (i.e., vals)
          // then, invalidate (one-by-one) all candidates
          //            for which Z3 failed to find a model

          for (int i = 0; i < 3 /*weakeningPriorities.size() */; i++)
          {
            outs() << " weaken round " << i << "\n";
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

    // beginning of the preprocessing:
    // determine the alloca and mem vars
    // init defAlloc as const array (of type from alloc/mem storages)
    void initAlloca()
    {
      memTy = NULL;
      for (auto &d : ruleManager.decls)
      {
        memMap[d->left()] = -1;
        allocMap[d->left()] = -1;
        offMap[d->left()] = -1;
        sizMap[d->left()] = -1;
        if (printLog >= 2)
          outs () << "initAlloca, decl: " << d->left() << "\n";
        auto & vs = ruleManager.invVars[d->left()];
        for (int i = 0; i < vs.size(); i++)
        {
          auto & v = vs[i];
          if (isArray(v))
          {
            if (isOpX<BVSORT>(typeOf(v)->left()) &&
                isOpX<BVSORT>(typeOf(v)->right()) &&
                lexical_cast<string>(v) == "_alloc")
            {
              if (printLog >= 2) outs () << "  alloca arg # "  << i << ": "
                  << v << "   " << typeOf(v) << "\n";
              assert(allocMap[d->left()] == -1);
              allocMap[d->left()] = i;
            }
            else if (isOpX<BVSORT>(typeOf(v)->left()) &&
                isOpX<BVSORT>(typeOf(v)->right()) &&
                lexical_cast<string>(v) == "_off")
            {
              if (printLog >= 2) outs () << "  off arg # "  << i << ": "
                  << v << "   " << typeOf(v) << "\n";
              assert(offMap[d->left()] == -1);
              offMap[d->left()] = i;
            }
            else if (isOpX<BVSORT>(typeOf(v)->left()) &&
                isOpX<BVSORT>(typeOf(v)->right()) &&
                lexical_cast<string>(v) == "_siz")
            {
              if (printLog >= 2) outs () << "  siz arg # "  << i << ": "
                  << v << "   " << typeOf(v) << "\n";
              assert(sizMap[d->left()] == -1);
              sizMap[d->left()] = i;
            }
            else if (isOpX<BVSORT>(typeOf(v)->left()) &&
                     isOpX<ARRAY_TY>(typeOf(v)->right()) &&
                     lexical_cast<string>(v) == "_mem")
            {
              if (printLog >= 2) outs () << "  mem arg # "  << i << ": "
                  << v << "   " << typeOf(v) << "\n";
              if(memMap[d->left()] != -1)
              {
                exit(9);
              }
              memMap[d->left()] = i;
            }
          }
        }
        assert(allocMap[d->left()] != -1);
        if(memMap[d->left()] == -1){
          exit(9);
        }
        Expr ty = typeOf(ruleManager.invVars[d->left()][memMap[d->left()]]);
        defAlloc = mk<CONST_ARRAY>(ty->left(), bvnum(mkMPZ(0, m_efac), ty->left()));
        if (memTy == NULL) memTy = ty;
        else assert (memTy == ty);
        if (allocTy == NULL) allocTy = ty->right();
        else assert (allocTy == ty->right());
      }
    }

    // find a fact CHC, r = body -> rel(..), look for its alloca storage (key -> val)
    // constucts \phi as `body(fact) /\ alloc[key] = value`
    // while \phi is sat,
    //       1) get m, s.t. m \models \phi, and
    //       2) update \phi to \phi /\ m(val) != val
    //       3) insert m(val) to alloca_vals [rel]
    //       4) insert m(key) to aliases [rel] [m(val)], and
    //       5) insert m(val) to aliases^-1 [rel] [m(key)]
    void getInitAllocaVals()
    {
      // get init vals, from fact
      for (auto & r : ruleManager.chcs)
      {
        if (r.isFact)
        {
          auto & v = r.dstVars[allocMap[r.dstRelation]];
          assert (isArray(v) && isOpX<BVSORT>(typeOf(v)->right()));

          if (printLog >= 6) outs () << "getInitAllocaVals, arr var (1):  " << v << "\n";
          auto key = mkConst(mkTerm<string> ("key", m_efac), typeOf(v)->left());
          auto val = mkConst(mkTerm<string> ("val", m_efac), typeOf(v)->right());
          ExprSet cnjs = {r.body, mk<EQ>(mk<SELECT>(v, key), val)};
          while (true == u.isSat(cnjs))
          {
            // if (!u.isSat(cnjs)) break;
            auto m = u.getModel(val);
            cnjs.insert(mk<NEQ>(val, m));
            if (lexical_cast<string>(toMpz(m)) != "0")
            {
              allocVals[r.dstRelation].insert(m);
              aliases[r.dstRelation][m].insert(u.getModel(key));
              aliasesRev[r.dstRelation][u.getModel(key)].insert(m);
              if (printLog >= 6) outs () << "    model:  " << toMpz(m) <<
                        " <- " << toMpz(u.getModel(key)) << "\n";
            }
          }
        }
      }
    }

    // Houdini-based method to find all alloca values
    // for each relation: create a candidate \forall key, alloc[key] = 0
    // check inductive: for each cti: extract new model
    //      and update cand to \forall key, (alloc[key] = 0 \/ alloc[key] = model)
    void getAllocaVals()
    {
      // get init cands, from vals
      for (auto & d : ruleManager.decls)
      {
        auto decl = d->left();
        if (printLog >= 6) outs () << "getAllocaVals, decl " << d->left()
                                  << ", ind = "<< getVarIndex(decl, decls)<< "\n";
        // if (allocVals[decl].empty()) continue;
        auto & v = ruleManager.invVars[decl][allocMap[decl]];
        if (printLog >= 6) outs () << "  v = " << v << ", " << typeOf(v) << "\n";
        allocVals[decl].insert(bvnum(mkMPZ(0, m_efac), typeOf(v)->left()));
        auto key = mkConst(mkTerm<string> ("key", m_efac), typeOf(v)->left());
        ExprSet dsjs;
        for (auto & val : allocVals[decl])
          dsjs.insert(mk<EQ>(mk<SELECT>(v, key), val));

        candidates[getVarIndex(decl, decls)] = { mknary<FORALL>(
                            ExprVector({key->left(), disjoin(dsjs, m_efac)})) };

        if (printLog >= 6) outs () << "  cands:\n";
        if (printLog >= 6) pprint(candidates[getVarIndex(decl, decls)], 4);
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
        if (printLog >= 6) outs () << "   mdl:  " << mdl << "\n";
        if (st.empty() && isOpX<CONST_ARRAY>(mdl))
          st = {mk<STORE>(mdl, mdl->last(), mdl->last())};    // hack for now;
        for (auto & m : st)
          if (is_bvnum(m->last()))
          {
            if (printLog >= 6) outs () << "   adding val " << m->last() << "\n";
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
      for (auto & d : ruleManager.decls)
      {
        auto decl = d->left();
        if (printLog >= 6) outs () << "\ngetAliases, decl " << d->left()
                            << ", ind = "<< getVarIndex(decl, decls)<< "\n";

        auto & v = ruleManager.invVars[decl][allocMap[decl]];
        allocVals[decl].insert(bvnum(mkMPZ(0, m_efac), typeOf(v)->left()));
        auto key = mkConst(mkTerm<string> ("key", m_efac), typeOf(v)->left());
        auto eq = mk<EQ>(mk<SELECT>(v, key), ptr);
        ExprSet dsjs;
        for (auto & k : aliases[decl][ptr])
          dsjs.insert(mk<EQ>(key, k));
        candidates[getVarIndex(decl, decls)] = {mknary<FORALL>(
              ExprVector({key->left(), mk<IMPL>(eq, disjoin(dsjs, m_efac))}))};
        if (printLog >= 6)
          outs () << "  decl: " << decl << ", " << ptr << "\ncands:\n";
        if (printLog >= 6)
          pprint(candidates[getVarIndex(decl, decls)], 4);
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
        if (printLog >= 6) outs () << "   mdl:  " << mdl << "\n";
        ExprVector st;
        filter (mdl, IsStore (), inserter(st, st.begin()));
        for (auto & m : st)
          if (is_bvnum(m->right()) && m->last() == ptr)
          {
            if (printLog >= 6) outs () << "   adding val " << m->right() << "\n";
            aliases[lastDecl][ptr].insert(m->right());
            aliasesRev[lastDecl][m->right()].insert(ptr);
          }
        getAliases(ptr);
      }
    }

    void getAliases()
    {
      ExprSet all;
      for (auto & d : ruleManager.decls)
        for (auto & val : allocVals[d->left()])
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
      for (auto & d : ruleManager.decls)
      {
        if (printLog >= 2)
          outs () << "  >> decl >>  " << d->left() << ":\n";
        for (auto & al : aliases[d->left()])
        {
          for (auto & a : al.second)
          {
            if (printLog >= 2)
              outs () << "    " << a << "  ->  " << al.first << "\n";
          }
        }
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
        for (auto & d : ruleManager.decls)
        {
          for (auto & al : aliases[d->left()])
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
      }

      for (auto & d : ruleManager.decls)
      {
        for (auto & al : aliases[d->left()])
        {
          auto name = getNewArrName(al.first);
          auto a = mkConst(mkTerm<string> (name, m_efac), memTy->right());
          auto b = mkConst(mkTerm<string> (name + "'", m_efac), memTy->right());
          allArrVars.insert(a);
          allArrVarsPr.insert(b);
          arrVars[d->left()][al.first] = a;
          arrVarsPr[d->left()][al.first] = b;
          auto ind = getVarIndex(d->left(), decls);
          invarVars[ind][
            ruleManager.invVars[d->left()].size() - 1 +
            arrVars[d->left()].size()] = a;
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

    // assumes small-block encoding:
    //  - fact has no memory manipulation, and actually is empty
    //  - every CHC has at most one operation (either for mem, alloca, or non-mem)
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
    void rewriteMem()
    {
      for (auto &r : ruleManager.chcs)
      {
        if (printLog >= 2)
          outs () << "-----------   processing CHC:   "
                  << r.srcRelation << " -> " << r.dstRelation <<
                  (r.isQuery ? " [Q]": (r.isInductive ? " [TR]": "")) << "\n";
        Expr body = r.body;
        Expr allocUpd = getUpdLiteral(body, allocTy, "_alloc'");
        if (!r.isQuery)
        {
          ExprVector toElim = {r.dstVars[allocMap[r.dstRelation]]};
          body = ufo::eliminateQuantifiers(body, toElim);
          if (allocUpd != NULL) body = mk<AND>(allocUpd, body);
        }
        if (r.isFact)
        {
          assert(!contains(body, memTy));
          for (auto & v : arrVarsPr[r.dstRelation])
            r.dstVars.push_back(v.second);
          continue;
        }
        if (printLog >= 4) pprint(body, 3);
        map<Expr, ExprSet> & ali = r.isFact ? aliases[r.dstRelation] :
                                              aliases[r.srcRelation];
        Expr rel = r.srcRelation;
        Expr alloSrc = r.srcVars[allocMap[rel]];
        Expr memVar = r.srcVars[memMap[r.srcRelation]];

        // step 0: find alloc-selects, replace them by respective vals
        //                      (need to be an invar)
        vector<ExprMap> aliasCombs;
        ExprVector se;
        ExprSet seSimp;
        filter (body, IsSelect (), inserter(se, se.begin()));
        for (auto s : se)
        {
          Expr sSimp = simplifyArr(simplifyBV(s));
          r.body = replaceAll(r.body, s, sSimp);
          seSimp.insert(sSimp);
        }

        ExprSet considered;
        for (auto sit = seSimp.rbegin(); sit != seSimp.rend(); ++sit)
        {
          auto s = *sit;

          if (s->left() == alloSrc)
          {
            ExprSet tmp;
            if (is_bvnum(s->last()))
            {
              tmp = aliasesRev[rel][s->last()];
            }
            else
            {
              for (auto & val : allocVals[rel])
                if (lexical_cast<string>(toMpz(val)) != "0")
                  tmp.insert(val);
            }

            if (aliasCombs.empty())
            {
              for (auto & a : tmp)
              {
                ExprMap tmp;
                tmp[s] = a;
                aliasCombs.push_back(tmp);
                if (printLog >= 2)
                  outs () << "alias:  " << s->last() << " -> " << a << "\n";
              }
            }
            else
            {
              vector<ExprMap> aliasCombsTmp = aliasCombs;
              aliasCombs.clear();
              for (auto & a : tmp)
              {
                for (auto & m : aliasCombsTmp)
                {
                  ExprMap tmp = m;
                  if (printLog >= 2)
                    outs () << "alias:  " << s->last() << " -> " << a << "\n";
                  assert (m[s] == NULL);
                  tmp[s] = a;
                  aliasCombs.push_back(tmp);
                }
              }
            }
          }
        }

        ExprVector newBodies;
        int it = 0;

        body = simplifyArr(simplifyBV(body));
        for (auto & m : aliasCombs)
        {
          Expr newBody = body;
          ExprVector newConstrs;
          ExprMap combEqs;
          for (auto & v : m)
          {
            auto tmp = replaceAll(v.first, combEqs);
            combEqs[tmp] = v.second;
          }
          Expr combEqss = conjoin(combEqs, m_efac);
          if (printLog >= 2)
          {
            outs () << "  combination # " << (it++) << ":\n";
            pprint(combEqss, 4);
          }

          newBody = replaceAll(newBody, m);

          ExprSet arrVarsMached;
          for (auto & al : ali)
          {
            Expr num = al.first;
            Expr replTo = arrVars[rel][num];
            if (printLog >= 2) outs () << "rewriting " << mk<SELECT>(memVar, num)
                                       << " with " << replTo << "\n";
            newBody = replaceAll(newBody, mk<SELECT>(memVar, num), replTo);
            combEqss = replaceAll(combEqss, mk<SELECT>(memVar, num), replTo);
          }
          if (printLog >= 2)
          {
            outs () << "  body after replacement:\n";
            pprint(newBody);
            outs() << "\n";
          }
          ExprVector st;
          filter (newBody, IsStore (), inserter(st, st.begin()));

          Expr newDef = NULL;
          for (auto s : st)   // find at most one new array update
          {
            s = simplifyArr(simplifyBV(s));
            if (!(typeOf(s->left()) == memTy && IsConst() (s->left()))) continue;
            Expr num = s->right();
            assert(newDef == NULL && "possibly, not a small-step encoding");
            if (!is_bvnum(num)) continue;

            newDef = s;
            newConstrs.push_back(mk<EQ>(arrVarsPr[rel][num], s->last()));
            arrVarsMached.insert(num);

            if (printLog >= 2)
            {
              outs () << "     found store:   ";
              pprint(newDef);
              outs () << "\n   new restr:  ";
              pprint(newConstrs.back());
            }
          }
          if (newDef == NULL)
          {
            if (printLog >= 2) outs () << "  no store found\n";
          }
          else
          {
            if (printLog >= 2) outs () << " - - \n done with this rewr\n";
            newBody = replaceAll(newBody, newDef, newDef->left());
            newBody = simplifyArr(simplifyBV(newBody));
          }

          // binding src/dst vars
          for (auto & a : arrVars[r.dstRelation])
          {
            Expr num = a.first;
            if (find(arrVarsMached.begin(), arrVarsMached.end(), num) ==
                     arrVarsMached.end())
              newConstrs.push_back(mk<EQ>(arrVars[r.dstRelation][num],
                                          arrVarsPr[r.dstRelation][num]));
          }

          Expr memUpd = getUpdLiteral(newBody, memTy, "_mem'");
          if (memUpd == NULL)
            newConstrs.push_back(newBody);
          else
            newConstrs.push_back(replaceAll(newBody, memUpd, mk<TRUE>(m_efac)));

          newBody = mk<IMPL>(combEqss, conjoin(newConstrs, m_efac));
          newBodies.push_back(newBody);
        }
        if (aliasCombs.empty())
        {
          // binding src/dst vars
          ExprSet newBody;
          getConj(r.body, newBody);
          for (auto & a : arrVars[r.dstRelation])
          {
            Expr num = a.first;
            newBody.insert(mk<EQ>(arrVars[r.dstRelation][num],
                                  arrVarsPr[r.dstRelation][num]));
          }
          r.body = conjoin(newBody, m_efac);

          for (auto & a : arrVars[r.dstRelation])
          {
            if (printLog >= 2)
              outs () << "rewriting " << mk<SELECT>(memVar, a.first)
                                      << " with " << a.second << "\n";
            r.body = replaceAll(r.body, mk<SELECT>(memVar, a.first), a.second);
          }

          if (printLog >= 2)
          {
            outs () << "aliasCombs empty:\n";
            pprint(r.body);
          }
        }
        else
        {
          r.body = conjoin(newBodies, m_efac);
          if (printLog >= 2) pprint(r.body, 4);

          // sanity check after repl:
          assert(!contains(r.body, memTy));
        }

        r.body = simplifyBool(simplifyArr(simplifyBV(r.body)));

        for (auto & v : arrVars[r.srcRelation])
          r.srcVars.push_back(v.second);
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
      vector<int> simplCHCs;
      for (int j = 0; j < ruleManager.chcs.size(); j++)
      {
        auto & c = ruleManager.chcs[j];
        if (c.isFact){
          simplCHCs.push_back(j);
          continue;
        }
        if (c.isQuery) {
          continue;
        }
        Expr srcVar = ruleManager.invVars[c.srcRelation][memMap[c.srcRelation]];
        Expr dstVar = ruleManager.invVarsPrime[c.dstRelation][memMap[c.dstRelation]];
        if (!u.implies(c.body, mk<EQ>(srcVar, dstVar))) simplCHCs.push_back(j);
      }
      ruleManager.doElim(false, simplCHCs);
    }
  };

  inline void process(string smt,
                      map<int, string>& varIds,  // obsolete, to remove
                      bool memsafety, bool serial, int debug,
                      int mem, int to)
  {
    ExprFactory m_efac;
    EZ3 z3(m_efac);
    CHCs ruleManager(m_efac, z3, memsafety, debug - 2);
    ruleManager.parse(smt, mem);

    if (debug > 0)
    {
      outs () << "Original CHCs:\n";
      outs () << "(size: " << ruleManager.chcs.size() << ", "
                         << ruleManager.decls.size() << ")\n";
      ruleManager.print(false, (debug > 2), "chc_orig");
      if (serial) ruleManager.serialize("chc_orig");
    }

    ruleManager.wtoSort();
    if (!ruleManager.hasBV) return;

    if (debug)
    {
      int n = ruleManager.getQum();
      outs () << "Queries detected: " << n << "\n";
    }

    if (!ruleManager.hasCycles())
    {
      BndExpl bnd(ruleManager, debug);
      bnd.exploreTraces(1, ruleManager.chcs.size(), true);
      outs () << "Found Cexes: " << bnd.foundCexes.size() << "\n";
      if (bnd.foundCexes.empty()) errs() << "sat\n";
      else errs() << "unsat\n";
      return;
    }

    MemProcessor ds(m_efac, z3, ruleManager, varIds, debug, to);

    for (auto dcl : ruleManager.decls)
      ds.initializeDecl(dcl->left());

    if (mem)
    {
      ds.initAlloca();
      ds.simplifyCHCstruct();
      if (debug > 0)
      {
        outs () << "Simplifying CHCs (in progress):\n";
        ruleManager.print((debug > 3), (debug > 2), "chc_interm");
      }
      ds.getInitAllocaVals();
      ds.getAllocaVals();
      ds.getAliases();
      ds.printAliases();
      ds.addAliasVars();
      ds.rewriteMem();
      auto res = ruleManager.doElim(false);
      ruleManager.wtoSort();
      if (debug > 0)
      {
        outs() << "\nCHCs after memory-rewriting and simplification:\n";
        ruleManager.print((debug > 3), false);
      }
      if (!res) return;
    }
    else
    {
      auto res = ruleManager.doElim(false);
      ruleManager.wtoSort();
    }

    if (debug)
    {
      outs () << "CHC size: " << ruleManager.chcs.size() << ", "
                              << ruleManager.decls.size() << "\n";
    }

    if (serial)
    {
      ruleManager.serialize("chc_optim");
      ruleManager.print(false, true, "chc_optim");
      return;
    }
    assert(0 && "Solving to come soon\n");
  }
}

#endif
