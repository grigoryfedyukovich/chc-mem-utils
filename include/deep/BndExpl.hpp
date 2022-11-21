#ifndef BNDEXPL__HPP__
#define BNDEXPL__HPP__

#include "Horn.hpp"
#include "ae/ExprSimplBv.hpp"
#include "ae/AeValSolver.hpp"
#include <limits>

using namespace std;
using namespace boost;
namespace ufo
{
  class BndExpl
  {
    private:

    ExprFactory &m_efac;
    SMTUtils u;
    CHCs& ruleManager;
    Expr extraLemmas;

    ExprVector bindVars1;

    int tr_ind; // helper vars
    int pr_ind;
    int k_ind;

    Expr inv;   // 1-inductive proof

    bool debug;

    public:

    BndExpl (CHCs& r, bool d) :
      m_efac(r.m_efac), ruleManager(r), u(m_efac), debug(d) {}

    BndExpl (CHCs& r, int to, bool d) :
      m_efac(r.m_efac), ruleManager(r), u(m_efac, to), debug(d) {}

    BndExpl (CHCs& r, Expr lms, bool d) :
      m_efac(r.m_efac), ruleManager(r), u(m_efac), extraLemmas(lms), debug(d) {}

    map<Expr, ExprSet> concrInvs;
    set<vector<int>> unsat_prefs;

    bool already_unsat(vector<int>& t)
    {
      bool unsat = false;
      for (auto u : unsat_prefs)
      {
        if (u.size() > t.size()) continue;
        bool found = true;
        for (int j = 0; j < u.size(); j ++)
        {
          if (u[j] != t[j])
          {
            found = false;
            break;
          }
        }
        if (found)
        {
          unsat = true;
          break;
        }
      }
      return unsat;
    }

    void getAllTraces (Expr src, Expr dst, int len, vector<int> trace, vector<vector<int>>& traces)
    {
      if (len == 1)
      {
        for (auto a : ruleManager.outgs[src])
        {
          if (ruleManager.chcs[a].dstRelation == dst &&
              !contains(foundCexes, a))
          {
            vector<int> newtrace = trace;
            newtrace.push_back(a);
            traces.push_back(newtrace);
          }
        }
      }
      else
      {
        if (already_unsat(trace)) return;
        for (auto a : ruleManager.outgs[src])
        {
          vector<int> newtrace = trace;
          newtrace.push_back(a);
          getAllTraces(ruleManager.chcs[a].dstRelation, dst, len-1, newtrace, traces);
        }
      }
    }

    vector<ExprVector> bindVars;

    Expr toExpr(vector<int>& trace)
    {
      ExprVector ssa;
      getSSA(trace, ssa);
      return conjoin(ssa, m_efac);
    }

    void getSSA(vector<int>& trace, ExprVector& ssa)
    {
      ExprVector bindVars2;
      bindVars.clear();
      ExprVector bindVars1 = ruleManager.chcs[trace[0]].srcVars;
      int bindVar_index = 0;
      int locVar_index = 0;

      for (int s = 0; s < trace.size(); s++)
      {
        auto &step = trace[s];
        bindVars2.clear();
        HornRuleExt& hr = ruleManager.chcs[step];
        assert(hr.srcVars.size() == bindVars1.size());

        Expr body = hr.body;
        if (!hr.isFact && extraLemmas != NULL) body = mk<AND>(extraLemmas, body);
        body = replaceAll(body, hr.srcVars, bindVars1);

        for (int i = 0; i < hr.dstVars.size(); i++)
        {
          bool kept = false;
          for (int j = 0; j < hr.srcVars.size(); j++)
          {
            if (hr.dstVars[i] == hr.srcVars[j])
            {
              bindVars2.push_back(bindVars1[i]);
              kept = true;
            }
          }
          if (!kept)
          {
            Expr new_name = mkTerm<string> ("__bnd_var_" + to_string(bindVar_index++), m_efac);
            bindVars2.push_back(cloneVar(hr.dstVars[i],new_name));
          }

          body = replaceAll(body, hr.dstVars[i], bindVars2[i]);
        }

        for (int i = 0; i < hr.locVars.size(); i++)
        {
          Expr new_name = mkTerm<string> ("__loc_var_" + to_string(locVar_index++), m_efac);
          Expr var = cloneVar(hr.locVars[i], new_name);

          body = replaceAll(body, hr.locVars[i], var);
        }

        ssa.push_back(body);
        bindVars.push_back(bindVars2);
        bindVars1 = bindVars2;
      }
    }

