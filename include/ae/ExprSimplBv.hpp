#ifndef EXPRSIMPLBV__HPP__
#define EXPRSIMPLBV__HPP__
#include <assert.h>

#include "ExprSimpl.hpp"

using namespace std;
using namespace expr::op::bind;
using namespace boost;
using namespace boost::multiprecision;

namespace ufo
{
  template<typename Range> static Expr mkbadd(Range& terms, Expr ty, ExprFactory &efac){
    if (terms.empty()) return bvnum(mkMPZ (0, efac), ty);
    if (terms.size() == 1) return *terms.begin();
    Expr tmp = mk<BADD>(terms[0], terms[1]);
    for (int i = 2; i < terms.size(); i++)
      tmp = mk<BADD>(tmp, terms[i]);
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

  inline static void getBaddTerm (Expr a, ExprVector &ptrms, ExprVector &ntrms)
  {
    if (isOpX<BADD>(a))
    {
      for (auto it = a->args_begin (); it != a->args_end (); ++it)
      {
        getBaddTerm(*it, ptrms, ntrms);
      }
    }
    else if (isOpX<BSUB>(a))
    {
      assert(a->arity() == 2);
      getBaddTerm(a->left(), ptrms, ntrms);
      getBaddTerm(a->right(), ntrms, ptrms);
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

  inline static void simplifyBterm (Expr a, ExprVector& ptrms,
      ExprVector& ntrms, cpp_int & comm)
  {
    getBaddTerm(a, ptrms, ntrms);
    comm += filterVec(ptrms);
    comm -= filterVec(ntrms);
  }

  inline static Expr repairBeq (Expr a)
  {
    assert (isOpX<EQ>(a));
    auto & efac = a->getFactory();
    ExprVector ptrms, ntrms;
    cpp_int comm = 0;
    simplifyBterm(a->right(), ntrms, ptrms, comm);
    comm = -comm;
    simplifyBterm(a->left(), ptrms, ntrms, comm);

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

    Expr ty = NULL;
    if (!ptrms.empty()) ty = typeOf(*ptrms.begin());
    else if (!ntrms.empty()) ty = typeOf(*ntrms.begin());
    else if (comm == 0) return mk<TRUE>(efac);
    else return mk<FALSE>(efac);

    if (comm > 0) ptrms.push_back(bvnum(mkMPZ(comm, efac), ty));
    if (comm < 0) ntrms.push_back(bvnum(mkMPZ(-comm, efac), ty));
    return mk<EQ>(mkbadd(ptrms, ty, efac), mkbadd(ntrms, ty, efac));
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
      if (isOpX<BEXTRACT>(exp))
      {
        if (isOpX<BEXTRACT>(exp->last()))
        {
          // to extend
          if (exp->left()  == exp->last()->left() &&
              exp->right() == exp->last()->right())
            return exp->last();
        }
      }
      return exp;
    }
  };

  inline static Expr simplifyBV (Expr exp)
  {
    RW<SimplifyBVExpr> rw(new SimplifyBVExpr());
    return dagVisit (rw, exp);
  }

}

#endif
