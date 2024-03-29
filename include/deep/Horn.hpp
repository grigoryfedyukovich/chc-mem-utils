#ifndef HORN__HPP__
#define HORN__HPP__

#include <fstream>
#include "ae/AeValSolver.hpp"
#include "ae/ExprSimplBv.hpp"
#include "SetEncoding.hpp"

using namespace std;
using namespace boost;

namespace ufo
{
  struct HornRuleExt
  {
    int id;

    ExprVector srcVars;
    ExprVector dstVars;
    ExprVector locVars;

    ExprSet readVars;
    ExprSet writeVars;

    ExprSet lin;

    ExprVector origSrc;
    ExprVector origDst;
    ExprMap origSrcVars;

    Expr body;
    ExprVector nested;

    Expr srcRelation;
    Expr dstRelation;

    bool isFact;
    bool isQuery;
    bool isInductive;

    void init (Expr fail)
    {
      if (srcRelation == NULL) srcRelation = mk<TRUE>(body->getFactory());
      isFact = isOpX<TRUE>(srcRelation);
      isQuery = (dstRelation == fail);
      isInductive = (srcRelation == dstRelation);
    }

    void assignVarsAndRewrite (map<Expr, ExprVector>& invSrc,
                               map<Expr, ExprVector>& invDst)
    {
      for (int i = 0; i < origSrc.size(); i++)
      {
        srcVars.push_back(invSrc[srcRelation][i]);
        lin.insert(mk<EQ>(origSrc[i], srcVars[i]));
        origSrcVars[srcVars[i]] = origSrc[i];
      }

      for (int i = 0; i < origDst.size(); i++)
      {
        dstVars.push_back(invDst[dstRelation][i]);
        lin.insert(mk<EQ>(origDst[i], dstVars[i]));
      }
    }

    bool splitBody (int debug = 0)
    {
      getConj (simplifyBool(body), lin);
      for (auto c = lin.begin(); c != lin.end(); )
      {
        Expr cnj = *c;
        if (isOpX<FALSE>(cnj)) return false;
        else if (isOpX<FAPP>(cnj))
        {
          Expr rel = cnj->left();
          {
            auto strName = lexical_cast<string>(rel->left());
            if (contains(supportedPreds, strName) ||
                contains(supportedPredsAss, strName))
            {
              if (debug > 0)
                outs () << "hr.body \"" << body << "\" has "
                        << strName << "\n";
              ++c;
            }
            else
            {
              if (srcRelation != NULL)
              {
                errs () << "Nonlinear CHC is currently unsupported: ["
                        << *srcRelation << " /\\ " << *rel->left() << " -> "
                        << *dstRelation << "]\n";
                exit(1);
              }
              srcRelation = rel->left();
              for (auto it = cnj->args_begin()+1; it != cnj->args_end(); ++it)
                origSrc.push_back(*it);
              c = lin.erase(c);
            }
          }
        }
        else ++c;
      }
      return true;
    }

    void shrinkLocVars()
    {
      for (auto it = locVars.begin(); it != locVars.end(); )
        if (contains(body, *it)) ++it;
        else it = locVars.erase(it);
    }
  };

  class CHCs
  {
    private:
    set<int> indeces;
    string varname = "_FH_";
    SMTUtils u;

    public:

    ExprFactory &m_efac;
    EZ3 &m_z3;

    Expr failDecl;
    vector<HornRuleExt> chcs;
    vector<HornRuleExt*> allCHCs, wtoCHCs;
    ExprSet decls;
    map<Expr, ExprVector> invVars, invVarsPrime;
    map<Expr, ExprMap> invVarsPrimeMap, invVarsMap;
    map<Expr, vector<int>> outgs;
    bool cycleSearchDone = false;
    ExprVector loopheads;
    map<Expr, vector<vector<int>>> cycles, prefixes;
    vector<vector<int>> acyclic;
    ExprVector seqPoints;
    map<Expr, ExprVector> newVars;
    map<Expr, bool> hasArrays;
    bool hasAnyArrays, hasBV = false;
    int debug;
    set<int> chcsToCheck1, chcsToCheck2, toEraseChcs;
    int glob_ind = 0;
    ExprSet origVrs;
    map<Expr, ExprMap> origSrcVars;
    bool mems;

    Expr alloc, mem, allocP, memP, off, offP, siz, sizP, aux, auxP; // set-enc

    // data structures for frontend-optimizations (lightweight. no SMT)
    map<int, set<pair<Expr, Expr>>> transit;
    map<int, pair<map<Expr, int>, map<Expr, int>>> transused;
    map<int, ExprSet> notused, singused;
    ExprSet tranRemoved;
    set<int> tranShrinked;
    ExprVector scalarInds;

    CHCs(ExprFactory &efac, EZ3 &z3, bool _mems = false, int d = false) :
      u(efac), m_efac(efac), m_z3(z3), mems(_mems), hasAnyArrays(false), debug(d) {};

    void addVar(Expr decl, Expr var, Expr varPr)
    {
      if (contains(invVars[decl], var)) return;
      invVars[decl].push_back(var);
      invVarsPrime[decl].push_back(varPr);
      invVarsPrimeMap[decl][var] = varPr;
      invVarsMap[decl][varPr] = var;
    }

    void addVar(Expr decl, Expr var)
    {
      Expr varPr = cloneVar(var,
         mkTerm<string>(lexical_cast<string>(var) + "'", m_efac));
      addVar(decl, var, varPr);
    }

    void removeVar(Expr decl, Expr var)
    {
      Expr varPr = invVarsPrimeMap[decl][var];
      for (auto it = invVars[decl].begin(); it != invVars[decl].end(); )
      {
        if (*it == var)
        {
          it = invVars[decl].erase(it);
          break;
        }
        else ++it;
      }
      for (auto it = invVarsPrime[decl].begin(); it != invVarsPrime[decl].end(); )
      {
        if (*it == varPr)
        {
          it = invVarsPrime[decl].erase(it);
          break;
        }
        else ++it;
      }
    }

    void removeVar(int i, Expr var)
    {
      ExprSet lin;
      getConj(chcs[i].body, lin);
      for (auto it = lin.begin(); it != lin.end(); )
        if (contains(*it, var)) it = lin.erase(it);
          else ++it;
      chcs[i].body = conjoin(lin, m_efac);
    }

    void resetVars()
    {
      invVars.clear();
      invVarsPrime.clear();
      invVarsPrimeMap.clear();
      invVarsMap.clear();
    }

    bool isFapp (Expr e)
    {
      if (isOpX<FAPP>(e))
        if (e->arity() > 0)
          if (isOpX<FDECL>(e->left()))
            if (e->left()->arity() >= 2)
              return true;
      return false;
    }

    Expr getDeclByName (Expr a)
    {
      for (auto & d : decls)
        if (d->left() == a) return d;
      return NULL;
    }

    bool addedDecl (Expr a)
    {
      return getDeclByName(a) != NULL;
    }

    void addDecl (Expr a)
    {
      if (invVars[a->left()].size() == 0)
      {
        decls.insert(a);
        int j = 0;
        for (int i = 1; i < a->arity()-1; i++)
        {
          Expr arg = a->arg(i);
          if (!isOpX<INT_TY>(arg) && !isOpX<REAL_TY>(arg) &&
              !isOpX<BOOL_TY>(arg) && !isOpX<ARRAY_TY>(arg) &&
              !isOpX<BVSORT>(arg) && !isOpX<AD_TY>(arg))
          {
            errs() << "Argument #" << i << " of " << a << " is not supported\n";
            exit(1);
          }
          while (true)
          {
            Expr name = mkTerm<string>(varname + to_string(j), m_efac);
            Expr var = fapp(constDecl(name, arg));
            name = mkTerm<string>(lexical_cast<string>(name) + "'", m_efac);
            Expr varPr = fapp(constDecl(name, arg));
            j++;
            if (find(origVrs.begin(), origVrs.end(), var) != origVrs.end())
              continue;
            if (find(origVrs.begin(), origVrs.end(), varPr) != origVrs.end())
              continue;
            addVar(a->left(), var, varPr);
            break;
          }
        }
      }
    }

