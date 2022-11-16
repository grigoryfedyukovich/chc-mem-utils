#ifndef EXPRSIMPLBV__HPP__
#define EXPRSIMPLBV__HPP__
#include <assert.h>

#include "ExprSimpl.hpp"

using namespace std;
using namespace expr::op::bind;
using namespace expr::op::bv;
using namespace boost;
using namespace boost::multiprecision;

namespace ufo
{
  template<typename Range> static Expr eraseLexicogrMinimal(Range& exprs)
  {
    auto cur = exprs.begin();
    for (auto it = std::next(cur); it != exprs.end(); ++it)
      if (lexical_cast<string>(*cur) > lexical_cast<string>(*it)) cur = it;
    Expr res = *cur;
    exprs.erase(cur);
    return res;
  }

  template<typename Range> static Expr mkbadd(Range& terms, Expr ty, ExprFactory &efac){
    if (terms.empty()) return bvnum(mkMPZ (0, efac), ty);
    if (terms.size() == 1) return *terms.begin();
    Expr tmp = mk<BADD>(eraseLexicogrMinimal(terms), eraseLexicogrMinimal(terms));
    while (!terms.empty())
      tmp = mk<BADD>(tmp, eraseLexicogrMinimal(terms));
    return tmp;
  }

  inline static Expr extractVal (Expr a, Expr k)
  {
    if (!is_bvnum(k)) return NULL;

    if (isOpX<CONST_ARRAY>(a))
    {
      return a->last();
    }
    else if (isOpX<STORE>(a))
    {
      if (!is_bvnum(a->right())) return NULL;
      if (!is_bvnum(a->last())) return NULL;
      if (a->right() == k) return a->last();
      return extractVal(a->left(), k);
    }
    return NULL;
  }

  inline static Expr evalSelect (Expr e)
  {
    if (isOpX<SELECT>(e))
      return extractVal(e->left(), e->right());
    return NULL;
  }

  inline static bool evalSelectEq(Expr e)
  {
    // sound only in returned true
    if (isOpX<EQ>(e))
    {
       Expr l = evalSelect(e->left());
       Expr r = evalSelect(e->right());
       if (l == NULL) l = e->left();
       if (r == NULL) r = e->right();
       return (l == r);
    }
    return false;
  }

  inline static Expr ssext (Expr v, Expr bvs)
  {
    if (isOpX<BSEXT>(v))
    {
      assert(width(v->last()) <= width(bvs));
      return ssext(v->left(), bvs);
    }
    return mk<BSEXT> (v, bvs);
  }

  inline static void getBAddTerm (Expr a, ExprVector &ptrms, ExprVector &ntrms)
  {
    if (isOpX<BSEXT>(a))
    {
      Expr bvs = a->last();
      ExprVector p, n;
      getBAddTerm(a->left(), p, n);
      for (auto & t : p) ptrms.push_back(ssext(t, bvs));
      for (auto & t : n) ntrms.push_back(ssext(t, bvs));
    }
    else if (isOpX<BADD>(a))
    {
      for (auto it = a->args_begin (); it != a->args_end (); ++it)
      {
        getBAddTerm(*it, ptrms, ntrms);
      }
    }
    else if (isOpX<BSUB>(a))
    {
      assert(a->arity() == 2);
      getBAddTerm(a->left(), ptrms, ntrms);
      getBAddTerm(a->right(), ntrms, ptrms);
    }
    else
    {
      ptrms.push_back(a);
    }
  }

  bool isZero(Expr t)
  {
    if (isOpX<BMUL>(t) && t->arity() == 2)
    {
      if (lexical_cast<cpp_int>(t->left()->left()) == 0)
        return true;
    }
    else if (isOpX<BEXTRACT>(t))
      return isZero(t->last());
    else if (isOpX<BSEXT>(t) || isOpX<BZEXT>(t))
      return isZero(t->left());
    return false;
  }

  inline cpp_int filterVec(ExprVector& v)
  {
    cpp_int comm = 0;
    int sz = v.size();
    for (auto it = v.begin(); it != v.end(); )
    {
      if (is_bvnum(*it))
      {
        comm += lexical_cast<cpp_int>((*it)->left());
        it = v.erase(it);
        continue;
      }
      else if (isZero(*it))
      {
        it = v.erase(it);
        continue;
      }
      ++it;
    }
    return comm;
  }

  inline static void simplifyBTerm (Expr a, ExprVector& ptrms,
      ExprVector& ntrms, cpp_int & comm)
  {
    getBAddTerm(a, ptrms, ntrms);
    comm += filterVec(ptrms);
    comm -= filterVec(ntrms);
  }

