#ifndef HORN__HPP__
#define HORN__HPP__

#include <fstream>
#include "ae/AeValSolver.hpp"
#include "SetEncoding.hpp"

using namespace std;
using namespace boost;

namespace ufo
{
  struct HornRuleExt
  {
    ExprVector srcVars;
    ExprVector dstVars;
    ExprVector locVars;

    ExprSet srcTmp;
    ExprSet dstTmp;

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
        if (isOpX<FAPP>(cnj))
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
      for (auto it = locVars.begin(); it != locVars.end();)
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
    vector<HornRuleExt*> allCHCs, wtoCHCs, dwtoCHCs;
    ExprVector wtoDecls;
    ExprSet decls;
    map<Expr, ExprVector> invVars,invVarsPrime;
    map<Expr, vector<int>> outgs;
    vector<vector<int>> prefixes, cycles;  // for cycles
    map<Expr, bool> hasArrays;
    bool hasAnyArrays, hasBV = false;
    int debug;
    set<int> chcsToCheck1, chcsToCheck2, toEraseChcs;
    int glob_ind = 0;
    ExprSet origVrs;
    map<Expr, ExprMap> origSrcVars;

    CHCs(ExprFactory &efac, EZ3 &z3, int d = false) :
      u(efac), m_efac(efac), m_z3(z3), hasAnyArrays(false), debug(d) {};

    bool isFapp (Expr e)
    {
      if (isOpX<FAPP>(e))
        if (e->arity() > 0)
          if (isOpX<FDECL>(e->left()))
            if (e->left()->arity() >= 2)
              return true;
      return false;
    }

    Expr getDeclByName(Expr a)
    {
      for (auto & d : decls)
        if (d->left() == a) return d;
      return NULL;
    }

    bool addedDecl(Expr a)
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
              !isOpX<BVSORT> (arg) && !isOpX<AD_TY>(arg))
          {
            errs() << "Argument #" << i << " of " << a << " is not supported\n";
            exit(1);
          }
          while (true)
          {
            Expr name = mkTerm<string> (varname + to_string(j), m_efac);
            Expr var = fapp (constDecl (name, arg));
            name = mkTerm<string> (lexical_cast<string>(name) + "'", m_efac);
            Expr varPr = fapp (constDecl (name, arg));
            j++;
            if (find(origVrs.begin(), origVrs.end(), var) != origVrs.end())
              continue;
            if (find(origVrs.begin(), origVrs.end(), varPr) != origVrs.end())
              continue;
            invVars[a->left()].push_back(var);
            invVarsPrime[a->left()].push_back(varPr);
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

    void parse(string smt, bool setEnc = true)
    {
      if (debug > 0) outs () << "\nPARSING" << "\n=======\n";
      std::unique_ptr<ufo::ZFixedPoint <EZ3> > m_fp;
      m_fp.reset (new ZFixedPoint<EZ3> (m_z3));
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
              if (r1 != replaceAll(r1, e.first, e.second))
                outs () << "rewriting " << (e.first)->left()
                        << " -> " << (e.second)->left() << "\n";
              r1 = replaceAll(r1, e.first, e.second);
            }
          }
          else
          {
             r1 = replaceAll(r, eqs);
          }
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
        {
          hr.body = r;
        }

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
            auto name_upd = mkTerm<string> (name_str, m_efac);
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

      Expr s = op::bv::bvsort (64, m_efac); // hardcode for now
      Expr alloc, mem, allocP, memP, off, offP, siz, sizP, aux, auxP;
      alloc = mkTerm<string> ("_alloc", m_efac);
      mem = mkTerm<string> ("_mem", m_efac);
      off = mkTerm<string> ("_off", m_efac);
      siz = mkTerm<string> ("_siz", m_efac);
      aux = mkTerm<string> ("_aux", m_efac);
      alloc = mkConst(alloc, mk<ARRAY_TY> (s, s));
      mem = mkConst(mem, mk<ARRAY_TY> (s, mk<ARRAY_TY> (s, s)));
      off = mkConst(off, mk<ARRAY_TY> (s, s));
      siz = mkConst(siz, mk<ARRAY_TY> (s, s));
      aux = mkConst(aux, mk<ARRAY_TY> (s, s));
      allocP = cloneVar(alloc, mkTerm<string> ("_alloc'", m_efac));
      memP = cloneVar(mem, mkTerm<string> ("_mem'", m_efac));
      offP = cloneVar(off, mkTerm<string> ("_off'", m_efac));
      sizP = cloneVar(siz, mkTerm<string> ("_siz'", m_efac));
      auxP = cloneVar(siz, mkTerm<string> ("_aux'", m_efac));