    bool normalize (Expr& r, HornRuleExt& hr)
    {
      r = regularizeQF(r);

      // TODO: support more syntactic replacements
      while (isOpX<FORALL>(r))
      {
        for (int i = 0; i < r->arity() - 1; i++)
        {
          hr.locVars.push_back(bind::fapp(r->arg(i)));
        }
        r = r->last();
      }

      if (isOpX<NEG>(r) && isOpX<EXISTS>(r->first()))
      {
        for (int i = 0; i < r->first()->arity() - 1; i++)
          hr.locVars.push_back(bind::fapp(r->first()->arg(i)));

        r = mk<IMPL>(r->first()->last(), mk<FALSE>(m_efac));
      }

      if (isOpX<NEG>(r))
      {
        r = mk<IMPL>(r->first(), mk<FALSE>(m_efac));
      }
      else if (isOpX<OR>(r) && r->arity() == 2 &&
               isOpX<NEG>(r->left()) && hasUninterp(r->left()))
      {
        r = mk<IMPL>(r->left()->left(), r->right());
      }
      else if (isOpX<OR>(r) && r->arity() == 2 &&
               isOpX<NEG>(r->right()) && hasUninterp(r->right()))
      {
        r = mk<IMPL>(r->right()->left(), r->left());
      }

      // for now: hack
      if (isOpX<IMPL>(r) && isOpX<ITE>(r->right()))
      {
        return true;
      }

      if (isOpX<IMPL>(r) && isOpX<IMPL>(r->right()))
      {
        r = mk<IMPL>(mk<AND>(r->left(), r->right()->left()), r->right()->right());
      }

      if (isOpX<IMPL>(r) && !isFapp(r->right()) && !isOpX<FALSE>(r->right()))
      {
        if (isOpX<TRUE>(r->right()))
        {
          return false;
        }
        r = mk<IMPL>(mk<AND>(r->left(), mkNeg(r->right())), mk<FALSE>(m_efac));
      }

      if (!isOpX<IMPL>(r)) r = mk<IMPL>(mk<TRUE>(m_efac), r);

      return true;
    }

