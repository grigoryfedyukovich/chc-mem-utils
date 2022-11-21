#ifndef RNDLEARNER__HPP__
#define RNDLEARNER__HPP__

#include "Horn.hpp"
#include "BndExpl.hpp"
#include "ae/SMTUtils.hpp"
#include "sampl/SeedMiner.hpp"
#include "sampl/Sampl.hpp"

#include <iostream>
#include <fstream>

using namespace std;
using namespace boost;
namespace ufo
{
  class RndLearner
  {
    protected:

    ExprFactory &m_efac;
    EZ3 &m_z3;
    SMTUtils u;
    ufo::ZSolver<ufo::EZ3> m_smt_solver;
    vector<ufo::ZSolver<ufo::EZ3>> m_smt_safety_solvers;
    map<int, bool> safety_progress;

    CHCs& ruleManager;
    ExprVector decls;
    vector<vector<SamplFactory>> sfs;
    ExprVector curCandidates;

    vector<map<int, Expr>> invarVars;
    vector<ExprVector> invarVarsShort;

    // for arrays
    vector<ExprSet> arrCands;
    vector<ExprVector> arrAccessVars;
    vector<ExprSet> arrIterRanges;

    int invNumber;
    int numOfSMTChecks;

    bool kind_succeeded;      // interaction with k-induction
    bool oneInductiveProof;

    bool densecode;           // catch various statistics about the code (mostly, frequences) and setup the prob.distribution based on them
    bool addepsilon;          // add some small probability to features that never happen in the code
    bool aggressivepruning;   // aggressive pruning of the search space based on SAT/UNSAT (WARNING: may miss some invariants)

    bool statsInitialized;
    int printLog;

    public:

    RndLearner (ExprFactory &efac, EZ3 &z3, CHCs& r, unsigned to, bool k, bool b1, bool b2, bool b3, int debug) :
      m_efac(efac), m_z3(z3), ruleManager(r), m_smt_solver (z3), u(efac, to),
      invNumber(0), numOfSMTChecks(0), oneInductiveProof(true), kind_succeeded (!k),
      densecode(b1), addepsilon(b2), aggressivepruning(b3),
      statsInitialized(false), printLog(debug){}

    void initializeDecl(Expr invDecl)
    {
      if (printLog > 2) outs () << "\nINITIALIZE PREDICATE " << invDecl << "\n====================\n";
//      assert (invDecl->arity() > 2);
      assert(decls.size() == invNumber);
      assert(sfs.size() == invNumber);
      assert(curCandidates.size() == invNumber);

      decls.push_back(invDecl);
      invarVars.push_back(map<int, Expr>());
      invarVarsShort.push_back(ExprVector());

      curCandidates.push_back(NULL);

      sfs.push_back(vector<SamplFactory> ());
      sfs.back().push_back(SamplFactory (m_efac, aggressivepruning));
      SamplFactory& sf = sfs.back().back();

      for (int i = 0; i < ruleManager.invVars[invDecl].size(); i++)
      {
        Expr var = ruleManager.invVars[invDecl][i];
        // if (sf.addVar(var))  // FIXME:
        {
          invarVars[invNumber][i] = var;
          invarVarsShort[invNumber].push_back(var);
        }
      }

      arrCands.push_back(ExprSet());
      arrAccessVars.push_back(ExprVector());
      arrIterRanges.push_back(ExprSet());

      invNumber++;
    }

    void printSolution(bool simplify = true)
    {
      for (int i = 0; i < decls.size(); i++)
      {
        Expr rel = decls[i];
        SamplFactory& sf = sfs[i].back();
        ExprSet lms = sf.learnedExprs;
        outs () << "(define-fun " << *rel << " (";
        for (auto & b : ruleManager.invVars[rel])
        {
          outs () << "(" << b << " ";
          u.print(typeOf(b));
          outs () << ")";
        }
        outs () << ") Bool\n  ";
        Expr tmp = conjoin(lms, m_efac);
        if (simplify && !containsOp<FORALL>(tmp)) u.removeRedundantConjuncts(lms);
        Expr res = simplifyArithm(conjoin(lms, m_efac));
        u.print(res);
        outs () << ")\n";
        outs().flush();
        assert(hasOnlyVars(res, ruleManager.invVars[rel]));
      }
    }
  };
}

#endif
