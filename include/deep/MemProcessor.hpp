#ifndef MEMPROCESSOR__HPP__
#define MEMPROCESSOR__HPP__

#include "RndLearnerV3.hpp"

using namespace std;
using namespace boost;
namespace ufo
{
  class MemProcessor : public RndLearnerV3
  {
    public:

    MemProcessor (ExprFactory &efac, EZ3 &z3, CHCs& r, int debug) :
      RndLearnerV3 (efac, z3, r, 1000, false, false, 0, 0, false, 0, false,
                          false, false, false, 0, false, false, 1000, debug) {}

    Expr lastDecl = NULL;
    bool multiHoudini (vector<HornRuleExt*> worklist, bool recur = true,
      bool fastExit = false)
    {
      if (printLog >= 3) outs () << "MultiHoudini\n";
      if (printLog >= 4) printCands();

      bool res1 = true;
      for (auto &hr: worklist)
      {
        if (printLog >= 3) outs () << "  Doing CHC check (" << hr->srcRelation << " -> "
                                   << hr->dstRelation << ")\n";
        if (hr->isQuery) continue;
        tribool b = checkCHC(*hr, candidates);
        if (b || indeterminate(b))
        {
          if (printLog >= 3) outs () << "    CHC check failed\n";
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
          }

          // first try to remove candidates immediately by their models (i.e., vals)
          // then, invalidate (one-by-one) all candidates for which Z3 failed to find a model

          for (int i = 0; i < 3 /*weakeningPriorities.size() */; i++)
            if (weaken (invNum, candidates[invNum], vals, hr, sf, (*weakeningPriorities[i]), i > 0))
              break;

          if (recur)
          {
            res1 = false;
            break;
          }
        }
        else if (printLog >= 3) outs () << "    CHC check succeeded\n";
      }
      if (fastExit) return true;
      if (!recur) return false;
      if (res1) return anyProgress(worklist);
      else return multiHoudini(worklist);
    }

    map<Expr, int> allocMap, memMap;
    map<Expr, ExprSet> allocVals;
    map<Expr, map<Expr, ExprSet>> aliases, aliasesRev;
    map<Expr, ExprMap> arrVars, arrVarsPr;
    Expr memTy, allocTy, defAlloc;

    void initAlloca()
    {
      memTy = NULL;
      for (auto &d : ruleManager.decls)
      {
        memMap[d->left()] = -1;
        allocMap[d->left()] = -1;
        if (printLog >= 2) outs () << "initAlloca, decl: " << d->left() << "\n";
        auto & vs = ruleManager.invVars[d->left()];
        for (int i = 0; i < vs.size(); i++)
        {
          auto & v = vs[i];
          if (isArray(v))
          {
            if (isOpX<BVSORT>(typeOf(v)->left()) &&
                isOpX<BVSORT>(typeOf(v)->right()))
            {
              if (printLog >= 2) outs () << "  alloca arg # "  << i << ": " << vs[i] << "   " << typeOf(vs[i]) << "\n";
              assert(allocMap[d->left()] == -1);
              allocMap[d->left()] = i;
            }
            else if (isOpX<BVSORT>(typeOf(v)->left()) &&
                     isOpX<ARRAY_TY>(typeOf(v)->right()))
            {
              if (printLog >= 2) outs () << "  mem arg # "  << i << ": " << vs[i] << "   " << typeOf(vs[i]) << "\n";
              assert(memMap[d->left()] == -1);
              memMap[d->left()] = i;
            }
          }
        }
        assert(allocMap[d->left()] != -1);
        assert(memMap[d->left()] != -1);
        Expr ty = typeOf(ruleManager.invVars[d->left()][memMap[d->left()]]);
        defAlloc = mk<CONST_ARRAY>(ty->left(), bvnum(mkMPZ(0, m_efac), ty->left()));
        if (memTy == NULL) memTy = ty;
        else assert (memTy == ty);
        if (allocTy == NULL) allocTy = ty->right();
        else assert (allocTy == ty->right());
      }
    }