    void parse(string smt, bool setEnc = true, bool opt = true)
    {
      if (debug > 0) outs () << "\nPARSING" << "\n=======\n";
      std::unique_ptr<ufo::ZFixedPoint <EZ3> > m_fp;
      m_fp.reset (new ZFixedPoint<EZ3>(m_z3));
      ZFixedPoint<EZ3> &fp = *m_fp;
      fp.loadFPfromFile(smt);
      chcs.reserve(fp.m_rules.size());

      ExprMap eqs;
      for (auto it = fp.m_rules.begin(); it != fp.m_rules.end(); )
      {
        if (isOpX<EQ>(*it))
        {
          eqs[(*it)->left()->left()] = (*it)->right()->left();
          it = fp.m_rules.erase(it);
        }
        else ++it;
      }

      ExprSet objNames;
      for (auto &r: fp.m_rules)
      {
        int chcNum = chcs.size();
        hasAnyArrays |= containsOp<ARRAY_TY>(r);
        chcs.push_back(HornRuleExt());
        HornRuleExt& hr = chcs[chcNum];
        while (true)
        {
          Expr r1;
          if (debug >= 0)
          {
            r1 = r;
            for (auto & e : eqs)
            {
              Expr r2 = replaceAll(r1, e.first, e.second);
              if (r1 != r2)
              {
                outs () << "rewriting " << (e.first)->left()
                        << " -> " << (e.second)->left() << "\n";
                r1 = r2;
              }
            }
          }
          else
            r1 = replaceAll(r, eqs);
          if (r == r1) break;
          else r = r1;
        }
        if (!normalize(r, hr))
        {
          chcs.pop_back();
          continue;
        }

        filter (r, bind::IsConst(), inserter (origVrs, origVrs.begin()));
        // hack:
        if (isOpX<ITE>(r->last()))
        {
          hr.body = mk<IMPL>(mk<AND>(r->left(), r->last()->left()),
                             r->last()->right());
          chcs.push_back(chcs.back());
          chcs.back().body = mk<IMPL>(mk<AND>(r->left(), mkNeg(r->last()->left())),
                             r->last()->last());
        }
        else
          hr.body = r;

        findObjectNames (hr.body, objNames);

        if (lexical_cast<string>(hr.body).find("element-address") != string::npos)
          continue;

        while (true) // flattening
        {
          Expr res = NULL;
          bool nested = false;
          findUpd (chcs[chcNum].body, res, nested);
          if (nested)
          {
            // GF: weird behavior of std::vector. when many (> 100) new elements
            //     are pushed back, the `hr` link expires and need refreshments.
            //     positional access (i.e., `chcs[chcNum]` solves it)
            chcs.push_back(HornRuleExt());
            chcs[chcNum].nested.push_back(res);
            chcs[chcNum].body = replaceAll(chcs[chcNum].body, res, res->right());
          }
          else
          {
            if (debug > 0 && !chcs[chcNum].nested.empty())
              outs () << "nesting level: " << chcs[chcNum].nested.size() << "\n";
            break;
          }
        }
      }

      ExprVector names;
      for (auto & a : objNames) names.push_back(a);

      for (auto & hr : chcs)
      {
        if (hr.body == NULL) continue;
        Expr head = hr.body->right();
        hr.body = hr.body->left();
        if (isOpX<FAPP>(head))
        {
          auto name = head->left()->left();
          auto ind = getVarIndex(lexical_cast<string>(name), supportedPreds);
          if (ind >= 0)
          {
            string name_str = supportedPredsAss[ind];
            auto name_upd = mkTerm<string>(name_str, m_efac);
            head = replaceAll(head, name, name_upd);
            hr.body = mk<AND>(hr.body, mk<NEG>(head));
            head = mk<FALSE>(m_efac);
          }
        }
        if (isOpX<FAPP>(head))
        {
          if (head->left()->arity() == 2 &&
              (find(fp.m_queries.begin(), fp.m_queries.end(), head) !=
               fp.m_queries.end()))
            addFailDecl(head->left()->left());
          else
            addDecl(head->left());
          hr.dstRelation = head->left()->left();

          for (auto it = head->args_begin()+1; it != head->args_end(); ++it)
            hr.origDst.push_back(*it); // to be rewritten later
        }
        else
        {
          if (!isOpX<FALSE>(head)) hr.body = mk<AND>(hr.body, mk<NEG>(head));
          addFailDecl(mk<FALSE>(m_efac));
          hr.dstRelation = mk<FALSE>(m_efac);
        }
        hasBV |= containsOp<BVSORT>(hr.body);
      }
      if (debug > 0) outs () << "Reserved space for " << chcs.size()
                          << " CHCs and " << decls.size() << " declarations\n";

      Expr s = bvsort (64, m_efac); // hardcode for now
      Expr z = bvnum(mkMPZ(0, m_efac), s);

      if (setEnc)
      {
        alloc = mkTerm<string>("_alloc", m_efac);
        mem = mkTerm<string>("_mem", m_efac);
        off = mkTerm<string>("_off", m_efac);
        siz = mkTerm<string>("_siz", m_efac);
        aux = mkTerm<string>("_aux", m_efac);
        alloc = mkConst(alloc, mk<ARRAY_TY>(s, s));
        mem = mkConst(mem, mk<ARRAY_TY>(s, mk<ARRAY_TY>(s, s)));
        off = mkConst(off, mk<ARRAY_TY>(s, s));
        siz = mkConst(siz, mk<ARRAY_TY>(s, s));
        aux = mkConst(aux, mk<ARRAY_TY>(s, s));
        allocP = cloneVar(alloc, mkTerm<string>("_alloc'", m_efac));
        memP = cloneVar(mem, mkTerm<string>("_mem'", m_efac));
        offP = cloneVar(off, mkTerm<string>("_off'", m_efac));
        sizP = cloneVar(siz, mkTerm<string>("_siz'", m_efac));
        auxP = cloneVar(siz, mkTerm<string>("_aux'", m_efac));
      }

      if (failDecl == NULL) failDecl = mk<FALSE>(m_efac);
      HornRuleExt & hrprev = *chcs.begin();
      for (auto it = chcs.begin(); it != chcs.end(); )
      {
        HornRuleExt & hr = *it;
        if (hr.splitBody() &&
           (hr.srcRelation == NULL || addedDecl(hr.srcRelation)))
            ++it;
        else
        {
          if (debug > 0)
            outs() << "Deleting vacuous CHC:\n"
                   << hr.srcRelation << " -> " << hr.dstRelation << "\n";
          it = chcs.erase(it);
          continue;
        }

        hr.init(failDecl);

        hr.assignVarsAndRewrite (invVars, invVarsPrime);
        if (!hr.origSrcVars.empty())
          origSrcVars[hr.srcRelation] = hr.origSrcVars;

        // repair for flattening
        auto it2 = it;
        string name = lexical_cast<string>(hr.srcRelation);
        Expr d = getDeclByName(hr.srcRelation);
        for (int i = 0; i < hr.nested.size(); i++)
        {
          Expr newName = mkTerm<string>(name + "_" + to_string(i), m_efac);
          HornRuleExt & hr2 = *it2;
          hr2.srcRelation = newName;
          decls.insert(replaceAll(d, d->left(), newName));
          invVars[hr2.srcRelation] = invVars[hr.srcRelation];
          invVarsPrime[hr2.srcRelation] = invVarsPrime[hr.srcRelation];
          invVarsPrimeMap[hr2.srcRelation] = invVarsPrimeMap[hr.srcRelation];
          invVarsMap[hr2.srcRelation] = invVarsMap[hr.srcRelation];
          hr2.dstRelation = hr.dstRelation;
          hr.dstRelation = newName;
          hr2.srcVars = hr.srcVars;
          hr2.dstVars = hr.dstVars;
          hr2.body = mk<EQ>(hr.nested[i], invVarsPrime[hr2.srcRelation][0]);
          ++it2;
        }
      }

      // small painless optimization
      int sz = chcs.size();
      eliminateVacDecls(true);
      if (debug > 0)
        outs () << "Vacuity elimination (1): " << sz << " -> " << chcs.size() << "\n";

      // set encoding is here
      if (setEnc) resetVars();
      for (auto & hr : chcs)
      {
        if (debug > 2)
          outs () << "\n\n PROC " << hr.srcRelation << " -> " << hr.dstRelation << "\n";

        if (setEnc)
        {
          ExprSet rlin;
          auto dstVars = invVars[hr.srcRelation];

          for (auto & l : hr.lin)
          {
            if (debug >= 3)
              outs () << "\n------\n  orig: " << l << "\n";
            Expr rl = l;
            if (!hr.isFact)
              rl = evalReplace(rl, hr.readVars, dstVars, alloc, mem, off,
                siz, aux, allocP, memP, offP, sizP, auxP, names);

            rl = rewriteSet(rl, hr.writeVars, dstVars, alloc, mem, off,
              siz, aux, allocP, memP, offP, sizP, auxP, names);

            if (debug >= 3)
            {
              outs () << "  conv: " << rl << "\n";
              if (!hr.readVars.empty())
              {
                outs () << "  new src vars: ";
                pprint(hr.readVars);
              }
              if (!hr.writeVars.empty())
              {
                outs () << "  new dst vars: ";
                pprint(hr.writeVars);
              }
            }

            if (!containsOp<AD_TY>(rl)) getConj(rl, rlin);
          }

          for (auto it = hr.readVars.begin(); it != hr.readVars.end(); ++it)
          {
            if (find(invVars[hr.srcRelation].begin(),
                     invVars[hr.srcRelation].end(), *it) ==
                     invVars[hr.srcRelation].end())
            {
              errs() << "WARNING: variable " << *it << " is not initialized\n";
              addVar(hr.srcRelation, *it);
            }
          }

          for (auto & a : hr.writeVars) unique_push_back(a, dstVars);

          if (hr.isFact)
          {
            Expr cArr = mk<CONST_ARRAY>(s, z);
            // assume that this fact does not rewrite _alloc
            for (auto & a : {allocP, auxP, offP, sizP})
              rlin.insert(mk<EQ>(a, cArr));
            for (auto & a : {mem, alloc, aux, off, siz})
              dstVars.push_back(a);
          }

          if (!hr.isQuery)
            for (auto & v : dstVars)
              addVar(hr.dstRelation, v);
          hr.body = conjoin(rlin, m_efac);
        }
        else
          hr.body = conjoin(hr.lin, m_efac);
      }

      // find cycles in CHC structure
      findCycles();

      // move vars out of mem
      if (setEnc) moveMemVars(z, names);

      for (auto & ind : scalarInds) // cleaning after vars are moved
        for (auto & r : chcs)
          r.body = fixpointRewrite(
            replaceAll(r.body, mk<SELECT>(alloc, ind), z),
              [](Expr e){ return
                simplifyBool(
                simplifyBV(e));});

      if (setEnc) for (auto & r : chcs) r.body = typeRepair(r.body);

      // memory safety checks generation
      if (setEnc && mems) addMemSafetyChecks();

      if (getQum() == 0)
      {
        errs () << "CHC system has no queries\n";
        exit(0);
      }

      if (setEnc)
      {
        fillVars();
        updateDecls();
      }

      for (auto & hr : chcs)
      {
        hr.body = simpleQE(hr.body, hr.locVars);
        hr.shrinkLocVars();
      }

      sz = chcs.size();
      eliminateVacDecls(false);
      if (debug > 0)
        outs () << "Vacuity elimination (2): " << sz << " -> " << chcs.size() << "\n";

      findCycles(); // the second time, after some optims

      if (opt)
      {
        if (debug > 1)
        {
          outs () << "  Parsed CHCs:\n";
          print(debug >= 1, true);
        }

        if (setEnc)
        {
          if (debug > 0)
          {
            outs() << "Points visited in all runs:\n";
            pprint(seqPoints);
          }
          for (auto & r : seqPoints)
            for (auto & h : chcs)
              if (h.dstRelation == r)
              {
                if (h.isFact)
                  newVars[r] = invVars[r];
                else
                  for (auto & v : invVars[r])
                    if (!contains(invVars[h.srcRelation], v))
                      unique_push_back(v, newVars[r]);
              }
          if (debug > 0)
            for (auto & a : newVars)
            {
              outs () << "New vars introduced at " << a.first << ":\n    ";
              pprint(a.second);
            }
        }

        eliminateTransitVars();
      }

      for (int i = 0; i < chcs.size(); i++) allCHCs.push_back(&chcs[i]);

      if (debug > 0)
      {
        if (opt)
          outs () << "  Optimized CHCs:\n";
        else
          outs () << "  Parsed CHCs:\n";
        print(debug >= 1, true);
      }
    }

    void fillVars()
    {
      bool toCont = true;
      while (toCont)   // propagate
      {
        toCont = false;
        for (auto & hr : chcs)
        {
          if (hr.isFact || hr.isQuery) continue;

          for (auto & s : invVars[hr.srcRelation])
          {
            bool f = true;
            for (auto & d : invVars[hr.dstRelation])
            {
              if (s == d)
              {
                f = false;
                break;
              }
            }
            if (f)
            {
              addVar(hr.dstRelation, s);
              toCont = true;
            }
          }
        }
      }

      for (auto & hr : chcs)
      {
        hr.srcVars = invVars[hr.srcRelation];
        hr.dstVars = invVarsPrime[hr.dstRelation];

        ExprSet lin;
        getConj(hr.body, lin);

        if (!hr.isFact && !hr.isQuery)
        {
          for (auto & vp : hr.dstVars)
          {
            auto v = invVarsMap[hr.dstRelation][vp];
            if (!contains(hr.writeVars, v))
              lin.insert(mk<EQ>(vp, v));
          }
        }

        hr.body = conjoin(lin, m_efac);

        ExprSet vrs;
        filter(hr.body, bind::IsConst(), std::inserter (vrs, vrs.begin ()));
        for (auto & v : vrs)
          if (!contains(hr.srcVars, v) && !contains(hr.dstVars, v))
            hr.locVars.push_back(v);
      }
    }

