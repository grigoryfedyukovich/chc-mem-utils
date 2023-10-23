#ifndef MEMPROCESSOR__HPP__
#define MEMPROCESSOR__HPP__

#include "PointerAnalysis.hpp"

using namespace std;
using namespace boost;
namespace ufo
{
  class MemProcessor : public PointerAnalysis
  {
    public:
      MemProcessor (ExprFactory &m_efac, EZ3 &z3, CHCs& r, int _norm, bool _dat,
        int _cnt, int _mut, int _prj, map<int, string>& v, int debug, int to) :
      PointerAnalysis (m_efac, z3, r, debug, to, v),
        norm(_norm), dat(_dat), cnts(_cnt), mutNum(_mut), prj(_prj) {}

    ExprSet offsetsSizes;
    bool dat;
    int norm, cnts, mutNum, prj;
    map<Expr, ExprSet> sizes, counters, negCounters, upbounds, lobounds;
    map<int, ExprVector> candidatesNext;
    map<int, map<int, ExprSet>> origs;

    void addToCandidates(int ind, Expr e, int debugMarker, bool split = true)
    {
      Expr rel = decls[ind];
      if (!hasOnlyVars(e, ruleManager.invVars[rel])) return;

      if (!containsOp<FAPP>(e)) return;

      e = simplifyBool(simplifyBV(e));
      if (find(candidates[ind].begin(), candidates[ind].end(), e) !=
               candidates[ind].end()) return;

      if (!containsOp<FAPP>(e)) return;
      if (split)
      {
        ExprSet factored;
        getConj(factor(e, prj), factored);
        if (factored.size() > 1)
        {
          for (auto & f : factored)
          {
            if (!containsOp<FAPP>(f)) continue;
            ExprSet dsjs;
            getDisj (f, dsjs);
            if (dsjs.size() < 3)
              addToCandidates(ind, f, debugMarker, false);
          }
          return;
        }
      }
      if (isOpX<OR>(e) && e->arity() > 3 /* to amend */) return;

      Expr lms = conjoin(sfs[ind].back().learnedExprs, m_efac);
      if (u.implies(lms, e)) return;

      candidates[ind].push_back(e);
      origs[ind][debugMarker].insert(e);

      if (printLog >= 2)
        outs () << "adding to candidates: " << rel << " / "
                << debugMarker << ": " << e << "\n";
    }

    void countersAnalysis()
    {
      map<Expr, set<pair<Expr, Expr>>> cntbnds, ncntbnds;
      map<Expr, map<Expr,ExprSet>> ccands;

      for(auto & c : ruleManager.chcs)
      {
        if (!c.isInductive) continue;

        auto rel = c.srcRelation;
        auto ind = getVarIndex(rel, decls);
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

        if (cnts < 3) continue;

        ExprSet bds;
        getConj(strbody, bds);
        for (auto & cnt : counters[rel])
          for (auto & b : bds)
            if ((isOpX<BULT>(b) || isOpX<BULE>(b)) && contains(b->left(), cnt))
              cntbnds[rel].insert({cnt, b});
            else if (isOpX<BUGT>(b) && contains(b->right(), cnt))
              cntbnds[rel].insert({cnt, mk<BULT>(b->right(), b->left())});
            else if (isOpX<BUGE>(b) && contains(b->right(), cnt))
              cntbnds[rel].insert({cnt, mk<BULE>(b->right(), b->left())});
             else if (contains (b, cnt) && emptyIntersect(b, c.dstVars))
              ccands[rel][cnt].insert(b);

        for (auto & cnt : negCounters[rel])
          for (auto & b : bds)
            if ((isOpX<BUGT>(b) || isOpX<BUGE>(b)) && contains(b->left(), cnt))
              ncntbnds[rel].insert({cnt, b});
            else if (isOpX<BULT>(b) && contains(b->right(), cnt))
              ncntbnds[rel].insert({cnt, mk<BUGT>(b->right(), b->left())});
            else if (isOpX<BULE>(b) && contains(b->right(), cnt))
              ncntbnds[rel].insert({cnt, mk<BUGE>(b->right(), b->left())});
      }

      if (cnts < 3) return;

      map<Expr, ExprSet> allCounters;
      for (auto & rel : decls)
      {
        allCounters[rel] = counters[rel];
        allCounters[rel].insert(
            negCounters[rel].begin(), negCounters[rel].end());
      }

      for (auto & c : ruleManager.chcs)
      {
        if (c.isInductive || c.isQuery) continue;
        auto rel = c.dstRelation;
        auto & vars = ruleManager.invVars[rel];
        auto & varsP = ruleManager.invVarsPrime[rel];

        ExprSet bds;
        getConj(replaceAll(c.body, varsP, vars), bds);

        map<Expr, ExprSet> init;
        for (auto & cnt : allCounters[rel])
        {
          ExprSet eqs;
          getTransitiveEqualities(bds, cnt, eqs);
          for (auto & e : eqs)
            if (e != cnt)
              init[cnt].insert(e);
        }

        auto ind = getVarIndex(rel, decls);
        set<pair<Expr, pair<Expr, Expr>>> candBlocks;
        for (auto & u : cntbnds[rel])
        {
          auto cnt = u.first;
          for (auto & in : init[cnt])
          {
            auto cand = mk<BULE>(u.second->left(),
              isOpX<BULT>(u.second) ? u.second->right() :
              mk<BADD>(u.second->right(),
                bvnum(mkMPZ(1, m_efac), typeOf(u.second->right()))));
            auto cond = replaceAll(u.second, cnt, in);
            candBlocks.insert({cond, {cand, mk<EQ>(cnt, in)}});
          }
        }

        for (auto & u : ncntbnds[rel])
        {
          auto cnt = u.first;
          for (auto & in : init[cnt])
          {
            auto cand = mk<BUGE>(u.second->left(),
              isOpX<BUGT>(u.second) ? u.second->right() :
              mk<BSUB>(u.second->right(),
                bvnum(mkMPZ(1, m_efac), typeOf(u.second->right()))));
            auto cond = replaceAll(u.second, cnt, in);
            candBlocks.insert({cond, {cand, mk<EQ>(cnt, in)}});
          }
        }

        for (auto & b : candBlocks)
        {
          Expr f = b.first;            // loop condition
          Expr c = b.second.first;     // candidate
          Expr g = b.second.second;    // init assignment
          addToCandidates(ind, c, 763);
          addToCandidates(ind, mk<IMPL>(f, c), 764);
          addToCandidates(ind, mk<IMPL>(mkNeg(f), g), 765);

          if (isOpX<BULE>(c) && isOpX<BULT>(f) && c->right() == f->right())
          {
            Expr r = mk<BULT>(f->left(), c->left());
            for (auto & a : ccands[rel][c->left()])
              addToCandidates(ind, mk<IMPL>(r,
                replaceAll(a, c->left(), f->left())), 767);
          }
        }
      }
    }

