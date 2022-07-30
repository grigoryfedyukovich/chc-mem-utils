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