    void updateDecls()
    {
      ExprSet updDecls;
      for (auto & d : decls)
      {
        ExprVector args = {d->left()};
        for (auto & a : invVars[d->left()])
          args.push_back(a->left()->last());
        args.push_back(mk<BOOL_TY>(m_efac));
        updDecls.insert(mknary<FDECL>(args));
      }
      decls = updDecls;
    }

    void addMemSafetyChecks()
    {
      vector<HornRuleExt> tmp;
      for (auto & hr : chcs)
      {
        ExprSet selstors, checks;
        filter (hr.body, IsSelect (), inserter(selstors, selstors.begin()));
        filter (hr.body, IsStore (), inserter(selstors, selstors.begin()));
        for (auto & s : selstors)
          if (isOpX<SELECT>(s->left()) && mem == s->left()->left())
          {
            assert(isOpX<SELECT>(s->left()->right()) &&
                   s->left()->right()->left() == alloc);
            checks.insert(mk<BULT>(s->right(),
                   mk<SELECT>(siz, s->left()->right())));
          }

        for (auto & c : checks)
        {
          // errs() << "ADD NEW Q\n";
          tmp.push_back(hr);
          tmp.back().isQuery = true;
          tmp.back().isInductive = false;
          tmp.back().isFact = false;
          tmp.back().dstRelation = failDecl;
          tmp.back().dstVars.clear();
          tmp.back().body = mk<NEG>(c);
        }
      }
      for (auto & c : tmp) chcs.push_back(c);
    }

    vector<int> simplCHCs;
    bool doElim(bool doArithm = true, vector<int> _simplCHCs = {})
    {
      //keep here:
      simplCHCs = _simplCHCs;

      int sz = chcs.size();
      for (int c = 0; c < chcs.size(); c++)
      {
        chcsToCheck1.insert(c);
        chcsToCheck2.insert(c);
      }
      if (!eliminateDecls()) return false;

      // eliminating all at once,
      // otherwise elements at chcsToCheck* need updates
      for (auto it = toEraseChcs.rbegin(); it != toEraseChcs.rend(); ++it)
        chcs.erase(chcs.begin() + *it);
      toEraseChcs.clear();

      // get rid of vacuous:
      while (true)
      {
        bool toBreak = true;
        for (auto & d : decls)
        {
          set<int> toEraseChcs;
          bool toCont = false;
          for (int c = 0; c < chcs.size(); c++)
          {
            if (chcs[c].dstRelation == d->left())
            {
              toCont = true;
              break;
            }
            if (chcs[c].srcRelation == d->left())
              toEraseChcs.insert(c);
          }
          if (toCont) continue;
          for (auto it = toEraseChcs.rbegin(); it != toEraseChcs.rend(); ++it)
          {
            toBreak = false;
            chcs.erase(chcs.begin() + *it);
          }
        }
        if (toBreak) break;
      }
      if (chcs.empty())
      {
        outs () << "Vacuity eliminated all\nSuccess\n";
        return false;
      }

      if (debug > 0)
        outs () << "Vacuity elimination: " << sz << " -> " << chcs.size() << "\n";

      set<int> redundandQs;
      for (int i = 0; i < chcs.size(); i++)
      {
        if (!chcs[i].isQuery) continue;
        if (contains(redundandQs, i)) continue;
        for (int j = i + 1; j < chcs.size(); j++)
        {
          if (!chcs[j].isQuery) continue;
          if (chcs[i].srcRelation != chcs[j].srcRelation) continue;
          if (chcs[i].body != chcs[j].body) continue;
          redundandQs.insert(j);
        }
      }

      for (auto rit = redundandQs.rbegin(); rit != redundandQs.rend(); rit++)
      {
        if (debug > 0)
          outs () << "  removing redundant query #" << *rit << "\n";
        chcs.erase(chcs.begin() + *rit);
      }

      if (debug >= 0)
      {
        outs () << "  Simplified CHCs:\n";
        print(debug >= 1, true);
      }

      findCycles();
      for (int i = 0; i < chcs.size(); i++) allCHCs.push_back(&chcs[i]);
      return true;
    }

    bool eliminateTrivTrueOrFalse()
    {
      set<int> toEraseChcsTmp;
      for (int i = 0; i < chcs.size(); i++)
      {
        if (find(toEraseChcs.begin(), toEraseChcs.end(), i)
                            != toEraseChcs.end()) continue;
        if (find(toEraseChcsTmp.begin(), toEraseChcsTmp.end(), i)
                            != toEraseChcsTmp.end()) continue;

        auto c = &chcs[i];
        if (c->isQuery && !c->isFact)
        {
          auto f = find(chcsToCheck1.begin(), chcsToCheck1.end(), i);
          if (f != chcsToCheck1.end())
          {
            if (u.isTrue(c->body))
            {
              // thus, c->srcRelation should be false
              for (int j = 0; j < chcs.size(); j++)
              {
                if (find(toEraseChcs.begin(), toEraseChcs.end(), j)
                            != toEraseChcs.end()) continue;
                if (find(toEraseChcsTmp.begin(), toEraseChcsTmp.end(), j)
                            != toEraseChcsTmp.end()) continue;

                HornRuleExt* s = &chcs[j];
                if (s->srcRelation == c->srcRelation)
                {
                  // search for vacuous cases where s == inv -> inv2   and
                                    // c == inv /\ true -> false
                  // then, inv can only be false, thus s does not give any constraint
                  toEraseChcsTmp.insert(j);  // could erase here, but ther will
                                                      //be a mess with pointers
                }
                else if (s->dstRelation == c->srcRelation)
                {
                  s->isQuery = true;
                  s->dstRelation = failDecl;
                  s->locVars.insert(s->locVars.end(), s->dstVars.begin(),
                                        s->dstVars.end());
                  s->dstVars.clear();
                  chcsToCheck1.insert(j);
                  chcsToCheck2.insert(j);
                }
              }
              decls.erase(c->srcRelation);
            }
            chcsToCheck1.erase(f);
          }
        }
        else if (c->isQuery && c->isFact)
          if (u.isSat(c->body))
          {
            outs () << "Counterexample found (during preprocessing)\n";
            return false;
          }
      }

      if (toEraseChcsTmp.empty()) return true;

      for (auto it = toEraseChcsTmp.rbegin(); it != toEraseChcsTmp.rend(); ++it)
      {
        if (debug >= 2) outs () << "  Eliminating vacuous CHC: " <<
                chcs[*it].srcRelation << " -> " << chcs[*it].dstRelation << "\n";
        if (debug >= 3) outs () << "    its body is true: " << chcs[*it].body << "\n";
        toEraseChcs.insert(*it);
      }

      return eliminateTrivTrueOrFalse();     // recursive call
    }