    vector<int> foundCexes;
    tribool exploreTraces(int cur_bnd, int bnd, bool print = false,
                                  bool toRemBad = true)
    {
      if (ruleManager.chcs.size() == 0)
      {
        if (debug) outs () << "CHC system is empty\n";
        if (print) outs () << "Success after complete unrolling\n";
        return false;
      }
      if (!ruleManager.hasCycles())
      {
        if (debug) outs () << "CHC system does not have cycles\n";
        bnd = ruleManager.chcs.size();
      }
      tribool res = indeterminate;
      while (cur_bnd <= bnd)
      {
        if (debug)
        {
          outs () << ".";
          outs().flush();
        }
        vector<vector<int>> traces;
        getAllTraces(mk<TRUE>(m_efac), ruleManager.failDecl, cur_bnd++,
              vector<int>(), traces);
        bool toBreak = false;
        for (auto &a : traces)
        {
          if (contains(foundCexes, a.back())) continue;
          ExprVector ssa;
          getSSA(a, ssa);
          int sz;
          // for (int u = 0; u < ssa.size(); u++)
          // {
          //   outs () << " chc: " << u << ": " <<
          //   ruleManager.chcs[a[u]].srcRelation << " -> " << ruleManager.chcs[a[u]].Relation << "\n";
          // }
          res = u.isSatIncrem(ssa, sz);

          if (res || indeterminate (res))
          {
            if (debug)
            {
              outs () << "\ntrue";
              for (auto & b : a)
                outs () << " (" << b << ") -> " << ruleManager.chcs[b].dstRelation;
              outs () << "\n";
              pprint(ruleManager.chcs[a.back()].body);
            }
            if (toRemBad)
              foundCexes.push_back(a.back());
            else
            {
              toBreak = true;
              break;
            }
          }
          else
          {
            a.resize(sz);
            unsat_prefs.insert(a);
          }
        }
        if (toBreak) break;
      }

      if (debug || print)
      {
        if (indeterminate(res))
        {
          errs () << "unknown\n";
          exit(0);
        }
        else if (res)
        {
          outs () << "Counterexample of length " << (cur_bnd - 1) << " found\n";
          pprint(u.getModel());
        }
        else if (ruleManager.hasCycles())
          outs () << "No counterexample found up to length " << cur_bnd << "\n";
        else
          outs () << "Success after complete unrolling\n";
      }
      return res;
    }

    Expr getInv() { return inv; }

    void fillVars(Expr srcRel, ExprVector& srcVars, ExprVector& vars, int l, int s,
          vector<int>& mainInds, vector<ExprVector>& versVars, ExprSet& allVars)
    {
      for (int l1 = l; l1 < bindVars.size(); l1 = l1 + s)
      {
        ExprVector vers;
        int ai = 0;

        for (int i = 0; i < vars.size(); i++)
        {
          int var = mainInds[i];
          Expr bvar;
          if (var >= 0)
          {
            if (ruleManager.hasArrays[srcRel])
              bvar = bindVars[l1-1][var];
            else
              bvar = bindVars[l1][var];
          }
          else
          {
            bvar = replaceAll(vars[i], srcVars, bindVars[l1-1]);
            bvar = replaceAll(bvar, bindVars[l1-1][-var-1], bindVars[l1][-var-1]);
            allVars.insert(bindVars[l1][-var-1]);
            ai++;
          }
          vers.push_back(bvar);
        }
        versVars.push_back(vers);
        allVars.insert(vers.begin(), vers.end());
      }
    }