  template<typename OP> static Expr repairBComp (Expr l, Expr r)
  {
    auto & efac = l->getFactory();
    ExprVector ptrms, ntrms;
    cpp_int comm = 0;
    simplifyBTerm(r, ntrms, ptrms, comm);
    comm = -comm;
    simplifyBTerm(l, ptrms, ntrms, comm);

    for (auto it1 = ptrms.begin(); it1 != ptrms.end();)
    {
      bool toCont = false;
      for (auto it2 = ntrms.begin(); it2 != ntrms.end();)
      {
        if (*it1 == *it2)
        {
          it2 = ntrms.erase(it2);
          toCont = true;
          break;
        }
        else ++it2;
      }
      if (toCont)
      {
        it1 = ptrms.erase(it1);
        continue;
      }
      else ++it1;
    }

    Expr ty = typeOf(l);
    if (ptrms.empty() && ntrms.empty())
    {
      Expr tmp = mk<OP>(l, l);
      if (comm == 0)
      {
        if (isOpX<EQ>(tmp) || isOpX<BULE>(tmp) || isOpX<BUGE>(tmp) ||
                              isOpX<BSLE>(tmp) || isOpX<BSGE>(tmp))
          return mk<TRUE>(efac);
        else return mk<FALSE>(efac);
      }
      if (isOpX<EQ>(tmp)) return mk<FALSE>(efac);
    }

    if (comm > 0) ptrms.push_back(bvnum(mkMPZ(comm, efac), ty));
    if (comm < 0) ntrms.push_back(bvnum(mkMPZ(-comm, efac), ty));
    return mk<OP>(mkbadd(ptrms, ty, efac), mkbadd(ntrms, ty, efac));
  }

  inline static Expr repairBPrio (Expr e, Expr lhs)
  {
    auto & efac = e->getFactory();
    ExprVector ptrms, ntrms;
    cpp_int comm = 0;
    simplifyBTerm(e->right(), ntrms, ptrms, comm);
    comm = -comm;
    simplifyBTerm(e->left(), ptrms, ntrms, comm);

    Expr ty = typeOf(e->left());
    for (auto pit = ptrms.begin(); pit != ptrms.end(); )
    {
      if (contains(*pit, lhs)) pit++;
      else
      {
        ntrms.push_back(mk<BMUL>(bvnum(mkMPZ(-1, efac), ty), *pit));
        pit = ptrms.erase(pit);
      }
    }
    if (ptrms.size() != 1) return NULL;

    if (comm != 0) ntrms.push_back(bvnum(mkMPZ(-comm, efac), ty));
    return reBuildCmp(e, mkbadd(ptrms, ty, efac), mkbadd(ntrms, ty, efac));
  }

  struct SimplifyBVExpr
  {
    SimplifyBVExpr () {};

    // just started here; to extend
    Expr operator() (Expr exp)
    {
      if (isOpX<EQ>(exp))
      {
        if (is_bvnum(exp->left()) && is_bvnum(exp->right()))
        {
          if (exp->left() == exp->right())
          {
            return mk<TRUE>(exp->getFactory());
          }
          else
          {
            return mk<FALSE>(exp->getFactory());
          }
        }
      }
      if (isOpX<NEQ>(exp))
      {
        if (is_bvnum(exp->left()) && is_bvnum(exp->right()))
        {
          if (exp->left() == exp->right())
          {
            return mk<FALSE>(exp->getFactory());
          }
          else
          {
            return mk<TRUE>(exp->getFactory());
          }
        }
      }
      if (isOpX<NEG>(exp))
      {
        return mkNeg(exp->left());
      }
      if (isOpX<EQ>(exp) || isOpX<NEQ>(exp) ||
          isOpX<BULT>(exp) || isOpX<BSLT>(exp) ||
          isOpX<BULE>(exp) || isOpX<BSLE>(exp) ||
          isOpX<BUGT>(exp) || isOpX<BSGT>(exp) ||
          isOpX<BUGE>(exp) || isOpX<BSGE>(exp))
      {
        if ((isOpX<BSEXT>(exp->left()) && isOpX<BSEXT>(exp->right())) ||
            (isOpX<BZEXT>(exp->left()) && isOpX<BZEXT>(exp->right())))
          if (width(typeOf(exp->left()->left())) ==
              width(typeOf(exp->right()->left())))
              return reBuildCmp(exp, exp->left()->left(), exp->right()->left());
      }
      if (isOpX<BEXTRACT>(exp))
      {
        if (isOpX<BEXTRACT>(exp->last()))
          // to extend
          if (exp->left()  == exp->last()->left() &&
              exp->right() == exp->last()->right())
              return exp->last();

        if (width(typeOf(exp->last())) == high(exp) - low(exp) + 1)
          return exp->last();
      }
      if (isOpX<BADD>(exp) && exp->arity() == 2)
      {
        if (is_bvnum(exp->left()) && lexical_cast<cpp_int>(exp->left()->left()) == 0)
          return exp->right();
        if (is_bvnum(exp->right()) && lexical_cast<cpp_int>(exp->right()->left()) == 0)
          return exp->left();
        ExprVector terms;
        // to extend...
        for (auto it = exp->args_begin (), end = exp->args_end (); it != end; ++it)
          terms.push_back(*it);
        return mkbadd(terms, typeOf(exp), exp->getFactory());
      }
      if (isOpX<BMUL>(exp) && exp->arity() == 2)
      {
        if (is_bvnum(exp->left()) && lexical_cast<cpp_int>(exp->left()->left()) == 1)
          return exp->right();
        if (is_bvnum(exp->right()) && lexical_cast<cpp_int>(exp->right()->left()) == 1)
          return exp->left();
      }
      if (isOpX<BMUL>(exp) && exp->arity() == 2)
      {
        if (is_bvnum(exp->left()) && lexical_cast<cpp_int>(exp->left()->left()) == 0)
          return exp->left();
        if (is_bvnum(exp->right()) && lexical_cast<cpp_int>(exp->right()->left()) == 0)
          return exp->right();
      }
      return exp;
    }
  };