    // lighter version (compare to the coming `eliminateDecls`)
    void eliminateVacDecls(bool fwd)
    {
      bool toRepeat = false;
      for (auto it = decls.begin(); it != decls.end(); )
      {
        auto & d = *it;
        vector<int> outgs;
        bool toDel = true;
        for (int i = 0; i < chcs.size(); i++)
        {
          if ((fwd && chcs[i].dstRelation == d->left()) ||
             (!fwd && chcs[i].srcRelation == d->left()))
          {
            toDel = false;
            break;
          }
          if ((fwd && chcs[i].srcRelation == d->left()) ||
             (!fwd && chcs[i].dstRelation == d->left()))
            outgs.push_back(i);
        }
        if (toDel)
        {
          if (debug > 0)
          {
             outs () << "  Removing decl " << d->left() << "\n"
                                         << "  and the following CHCs:\n";
             for (int i = 0; i < outgs.size(); i++)
             {
               outs () << "    " << chcs[outgs[i]].srcRelation << " -> "
                                 << chcs[outgs[i]].dstRelation << "\n";
             }
          }
          for (int i = outgs.size() - 1; i >= 0; i--)
            chcs.erase(chcs.begin() + outgs[i]);
          it = decls.erase(it);
          toRepeat = true;
        }
        else ++it;
      }
      if (toRepeat) eliminateVacDecls(fwd);
    }

    bool eliminateDecls()
    {
      pair<int,int> preElim = {chcs.size() - toEraseChcs.size(), decls.size()};
      if (debug > 0)
        outs () << "Reducing the number of CHCs: " << preElim.first <<
              "; and the number of declarations: " << preElim.second << "...\n";

      if (debug >= 3)
      {
        outs () << "  Current CHC topology:\n";
        print(false);
      }

      // first, remove relations which are trivially false
      // and find any trivially unsatisfiable queries
      if (!eliminateTrivTrueOrFalse()) return false;

      Expr declToRemove = NULL;
      vector<int> srcMax, dstMax;
      set<int> toEraseChcsTmp;

      for (auto d = decls.begin(); d != decls.end(); )
      {
        if (contains(loopheads, (*d)->left()))
        {
          ++d;
          continue;
        }
        vector<int> src, dst;
        for (int i = 0; i < chcs.size(); i++)
        {
          if (find(toEraseChcs.begin(), toEraseChcs.end(), i)
                                            != toEraseChcs.end()) continue;
          if (find(toEraseChcsTmp.begin(), toEraseChcsTmp.end(), i)
                                            != toEraseChcsTmp.end()) continue;
          if (chcs[i].srcRelation == (*d)->left()) src.push_back(i);
          if (chcs[i].dstRelation == (*d)->left()) dst.push_back(i);
        }

        if (src.size() > 0 && dst.size() > 0 && emptyIntersect(src, dst) &&
            emptyIntersect(simplCHCs, src) && emptyIntersect(simplCHCs, dst))
        {
          if (src.size() == 1 || dst.size() == 1)
          {
            if (declToRemove != NULL)
              if (declToRemove->arity() > (*d)->arity())
                { ++d; continue; }
            if (declToRemove != NULL)
              if (declToRemove->arity() == (*d)->arity() &&
                  src.size() * dst.size() > srcMax.size() * dstMax.size())
                { ++d; continue; }

            srcMax = src;
            dstMax = dst;
            declToRemove = *d;
          }
          else
          {
            bool allQs = true;
            for (int i : src)
              if (!chcs[i].isQuery)
                allQs = false;

            if (allQs)
              for (int j : dst)
                if (chcs[j].isFact)
                {
                  for (int i : src)
                    if (u.isSat(getConcatBody(i, j)))
                    {
                      errs () << "Counterexample found (during preprocessing)\n";
                      return false;
                    }

                  // all unsat at this point => remove chc
                  toEraseChcsTmp.insert(j);
                }
          }
        }

        if (src.size() == 0) // found dangling CHCs
        {
          toEraseChcsTmp.insert(dst.begin(), dst.end());
          d = decls.erase(d);
        }
        else ++d;
      }

      // first, it will remove dangling CHCs since it's cheaper
      if (declToRemove != NULL && toEraseChcsTmp.empty())
      {
        for (int i : srcMax)
          for (int j : dstMax)
            concatenateCHCs(i, j);

        toEraseChcsTmp.insert(srcMax.begin(), srcMax.end());
        toEraseChcsTmp.insert(dstMax.begin(), dstMax.end());
        decls.erase(declToRemove);
      }

      for (auto a = toEraseChcsTmp.rbegin(); a != toEraseChcsTmp.rend(); ++a)
      {
        if (debug >= 2)
          outs () << "  Eliminating CHC: " << chcs[*a].srcRelation
                  << " -> " << chcs[*a].dstRelation << "\n";
        toEraseChcs.insert(*a);
      }

      // get rid of CHCs that don't add any _new_ constraints
      removeTautologies();

      if (preElim.first > (chcs.size() - toEraseChcs.size()) ||
          preElim.second > decls.size())
        return eliminateDecls();
      else
      {
        int preComb = (chcs.size() - toEraseChcs.size());
        combineCHCs();
        if (preComb > (chcs.size() - toEraseChcs.size()))
          return eliminateDecls();
      }
      return true;
    }

    Expr getConcatBody(int i, int j, ExprVector& vars, ExprVector& dvars)
    {
      HornRuleExt* s = &chcs[i];
      HornRuleExt* d = &chcs[j];
      dvars = d->dstVars;

      for (int i = 0; i < d->dstVars.size(); i++)
        vars.push_back(cloneVar(d->dstVars[i],
          mkTerm<string> ("__bnd_var_" + to_string(glob_ind++), m_efac)));

      Expr body = replaceAll(s->body, s->srcVars, vars);
      dvars.insert(dvars.end(), d->locVars.begin(), d->locVars.end());
      for (int i = 0; i < d->locVars.size(); i++)
        vars.push_back(cloneVar(d->locVars[i],
          mkTerm<string> ("__loc_var_" + to_string(glob_ind++), m_efac)));

      return mk<AND>(replaceAll(d->body, dvars, vars), body);
    }

    Expr getConcatBody(int i, int j)
    {
      ExprVector vars, dvars;
      return getConcatBody(i, j, vars, dvars);
    }

    void concatenateCHCs(int i, int j)
    {
      chcs.push_back(HornRuleExt());
      HornRuleExt* s = &chcs[i];
      HornRuleExt* d = &chcs[j];
      HornRuleExt* n = &chcs.back();
      if (debug >= 2)
      {
        outs () << "  Concatenating two CHCs: "
                << d->srcRelation << " -> " << d->dstRelation << " and "
                << s->srcRelation << " -> " << s->dstRelation << "\n";
      }
      n->srcRelation = d->srcRelation;
      n->dstRelation = s->dstRelation;
      n->srcVars = d->srcVars;
      Expr mergedBody = getConcatBody(i, j, n->locVars, n->dstVars);
      n->locVars.insert(n->locVars.end(), s->locVars.begin(), s->locVars.end());
      mergedBody = simpleQE(mergedBody, n->locVars);
      mergedBody = u.removeITE(mergedBody);
      n->body = fixpointRewrite(mergedBody,
        [](Expr e){ return
          simplifyArr(
          simpConstArr(
          simplifyBV(e)));});
      n->shrinkLocVars();
      n->dstVars = s->dstVars;
      n->isInductive = n->srcRelation == n->dstRelation;
      n->isFact = isOpX<TRUE>(n->srcRelation);
      n->isQuery = n->dstRelation == failDecl;
      chcsToCheck1.insert(chcs.size()-1);
      chcsToCheck2.insert(chcs.size()-1);
    }

    void removeTautologies()
    {
      for (int i = 0; i < chcs.size(); i++)
      {
        if (find(toEraseChcs.begin(), toEraseChcs.end(), i) != toEraseChcs.end())
          continue;

        auto h = &chcs[i];
        auto f = find(chcsToCheck2.begin(), chcsToCheck2.end(), i);
        if (f != chcsToCheck2.end())
        {
          if (u.isFalse(h->body))
          {
            if (debug >= 2)
              outs () << "  Eliminating CHC: " << h->srcRelation
                      << " -> " << h->dstRelation << "\n";
            if (debug >= 3)
            {
              outs () << "    its body is false: ";
              pprint(h->body);
              outs () << "\n";
            }
            toEraseChcs.insert(i);
            continue;
          }
          chcsToCheck2.erase(f);
        }

        bool found = false;
        if (found)
        {
          if (debug >= 2)
            outs () << "  Eliminating CHC: " << h->srcRelation
                    << " -> " << h->dstRelation << "\n";
          if (debug >= 3)
          {
            outs () << "    inductive but does not change vars: \n";
            pprint(h->body);
          }
          toEraseChcs.insert(i);
        }
        else ++h;
      }
    }