    void getInitAllocaVals()
    {
      // get init vals, from fact
      for (auto & r : ruleManager.chcs)
      {
        if (r.isFact)
        {
          auto & v = r.dstVars[allocMap[r.dstRelation]];
          assert (isArray(v) && isOpX<BVSORT>(typeOf(v)->right()));

          if (printLog >= 2) outs () << "getInitAllocaVals, arr var (1):  " << v << "\n";
          auto v1 = mkConst(mkTerm<string> ("key", m_efac), typeOf(v)->left());
          auto v2 = mkConst(mkTerm<string> ("val", m_efac), typeOf(v)->right());
          ExprSet cnjs = {r.body, mk<EQ>(mk<SELECT>(v, v1), v2)};
          while (true)
          {
            if (!u.isSat(cnjs)) break;
            auto m = u.getModel(v2);
            cnjs.insert(mk<NEQ>(v2, m));
            if (lexical_cast<string>(toMpz(m)) != "0")
            {
              allocVals[r.dstRelation].insert(m);
              aliases[r.dstRelation][m].insert(u.getModel(v1));
              aliasesRev[r.dstRelation][u.getModel(v1)].insert(m);
              if (printLog >= 2) outs () << "    model:  " << toMpz(m) <<
                        " <- " << toMpz(u.getModel(v1)) << "\n";
            }
          }
        }
      }
    }

    void getAllocaVals()
    {
      // get init cands, from vals
      for (auto & d : ruleManager.decls)
      {
        auto decl = d->left();
        if (printLog >= 2) outs () << "getAllocaVals, decl " << d->left()
                                << ", ind = "<< getVarIndex(decl, decls)<< "\n";
        // if (allocVals[decl].empty()) continue;
        auto & v = ruleManager.invVars[decl][allocMap[decl]];
        if (printLog >= 2) outs () << "  v = " << v << ", " << typeOf(v) << "\n";
        allocVals[decl].insert(bvnum(mkMPZ(0, m_efac), typeOf(v)->left()));
        auto key = mkConst(mkTerm<string> ("key", m_efac), typeOf(v)->left());
        ExprSet dsjs;
        for (auto & val : allocVals[decl])
          dsjs.insert(mk<EQ>(mk<SELECT>(v, key), val));

        candidates[getVarIndex(decl, decls)] = { mknary<FORALL>(
                            ExprVector({key->left(), disjoin(dsjs, m_efac)})) };

        if (printLog >= 2) outs () << "  cands:\n";
        if (printLog >= 2) pprint(candidates[getVarIndex(decl, decls)], 4);
      }
      if (multiHoudini(ruleManager.dwtoCHCs, false, true))
      {
        if (printLog >= 2) outs () << "ALL found\n";
      }
      else
      {
        if (printLog >= 2) outs () << "fail for " << lastDecl << "\n";
        Expr mdl = u.getModel(ruleManager.invVarsPrime[lastDecl][allocMap[lastDecl]]);
        ExprVector st;
        filter (mdl, IsStore (), inserter(st, st.begin()));
        if (printLog >= 2) outs () << "   mdl:  " << mdl << "\n";
        for (auto & m : st)
          if (is_bvnum(m->last()))
          {
            if (printLog >= 2) outs () << "   adding val " << m->last() << "\n";
            allocVals[lastDecl].insert(m->last());
          }
        getAllocaVals();
      }
    }