    void simplifyCHCs()
    {
      if (printLog) outs() << "  Lemma-based CHC simplification\n";
      for (auto & c : ruleManager.chcs)
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

        b = simpleQE(b, c.locVars);
        b = fixpointRewrite(b,
          [](Expr e){return simplifyArr(
                            simpConstArr(
                            simplifyBV(
                            simplifyBool(e))));});
        b = unitPropagate(b);
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

      ruleManager.prepTransitVars();
    }

    void preproAdder(Expr a, ExprVector& vars, ExprVector& varsp,
                     int ind, int debugMarker)
    {
      auto b = simplifyArr(a);
      b = rewriteSelectStore(b);
      b = simplifyBV(b);
      b = simplifyBool(b);
      b = keepQuantifiersRepl(b, varsp, true);
      ExprSet cnjs;
      getConj(factor(b, prj), cnjs);
      if (isOpX<EQ>(a) && isArray(a->left())) cnjs.insert(a); // for arrays and further simplification

      for (auto f : cnjs)
      {
        f = projectOnlyVars(f, varsp);
        if (vars != varsp) f = replaceAll(f, varsp, vars);
        addToCandidates(ind, f, debugMarker, false);
        if (!isOpX<EQ>(f)) continue;
        const pair<Expr, Expr> subs[] = {{f->left(), f->right()},
                                         {f->right(), f->left()}};
        for (auto a : subs)
          if (isOpX<SELECT>(a.first))
            if (contains(allArrVars, a.first->left()) && isZero(a.second))
            {
              if (printLog)
                outs () << "  storing zero at " << a.first->right() << "\n";
              upbounds[decls[ind]].insert(a.first->right());
            }
      }
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
          for (auto it2 = it1; it2 != (prj > 1 ? v.end() : std::next(it1)); it2++)
          {
            for (auto it3 = it2; it3 != (prj > 2 ? v.end() : std::next(it2)); it3++)
            {
              for (auto it4 = it3; it4 != (prj > 3 ? v.end() : std::next(it3)); it4++)
              {
                ExprSet vars = {*it1, *it2, *it3, *it4};
                bodyProjs.insert(projectOnlyVars(a, vars));
              }
            }
          }
        }
      }

      bodyCnjs.insert(bodyProjs.begin(), bodyProjs.end());

      for (auto a : bodyCnjs)
        preproAdder(a, ruleManager.invVars[c.dstRelation],
                       ruleManager.invVarsPrime[c.dstRelation], indDst, 0);
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

      ExprMap repl;
      for (auto & t : ruleManager.transit[c.id])
        repl[t.second] = t.first;