    void combineCHCs()
    {
      for (int i = 0; i < chcs.size(); i++)
      {
        if (find(toEraseChcs.begin(), toEraseChcs.end(), i) != toEraseChcs.end())
          continue;
        if (find(simplCHCs.begin(), simplCHCs.end(), i) != simplCHCs.end())
          continue;
        if (chcs[i].isQuery) continue;

        set<int> toComb = {i};
        HornRuleExt& s = chcs[i];
        for (int j = i + 1; j < chcs.size(); j++)
        {
          if (find(toEraseChcs.begin(), toEraseChcs.end(), j) != toEraseChcs.end())
            continue;
          if (find(simplCHCs.begin(), simplCHCs.end(), j) != simplCHCs.end())
            continue;

          HornRuleExt& d = chcs[j];
          if (s.srcRelation == d.srcRelation && s.dstRelation == d.dstRelation)
          {
            for (int k = 0; k < s.srcVars.size(); k++)
              assert (s.srcVars[k] == d.srcVars[k]);
            for (int k = 0; k < s.dstVars.size(); k++)
              assert (s.dstVars[k] == d.dstVars[k]);
            toComb.insert(j);
          }
        }
        if (toComb.size() > 1)
        {
          if (debug >= 2)
          {
            outs () << "    Disjoing bodies of " << toComb.size() << " CHCs: "
                    << s.srcRelation << " -> " << s.dstRelation << "\n";
          }
          ExprVector all;
          for (auto it = toComb.rbegin(); it != toComb.rend(); ++it)
          {
            auto h = chcs[*it];
            all.push_back(h.body);
            s.locVars.insert(s.locVars.end(), h.locVars.begin(), h.locVars.end());
            if (*it != i) toEraseChcs.insert(*it);
          }
          s.body = fixpointRewrite(distribDisjoin(all, m_efac),
            [](Expr e){ return simplifyBool(
                simplifyArr(
                simpConstArr(
                simplifyBV(e))));});
          chcsToCheck1.insert(i);
          chcsToCheck2.insert(i);
          return combineCHCs();
        }
      }
    }

    // helper for `moveMemVars`
    bool findVarsSetEnc (vector<vector<int>>& traces, ExprVector& lh, Expr var)
    {
      for (auto & a : traces)
      {
        for (int i : a)
        {
          auto & hr = chcs[i];
          auto it = find(lh.begin(), lh.end(), hr.dstRelation);
          if (it != lh.end())
          {
            lh.erase(it);
            if (findVarsSetEnc(cycles[hr.dstRelation], lh, var))
              return true;
          }

          for (auto & c : hr.lin)
            if (isOpX<EQ>(c) && c->left() == allocP &&
                isOpX<STORE>(c->right()) && c->right()->last() == var)
              return true;
        }
      }
      return false;
    }

    void moveMemVars(Expr z, ExprVector& names)
    {
      ExprSet posvars;
      for (auto & hr : chcs)
      {
        hr.lin.clear();
        getConj(hr.body, hr.lin); // prepares body conjuncts, all at once
        ExprVector se;
        filter (hr.body, IsSelect (), inserter(se, se.begin()));
        for (auto & s : se)
          if (s->left() == mem && is_bvnum(s->right()))
            posvars.insert(s->right());
      }

      for (auto it = posvars.begin(); it != posvars.end(); )
      {
        ExprVector loopheadsTmp = loopheads;
        if (findVarsSetEnc(acyclic, loopheadsTmp, *it))
          it = posvars.erase(it);
        else
          ++it;
      }

      for (auto & v : posvars)
      {
        Expr newVar = names[lexical_cast<int>(v->left()) - 1],
             newVarP = cloneVar(newVar,
                  mkTerm<string>(lexical_cast<string>(newVar) + "'", m_efac));
        scalarInds.push_back(v);
        for (auto & hr : chcs)
          moveVarCHC(hr, newVar, newVarP, z, v);
      }

      // clean after it's done, all at once
      for (auto & hr : chcs) hr.lin.clear();
    }

    void moveVarCHC(HornRuleExt& hr, Expr newVar, Expr newVarP, Expr z, Expr v)
    {
      if (debug > 2) outs () << "Moving " << newVar << " from memory in " <<
        hr.srcRelation << " -> " << hr.dstRelation << "\n";
      bool changed = false;
      ExprVector toAdd;
      for (auto it2 = hr.lin.begin(); it2 != hr.lin.end(); )
      {
        auto b = *it2, repl = mk<SELECT>(mk<SELECT>(mem, v), z),
             c = replaceAll(b, repl, newVar);
        bool repled = b != c;
        if (repled)
        {
          if (debug > 2)
            outs () << "  Replaced:\n    " << repl << "\n";
          addVar(hr.srcRelation, newVar);
        }

        bool varmoved = moveVarEq(hr, c, toAdd, newVar, newVarP, v);

        if (repled || varmoved)
        {
          if (debug > 2)
            outs () << "  Erased from the body:\n    " << *it2 << "\n";
          it2 = hr.lin.erase(it2);
        }
        else
          ++it2;
        if (repled && !varmoved)
          toAdd.push_back(c);
      }

      if (!toAdd.empty())
      {
        hr.lin.insert(toAdd.begin(), toAdd.end());
        if (debug > 2)
        {
          outs () << "  Extra constraints in the body:\n";
          pprint(toAdd, 4);
        }
      }

      hr.body = conjoin(hr.lin, m_efac);
    }

    bool moveVarEq(HornRuleExt& hr, Expr c, ExprVector& toAdd,
                 Expr newVar, Expr newVarP, Expr pv)
    {
      if (!isOpX<EQ>(c)) return false;
      if (memP != c->left() || !isOpX<STORE>(c->right())) return false;
      if (mem != c->right()->left() || pv != c->right()->right()) return false;

      auto l = c->right()->last();
      if (!isOpX<STORE>(l)) return false;
      if (!isOpX<SELECT>(l->left()) || !isZero(l->right())) return false;
      if (mem != l->left()->left() || pv != l->left()->right()) return false;

      toAdd.push_back(
          mk<EQ>(newVarP, simpextract(typeOf(newVarP), 0, l->last())));
      addVar(hr.dstRelation, newVar);
      hr.writeVars.insert(newVar);
      hr.writeVars.erase(mem);
      return true;
    }

    // front-end optimizations: helper methods for `eliminateTransitVars`
    bool exploreLoops (Expr decl, Expr var)
    {
      for (auto & a : cycles[decl])
      {
        for (int i : a)
        {
          auto & hr = chcs[i];
          if (hr.dstRelation != decl && contains(loopheads, hr.dstRelation))
            if (!exploreLoops(hr.dstRelation, var))
              return false;
          int b = 0;
          for (auto & t : transit[i])
          {
            if (t.first == var)
            {
              if (transused[i].first[var] + transused[i].second[t.second] == 0)
              {
                var = invVarsMap[hr.dstRelation][t.second];
                b = 1;
              }
              else return false;
            }
          }
          if (b == 0) return false;
        }
      }
      return true;
    }