    void fillVarsSimp(Expr srcRel, ExprVector& srcVars, ExprVector& vars, int l, int s,
          vector<ExprVector>& versVars, ExprSet& allVars)
    {
      for (int l1 = l; l1 < bindVars.size(); l1 = l1 + s)
      {
        ExprVector vers;
        for (int i = 0; i < vars.size(); i++)
        {
          Expr bvar = replaceAll(vars[i], srcVars, bindVars[l1]);
          vers.push_back(bvar);
        }
        versVars.push_back(vers);
        allVars.insert(vers.begin(), vers.end());
      }
    }

    void getOptimConstr(vector<ExprVector>& versVars, int vs, ExprVector& srcVars,
                            ExprSet& constr, Expr phaseGuard, ExprVector& diseqs)
    {
      for (auto v : versVars)
        for (int i = 0; i < v.size(); i++)
          for (int j = i + 1; j < v.size(); j++)
            diseqs.push_back(mk<ITE>(mk<NEQ>(v[i], v[j]), mkMPZ(1, m_efac), mkMPZ(0, m_efac)));

      for (int i = 0; i < vs; i++)
        for (int j = 0; j < versVars.size(); j++)
          for (int k = j + 1; k < versVars.size(); k++)
            diseqs.push_back(mk<ITE>(mk<NEQ>(versVars[j][i], versVars[k][i]), mkMPZ(1, m_efac), mkMPZ(0, m_efac)));

      Expr extr = disjoin(constr, m_efac);
      if (debug) outs () << "Adding extra constraints to every iteration: " << extr << "\n";
      for (auto & bv : bindVars)
        diseqs.push_back(mk<ITE>(replaceAll(extr, srcVars, bv), mkMPZ(0, m_efac), mkMPZ(1, m_efac)));
      if (phaseGuard != NULL)
        for (auto & bv : bindVars)
          diseqs.push_back(mk<ITE>(replaceAll(phaseGuard, srcVars, bv), mkMPZ(0, m_efac), mkMPZ(1, m_efac)));
    }

    Expr findSelect(int t, int i)
    {
      Expr tr = ruleManager.chcs[t].body;
      ExprVector& srcVars = ruleManager.chcs[t].srcVars;
      ExprVector st;
      filter (tr, IsStore (), inserter(st, st.begin()));
      for (auto & s : st)
      {
        if (!contains(s->left(), srcVars[i])) continue;
        auto ty = typeOf(s)->last();
        if (!isOpX<INT_TY>(ty) && !isOpX<BVSORT> (ty)) continue;
        if (!hasOnlyVars(s, srcVars)) continue;
        return mk<SELECT>(s->left(), s->right());
      }
      st.clear();
      filter (tr, IsSelect (), inserter(st, st.begin()));
      for (auto & s : st)
      {
        if (!contains(s->left(), srcVars[i])) continue;
        auto ty = typeOf(s)->last();
        if (!isOpX<INT_TY>(ty) && !isOpX<BVSORT> (ty)) continue;
        if (!hasOnlyVars(s, srcVars)) continue;
        return s;
      }
      return NULL;
    }