      for (auto a : bodyCnjs)
      {
        preproAdder(
          replaceAllRev(a, repl),
          ruleManager.invVars[c.dstRelation],
                       ruleManager.invVarsPrime[c.dstRelation], indDst, 2);

        preproAdder(
          replaceAll(a, repl),
          ruleManager.invVars[c.srcRelation],
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
            auto b = rewriteSelectStore(a);
            b = simplifyBool(simplifyBV(b));
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
      for (int ind = 0; ind < decls.size(); ind++)
      {
        Expr d = decls[ind];

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

        if (sizMap[d] >= 0)
        {
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

    void shrinkDecls()
    {
      auto sit = sfs.begin();
      auto vit1 = invarVars.begin();
      auto vit2 = invarVarsShort.begin();
      for (auto it = decls.begin(); it != decls.end();)
      {
        bool found = false;
        for (auto decl : ruleManager.decls)
          if (decl->left() == *it)
            found = true;
        if (found)
        {
          ++it; ++sit; ++vit1; ++vit2;
        }
        else
        {
          it = decls.erase(it);
          sit = sfs.erase(sit);
          vit1 = invarVars.erase(vit1);
          vit2 = invarVarsShort.erase(vit2);
        }
      }
    }

    tribool invSyn(int ser, int b = 0)
    {
      candidates.clear();

      if (b == 0)
      {
        for (int ind = 0; ind < decls.size(); ind++)
        {
          Expr decl = decls[ind];
          auto ov = ruleManager.invVars[decl][offMap[decl]];
          auto z = bvnum(mkMPZ(0, m_efac), typeOf(ov)->last());
          for (auto & al : aliasesRev[decl])
            for (auto & a : al.second)
              addToCandidates(ind, mk<EQ>(mk<SELECT>(ov, al.first), z), 105);
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

      for (int i = 0; i < decls.size() - 1; i++)
      {
        for (auto & c : ruleManager.chcs)
          if (!c.isQuery && !c.isFact && !c.isInductive)
            processProp(c);
      }

      auto tmp = candidates;
      bool toGiveUp = true;
      for (int i = 0; i < mutNum; i++)
      {
        mutateCandidates(i);
        for (int j = 0; j < 2; j++)
        {
          if (j > 0) candidates = candidatesNext;
          removeDupCandidates();
          auto res = multiHoudini(ruleManager.allCHCs);
          if (res) res = assignPrioritiesForLearned();
          if (res)
          {
            toGiveUp = false;
            if (printLog) printStats();
            if (ser == 0 && checkAllLemmas())
              return true;
          }
        }
        candidates = tmp;
      }
      if (toGiveUp)
      {
        errs () << "give up after " << b << " iters\n";
        return indeterminate;
      }
      return invSyn(ser, b + 1);
    }

    void printStats()
    {
      set<int> all, used;
      for (int ind = 0; ind < decls.size(); ind++)
      {
        Expr d = decls[ind];
        for (auto & b : origs[ind])
        {
          all.insert(b.first);
          for (auto & l : sfs[ind].back().learnedExprs)
            if (find(b.second.begin(), b.second.end(), l) != b.second.end())
              used.insert(b.first);
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

  inline void process(string smt,
                map<int, string>& varIds,  // obsolete, to remove
                bool memsafety, int norm, bool dat, int cnt, int mut, int prj,
                int serial, int debug, int mem, bool opt, int to)
  {
    ExprFactory m_efac;
    EZ3 z3(m_efac);

    CHCs ruleManager(m_efac, z3, memsafety, debug - 2);
    ruleManager.parse(smt, mem, opt);

    if (debug > 0)
    {
      outs () << "Original CHCs:\n";
      outs () << "(size: " << ruleManager.chcs.size() << ", "
                           << ruleManager.decls.size() << ")\n";
      ruleManager.print(false, (debug > 2), "chc_orig");
      if (serial) ruleManager.serialize("chc_orig");
    }

    if (!ruleManager.hasBV) return;

    if (debug)
      outs () << "Queries detected: " << ruleManager.getQum() << "\n";

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

    mem &= ds.initAlloca();
    ds.simplifyCHCstruct();
    for (auto dcl : ruleManager.decls) ds.initializeDecl(dcl->left());

    if (debug > 0)
    {
      outs () << "Simplifying CHCs (in progress):\n";
      ruleManager.print((debug > 3), (debug > 2), "chc_interm");
    }

    if (debug)
      outs () << "Preprocess SMT calls: " << ruleManager.getStat() << "\n";

    if (mem)
    {
      ds.computeStableVars();
      ds.getInitAllocaVals();
      ds.getAllocaVals();
      ds.getAliases();
      ds.printAliases();
      ds.addAliasVars();
      ds.rewriteMem();
      auto res = ruleManager.doElim(false);
      if (debug > 0)
      {
        outs() << "\nCHCs after memory-rewriting and simplification:\n";
        ruleManager.print((debug > 3), false);
      }
      if (res) ds.shrinkDecls();
      else
      {
        errs() << "unsat\n";
        return;
      }
    }

    if (debug)
      outs () << "CHC size: " << ruleManager.chcs.size() << ", "
                              << ruleManager.decls.size() << "\n";

    if (debug)
      outs() << "SMT calls for alias analysis: " <<
              (ruleManager.getStat() + ds.getStat()) << "\n";
    outs().flush();

    if (ds.invSyn(serial, !mem))
    {
      ds.printSolution(false);
      errs() << "sat\n";
      outs() << "Total SMT calls: " <<
                (ruleManager.getStat() + ds.getStat()) << "\n";
    }
    else
      errs() << "unknown\n";
  }
}

#endif