    bool exploreAcyclic (Expr decl, Expr var)
    {
      for (auto & a : acyclic)
      {
        int use;
        bool met = false;
        for (int i : a)
        {
          auto & hr = chcs[i];
          met |= (hr.dstRelation == decl);
          if (!met) continue;

          if (contains(loopheads, hr.dstRelation))
            if (!exploreLoops(hr.dstRelation, var))
              return false;

          if (hr.dstRelation == decl &&
            (contains(singused[i], var)) || contains(notused[i], var))
          {
            use = contains(singused[i], var);
            continue;
          }

          int b = 0;
          for (auto & t : transit[i])
          {
            if (t.first == var)
            {
              use += transused[i].first[var] + transused[i].second[t.second];
              if (use <= 1)
              {
                var = invVarsMap[hr.dstRelation][t.second];
                b = 1;
              }
              else return false;
            }
          }
          if (b == 0) return false;
        }
      }
      return true;
    }

    void remTransLoops (Expr decl, Expr var)
    {
      for (auto & a : cycles[decl])
      {
        for (int i : a)
        {
          auto & hr = chcs[i];
          if (hr.dstRelation != decl && contains(loopheads, hr.dstRelation))
            remTransLoops(var, hr.dstRelation);

          for (auto & t : transit[i])
          {
            if (t.first == var)
            {
              if (!contains(tranRemoved, hr.dstRelation))
              {
                removeVar(hr.dstRelation, var);
                tranRemoved.insert(hr.dstRelation);
              }
              if (!contains(tranShrinked, i))
              {
                removeVar(i, var);
                tranShrinked.insert(i);
              }
              var = invVarsMap[hr.dstRelation][t.second];
              break;
            }
          }
        }
      }
    }

    void remTransAcyclic (Expr decl, Expr var)
    {
      for (auto & a : acyclic)
      {
        bool met = false;
        for (int i : a)
        {
          auto & hr = chcs[i];
          met |= (hr.dstRelation == decl);
          if (!met) continue;

          if (hr.dstRelation == decl)
          {
            removeVar(hr.dstRelation, var);
            removeVar(i, invVarsPrimeMap[decl][var]);
          }

          if (contains(loopheads, hr.dstRelation))
            remTransLoops(hr.dstRelation, var);

          for (auto & t : transit[i])
          {
            if (t.first == var)
            {
              if (!contains(tranRemoved, hr.dstRelation))
              {
                removeVar(hr.dstRelation, var);
                tranRemoved.insert(hr.dstRelation);
              }
              if (!contains(tranShrinked, i))
              {
                removeVar(i, var);
                removeVar(i, invVarsPrimeMap[hr.dstRelation][t.second]);
                tranShrinked.insert(i);
              }
              var = invVarsMap[hr.dstRelation][t.second];
              break;
            }
          }
        }
      }
    }

    void prepTransitVars(bool onlyTrans = true)
    {
      notused.clear();
      singused.clear();
      transit.clear();
      for (int i = 0; i < chcs.size(); i++)
      {
        auto & hr = chcs[i];
        hr.id = i;
        ExprSet cnjs;
        getConj(hr.body, cnjs);
        map<Expr, ExprSet> vars;      // preps
        for (auto & b : cnjs)
          filter(b, bind::IsConst(), std::inserter (vars[b], vars[b].begin ()));

        ExprSet transSrc, transDst;
        for (auto & b : cnjs)
        {
          if (isOpX<EQ>(b))
          {
            Expr l = b->left();
            Expr r = b->right();
            Expr s = NULL, d = NULL;
            if (contains(hr.srcVars, l) && contains(hr.dstVars, r))
              { s = l; d = r; }
            else if (contains(hr.srcVars, r) && contains(hr.dstVars, l))
              { s = r; d = l; }
            if (s != NULL && d != NULL)
            {
              transSrc.insert(s);
              transDst.insert(d);
              transit[i].insert({s, d});
              int f1 = 0, f2 = 0;
              for (auto b1 : cnjs)
              {
                if (b == b1) continue;
                if (contains(vars[b1], s)) f1++;
                if (contains(vars[b1], d)) f2++;
              }
              transused[i].first[s] = f1;
              transused[i].second[d] = f2;
            }
          }
        }

        if (onlyTrans) continue;

        for (auto & v : hr.srcVars)
        {
          if (contains(transSrc, v)) continue;
          int f = 0;
          for (auto & b : cnjs)
            if (contains(vars[b], v))
              f++;
          if (f == 0) notused[i].insert(v);
          if (f == 1) singused[i].insert(v);
        }

        for (auto & v : hr.dstVars)
        {
          if (contains(transDst, v)) continue;
          int f = 0;
          for (auto & b : cnjs)
            if (contains(vars[b], v))
              f++;
          auto w = invVarsMap[hr.dstRelation][v];
          if (f == 0) notused[i].insert(w);
          if (f == 1) singused[i].insert(w);
        }
      }
    }

    void eliminateTransitVars()
    {
      prepTransitVars(false);
      for (auto & a : newVars)
      {
        for (auto & v : a.second)
        {
          if (exploreAcyclic(a.first, v))
          {
            tranRemoved.clear();
            tranShrinked.clear();
            remTransAcyclic(a.first, v);
          }
        }
      }
      updateDecls();
      for (auto & hr : chcs)
      {
        hr.srcVars = invVars[hr.srcRelation];
        hr.dstVars = invVarsPrime[hr.dstRelation];
      }
    }

    // helper methods for `findCycles`
    void getAllTraces (Expr src, Expr dst, int len, vector<int> trace,
                vector<vector<int>>& traces, bool once = false)
    {
      if (len == 1)
      {
        for (auto a : outgs[src])
        {
          if (chcs[a].dstRelation == dst)
          {
            if (once && find(trace.begin(), trace.end(), a) != trace.end())
              continue;
            vector<int> newtrace = trace;
            newtrace.push_back(a);
            traces.push_back(newtrace);
          }
        }
      }
      else
      {
        for (auto a : outgs[src])
        {
          if (once && find(trace.begin(), trace.end(), a) != trace.end())
            continue;
          vector<int> newtrace = trace;
          newtrace.push_back(a);
          getAllTraces(chcs[a].dstRelation, dst, len-1, newtrace, traces, once);
        }
      }
    }

    bool isRelVisited(vector<int>& trace, ExprVector& av, Expr rel)
    {
      for (auto t : trace)
        if (chcs[t].dstRelation == rel)
          return true;
      return find(av.begin(), av.end(), rel) != av.end();
    }

    void getAllAcyclicTraces (Expr src, Expr dst, int len, vector<int> trace,
                  vector<vector<int>>& traces, ExprVector& av)
    {
      if (len == 1)
      {
        for (auto a : outgs[src])
        {
          if (chcs[a].dstRelation == dst)
          {
            vector<int> newtrace = trace;
            newtrace.push_back(a);
            traces.push_back(newtrace);
          }
        }
      }
      else
      {
        for (auto a : outgs[src])
        {
          if (chcs[a].dstRelation == dst ||
              isRelVisited(trace, av, chcs[a].dstRelation))
            continue;
          vector<int> newtrace = trace;
          newtrace.push_back(a);
          getAllAcyclicTraces(chcs[a].dstRelation, dst, len-1, newtrace, traces, av);
        }
      }
    }

    void findCycles()
    {
      ExprVector endRels;
      outgs.clear(); acyclic.clear(); cycles.clear(); allCHCs.clear();
      prefixes.clear(); seqPoints.clear(); wtoCHCs.clear();
      for (int i = 0; i < chcs.size(); i++)
      {
        outgs[chcs[i].srcRelation].push_back(i);
        if (chcs[i].isQuery) unique_push_back(chcs[i].dstRelation, endRels);
      }

      ExprVector av;
      for (auto & d : decls)
        if (outgs[d->left()].empty())
          endRels.push_back(d->left());

      for (auto & r : endRels)
        findCycles(mk<TRUE>(m_efac), r, av);

      if (debug > 0)
      {
        outs () << "global traces num: " << acyclic.size() << "\n";
        for (auto & a : cycles)
          outs () << "  traces num for: " << a.first << ": " << a.second.size() << "\n";
      }
      for (auto & a : acyclic)
      {
        if (seqPoints.empty())
          for (auto & i : a) seqPoints.push_back(chcs[i].dstRelation);
        else
          for (auto it = seqPoints.begin(); it != seqPoints.end(); )
          {
            bool f = false;
            for (auto & i : a)
              if (*it == chcs[i].dstRelation)
                { f = true; break;}
            if (f) ++it;
            else it = seqPoints.erase(it);
          }
        if (seqPoints.empty()) break;
      }
      cycleSearchDone = true;
    }

