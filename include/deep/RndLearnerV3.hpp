#ifndef RNDLEARNERV3__HPP__
#define RNDLEARNERV3__HPP__

#include "RndLearner.hpp"

#ifdef HAVE_ARMADILLO
#include "DataLearner.hpp"
#endif

using namespace std;
using namespace boost;
namespace ufo
{
  static bool (*weakeningPriorities[])(Expr, tribool) =
  {
    [](Expr cand, tribool val) { return bool(!val); },
    [](Expr cand, tribool val) { return indeterminate(val) &&
          (containsOp<EXISTS>(cand) || containsOp<FORALL>(cand)); },
    [](Expr cand, tribool val) { return indeterminate(val); }
  };

  class RndLearnerV3 : public RndLearner
  {
    protected:

    ExprSet checked;
    set<HornRuleExt*> propped;
    map<int, deque<Expr>> deferredCandidates;
    map<int, ExprSet> allDataCands;
    map<int, ExprSet> tmpFailed;
    map<int, ExprTree> mbpDt, strenDt;
    bool boot = true;
    int mut;
    int dat;
    int to;

    // extra options for disjunctive invariants
    bool dDisj;
    bool dAllMbp;
    bool dAddProp;
    bool dAddDat;
    bool dStrenMbp;
    bool dRecycleCands;
    bool dGenerous;
    int dFwd;
    int mbpEqs;

    map<int, Expr> postconds;
    map<int, Expr> ssas;
    map<int, Expr> prefs;
    map<int, ExprSet> mbps;

    map<int, ExprVector> candidates;

    public:

    RndLearnerV3 (ExprFactory &efac, EZ3 &z3, CHCs& r, unsigned to, bool freqs, bool aggp,
                  int _mu, int _da, bool _d, int _m, bool _dAllMbp, bool _dAddProp,
                  bool _dAddDat, bool _dStrenMbp, int _dFwd, bool _dR, bool _dG, int _to, int debug) :
      RndLearner (efac, z3, r, to, false, freqs, true, aggp, debug),
                  mut(_mu), dat(_da), dDisj(_d), mbpEqs(_m), dAllMbp(_dAllMbp),
                  dAddProp(_dAddProp), dAddDat(_dAddDat), dStrenMbp(_dStrenMbp),
                  dFwd(_dFwd), dRecycleCands(_dR), dGenerous(_dG), to(_to) {}


    bool addCandidate(int invNum, Expr cnd)
    {
      if (printLog >= 3) outs () << "Adding candidate [" << invNum << "]: " << cnd << "\n";
      SamplFactory& sf = sfs[invNum].back();
      Expr allLemmas = sf.getAllLemmas();
      if (containsOp<FORALL>(cnd) || containsOp<FORALL>(allLemmas))
      {
        candidates[invNum].push_back(cnd);
        return true;
      }
      if (!isOpX<TRUE>(allLemmas) && u.implies(allLemmas, cnd)) return false;

      for (auto & a : candidates[invNum])
        if (a == cnd) return false;
      candidates[invNum].push_back(cnd);
      return true;
    }

    void addCandidates(Expr rel, ExprSet& cands)
    {
      int invNum = getVarIndex(rel, decls);
      for (auto & a : cands) addCandidate(invNum, a);
    }

    bool addLemma (int invNum, SamplFactory& sf, Expr l)
    {
      if (printLog)
        outs () << "Added lemma for " << decls[invNum] << ": " << l
                << (printLog >= 2 ? " 👍\n" : "\n");
      sf.learnedExprs.insert(l);
      return true;
    }

    bool filterUnsat()
    {
      vector<HornRuleExt*> worklist;
      for (int i = 0; i < invNumber; i++)
      {
        if (!u.isSat(candidates[i]))
        {
          for (auto & hr : ruleManager.chcs)
          {
            if (hr.dstRelation == decls[i]) worklist.push_back(&hr);
          }
        }
      }

      // basically, just checks initiation and immediately removes bad candidates
      multiHoudini(worklist, false);

      for (int i = 0; i < invNumber; i++)
      {
        if (!u.isSat(candidates[i]))
        {
          ExprVector cur;
          deque<Expr> def;
          u.splitUnsatSets(candidates[i], cur, def);
          deferredCandidates[i] = def;
          candidates[i] = cur;
        }
      }

      return true;
    }

    bool weaken (int invNum, ExprVector& ev,
                 map<Expr, tribool>& vals, HornRuleExt* hr, SamplFactory& sf,
                 function<bool(Expr, tribool)> cond, bool singleCand)
    {
      bool weakened = false;
      for (auto it = ev.begin(); it != ev.end(); )
      {
        if (cond(*it, vals[*it]))
        {
          weakened = true;
          if (printLog >= 2)
          {
            outs () << "    Failed cand for " << hr->dstRelation << ": " << *it << " 🔥\n";
          }

          if (hr->isFact && !containsOp<ARRAY_TY>(*it) && !containsOp<BOOL_TY>(*it) && !findNonlin(*it))
          {
            Expr failedCand = normalizeDisj(*it, invarVarsShort[invNum]);
            if (statsInitialized)
            {
              Sampl& s = sf.exprToSampl(failedCand);
              sf.assignPrioritiesForFailed();
            }
            else tmpFailed[invNum].insert(failedCand);
          }
          if (boot)
          {
            if (isOpX<EQ>(*it)) deferredCandidates[invNum].push_back(*it);  //  prioritize equalities
            else
            {
              ExprSet disjs;
              getDisj(*it, disjs);
              for (auto & a : disjs)  // to try in the `--disj` mode
              {
                deferredCandidates[invNum].push_front(a);
                deferredCandidates[invNum].push_front(mkNeg(a));
              }
            }
          }
          it = ev.erase(it);
          if (singleCand) break;
        }
        else
        {
          ++it;
        }
      }
      return weakened;
    }