  inline static Expr simplifyBV (Expr exp)
  {
    RW<SimplifyBVExpr> rw(new SimplifyBVExpr());
    return dagVisit (rw, exp);
  }

  template <typename OP> static Expr rep(Expr exp)
  {
    // `isOpX<OP>(exp)` should hold at most once,
    // so types and widths are computed at most once too
    if (isOpX<OP>(exp) && exp->arity() == 2)
    {
      Expr t1 = typeOf(exp->left());
      Expr t2 = typeOf(exp->right());
      if (isOpX<BVSORT>(t1) && isOpX<BVSORT>(t2))
      {
        int w1 = width(t1);
        int w2 = width(t2);
        if (w1 > w2)
          exp = mk<OP>(exp->left(), sext(exp->right(), w1));
        else if (w2 > w1)
          exp = mk<OP>(sext(exp->left(), w2), exp->right());
      }
    }
    return exp;
  }

  struct TypeRep
  {
    TypeRep () {}
    Expr operator() (Expr exp)
    {
      exp = rep<EQ>(exp);
      exp = rep<NEQ>(exp);
      exp = rep<BULT>(exp);
      exp = rep<BSLT>(exp);
      exp = rep<BULE>(exp);
      exp = rep<BSLE>(exp);
      exp = rep<BUGT>(exp);
      exp = rep<BSGT>(exp);
      exp = rep<BUGE>(exp);
      exp = rep<BSGE>(exp);
      exp = rep<BAND>(exp);
      exp = rep<BOR>(exp);
      exp = rep<BADD>(exp);
      exp = rep<BSUB>(exp);
      exp = rep<BMUL>(exp);
      exp = rep<BUDIV>(exp);
      exp = rep<BSDIV>(exp);
      exp = rep<BUREM>(exp);
      exp = rep<BSREM>(exp);

      if (isOpX<SELECT>(exp) && typeOf(exp->left())->left() != typeOf(exp->right()))
      {
        int w1 = width(typeOf(exp->left())->left());
        int w2 = width(typeOf(exp->right()));
        if (w1 > w2)
          exp = mk<SELECT>(exp->left(), sext(exp->right(), w1));
        else
          assert(0);
      }
      if (isOpX<STORE>(exp) && typeOf(exp->left())->left() != typeOf(exp->right()))
      {
        int w1 = width(typeOf(exp->left())->left());
        int w2 = width(typeOf(exp->right()));
        if (w1 > w2)
          exp = mk<STORE>(exp->left(), sext(exp->right(), w1), exp->last());
        else
          assert(0);
      }
      if (isOpX<STORE>(exp) && typeOf(exp->left())->right() != typeOf(exp->last()))
      {
        int w1 = width(typeOf(exp->left())->left());
        int w2 = width(typeOf(exp->last()));
        if (w1 > w2)
          exp = mk<STORE>(exp->left(), exp->right(), sext(exp->last(), w1));
        else
          assert(0);
      }
      return exp;
    }
  };

  inline static Expr typeRepair (Expr exp)
  {
    RW<TypeRep> rw(new TypeRep());
    return dagVisit (rw, exp);
  }
}

#endif