      if (failDecl == NULL) failDecl = mk<FALSE>(m_efac);
      HornRuleExt & hrprev = *chcs.begin();
      for (auto it = chcs.begin(); it != chcs.end();)
      {
        HornRuleExt & hr = *it;
        hr.splitBody();
        if (hr.srcRelation == NULL || addedDecl(hr.srcRelation)) ++it;
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
          Expr newName = mkTerm<string> (name + "_" + to_string(i), m_efac);
          HornRuleExt & hr2 = *it2;
          hr2.srcRelation = newName;
          decls.insert(replaceAll(d, d->left(), newName));
          invVars[hr2.srcRelation] = invVars[hr.srcRelation];
          invVarsPrime[hr2.srcRelation] = invVarsPrime[hr.srcRelation];
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
      eliminateVacDecls();
      if (debug > 0)
        outs () << "Vacuity elimination: " << sz << " -> " << chcs.size() << "\n";

      // set encoding is here
      ExprVector names;
      vector<HornRuleExt> tmp;
      for (auto & hr : chcs)
      {
        if (debug >= 3)
          outs () << "\n\n PROC " << hr.srcRelation << " -> " << hr.dstRelation << "\n";
        ExprSet rlin;
        ExprSet checks;
        if (setEnc)
        {
          for (auto & l : hr.lin)
          {
            if (debug >= 3)
              outs () << "\n------\n  orig: " << l << "\n";
            Expr rl = l;
            if (!hr.isFact) rl = evalReplace(rl, hr.srcTmp,
              alloc, mem, off, siz, aux, allocP, memP, offP, sizP, auxP, names);
            rl = rewriteSet(rl, hr.dstTmp,
              alloc, mem, off, siz, aux, allocP, memP, offP, sizP, auxP, names);
            rl = typeRepair(rl);
            rl = simplifyBool(rl);
            if (debug >= 3)
              outs () << "  conv: " << rl << "\n";
            if (!containsOp<AD_TY>(rl))
            {
              getConj(rl, rlin);

              // memsafety checks generation
              ExprSet selstors;
              filter (rl, IsSelect (), inserter(selstors, selstors.begin()));
              filter (rl, IsStore (), inserter(selstors, selstors.begin()));
              for (auto & s : selstors)
                if (isOpX<SELECT>(s->left()) && mem == s->left()->left())
                  checks.insert(mk<BULT>(s->right(),
                    mk<SELECT>(siz, mk<SELECT>(alloc, s->left()->right()))));
            }
          }
          hr.body = conjoin(rlin, m_efac);
        }
        else
        {
          hr.body = conjoin(hr.lin, m_efac);
        }

        // memsafety checks insertion
        for (auto & c : checks)
        {
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

      ExprSet allVars = {alloc, mem, off, siz, aux};
      ExprVector allVarsV, allVarsVp;
      if (setEnc)   // set-encoding
      {
        for (auto & hr : chcs)
        {
          allVars.insert(hr.srcTmp.begin(), hr.srcTmp.end());
          allVars.insert(hr.dstTmp.begin(), hr.dstTmp.end());
        }

        minusSets(allVars, names);

        for (auto & a : allVars)
        {
          allVarsV.push_back(a);
          allVarsVp.push_back(cloneVar(a,
                       mkTerm<string> (lexical_cast<string>(a) + "'", m_efac)));
        }
        ExprSet updDecls;
        for (auto & d : decls)
        {
          ExprVector args;
          args.push_back(*d->args_begin ());
          for (auto & a : allVars) args.push_back(a->left()->last());
          args.push_back(d->last());
          updDecls.insert(mknary<FDECL>(args));
        }
        decls = updDecls;
      }

      for (auto & hr : chcs)
      {
        ExprSet lin;
        getConj(hr.body, lin);

        if (setEnc)   // set-encoding
        {

          if (!hr.isFact)
          {
            hr.srcVars = allVarsV;
          }
          else
          {
            hr.locVars.insert(hr.locVars.end(), allVarsV.begin(), allVarsV.end());
          }

          if (!hr.isQuery)
          {
            hr.dstVars = allVarsVp;
            invVars[hr.dstRelation] = allVarsV;
            invVarsPrime[hr.dstRelation] = allVarsVp;
          }
          else
          {
            hr.locVars.insert(hr.locVars.end(), allVarsVp.begin(), allVarsVp.end());
          }
          if (hr.isFact)
          {
            Expr cArr = mk<CONST_ARRAY>(s, op::bv::bvnum(mkMPZ(0, m_efac), s));
            // assume that this fact does not rewrite _alloc
            lin.insert(mk<EQ>(allocP, cArr));
            lin.insert(mk<EQ>(auxP, cArr));
            lin.insert(mk<EQ>(offP, cArr));
          }
          else if (!hr.isQuery)
          {
            for (int i = 0; i < allVarsV.size(); i++)
            {
              if (find(hr.dstTmp.begin(), hr.dstTmp.end(), allVarsV[i])
                                                            == hr.dstTmp.end())
                lin.insert(mk<EQ>(allVarsVp[i], allVarsV[i]));
            }
          }
          hr.body = conjoin(lin, m_efac);
        }
        hr.body = simpleQE(hr.body, hr.locVars);
      }
      for (int i = 0; i < chcs.size(); i++)
      {
        outgs[chcs[i].srcRelation].push_back(i);
        allCHCs.push_back(&chcs[i]);
      }

      if (debug >= 0)
      {
        outs () << "  Parsed CHCs:\n";
        print(debug >= 1, true);
      }

      // TODO: wto and cycles to be completely rewritten
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

      allCHCs.clear();
      for (int i = 0; i < chcs.size(); i++)
      {
        outgs[chcs[i].srcRelation].push_back(i);
        allCHCs.push_back(&chcs[i]);
      }

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
    void eliminateVacDecls()
    {
      bool toRepeat = false;
      for (auto it = decls.begin(); it != decls.end(); )
      {
        auto & d = *it;
        vector<int> outgs;
        bool toDel = true;
        for (int i = 0; i < chcs.size(); i++)
        {
          if (chcs[i].dstRelation == d->left())
          {
            toDel = false;
            break;
          }
          if (chcs[i].srcRelation == d->left())
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
      if (toRepeat) eliminateVacDecls();
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

      for (auto d = decls.begin(); d != decls.end();)
      {
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
            emptyIntersect(simplCHCs, src) && emptyIntersect(simplCHCs, dst) &&
            (src.size() == 1 || dst.size() == 1))
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
        // remove unrelated constraints and shrink arities of predicates
        // currently disabled
        // if (!hasAnyArrays) slice();

        int preComb = (chcs.size() - toEraseChcs.size());
        combineCHCs();
        if (preComb > (chcs.size() - toEraseChcs.size()))
          return eliminateDecls();
      }
      return true;
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
      n->dstVars = d->dstVars;

      ExprVector newVars;
      for (int i = 0; i < d->dstVars.size(); i++)
      {
        Expr new_name = mkTerm<string> ("__bnd_var_" +
          to_string(glob_ind++), m_efac);
        newVars.push_back(cloneVar(d->dstVars[i], new_name));
      }
      Expr mergedBody = replaceAll(s->body, s->srcVars, newVars);
      n->dstVars.insert(n->dstVars.end(), d->locVars.begin(), d->locVars.end());
      for (int i = 0; i < d->locVars.size(); i++)
      {
        Expr new_name = mkTerm<string> ("__loc_var_" +
          to_string(glob_ind++), m_efac);
        newVars.push_back(cloneVar(d->locVars[i], new_name));
      }

      mergedBody = mk<AND>(replaceAll(d->body, n->dstVars, newVars), mergedBody);
      n->locVars = newVars;
      n->locVars.insert(n->locVars.end(), s->locVars.begin(), s->locVars.end());
      n->body = simpleQE(mergedBody, n->locVars);
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
            all.push_back(chcs[*it].body);
            if (*it != i) toEraseChcs.insert(*it);
          }
          s.body = distribDisjoin(all, m_efac);
          chcsToCheck1.insert(i);
          chcsToCheck2.insert(i);
          return combineCHCs();
        }
      }
    }

    // (recursive) multi-stage slicing begins here
    set<int> chcsToVisit;
    map<Expr, ExprSet> varsSlice;

    void updateTodo(Expr decl, int num)
    {
      for (int i = 0; i < chcs.size(); i++)
      {
        if (find(toEraseChcs.begin(), toEraseChcs.end(), i) != toEraseChcs.end())
          continue;

        if (i != num &&
            !chcs[i].isQuery &&
            (chcs[i].srcRelation == decl || chcs[i].dstRelation == decl))
              chcsToVisit.insert(i);
      }
    }

    void slice()
    {
      chcsToVisit.clear();
      varsSlice.clear();
      // first, compute sets of dependent variables
      for (int i = 0; i < chcs.size(); i++)
      {
        if (find(toEraseChcs.begin(), toEraseChcs.end(), i) != toEraseChcs.end())
          continue;

        if (chcs[i].isQuery)
        {
          chcs[i].body = keepQuantifiers(chcs[i].body, chcs[i].srcVars);
          Expr decl = chcs[i].srcRelation;
          filter (chcs[i].body, bind::IsConst(),
            std::inserter (varsSlice[decl], varsSlice[decl].begin ()));
          updateTodo(chcs[i].srcRelation, i);
        }
      }
      while (!chcsToVisit.empty()) slice(*chcsToVisit.begin());

      // now, prepare for variable elimination
      for (auto & d : varsSlice)
      {
//      if (invVars[d.first].size() > d.second.size())
//        outs () << "sliced for " << *d.first << ": " << invVars[d.first].size()
//                << " -> "    << d.second.size() << "\n";
        ExprSet varsPrime;
        for (auto & v : d.second)
        {
          Expr pr = replaceAll(v, invVars[d.first], invVarsPrime[d.first]);
          varsPrime.insert(pr);
        }

        keepOnly(invVars[d.first], d.second);
        keepOnly(invVarsPrime[d.first], varsPrime);
      }

      // finally, update bodies and variable vectors
      for (int i = 0; i < chcs.size(); i++)
      {
        if (find(toEraseChcs.begin(), toEraseChcs.end(), i) != toEraseChcs.end())
          continue;
        auto & c = chcs[i];

        if (u.isFalse(c.body) || u.isTrue(c.body)) continue;

        ExprSet bd;
        getConj(c.body, bd);
        for (auto b = bd.begin(); b != bd.end();)
        {
          if (emptyIntersect(*b, invVars[c.srcRelation]) &&
              emptyIntersect(*b, invVarsPrime[c.dstRelation]))
            b = bd.erase(b);
          else ++b;
        }
        if (!c.isFact) c.srcVars = invVars[c.srcRelation];
        if (!c.isQuery) c.dstVars = invVarsPrime[c.dstRelation];
        c.body = conjoin(bd, m_efac);
      }
    }

    void slice(int num)
    {
      HornRuleExt* hr = &chcs[num];
      assert (!hr->isQuery);
      ExprSet srcCore, dstCore, srcDep, dstDep, varDeps, cnjs;
      auto & dst = hr->dstVars;
      auto & src = hr->srcVars;

      if (qeUnsupported(hr->body))
      {
        varDeps.insert(src.begin(), src.end());
        varDeps.insert(hr->locVars.begin(), hr->locVars.end());
        varDeps.insert(dst.begin(), dst.end());
      }
      else
      {
        // all src vars from the preconditions are dependent
        varDeps = varsSlice[hr->srcRelation];
        filter (getPrecondition(hr), bind::IsConst(),
                      std::inserter (varDeps, varDeps.begin ()));

        for (auto & v : varsSlice[hr->dstRelation])
          varDeps.insert(replaceAll(v, invVars[hr->dstRelation], dst));

        srcCore = varsSlice[hr->dstRelation];
        dstCore = varDeps;

        getConj(hr->body, cnjs);
        while(true)
        {
          int vars_sz = varDeps.size();
          for (auto & c : cnjs)
          {
            ExprSet varsCnj;
            filter (c, bind::IsConst(),
                          std::inserter (varsCnj, varsCnj.begin ()));
            if (!emptyIntersect(varDeps, varsCnj))
              varDeps.insert(varsCnj.begin(), varsCnj.end());
          }
          if (hr->isInductive)
          {
            for (auto & v : varDeps)
            {
              varDeps.insert(replaceAll(v, dst, src));
              varDeps.insert(replaceAll(v, src, dst));
            }
          }
          if (vars_sz == varDeps.size()) break;
        }
      }

      bool updateSrc = false;
      bool updateDst = false;
      if (!hr->isFact)
      {
        ExprSet& srcVars = varsSlice[hr->srcRelation];
        for (auto v = varDeps.begin(); v != varDeps.end();)
        {
          if (find(src.begin(), src.end(), *v) != src.end())
          {
            if (find(srcVars.begin(), srcVars.end(), *v) == srcVars.end())
            {
              updateSrc = true;
              srcVars.insert(*v);
            }
            v = varDeps.erase(v);
          }
          else ++v;
        }
      }

      srcDep = varsSlice[hr->srcRelation];
      dstDep = varDeps;

      if (!hr->isQuery)
      {
        ExprSet& dstVars = varsSlice[hr->dstRelation];
        for (auto v = varDeps.begin(); v != varDeps.end();)
        {
          if (find(dst.begin(), dst.end(), *v) != dst.end())
          {
            Expr vp = replaceAll(*v, dst, invVars[hr->dstRelation]);
            if (find(dstVars.begin(), dstVars.end(), vp) == dstVars.end())
            {
              updateDst = true;
              dstVars.insert(vp);
            }
            v = varDeps.erase(v);
          }
          else ++v;
        }
      }

      if (!varDeps.empty())
        hr->body = eliminateQuantifiers(hr->body, varDeps, false);

      if (updateSrc) updateTodo(hr->srcRelation, num);
      if (updateDst) updateTodo(hr->dstRelation, num);
      chcsToVisit.erase(num);
    }

    bool hasCycles()
    {
      if (cycles.size() > 0) return true;

      for (int i = 0; i < chcs.size(); i++)
        if (chcs[i].isFact) findCycles(i, vector<int>());

      assert (cycles.size() == prefixes.size());
      if (debug >= 3)
        for (int i = 0; i < cycles.size(); i++)
        {
          auto & c = prefixes[i];
          outs () << "      pref: ";
          for (auto & chcNum : c) outs () << *chcs[chcNum].srcRelation << " -> ";
          outs () << "    [";
          for (auto & chcNum : c) outs () << chcNum << " -> ";
          outs () << "]  ";
          auto & d = cycles[i];
          outs () << "      cycle: ";
          for (auto & chcNum : d) outs () << *chcs[chcNum].srcRelation << " -> ";
          outs () << "    [";
          for (auto & chcNum : d) outs () << chcNum << " -> ";
          outs () << "]\n";
        }
      return (cycles.size() > 0);
    }

    void findCycles(int chcNum, vector<int> vec)
    {
      Expr srcRel = chcs[chcNum].srcRelation;
      Expr dstRel = chcs[chcNum].dstRelation;
      bool res = false;
      for (int i = 0; i < vec.size(); i++)
      {
        auto c = vec[i];
        bool newCycle = (chcs[c].srcRelation == srcRel);
        // TODO: some cycles can be redundant
        if (newCycle)
        {
          cycles.push_back(vector<int>());
          prefixes.push_back(vector<int>());
          for (int j = 0; j < i; j++) prefixes.back().push_back(vec[j]);
          res = true;
        }
        if (res)
        {
          cycles.back().push_back(c);
        }
      }

      if (! res)
      {
        vec.push_back(chcNum);

        for (auto & i : outgs[dstRel])
        {
          if (chcs[i].dstRelation == failDecl) continue;
          bool newRel = true;
          for (auto & c : cycles)
          {
            if (c[0] == i)
            {
              newRel = false;
              break;
            }
          }
          if (newRel) findCycles(i, vec);
        }
      }
    }

    vector<int> empt;
    vector<int>& getCycleForRel(Expr rel)
    {
      for (auto & c : cycles)
        if (chcs[c[0]].srcRelation == rel)
          return c;
      return empt;
    }

    vector<int>& getCycleForRel(int chcNum)
    {
      return getCycleForRel(chcs[chcNum].srcRelation);
    }

    HornRuleExt* getFirstRuleOutside (Expr rel)
    {
      for (auto & c : cycles)
      {
        if (chcs[c[0]].srcRelation == rel)
        {
          for (auto & a : outgs[rel])
          {
            if (a != c.back()) return &chcs[a];
          }
        }
      }
      return NULL;
    }

    void addRule (HornRuleExt* r)
    {
      chcs.push_back(*r);
      Expr srcRel = r->srcRelation;
      if (!isOpX<TRUE>(srcRel))
      {
        if (invVars[srcRel].size() == 0)
        {
          addDeclAndVars(srcRel, r->srcVars);
        }
      }
      outgs[srcRel].push_back(chcs.size()-1);
    }

    void addDeclAndVars(Expr rel, ExprVector& args)
    {
      ExprVector types;
      for (auto &var: args) {
        types.push_back(bind::typeOf(var));
      }
      types.push_back(mk<BOOL_TY>(m_efac));

      decls.insert(bind::fdecl (rel, types));
      for (auto & v : args)
      {
        invVars[rel].push_back(v);
      }
    }

    void addFailDecl(Expr decl)
    {
      if (failDecl == NULL)
      {
        failDecl = decl;
      }
      else
      {
        if (failDecl != decl)
        {
          errs () << "Multiple queries are unsupported\n";
          exit (1);
        }
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

    // TODO: repair cycles
    void wtoSort()
    {
      wtoCHCs.clear();
      wtoDecls.clear();

      // workaround, until cycles are repaired
      for (auto & c : chcs) wtoCHCs.push_back(&c);
      for (auto & c : decls) wtoDecls.push_back(c);

      cycles.clear();
      prefixes.clear();
      hasCycles();
      return;

      int r1 = 0;

      for (auto & c : cycles)
      {
        unique_push_back(chcs[c[0]].srcRelation, wtoDecls);
        for (int i = 1; i < c.size(); i++)
        {
          unique_push_back(chcs[c[i]].dstRelation, wtoDecls);
          unique_push_back(&chcs[c[i]], wtoCHCs);
        }
      }

      int r2 = wtoDecls.size();
      if (r2 == 0) return;

      while (r1 != r2)
      {
        for (int i = r1; i < r2; i++)
        {
          auto dcl = wtoDecls[i];
          for (auto &hr : chcs)
          {
            if (find(wtoCHCs.begin(), wtoCHCs.end(), &hr) != wtoCHCs.end())
              continue;

            if (hr.srcRelation == dcl)
            {
              unique_push_back(hr.dstRelation, wtoDecls);
              unique_push_back(&hr, wtoCHCs);
            }
            else if (hr.dstRelation == dcl)
            {
              unique_push_back(hr.srcRelation, wtoDecls);
              unique_push_back(&hr, wtoCHCs);
            }
          }
        }
        r1 = r2;
        r2 = wtoDecls.size();
      }
      assert(wtoCHCs.size() == chcs.size());

      // filter wtoDecls
      for (auto it = wtoDecls.begin(); it != wtoDecls.end();)
      {
        if (*it == failDecl || isOpX<TRUE>(*it)) it = wtoDecls.erase(it);
        else ++it;
      }
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
  };
}


#endif