    // used for individual loops in BV
    bool unrollAndExecuteOne(
          int chc,
          Expr srcRel,
          ExprVector& invVars,
				  vector<vector<double> >& models,
          Expr invs, bool useVars, ExprSet& addVars,
          int k = 10)
    {
      // helper var
      string str = to_string(numeric_limits<double>::max());
      str = str.substr(0, str.find('.'));
      cpp_int max_double = lexical_cast<cpp_int>(str);

      for (auto & a : ruleManager.cycles)
      for (int cyc = 0; cyc < a.second.size(); cyc++)
      {
        auto & loop = a.second[cyc];
        ExprVector& srcVars = ruleManager.chcs[loop[0]].srcVars;
        if (srcRel != ruleManager.chcs[loop[0]].srcRelation) continue;
        if (models.size() > 0) continue;

        ExprVector vars;
        if (useVars)
        {
          for (int i = 0; i < srcVars.size(); i++)
          {
            // if (lexical_cast<string>(srcVars[i]).find("$tmp") != string::npos) continue;
            // if (lexical_cast<string>(srcVars[i]).find("__CPROVER") != string::npos) continue;
            // if (lexical_cast<string>(srcVars[i]).find("VERIFIER") != string::npos) continue;
            Expr t = typeOf(srcVars[i]);
            if (isOpX<INT_TY>(t) || isOpX<BVSORT> (t))
            {
              vars.push_back(srcVars[i]);
            }
          }
        }
        for (auto & v : addVars)
        {
          if (is_bvnum(v) || isOpX<MPZ>(v)) continue;
          vars.push_back(v);
        }

        if (debug)
        {
          outs () << "adding these vars/terms for " << srcRel << "\n";
          pprint(vars, 4);
        }

        // if (vars.size() < 2 && cyc == ruleManager.cycles.size() - 1)
        //   continue; // does not make much sense to run with only one var when it is the last cycle
        invVars = vars;

        auto prefix = ruleManager.getPrefix(srcRel);
        vector<int> trace = {chc};
        int l = 1;                              // starting index (before the loop)
        if (ruleManager.hasArrays[srcRel]) l++; // first iter is usually useless

        for (int j = 0; j < k; j++)
          for (int m = 0; m < loop.size(); m++)
            trace.push_back(loop[m]);

        ExprVector ssa;
        getSSA(trace, ssa);

        // compute vars for opt constraint
        vector<ExprVector> versVars;
        ExprSet allVars;
        ExprVector diseqs;
        fillVarsSimp(srcRel, srcVars, vars, l, loop.size(), versVars, allVars);

        allVars.insert(bindVars.back().begin(), bindVars.back().end());
        auto res = u.isSat(ssa);
        if (indeterminate(res) || !res)
        {
          if (debug) outs () << "Unable to solve the BMC formula for " <<  srcRel << "\n";
          continue;
        }
        ExprMap allModels;
        u.getModel(allVars, allModels);

        if (debug) outs () << "\nUnroll and execute the cycle for " <<  srcRel << "\n";

        for (int j = 0; j < versVars.size(); j++)
        {
          vector<double> model;
          if (debug) outs () << "  model for " << j << ": [";
          bool toSkip = false;

          for (int i = 0; i < vars.size(); i++) {
            Expr bvar = versVars[j][i];
            Expr m = allModels[bvar];
            double value;
            if (m != NULL && (is_bvnum(m) || isOpX<MPZ>(m)))
            {
              if (is_bvnum(m)) m = mkTerm(toMpz(m), m_efac);
              if (lexical_cast<cpp_int>(m) > max_double ||
                  lexical_cast<cpp_int>(m) < -max_double)
              {
                toSkip = true;
                break;
              }
              value = lexical_cast<double>(m);
            }
            else
            {
              toSkip = true;
              break;
            }
            model.push_back(value);
            if (debug) outs () << *vars[i] << " = " << *allModels[bvar] << ", ";
          }
          if (!toSkip) models.push_back(model);
          if (debug) outs () << "\b\b]\n";
        }
      }

      return true;
    }
  };

  inline void unrollAndCheck(string smt, int bnd1, int bnd2, int to, bool skip_elim, int debug)
  {
    ExprFactory m_efac;
    EZ3 z3(m_efac);
    CHCs ruleManager(m_efac, z3, debug);
    ruleManager.parse(smt, false);
    if (!skip_elim)
      if (!ruleManager.doElim()) return;
    BndExpl bnd(ruleManager, to, debug);
    bnd.exploreTraces(bnd1, bnd2, true, false);
  };
}

#endif
