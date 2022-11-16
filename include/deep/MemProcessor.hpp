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

    MemProcessor (ExprFactory &m_efac, EZ3 &z3, CHCs& r, int _norm, bool _dat,
        int _cnt, int _mut, int _prj, map<int, string>& _v, int debug, int to) :
      RndLearnerV3 (m_efac, z3, r, to, false, false, 0, 1, false, 0, false,
        false, true, false, 0, false, false, to, debug),
        norm(_norm), dat(_dat), cnts(_cnt), mutNum(_mut), prj(_prj), varIds(_v) {}

    map<Expr, int> allocMap, offMap, sizMap, memMap;
    map<Expr, ExprSet> allocVals;
    map<Expr, map<Expr, ExprSet>> aliases, aliasesRev;
    map<Expr, ExprMap> arrVars, arrVarsPr;
    Expr memTy, allocTy, defAlloc;
    map<int, string>& varIds;
    Expr lastDecl = NULL;
    ExprSet offsetsSizes;
    bool dat;
    int norm, cnts, mutNum, prj;
    map<Expr, ExprSet> sizes, counters, negCounters, upbounds, lobounds;
    map<int, ExprVector> candidatesNext;

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
        if (printLog >= 3)
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
              if (printLog >= 3) outs () << "  alloca arg # "  << i << ": "
                  << v << "   " << typeOf(v) << "\n";
              assert(allocMap[d->left()] == -1);
              allocMap[d->left()] = i;
            }
            else if (isOpX<BVSORT>(typeOf(v)->left()) &&
                isOpX<BVSORT>(typeOf(v)->right()) &&
                lexical_cast<string>(v) == "_off")
            {
              if (printLog >= 3) outs () << "  off arg # "  << i << ": "
                  << v << "   " << typeOf(v) << "\n";
              assert(offMap[d->left()] == -1);
              offMap[d->left()] = i;
            }
            else if (isOpX<BVSORT>(typeOf(v)->left()) &&
                isOpX<BVSORT>(typeOf(v)->right()) &&
                lexical_cast<string>(v) == "_siz")
            {
              if (printLog >= 3) outs () << "  siz arg # "  << i << ": "
                  << v << "   " << typeOf(v) << "\n";
              assert(sizMap[d->left()] == -1);
              sizMap[d->left()] = i;
            }
            else if (isOpX<BVSORT>(typeOf(v)->left()) &&
                     isOpX<ARRAY_TY>(typeOf(v)->right()) &&
                     lexical_cast<string>(v) == "_mem")
            {
              if (printLog >= 3) outs () << "  mem arg # "  << i << ": "
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
            if (printLog >= 4) pprint(r.body);
          }
        }
        else
        {
          r.body = conjoin(newBodies, m_efac);
          if (printLog >= 4) pprint(r.body, 4);
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

    map<int, map<int, ExprSet>> origs;
    void addToCandidates(int ind, Expr e, int debugMarker, bool split = true)
    {
      e = simplifyBool(simplifyBV(e));
      if (find(candidates[ind].begin(), candidates[ind].end(), e) !=
               candidates[ind].end()) return;

      if (split)
      {
        ExprSet factored;
        getConj(factor(e, prj), factored);
        if (factored.size() > 1)
        {
          for (auto & f : factored)
          {
            ExprSet dsjs;
            getDisj (f, dsjs);
            if (dsjs.size() < 3)
              addToCandidates(ind, f, debugMarker, false);
          }
          return;
        }
      }
      if (isOpX<OR>(e) && e->arity() > 3 /* to amend */) return;

      Expr rel = decls[ind];
      Expr lms = conjoin(sfs[ind].back().learnedExprs, m_efac);
      if (u.implies(lms, e)) return;

      assert(hasOnlyVars(e, ruleManager.invVars[rel]));
      candidates[ind].push_back(e);
      origs[ind][debugMarker].insert(e);

      if (printLog >= 2)
        outs () << "adding to candidates: " << rel << " / "
                << debugMarker << ": " << e << "\n";
    }

    void countersAnalysis()
    {
      for(auto & c : ruleManager.chcs)
      {
        if (!c.isInductive) continue;

        auto rel = c.srcRelation;
        auto ind = getVarIndex(c.dstRelation, decls);
        auto lems = sfs[ind].back().learnedExprs;
        lems.insert(c.body);
        auto strbody = conjoin(lems, m_efac);

        // find counters
        // consider only one-by-one counters
        for (auto i = 0; i < c.srcVars.size(); i++)
        {
          Expr var = c.srcVars[i];
          if (is_bvconst(var))
          {
            auto ty = typeOf(var);
            Expr one = bvnum(mkMPZ(1, m_efac), ty);   // one-by-one counters
            if (u.implies(strbody, mk<EQ>(mk<BADD>(one, var), c.dstVars[i])))
            {
              if (printLog)
                outs () << "possibly counter for " << rel << ": " << var << "\n";
              counters[rel].insert(var);
              lobounds[rel].insert(bvnum(mkMPZ(0, m_efac), ty)); // TO FIX
            }

            if (u.implies(strbody, mk<EQ>(mk<BSUB>(var, one), c.dstVars[i])))
            {
              if (printLog)
                outs () << "possibly NEG counter for " << rel << ": " << var << "\n";
              negCounters[rel].insert(var);
            }
          }

          if (i == offMap[rel])
          {
            for (auto & al : aliases[rel])
            {
              for (auto & a : al.second)
              {
                auto ty = typeOf(var)->right();
                Expr one = bvnum(mkMPZ(1, m_efac), ty); // one-by-one counters
                Expr sel = mk<SELECT>(var, a);
                if (u.implies(strbody,
                         mk<EQ>(mk<BADD>(one, sel), mk<SELECT>(c.dstVars[i], a))))
                {
                  if (printLog)
                    outs () << "possibly counter for " << rel << ": " << sel  << "\n";
                  counters[rel].insert(sel);
                  lobounds[rel].insert(bvnum(mkMPZ(0, m_efac), ty)); // TO FIX
                }
              }
            }
          }
        }
      }
    }

    void simplifyCHCs()
    {
      if (printLog) outs() << "  Lemma-based CHC simplification\n";
      for(auto & c : ruleManager.chcs)
      {
        auto b = c.body;
        ExprSet impls;
        getImpls(b, impls);

        if (!c.isFact)
        {
          auto ind = getVarIndex(c.srcRelation, decls);
          auto lems = sfs[ind].back().learnedExprs;
          lems.insert(b);
          b = conjoin(lems, m_efac);
        }

        if (impls.size() > 0)
        {
          for (auto & a : impls)
          {
            auto tmp = replaceAll(b, a, a->right());
            if (u.isEquiv(b, tmp)) b = tmp;
          }
        }

        ExprVector toAvoid;
        for (auto & v : c.dstVars)
          if (lexical_cast<string>(v).find("$tmp") != string::npos)
            toAvoid.push_back(v);

        b = simpleQE(b, c.locVars, toAvoid);

        b = factor(b, norm); // to elaborate on

        c.body = b;

        if (printLog >= 3)
        {
          outs () << "\n--------\n   Simplified CHC " <<
                     c.srcRelation << " -> " << c.dstRelation << ":\n";
          ExprSet cnj;
          getConj(b, cnj);
          outs () << " // preconds:\n";
          for (auto it = cnj.begin(); it != cnj.end(); )
            if (hasOnlyVars(*it, c.srcVars))
            {
              outs () << "  " << *it << "\n";
              it = cnj.erase(it);
            }
            else ++it;

          for (auto it = cnj.begin(); it != cnj.end(); )
            if (hasOnlyVars(*it, c.dstVars))
            {
              outs () << "  " << *it << "\n";
              it = cnj.erase(it);
            }
            else ++it;
          outs () << " // mixed:\n";
          pprint(cnj, 2);
        }
      }
    }

    void preproAdder(Expr a, ExprVector& vars, ExprVector& varsp,
                     int ind, int debugMarker)
    {
      auto b = simplifyBV(simplifyArr(a));
      b = rewriteSelectStore(b);
      b = u.removeITE(b);
      b = keepQuantifiersReplWeak(b, varsp);
      ExprSet cnjs;
      getConj(b, cnjs);

      if (isOpX<EQ>(a)) cnjs.insert(a); // for arrays and further simplification

      for (auto bb : cnjs)
      {
        if (hasOnlyVars(bb, varsp))
        {
          if (vars != varsp) bb = replaceAll(bb, varsp, vars);
          {
            ExprSet factored;
            getConj(factor(bb, prj), factored);
            {
              for (auto & f : factored)
              {
                addToCandidates(ind, f, debugMarker, false);
                if (isOpX<EQ>(f))
                {
                  if (isOpX<SELECT>(f->left()) && contains(allArrVars, f->left()->left()))
                  {
                    if (true == u.equalsTo(f->right(), mkMPZ(0, m_efac)))
                    {
                      Expr up = f->left()->right();
                      if (printLog)
                        outs () << "  storing zero at " << up << "\n";
                      upbounds[decls[ind]].insert(up);
                    }
                  }
                  else if (isOpX<SELECT>(f->right()) && contains(allArrVars, f->right()->left()))
                  {
                    if (true == u.equalsTo(f->left(), mkMPZ(0, m_efac)))
                    {
                      Expr up = f->right()->right();
                      if (printLog)
                        outs () << "  storing zero at " << up << "\n";
                      upbounds[decls[ind]].insert(up);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    bool varNoise(Expr var)
    {
      if (lexical_cast<string>(var).find("$tmp") != string::npos) return true;
      if (lexical_cast<string>(var).find("__CPROVER") != string::npos) return true;
      if (lexical_cast<string>(var).find("VERIFIER") != string::npos) return true;
      return false;
    }

    void processFact(HornRuleExt& c)
    {
      int indDst = getVarIndex(c.dstRelation, decls);

      ExprSet bodyCnjs, bodyProjs;
      getConj(c.body, bodyCnjs);
      ExprVector& v = ruleManager.invVarsPrime[c.dstRelation];

      for (auto a : bodyCnjs)
      {
        for (auto it1 = v.begin(); it1 != v.end(); it1++)
        {
          if (varNoise(*it1)) continue;
          for (auto it2 = it1; it2 != (prj > 1 ? v.end() : std::next(it1)); it2++)
          {
            if (varNoise(*it2)) continue;
            for (auto it3 = it2; it3 != (prj > 2 ? v.end() : std::next(it2)); it3++)
            {
              if (varNoise(*it3)) continue;
              for (auto it4 = it3; it4 != (prj > 3 ? v.end() : std::next(it3)); it4++)
              {
                if (varNoise(*it4)) continue;
                ExprSet vars = {*it1, *it2, *it3, *it4};
                bodyProjs.insert(projectOnlyVars(a, vars));
              }
            }
          }
        }
      }

      bodyCnjs.insert(bodyProjs.begin(), bodyProjs.end());

      for (auto a : bodyCnjs)
      {
        a = simplifyBV(simplifyArr(a));
        preproAdder(a, ruleManager.invVars[c.dstRelation],
                       ruleManager.invVarsPrime[c.dstRelation], indDst, 0);
      }
    }

    void processQuery(HornRuleExt& c)
    {
      int indSrc = getVarIndex(c.srcRelation, decls);

      ExprSet bodyCnjs, bodyCnjsTmp;
      getConj(c.body, bodyCnjsTmp);
      for (auto a : bodyCnjsTmp) bodyCnjs.insert(mkNeg(a));

      auto & lems = sfs[indSrc].back().learnedExprs;
      for (auto it = bodyCnjsTmp.begin(); it != bodyCnjsTmp.end();)
      {
        if (contains(lems, *it)) it = bodyCnjsTmp.erase(it);
        else ++it;
      }
      getConj(mkNeg(conjoin(bodyCnjsTmp, m_efac)), bodyCnjs);

      for (auto a : bodyCnjs)
        preproAdder(a, ruleManager.invVars[c.srcRelation],
                       ruleManager.invVars[c.srcRelation], indSrc, 1);
    }

    void processTrans(HornRuleExt& c)
    {
      int indSrc = getVarIndex(c.srcRelation, decls);
      int indDst = getVarIndex(c.dstRelation, decls);

      ExprSet bodyCnjs;
      getConj(c.body, bodyCnjs);

      for (auto a : bodyCnjs)
      {
        preproAdder(a, ruleManager.invVars[c.dstRelation],
                       ruleManager.invVarsPrime[c.dstRelation], indDst, 2);

        preproAdder(a, ruleManager.invVars[c.srcRelation],
                       ruleManager.invVars[c.srcRelation], indSrc, 299);
      }
    }

    Expr unprime(Expr e, Expr rel)
    {
      return replaceAll(e,
        ruleManager.invVarsPrime[rel], ruleManager.invVars[rel]);
    }

    Expr splitVar(Expr e, ExprVector& vars)
    {
      if (!isOpX<EQ>(e)) return NULL;

      ExprVector v;
      filter (e->left(), IsConst (), inserter(v, v.begin()));

      if (v.size() == 1 && contains(vars, *v.begin()))
      {
        auto tmp = repairBPrio(e, *v.begin());
        if (tmp != NULL) return tmp;
      }

      v.clear();
      filter (e->right(), IsConst (), inserter(v, v.begin()));

      if (v.size() == 1 && contains(vars, *v.begin()))
      {
        auto tmp = repairBPrio(mk<EQ>(e->right(), e->left()), *v.begin());
        if (tmp != NULL) return tmp;
      }

      return NULL;
    }

    void processProp(HornRuleExt& c)
    {
      ExprSet bodyCnjs, bodyCnjsExt;
      getConj(c.body, bodyCnjs);
      ExprMap bodySEqs, bodyDEqs; // for forward and backward prop, resp.
      ExprSet extraEqs;
      while(true)
      {
        bodyCnjsExt.clear();
        for (auto a : bodyCnjs)
        {
          if (!isOpX<EQ>(a)) continue;
          if (isArray(a->left()))
          {
            auto b = u.removeITE(rewriteSelectStore(a));
            if (a != b)
              getConj(b, bodyCnjsExt);
          }

          Expr sp = splitVar(a, c.dstVars);
          if (sp != NULL)
          {
            if (bodyDEqs[sp->left()] != NULL)
              extraEqs.insert(unprime(mk<EQ>(bodyDEqs[sp->left()], sp->right()), c.dstRelation));
            bodyDEqs[sp->left()] = sp->right();
          }
          sp = splitVar(a, c.srcVars);
          if (sp != NULL)
          {
            if (bodySEqs[sp->left()] != NULL)
              extraEqs.insert(unprime(mk<EQ>(bodySEqs[sp->left()], sp->right()), c.dstRelation));
            bodySEqs[sp->left()] = sp->right();
          }
        }
        if (bodyCnjsExt.empty()) break;
        bodyCnjs = bodyCnjsExt;
      }

      int indSrc = getVarIndex(c.srcRelation, decls);
      int indDst = getVarIndex(c.dstRelation, decls);

      for (auto & a : extraEqs)
        preproAdder(a,
                     ruleManager.invVars[c.dstRelation],
                     ruleManager.invVars[c.dstRelation], indDst, 128);

      ExprMap bodySEqsNew, bodyDEqsNew, bodySEqsM, bodyDEqsM;
      for (auto & a : bodySEqs)
      {
        Expr tmp = replaceAll(a.second, bodySEqs);
        if (hasOnlyVars(tmp, ruleManager.invVarsPrime[c.dstRelation]))
          bodySEqsNew[a.first] = unprime(tmp, c.dstRelation);
        tmp = replaceAll(a.second, bodyDEqs);
        if (hasOnlyVars(tmp, ruleManager.invVars[c.srcRelation]))
          bodySEqsM[a.first] = tmp;
      }

      for (auto & a : bodyDEqs)
      {
        Expr tmp = replaceAll(a.second, bodyDEqs);
        if (hasOnlyVars(tmp, ruleManager.invVars[c.srcRelation]))
          bodyDEqsNew[unprime(a.first, c.srcRelation)] = tmp;
        tmp = replaceAll(a.second, bodySEqs);
        if (hasOnlyVars(tmp, ruleManager.invVarsPrime[c.dstRelation]))
          bodyDEqsM[a.first] = tmp;
      }

      bodySEqs = bodySEqsNew;
      bodyDEqs = bodyDEqsNew;

      for (auto & a : bodySEqs)
        if (true == u.isSat(mk<EQ>(a.first, a.second)))
          preproAdder(mk<EQ>(a.first, a.second),
                       ruleManager.invVars[c.dstRelation],
                       ruleManager.invVars[c.dstRelation], indDst, 28);

      for (auto & a : bodyDEqs)
        if (true == u.isSat(mk<EQ>(a.first, a.second)))
          preproAdder(mk<EQ>(a.first, a.second),
                       ruleManager.invVars[c.dstRelation],
                       ruleManager.invVars[c.dstRelation], indDst, 28);

      for (auto & a : bodySEqsM)
        if (true == u.isSat(mk<EQ>(a.first, a.second)))
           preproAdder(mk<EQ>(a.first, a.second),
                      ruleManager.invVars[c.dstRelation],
                      ruleManager.invVars[c.dstRelation], indDst, 281);

      for (auto & a : bodyDEqsM)
        if (true == u.isSat(mk<EQ>(a.first, a.second)))
           preproAdder(unprime(mk<EQ>(a.first, a.second), c.dstRelation),
                      ruleManager.invVars[c.dstRelation],
                      ruleManager.invVars[c.dstRelation], indDst, 281);

      for (auto a : sfs[indSrc].back().learnedExprs)
        addToCandidates(indDst, replaceAll(a, bodySEqs, 1), 3);
      for (auto a : sfs[indDst].back().learnedExprs)
        addToCandidates(indSrc, replaceAll(a, bodyDEqs, 1), 4);
      for (auto a : candidates[indSrc])
        addToCandidates(indDst, replaceAll(a, bodySEqs, 1), 5);
      for (auto a : candidates[indDst])
        addToCandidates(indSrc, replaceAll(a, bodyDEqs, 1), 6);

      if (cnts)
      {
        for (auto & l : sfs[indSrc].back().learnedExprs)
        {
          if (!isOpX<EQ>(l)) continue;

          for (auto & cnt : counters[c.dstRelation])
          {
            if (cnt == l->left())
            {
              lobounds[c.dstRelation].insert(l->right());
              if (printLog)
                outs () << "  lower bound obtained from lemma: "
                        << replaceAll(l->right(), bodySEqs, 1) << "\n";
            }
            else if (cnt == l->right())
            {
              lobounds[c.dstRelation].insert(l->left());
              if (printLog)
                outs () << "  lower bound obtained from lemma: "
                        << replaceAll(l->left(), bodySEqs, 1) << "\n";
            }
          }
          for (auto & cnt : negCounters[c.dstRelation])
          {
            if (cnt == l->left())
            {
              upbounds[c.dstRelation].insert(l->right());
              if (printLog)
                outs () << "  upper bound obtained from lemma: "
                        << replaceAll(l->right(), bodySEqs, 1) << "\n";
            }
            else if (cnt == l->right())
            {
              upbounds[c.dstRelation].insert(l->left());
              if (printLog)
                outs () << "  upper bound obtained from lemma: "
                        << replaceAll(l->left(), bodySEqs, 1) << "\n";
            }
          }
        }

        for (auto & cnt1 : counters[c.srcRelation])
        {
          for (auto & cnt2 : counters[c.dstRelation])
          {
            if (cnt1 == cnt2)
            {
              for (auto u1 = upbounds[c.srcRelation].begin(); u1 != upbounds[c.srcRelation].end(); u1++)
              {
                for (auto & cnt3 : counters[c.dstRelation])
                {
                  if (cnt1 != cnt3)
                  {
                    Expr tmp = typeRepair(mk<BADD>(cnt3, *u1));
                    addToCandidates(indDst, mk<BSLT>(cnt1, tmp), 99);
                    addToCandidates(indDst, mk<BSLE>(cnt1, tmp), 99);
                  }
                }
                for (auto & cnt3 : negCounters[c.dstRelation])
                {
                  if (cnt1 != cnt3)
                  {
                    for (auto u2 = std::next(u1); u2 != upbounds[c.srcRelation].end(); u2++)
                    {
                      Expr tmp1 = typeRepair(mk<BSUB>(*u1, cnt3));
                      Expr tmp2 = typeRepair(mk<BADD>(cnt3, *u2));
                      addToCandidates(indDst, mk<BSLT>(cnt1, tmp2), 99);
                      addToCandidates(indDst, mk<BSLE>(cnt1, tmp2), 99);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    void processInitCounters(HornRuleExt& c)
    {
      int indSrc = -1;
      int indDst = -1;
      if (!c.isFact) indSrc = getVarIndex(c.srcRelation, decls);
      if (!c.isQuery) indDst = getVarIndex(c.dstRelation, decls);

      for (auto u : upbounds[c.srcRelation])
      {
        // last vals of prev counters
        for (auto & cnt : counters[c.srcRelation])
          addToCandidates(indDst, typeRepair(mk<EQ>(cnt, u)), 24);
        if (printLog)
          outs () << "  copying upper bound " << u << " from " << c.srcRelation
                  << " to " << c.dstRelation << "\n";
        lobounds[c.dstRelation].insert(u);
        upbounds[c.dstRelation].insert(u);
      }

      for (auto l : lobounds[c.srcRelation])
      {
        // last vals of prev negcounters
        if (printLog)
          outs () << "  copying lower bound " << l << " from " << c.srcRelation
                  << " to " << c.dstRelation << "\n";
        for (auto & cnt : negCounters[c.srcRelation])
          addToCandidates(indDst, typeRepair(mk<EQ>(cnt, l)), 25);
        lobounds[c.dstRelation].insert(l);
        upbounds[c.dstRelation].insert(l);
      }

      if (indSrc < 0) return;

      if (cnts > 1)
        for (auto & al : aliases[c.srcRelation])
          for (auto & a : al.second)
          {
            for (auto & cnt : counters[c.srcRelation])
              addToCandidates(indSrc, typeRepair(mk<EQ>(cnt,
                mk<SELECT>(c.srcVars[offMap[c.srcRelation]], a))), 26);
            for (auto & cnt : negCounters[c.srcRelation])
              addToCandidates(indSrc, typeRepair(mk<EQ>(cnt,
                mk<SELECT>(c.srcVars[offMap[c.srcRelation]], a))), 27);
          }
    }

    void addRelCntsCands(int ind, Expr d, Expr cnt1, Expr cnt2, int debugMarker)
    {
      Expr a = typeRepair(mk<BSUB>(cnt1, cnt2));
      for (auto & l: lobounds[d])
      {
        for (auto & u: upbounds[d])
        {
          Expr b = typeRepair(mk<BADD>(l, u));
          addToCandidates(ind, typeRepair(repairBComp<BSLE>(a, b)), debugMarker);
          addToCandidates(ind, typeRepair(repairBComp<BSGE>(a, b)), debugMarker);
          addToCandidates(ind, typeRepair(repairBComp<BSLT>(a, b)), debugMarker);
          addToCandidates(ind, typeRepair(repairBComp<BSGT>(a, b)), debugMarker);
        }
      }
    }

    void processBounds()
    {
      for (auto & decl : ruleManager.decls)
      {
        auto d = decl->left();
        int ind = getVarIndex(d, decls);

        // get more bounds from lemmas
        for (auto & c : sfs[ind].back().learnedExprs)
        {
          ExprVector cnts;
          cnts.insert(cnts.end(), counters[d].begin(), counters[d].end());
          cnts.insert(cnts.end(), negCounters[d].begin(), negCounters[d].end());
          for (auto & cnt : cnts)
          {
            Expr newLBound = NULL, newUBound = NULL;
            if (isOpX<BUGE>(c) || isOpX<BSGE>(c) || isOpX<BUGT>(c) || isOpX<BSGT>(c))
            {
              if (cnt == c->left()) newLBound = c->right();
              if (cnt == c->right()) newUBound = c->right();
            }
            if (isOpX<BULE>(c) || isOpX<BSLE>(c) || isOpX<BULT>(c) || isOpX<BSLT>(c))
            {
              if (cnt == c->left()) newUBound = c->right();
              if (cnt == c->right()) newLBound = c->right();
            }
            if (newLBound != NULL)
            {
              lobounds[d].insert(newLBound);
              if (printLog > 1)
                outs () << " new low bound for " << d << " from lemma: " << newLBound << "\n";
            }
            if (newUBound != NULL)
            {
              upbounds[d].insert(newUBound);
              if (printLog > 1)
                outs () << " new upper bound for " << d << " from lemma: " << newUBound << "\n";
            }
          }
        }

        if (printLog > 1)
        {
          outs () << "    lobounds[" << d << "] (" << lobounds[d].size() << "):\n";
          pprint(lobounds[d], 6);
          outs () << "    upbounds[" << d << "] (" << upbounds[d].size() << "):\n";
          pprint(upbounds[d], 6);
        }
        auto sv = ruleManager.invVars[d][sizMap[d]];
        auto ov = ruleManager.invVars[d][offMap[d]];
        auto ty = typeOf(sv)->right();
        Expr zero = bvnum(mkMPZ(0, m_efac), ty);

        for (auto & al : aliases[d])
        {
          addToCandidates(ind, mk<BSGT>(mk<SELECT>(sv, al.first), zero), 12);
          sizes[d].insert(mk<SELECT>(sv, al.first));

          if (cnts > 1)
          {
            for (auto & v : ruleManager.invVars[d])
              if (is_bvconst(v))
                addToCandidates(ind,
                  typeRepair(mk<EQ>(v, mk<SELECT>(sv, al.first))), 13);

            for (auto & a : al.second)
            {
              offsetsSizes.insert(mk<SELECT>(ov, a));
              offsetsSizes.insert(mk<SELECT>(sv, al.first));

              addToCandidates(ind,
                mk<BSLT>(mk<SELECT>(ov, a), mk<SELECT>(sv, al.first)), 15);
              addToCandidates(ind,
                mk<BSLE>(mk<SELECT>(ov, a), mk<SELECT>(sv, al.first)), 16);
            }
          }
        }

        if (cnts > 1)
        {
          for (auto & cnt1 : counters[d])
            for (auto & cnt2 : counters[d])
              if (cnt1 != cnt2)
                addRelCntsCands(ind, d, cnt1, cnt2, 17);

          for (auto & cnt1 : counters[d])
            for (auto cnt2 : negCounters[d])
              addRelCntsCands(ind, d, cnt1,
                mk<BSUB>(bvnum(mkMPZ(0, m_efac), typeOf(cnt2)), cnt2), 18);

          for (auto cnt1 : negCounters[d])
            for (auto cnt2 : negCounters[d])
              if (cnt1 != cnt2)
                addRelCntsCands(ind, d, cnt1, cnt2, 19);
        }

        for (auto & cnt : counters[d])
        {
          for (auto l : lobounds[d])
          {
            addToCandidates(ind, typeRepair(mk<BSGE>(cnt, l)), 20);
            if (cnts > 1)
              for (auto u : upbounds[d])
                addToCandidates(ind, typeRepair(mk<BSLE>(cnt, typeRepair(mk<BADD>(l, u)))), 21);
          }
          for (auto u : upbounds[d])
            addToCandidates(ind, typeRepair(mk<BSLE>(cnt, u)), 209);
        }

        for (auto & cnt : negCounters[d])
        {
          for (auto u : upbounds[d])
          {
            addToCandidates(ind, typeRepair(mk<BSLE>(cnt, u)), 22);
            if (cnts > 1)
              for (auto l : lobounds[d])
                addToCandidates(ind, typeRepair(mk<BSLE>(cnt, typeRepair(mk<BADD>(l, u)))), 23);
          }
          for (auto l : lobounds[d])
            addToCandidates(ind, typeRepair(mk<BSGE>(cnt, l)), 23);
        }
      }
    }

    void processData(int chc)
    {
      auto & c = ruleManager.chcs[chc];
      int indSrc = -1;
      if (!c.isFact) indSrc = getVarIndex(c.srcRelation, decls);

      Expr lems = indSrc >= 0 ?
        conjoin(sfs[indSrc].back().learnedExprs, m_efac) : mk<TRUE>(m_efac);

      ExprSet extrVars;
      if (cnts)
      {
        for (auto & d : decls)
        {
          extrVars.insert(counters[d].begin(), counters[d].end());
          extrVars.insert(negCounters[d].begin(), negCounters[d].end());
        }
        if (!extrVars.empty())
        {
          for (auto & d : decls)
          {
            extrVars.insert(sizes[d].begin(), sizes[d].end());
            extrVars.insert(upbounds[d].begin(), upbounds[d].end());
          }
        }
      }

      // first, see if we have any counters, then also add their sizes
      // otherwise, proceed with all vars
      if (extrVars.empty()) extrVars = offsetsSizes;
      getDataCandidates(chc, extrVars, lems);
    }

    template<typename Op1, typename Op2> void cndadd(Expr e1, Expr e2,
                    ExprVector& cands)
    {
      Expr c1 = mk<Op1>(e1, e2);
      Expr c2 = mk<Op2>(e2, e1);
      if (contains(cands, c1)) return;
      if (contains(cands, c2)) return;
      cands.push_back(c1);
    }

    void removeDupCandidates()
    {
      for (auto & c : candidates)
      {
        int sz = c.second.size();
        map<Expr, ExprSet> eqs;
        ExprVector tmp;
        for (auto a : sfs[c.first].back().learnedExprs)
        {
          if (isOpX<EQ>(a))
          {
            if (!isArray(a->left()))
            {
              if (!is_bvnum(a->left()))
                eqs[a->left()].insert(a->right());
              if (!is_bvnum(a->right()))
                eqs[a->right()].insert(a->left());
            }
          }
          else
          {
            if (!containsOp<OR>(a) && !containsOp<AND>(a) &&
                !containsOp<IMPL>(a))
              tmp.push_back(a);
          }
        }

        for (auto & e : eqs)
        {
          for (auto & f : e.second)
          {
            for (auto & a : tmp)
            {
              Expr b = replaceAll(a, e.first, f);
              if (a != b)
              {
                auto it = find(c.second.begin(), c.second.end(), b);
                if (it != c.second.end()) c.second.erase(it);
              }
            }

            set<int> toRem;
            for (auto & a : c.second)
            {
              Expr b = replaceAll(a, e.first, f);
              if (a != b)
              {
                for (int i = 0; i < c.second.size(); i++)
                {
                  if (c.second[i] == b)
                  {
                    toRem.insert(i);
                  }
                }
              }
            }
            for (auto rit = toRem.rbegin(); rit != toRem.rend(); rit++)
            {
              c.second.erase(c.second.begin() + *rit);
            }
          }
        }
        if (printLog > 2)
          outs () << "  rem dupl: " << sz << " -> " << c.second.size() << "\n";
      }
    }

    void mutateCandidates(int round = 0)
    {
      candidatesNext = candidates;
      candidates.clear();
      for (auto & c : candidatesNext)
      {
        map<Expr, ExprSet> eqs;
        ExprMap ults, slts, ules, sles;
        ExprVector tmp;

        auto & l = sfs[c.first].back().learnedExprs;
        for (auto it = c.second.begin(); it != c.second.end(); )
        {
          auto a = simplifyBV(*it);

          if (isOpX<EQ>(a))
          {
            auto f = find(l.begin(), l.end(), *it);
            if (f != l.end())
            {
              it = c.second.erase(it);
              continue;
            }

            candidates[c.first].push_back(a);
            if (!isArray(a->left()))
            {
              if (!is_bvnum(a->left()))
                eqs[a->left()].insert(a->right());
              if (!is_bvnum(a->right()))
                eqs[a->right()].insert(a->left());
            }
            it = c.second.erase(it);
          }
          else
          {
            if (!containsOp<OR>(a) && !containsOp<AND>(a) &&
                !containsOp<IMPL>(a))
              tmp.push_back(a);

            if (isOpX<BULT>(a))
              ults[a->left()] = a->right();
            if (isOpX<BSLT>(a))
              slts[a->left()] = a->right();
            if (isOpX<BUGT>(a))
              ults[a->right()] = a->left();
            if (isOpX<BSGT>(a))
              slts[a->right()] = a->left();
            if (isOpX<BULE>(a))
              ules[a->left()] = a->right();
            if (isOpX<BSLE>(a))
              sles[a->left()] = a->right();
            if (isOpX<BUGE>(a))
              ules[a->right()] = a->left();
            if (isOpX<BSGE>(a))
              sles[a->right()] = a->left();

            ++it;
          }
        }

        if (round == 0) continue;

        int j = c.second.size();

        for (int i = 0; i < tmp.size(); i++)
        {
          auto a = tmp[i];
          for (auto & e : eqs)
          {
            for (auto & f : e.second)
            {
              Expr b = replaceAll(a, e.first, f);
              if (a != b)
              {
                if (isOpX<BULT>(a))
                {
                  cndadd<BULT, BUGT>(b->left(), b->right(), c.second);
                  if (round == 2) cndadd<BULT, BUGT>(b->left(), b->right(), tmp);
                }
                else if (isOpX<BSLT>(a))
                {
                  cndadd<BSLT, BSGT>(b->left(), b->right(), c.second);
                  if (round == 2) cndadd<BSLT, BSGT>(b->left(), b->right(), tmp);
                }
                else if (isOpX<BULE>(a))
                {
                  cndadd<BULE, BUGE>(b->left(), b->right(), c.second);
                  if (round == 2) cndadd<BULE, BUGE>(b->left(), b->right(), tmp);
                }
                else if (isOpX<BSLE>(a))
                {
                  cndadd<BSLE, BSGE>(b->left(), b->right(), c.second);
                  if (round == 2) cndadd<BSLE, BSGE>(b->left(), b->right(), tmp);
                }
                else if (isOpX<BUGT>(a))
                {
                  cndadd<BUGT, BULT>(b->left(), b->right(), c.second);
                  if (round == 2) cndadd<BUGT, BULT>(b->left(), b->right(), tmp);
                }
                else if (isOpX<BSGT>(a))
                {
                  cndadd<BSGT, BSLT>(b->left(), b->right(), c.second);
                  if (round == 2) cndadd<BSGT, BSLT>(b->left(), b->right(), tmp);
                }
                else if (isOpX<BUGE>(a))
                {
                  cndadd<BUGE, BULE>(b->left(), b->right(), c.second);
                  if (round == 2) cndadd<BUGE, BULE>(b->left(), b->right(), tmp);
                }
                else if (isOpX<BSGE>(a))
                {
                  cndadd<BSGE, BSLE>(b->left(), b->right(), c.second);
                  if (round == 2) cndadd<BSGE, BSLE>(b->left(), b->right(), tmp);
                }
                else
                {
                  unique_push_back(b, c.second);
                  if (round == 2) unique_push_back(b, tmp);
                }
              }
            }
          }
        }

        j = c.second.size() - j;
        int j1 = c.second.size();

        // transitivity
        for (auto & a : ults)
        {
          for (auto & b : ults)
          {
            if (a.second == b.first)
            {
              cndadd<BULT, BUGT>(a.first, b.second, c.second);
            }
          }
        }
        for (auto & a : slts)
        {
          for (auto & b : slts)
          {
            if (a.second == b.first)
            {
              cndadd<BSLT, BSGT>(a.first, b.second, c.second);
            }
          }
        }
        for (auto & a : ults)
        {
          for (auto & b : ules)
          {
            if (a.second == b.first)
            {
              cndadd<BULT, BUGT>(a.first, b.second, c.second);
            }
          }
        }
        for (auto & a : slts)
        {
          for (auto & b : sles)
          {
            if (a.second == b.first)
            {
              cndadd<BSLT, BSGT>(a.first, b.second, c.second);
            }
          }
        }
        for (auto & a : ules)
        {
          for (auto & b : ults)
          {
            if (a.second == b.first)
            {
              cndadd<BULT, BUGT>(a.first, b.second, c.second);
            }
          }
        }
        for (auto & a : sles)
        {
          for (auto & b : slts)
          {
            if (a.second == b.first)
            {
              cndadd<BSLT, BSGT>(a.first, b.second, c.second);
            }
          }
        }
        for (auto & a : ules)
        {
          for (auto & b : ules)
          {
            if (a.second == b.first)
            {
              cndadd<BULE, BUGE>(a.first, b.second, c.second);
            }
          }
        }
        for (auto & a : sles)
        {
          for (auto & b : sles)
          {
            if (a.second == b.first)
            {
              cndadd<BSLE, BSGE>(a.first, b.second, c.second);
            }
          }
        }

        int sizeEqs = 0;
        for (auto & e : eqs) sizeEqs += e.second.size();

        if (printLog > 2)
          outs () << "mut stat: " << tmp.size() << " + " << eqs.size()<< " / "
                  << sizeEqs << ": " << ults.size() << " + "
                  << slts.size() << " + "<< ules.size() << " + "<< sles.size()
                  << " -> " << j << ", " << (c.second.size() - j1)<< "\n";
      }
    }

    tribool invSyn(int ser, int b = 0)
    {
      candidates.clear();
      if (b == 0)
      {
        for(auto & c : ruleManager.chcs)
        {
          if (!c.isQuery)
          {
            auto a = allocMap[c.dstRelation];
            auto av = ruleManager.invVars[c.dstRelation][a];
            auto avp = ruleManager.invVarsPrime[c.dstRelation][a];
            u.isSat(c.body);
            auto mdl = u.getModel(avp);
            if (avp != mdl)
            {
              auto cand = mk<EQ>(av, mdl);
              addToCandidates(getVarIndex(c.dstRelation, decls), cand, 100);

              ExprSet tmp;
              getConj(u.removeITE(rewriteSelectStore(cand)), tmp);
              for (auto & a : tmp)
                addToCandidates(getVarIndex(c.dstRelation, decls), a, 100);
            }
          }
        }
      }
      else
      {
        simplifyCHCs();

        if (cnts) countersAnalysis();
        processBounds();
        for(int chc = 0; chc < ruleManager.chcs.size(); chc++)
        {
          auto & c = ruleManager.chcs[chc];
          if (printLog)
            outs () << "  Synthesize [" << b << "] for CHC "
                    << c.srcRelation << " -> " << c.dstRelation << "\n";

          if (c.isFact) processFact(c);
          else if (c.isQuery) processQuery(c);
          else processTrans(c);
          if (!c.isInductive && !c.isQuery) processInitCounters(c);
          if (!c.isInductive && !c.isQuery && dat) processData(chc);
        }
      }

      if (ser == b + 1)
      {
        ruleManager.serialize("chc_optim");
        ruleManager.print(false, true, "chc_optim");
        exit(0);
      }

      for (int i = 0; i < ruleManager.decls.size() - 1; i++)
      {
        for (auto & c : ruleManager.chcs)
          if (!c.isQuery && !c.isFact && !c.isInductive)
            processProp(c);
      }

      auto tmp = candidates;
      for (int i = 0; i < mutNum; i++)
      {
        mutateCandidates(i);
        for (int j = 0; j < 2; j++)
        {
          if (j > 0) candidates = candidatesNext;
          removeDupCandidates();
          auto res = multiHoudini(ruleManager.allCHCs);
          if (res)
          {
            assignPrioritiesForLearned();
            if (printLog) printStats();
            if (ser == 0 && checkAllLemmas())
            {
              printSolution();
              return true;
            }
          }
        }
        candidates = tmp;
      }

      return invSyn(ser, b + 1);
    }

    void printStats()
    {
      set<int> all, used;
      for (auto & decl : ruleManager.decls)
      {
        auto d = decl->left();
        int ind = getVarIndex(d, decls);
          for (auto & b : origs[ind])
          {
            all.insert(b.first);
            for (auto & l : sfs[ind].back().learnedExprs)
            {
              if (find(b.second.begin(), b.second.end(), l) != b.second.end())
                used.insert(b.first);
          }
        }
      }

      for (auto it = all.begin(); it != all.end();)
      {
        if (find(used.begin(), used.end(), *it) == used.end()) it++;
        else it = all.erase(it);
      }

      outs () << "UNUSED cands: ";
      for (auto & a : all) outs () << a << ", ";
      outs () << "\n";
    }

    void mutate(ExprSet& tmp, ExprSet &extraVars)
    {
      ExprSet v;
      for (auto cit = tmp.begin(); cit != tmp.end(); ++cit)
      {
        if (!isOpX<EQ>(*cit)) continue;
        auto cl = (*cit)->left();
        auto cr = (*cit)->right();
        v.insert(repairBComp<EQ>(cl, cr));
        for (auto dit = std::next(cit); dit != tmp.end(); ++dit)
        {
          auto dl = (*dit)->left();
          auto dr = (*dit)->right();
          v.insert(repairBComp<EQ>(mk<BSUB>(cl, dl), mk<BSUB>(cr, dr)));
          v.insert(repairBComp<EQ>(mk<BSUB>(cl, dr), mk<BSUB>(cr, dl)));
          v.insert(repairBComp<EQ>(mk<BADD>(cl, dl), mk<BADD>(cr, dr)));
          v.insert(repairBComp<EQ>(mk<BADD>(cl, dr), mk<BADD>(cr, dl)));
        }
      }
      tmp.clear();
      tmp.insert(v.begin(), v.end());
    }

    void getDataCandidates(int chc, ExprSet& extraVars, Expr invs, int mut = 1)
    {
      DataLearner dl(ruleManager, m_z3, to, printLog);
      auto & r = ruleManager.chcs[chc].dstRelation;
      auto ind = getVarIndex(r, decls);

      vector<bool> range = {false};
      if (!cnts) range.push_back(true);

      for (bool b : range)
      {
        dl.computeDataOne(chc, r, invs, b, extraVars);
        ExprSet tmp;
        dl.computePolynomials(r, tmp);
        for (int i = 0; i < mut; i++) mutate(tmp, extraVars);
        for (auto & t : tmp) addToCandidates(ind, t, 30);
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

    int getStat()
    {
      return u.getStat();
    }
  };

  inline void process(string smt,
                map<int, string>& varIds,  // obsolete, to remove
                bool memsafety, int norm, bool dat, int cnt, int mut, int prj,
                int serial, int debug, int mem, int to)
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

    MemProcessor ds(m_efac, z3, ruleManager,
                      norm, dat, cnt, mut, prj, varIds, debug, to);

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

    if (ds.invSyn(serial))
    {
      errs() << "sat\n";
      outs() << "Total # of SMT calls: " <<
                (ruleManager.getStat() + ds.getStat()) << "\n";
    }
    else
      errs() << "unknown\n";
  }
}

#endif