    bool findCycles(Expr src, Expr dst, ExprVector& avoid)
    {
      if (debug >= 2) outs () << "\nfindCycles:  " << src << " => " << dst << "\n";
      vector<vector<int>> nonCycleTraces;
      ExprVector highLevelRels;
      for (int i = 1; i <= chcs.size(); i++)
      {
        if (debug >= 2)
        {
          outs () << ".";
          outs().flush();
        }
        getAllAcyclicTraces(src, dst, i, vector<int>(), nonCycleTraces, avoid);
      }

      bool tracesFound = nonCycleTraces.size() > 0;
      map <Expr, vector<vector<int>>> prefs;
      for (auto & d : nonCycleTraces)
      {
        vector<int> tmp;
        for (auto & chcNum : d)
        {
          if (chcs[chcNum].isQuery) break;      // last iter anyway
          Expr& r = chcs[chcNum].dstRelation;
          tmp.push_back(chcNum);
          if (find(avoid.begin(), avoid.end(), r) == avoid.end())
          {
            prefs[r].push_back(tmp);
            unique_push_back(r, highLevelRels);
          }
        }
      }

      if (tracesFound)
        if (src == dst)
          for (auto & c : nonCycleTraces) unique_push_back(c, cycles[src]);
        else
          for (auto & c : nonCycleTraces) unique_push_back(c, acyclic);
      else
        assert(src == dst);

      ExprVector avoid2 = avoid;
      for (auto & d : highLevelRels)
      {
        avoid2.push_back(d);
        bool nestedCycle = findCycles(d, d, avoid2);
        if (nestedCycle)
        {
          prefixes[d] = prefs[d]; // to debug
        }
      }

      // WTO sorting is here now:
      if (tracesFound)
      {
        if (src == dst)
        {
          unique_push_back(src, loopheads);      // could there be duplicates?
          if (debug > 0) outs () << "  loophead found: " << src << "\n";
        }
        else if (debug > 0) outs () << "  global:\n";
      }

      for (auto c : nonCycleTraces)
      {
        if (debug > 5)
        {
          outs () << "    trace: " << chcs[c[0]].srcRelation;
          for (auto h : c)
            outs () << " -> " << chcs[h].dstRelation << " ";
          outs () << "\n";
        }
        // else if (debug)
        // {
        //   outs () << "traces num for " << chcs[c[0]].srcRelation << ": "
        //           << c.size() << "\n";
        // }

        for (auto h : c)
          unique_push_back(&chcs[h], wtoCHCs);
      }

      return tracesFound;
    }

    vector<int> getPrefix(Expr rel) // get only first one; to extend
    {
      assert(!cycles[rel].empty());
      assert(!prefixes[rel].empty());
      vector<int> pref = prefixes[rel][0];
      assert(!pref.empty());
      if (chcs[pref[0]].isFact)
        return pref;
      vector<int> ppref = getPrefix(chcs[pref[0]].srcRelation);
      ppref.insert(ppref.end(), pref.begin(), pref.end());
      return ppref;
    }

    bool hasCycles()
    {
      if (cycleSearchDone) return cycles.size() > 0;
      findCycles();
      return (cycles.size() > 0);
    }

    void addFailDecl(Expr decl)
    {
      if (failDecl == NULL)
        failDecl = decl;
      else
        if (failDecl != decl)
        {
          errs () << "Multiple queries are unsupported\n";
          exit (1);
        }
    }

    Expr getPrecondition (HornRuleExt* hr)
    {
      Expr tmp = keepQuantifiers(hr->body, hr->srcVars);
      return weakenForHardVars(tmp, hr->srcVars);
    }

    int getQum()
    {
      int n = 0;
      for (auto & c : chcs) if (c.isQuery) n++;
      return n;
    }

    void print (bool full = false, bool dump_cfg = false, string name = "chc")
    {
      std::ofstream enc_chc;
      if (dump_cfg)
      {
        enc_chc.open("chc.dot");
        enc_chc <<("digraph print {\n");
      }
      for (int i = 0; i < chcs.size(); i++)
      {
        if (find(toEraseChcs.begin(), toEraseChcs.end(), i) != toEraseChcs.end())
          continue;
        auto & hr = chcs[i];
        if (full)
        {
          if (hr.isFact) outs() << "  INIT:\n";
          else if (hr.isInductive) outs() << "  TR:\n";
          else if (hr.isQuery) outs() << "  BAD:\n";
          else outs() << "  CHC:\n";
        }

        outs () << "    " << * hr.srcRelation;
        if (full && hr.srcVars.size() > 0)
        {
          outs () << " (";
          pprint(hr.srcVars, 0, false);
          outs () << ")";
        }
        else outs () << "[#" << hr.srcVars.size() << "]";
        if (full) outs () << "\n         ";
        outs () << " -> " << * hr.dstRelation;

        if (full && hr.dstVars.size() > 0)
        {
          outs () << " (";
          pprint(hr.dstVars, 0, false);
          outs () << ")";
        }
        else outs () << "[#" << hr.dstVars.size() << "]";
        if (full)
        {
          outs() << "\n    body: \n";
          if (treeSize(hr.body) < 1000)
            pprint(hr.body, 4);
          else outs () << " < . . . . too large . . . . >\n";
        }
        else outs() << "\n";
        if (full && hr.locVars.size() > 0)
        {
          outs () << "    loc vars: ";
          pprint(hr.locVars, 0, false);
          outs () << "\n";
        }
        if (dump_cfg)
        {
          enc_chc << "\"" << hr.srcRelation << "\"";
          enc_chc << " -> ";
          enc_chc << "\"" << hr.dstRelation << "\"";
          enc_chc << '\n';
        }
      }
      if (dump_cfg)
      {
        enc_chc <<("}");
        enc_chc.close();
        // this needs a graphiz package installed:
        // system(string("dot -Tpdf -o " + name + ".pdf chc.dot").c_str());
      }
    }

    void serialize (string name = "chc")
    {
      std::ofstream enc_chc;
      enc_chc.open(name + ".smt2");
      enc_chc << "(set-logic HORN)\n";
      for (auto & d : decls)
      {
        enc_chc << "(declare-fun " << d->left() << " (";
        for (int i = 1; i < d->arity()-1; i++)
        {
          u.print(d->arg(i), enc_chc);
          if (i < d->arity()-2) enc_chc << " ";
        }
        enc_chc << ") Bool)\n";
      }
      enc_chc << "\n";
      enc_chc.flush();
      for (auto & c : chcs)
      {
        Expr src, dst;
        if (c.isFact)
        {
          src = mk<TRUE>(m_efac);
        }
        else
        {
          for (auto & d : decls)
          {
            if (d->left() == c.srcRelation)
            {
              src = fapp(d, c.srcVars);
              break;
            }
          }
        }

        if (c.isQuery)
        {
          dst = mk<FALSE>(m_efac);
        }
        else
        {
          for (auto & d : decls)
          {
            if (d->left() == c.dstRelation)
            {
              dst = fapp(d, c.dstVars);
              break;
            }
          }
        }
        if (c.isInductive) enc_chc << "; inductive\n";
        else if (c.isFact) enc_chc << "; fact\n";
        else if (c.isQuery) enc_chc << "; query\n";

        enc_chc << "(assert ";
        u.print(mkQFla(mk<IMPL>(mk<AND>(src, c.body), dst), true), enc_chc);
        enc_chc << ")\n\n";
      }
      enc_chc << "(check-sat)\n";
    }

    int getStat()
    {
      return u.getStat();
    }
  };
}


#endif