    bool multiHoudini (vector<HornRuleExt*> worklist, bool recur = true)
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
      if (!recur) return false;
      if (res1) return anyProgress(worklist);
      else return multiHoudini(worklist);
    }

    bool anyProgress(vector<HornRuleExt*> worklist)
    {
      for (int i = 0; i < invNumber; i++)
        // subsumption check
        for (auto & hr : worklist)
          if (hr->srcRelation == decls[i] || hr->dstRelation == decls[i])
            if (candidates[i].size() > 0)
              if (!u.implies (conjoin(sfs[i].back().learnedExprs, m_efac),
                conjoin(candidates[i], m_efac)))
                  return true;
      return false;
    }

    bool checkAllLemmas()
    {
      candidates.clear();
      for (int i = ruleManager.wtoCHCs.size() - 1; i >= 0; i--)
      {
        auto & hr = *ruleManager.wtoCHCs[i];
        tribool b = checkCHC(hr, candidates, true);
        if (b)
        {
          if (!hr.isQuery)
          {
            if (printLog) outs() << "WARNING: Something went wrong" <<
              (ruleManager.hasArrays[hr.srcRelation] || ruleManager.hasArrays[hr.dstRelation] ?
              " (possibly, due to quantifiers)" : "") <<
              ". Restarting...\n";

            for (int i = 0; i < decls.size(); i++)
            {
              SamplFactory& sf = sfs[i].back();
              for (auto & l : sf.learnedExprs) candidates[i].push_back(l);
              sf.learnedExprs.clear();
            }
            multiHoudini(ruleManager.wtoCHCs);
            assignPrioritiesForLearned();

            return false;
          }
          else
          {
            if (printLog > 2)
            {
              outs () << "SAFETY broken:\n";
              if (u.canGetModel())
              {
                Expr mdl = u.getModel();
                pprint(mdl);
              }
            }
            return false; // TODO: use this fact somehow
          }
        }
        else if (indeterminate(b)) return false;
      }
      return true;
    }

    tribool checkCHC (HornRuleExt& hr, map<int, ExprVector>& annotations, bool checkAll = false)
    {
      int srcNum = getVarIndex(hr.srcRelation, decls);
      int dstNum = getVarIndex(hr.dstRelation, decls);

      if (!hr.isQuery)       // shortcuts
      {
        if (dstNum < 0)
        {
          if (printLog >= 4) outs () << "      Trivially true since "
                        << hr.dstRelation << " is not initialized\n";
          return false;
        }
        if (checkAll && annotations[dstNum].empty())
          return false;
      }

      ExprSet exprs = {hr.body};

      if (!hr.isFact)
      {
        ExprSet lms = sfs[srcNum].back().learnedExprs;
        lms.insert(annotations[srcNum].begin(), annotations[srcNum].end());
        for (auto a : lms)
        {
          for (auto & v : invarVars[srcNum])
            a = replaceAll(a, v.second, hr.srcVars[v.first]);
          exprs.insert(a);
        }
      }

      ExprSet negged;
      if (hr.isQuery)
        negged = {mk<TRUE>(m_efac)};
      else
      {
        ExprSet lms;
        if (checkAll) lms = sfs[dstNum].back().learnedExprs;
        lms.insert(annotations[dstNum].begin(), annotations[dstNum].end());
        for (auto a : lms)
        {
          for (auto & v : invarVars[dstNum])
            a = replaceAll(a, v.second, hr.dstVars[v.first]);
          negged.insert(mkNeg(a));
        }
      }
      return wrapSMT(hr, exprs, negged);
    }

    // could be redefined and extra tricks added
    virtual tribool wrapSMT(HornRuleExt& hr, ExprSet& exprs, ExprSet& negged)
    {
      exprs.insert(disjoin(negged, m_efac));
      return u.isSat(exprs);
    }

    bool assignPrioritiesForLearned()
    {
      bool ret = false;
      for (auto & cand : candidates)
      {
        if (cand.second.size() > 0)
        {
          SamplFactory& sf = sfs[cand.first].back();
          for (auto b : cand.second)
          {
            b = simplifyArithm(b);
            if (!statsInitialized || containsOp<ARRAY_TY>(b)
                    || containsOp<BOOL_TY>(b) || findNonlin(b))
              ret |= addLemma(cand.first, sf, b);
            else
            {
              Expr learnedCand = normalizeDisj(b, invarVarsShort[cand.first]);
              Sampl& s = sf.exprToSampl(learnedCand);
              sf.assignPrioritiesForLearned();
              if (!u.implies(sf.getAllLemmas(), learnedCand))
                ret |= addLemma(cand.first, sf, learnedCand);
            }
          }
        }
      }
      return ret;
    }

    void printCands(bool proof = false)
    {
      for (auto & c : candidates)
        if (c.second.size() > 0)
        {
          outs() << (proof ? "  Invariants for " : "  Candidates for ")
                 << *decls[c.first] << ":\n";
          pprint(c.second, 4);
        }
    }
  };
}

#endif