    void getAliases(Expr ptr)
    {
      for (auto & d : ruleManager.decls)
      {
        auto decl = d->left();
        if (printLog >= 2) outs () << "getAliases, decl " << d->left()
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
        if (printLog >= 2) outs () << "  decl: " << decl << ", " << ptr << "\ncands:\n";
        if (printLog >= 2) pprint(candidates[getVarIndex(decl, decls)]);
      }
      if (multiHoudini(ruleManager.dwtoCHCs, false, true))
      {
        if (printLog >= 2) outs () << "\nALL found\n";
      }
      else
      {
        if (printLog >= 2) outs () << "fail for " << lastDecl << "\n";
        Expr mdl = u.getModel(ruleManager.invVarsPrime[lastDecl][allocMap[lastDecl]]);
        if (printLog >= 2) outs () << "   mdl:  " << mdl << "\n";
        ExprVector st;
        filter (mdl, IsStore (), inserter(st, st.begin()));
        for (auto & m : st)
          if (is_bvnum(m->right()) && m->last() == ptr)
          {
            if (printLog >= 2) outs () << "   adding val " << m->right() << "\n";
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
        if (printLog >= 2) outs () << "  >> decl >>  " << d->left() << ":\n";
        for (auto & al : aliases[d->left()])
        {
          for (auto & a : al.second)
          {
            if (printLog >= 2) outs () << "    " << a << "  ->  " << al.first << "\n";
          }
        }
      }
    }

    void addAliasVars()
    {
      for (auto & d : ruleManager.decls)
      {
        for (auto & al : aliases[d->left()])
        {
          auto a = mkConst(mkTerm<string> ("_new_ar_" +
              lexical_cast<string>(toMpz(al.first)), m_efac),
                memTy->right());
          auto b = mkConst(mkTerm<string> ("_new_ar_" +
              lexical_cast<string>(toMpz(al.first)) + "'", m_efac),
                memTy->right());
          arrVars[d->left()][al.first] = a;
          arrVarsPr[d->left()][al.first] = b;
        }
      }
    }

    Expr getUpdLiteral(Expr body, Expr ty)
    {
      ExprSet tmp;
      getConj(body, tmp);
      Expr upd = NULL;
      for (auto c : tmp)
      {
        if (isOpX<EQ>(c) && typeOf(c->left()) == ty)
        {
          assert(upd == NULL && "cannot have several updates of this type");
          upd = c;
        }
      }
      return upd;
    }

    void rewriteMem()
    {
      for (auto &r : ruleManager.chcs)
      {
        if (printLog >= 2) outs () << "-----------   processing CHC:   " << r.srcRelation
                                            << " -> " << r.dstRelation << "\n";
        Expr body = r.body;
        Expr allocUpd = getUpdLiteral(body, allocTy);
        if (r.isInductive && allocMap[r.dstRelation] != -1)
        {
          ExprVector toElim = {r.dstVars[allocMap[r.dstRelation]]};
          body = ufo::eliminateQuantifiers(body, toElim);
          if (allocUpd != NULL) body = mk<AND>(allocUpd, body);
        }
        if (r.isFact && !contains(body, memTy))
        {
          for (auto & v : arrVars[r.srcRelation])
            r.srcVars.push_back(v.second);
          for (auto & v : arrVarsPr[r.dstRelation])
            r.dstVars.push_back(v.second);
          continue;
        }
        if (printLog >= 2) pprint(body, 3);
        Expr rel, alloSrc, memDst = NULL;
        ExprSet memVars;
        map<Expr, ExprSet> & ali = r.isFact ? aliases[r.dstRelation] :
                                              aliases[r.srcRelation];
        if (r.isFact)
        {
          rel = r.dstRelation;
          alloSrc = r.dstVars[allocMap[rel]];
          for (auto & v : r.locVars)
            if (typeOf(v) == memTy)
              memVars.insert(v);
        }
        else
        {
          rel = r.srcRelation;
          alloSrc = r.srcVars[allocMap[rel]];
        }

        if (!r.isQuery)
        {
          memDst = r.dstVars[memMap[r.dstRelation]];
          memVars.insert(memDst);
        }
        if (!r.isFact)
          memVars.insert(r.srcVars[memMap[r.srcRelation]]);

        // step 0: find alloc-selects, replace them by respective vals (need to be an invar)
        vector<ExprMap> aliasCombs;
        ExprVector se;
        ExprSet seSimp;
        filter (body, IsSelect (), inserter(se, se.begin()));
        for (auto s : se) seSimp.insert(simplifyArr(s));

        for (auto s : seSimp)
        {
          if (s->left() == alloSrc)
          {
            assert(is_bvnum(s->last()) && "possibly, an encoding issue");

            if (aliasCombs.empty())
            {
              for (auto & a : aliasesRev[rel][s->last()])
              {
                ExprMap tmp;
                tmp[s] = a;
                aliasCombs.push_back(tmp);
                if (printLog >= 2) outs () << "alias:  " << s->last()
                                           << " -> " << a << "\n";
              }
            }
            else
            {
              vector<ExprMap> aliasCombsTmp = aliasCombs;
              aliasCombs.clear();
              for (auto & a : aliasesRev[rel][s->last()])
              {
                for (auto & m : aliasCombsTmp)
                {
                  ExprMap tmp = m;
                  if (printLog >= 2) outs () << "alias:  " << s->last() << " -> " << a << "\n";
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
        for (auto & m : aliasCombs)
        {
          Expr newBody = body;
          ExprVector newConstrs;
          for (auto & v : m) newConstrs.push_back(mk<EQ>(v.first, v.second));
          if (printLog >= 2) outs () << "  >>>>>>>>>>  comb " << (it++) << "  >>>>>>>>>>>>>>\n";
          newBody = replaceAll(newBody, m);

          // step 1:
          int iter = 0;
          map<Expr, pair<int, Expr>> numUpds;
          while (true)
          {
            for (auto & al : ali)
            {
              Expr num = al.first;
              for (auto & me : memVars)
              {
                if (me == NULL) continue;
                Expr replTo = (me == memDst     ? arrVarsPr[rel][num] :
                       (numUpds[num].first == 0 ? arrVars[rel][num] :
                                                  numUpds[num].second));
                if (printLog >= 2) outs () << "rewriting " << mk<SELECT>(me, num)
                                           << " with " << replTo << "\n";
                newBody = replaceAll(newBody, mk<SELECT>(me, num), replTo);
              }
            }

            if (printLog >= 2) outs () << "  body cnj (1), iter " << iter++ << "\n  ";
            if (printLog >= 2) pprint(newBody);
            if (printLog >= 2) outs() << "\n";
            ExprVector st;
            filter (newBody, IsStore (), inserter(st, st.begin()));

            Expr newDef = NULL;
            for (auto s : st)
            {
              s = simplifyArr(s);
              if (!(typeOf(s->left()) == memTy && IsConst() (s->left()))) continue;
              Expr num = s->right();
              if (!is_bvnum(num)) continue;

              Expr tmp = s->last();
              for (auto & al : ali)
              {
                Expr num = al.first;
                for (auto & me : memVars)
                {
                  if (me == NULL) continue;
                  Expr replTo = (me == memDst     ? arrVarsPr[rel][num] :
                         (numUpds[num].first == 0 ? arrVars[rel][num] :
                                                    numUpds[num].second));
                  if (printLog >= 2) outs () << "rewriting " << mk<SELECT>(me, num)
                                             << " with " << replTo << "\n";
                  tmp = replaceAll(tmp, mk<SELECT>(me, num), replTo);
                }
              }

              newDef = s;
              numUpds[num].first++;

              if (printLog >= 2) outs () << "     found store:   ";
              if (printLog >= 2) pprint(newDef);
              if (printLog >= 2) outs () << "\n";

              numUpds[num].second = mkConst(mkTerm<string> ("_new_ar_" +
                  lexical_cast<string>(toMpz(num)) + "_" +
                      to_string(numUpds[num].first), m_efac), memTy->right());

              newConstrs.push_back(mk<EQ>(numUpds[num].second, tmp));
              if (printLog >= 2) outs () << "   new restr:  ";
              if (printLog >= 2) pprint(newConstrs.back());
              if (printLog >= 2) outs ()  << "\n";
              break;
            }
            if (newDef == NULL)
            {
              if (printLog >= 2) outs () << "  no store found\n";
              break;
            }
            newBody = replaceAll(newBody, newDef, newDef->left());
            newBody = simplifyArr(newBody);
          }

          Expr memUpd = getUpdLiteral(newBody, memTy);

          if (memUpd == NULL)
            newConstrs.push_back(newBody);
          else
            newConstrs.push_back(replaceAll(newBody, memUpd, mk<TRUE>(m_efac)));

          // binding src/dst vars
          for (auto & a : arrVars[r.dstRelation])
          {
            Expr num = a.first;
            if (numUpds[num].first == 0)
              newConstrs.push_back(mk<EQ>(arrVars[r.dstRelation][num],
                                          arrVarsPr[r.dstRelation][num]));
            else
              for(auto & c : newConstrs)
                c = replaceAll(c, numUpds[num].second, arrVarsPr[r.dstRelation][num]);
          }

          newBodies.push_back(conjoin(newConstrs, m_efac));
        }
        if (!aliasCombs.empty())
        {
          r.body = disjoin(newBodies, m_efac);
          if (printLog >= 2) pprint(r.body,4);
          if (printLog >= 2) outs () << "\n\n   " << memTy << "\n";

          // sanity check after repl:
          assert(!contains(r.body, memTy));
        }

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
        for (auto & v : arrVars[d->left()])
          ruleManager.invVars[d->left()].push_back(v.second);
        for (auto & v : arrVarsPr[d->left()])
          ruleManager.invVarsPrime[d->left()].push_back(v.second);
      }
      ruleManager.serialize();
    }
  };

  inline void process(string smt, int debug)
  {
    ExprFactory m_efac;
    EZ3 z3(m_efac);
    SMTUtils u(m_efac);

    CHCs ruleManager(m_efac, z3, debug - 2);
    auto res = ruleManager.parse(smt, true, false);
    if (!res) return;

    if (ruleManager.hasBV)
    {
      MemProcessor ds(m_efac, z3, ruleManager, debug);
      for (int i = 0; i < ruleManager.cycles.size(); i++)
      {
        Expr dcl = ruleManager.chcs[ruleManager.cycles[i][0]].srcRelation;
        if (ds.initializedDecl(dcl)) continue;
        ds.initializeDecl(dcl);
      }
      ds.initAlloca();
      ds.getInitAllocaVals();
      ds.getAllocaVals();
      ds.getAliases();
      ds.printAliases();
      ds.addAliasVars();
      ds.rewriteMem();
    }
  }
}

#endif
