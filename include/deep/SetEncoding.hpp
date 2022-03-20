#ifndef SETENCODING__HPP__
#define SETENCODING__HPP__

#include "../ae/ExprSimpl.hpp"

using namespace std;
using namespace expr::op::bind;
using namespace boost;
using namespace boost::multiprecision;

namespace ufo
{
  struct SetRewriter
  {
    ExprSet& defs;

    SetRewriter (ExprSet& _defs) :  defs(_defs) {};

    Expr operator() (Expr exp)
    {
      if (isOpX<EQ>(exp))
      {
        auto l = exp->left();
        if (isOpX<FAPP>(l))
        {
          auto str = lexical_cast<string>(l->left()->left());
          auto pos = str.find("update-state");
          if (pos != string::npos)
          {
            auto obj = l->arg(2);
            assert(isOpX<FAPP>(obj) &&
                lexical_cast<string>(obj->left()->left()) == "object-address");

            str = lexical_cast<string>(obj->last());
            str = str.substr(1, str.length() - 2);
            auto name = mkTerm<string>(str, l->getFactory());
            Expr var = mk<FAPP>(mk<FDECL>(name, l->left()->arg(3)));
            typeSafeInsert(defs, var);
            name = mkTerm<string>(str.append("\'"), l->getFactory());
            var = mk<FAPP>(mk<FDECL>(name, l->left()->arg(3)));
            return mk<EQ>(var, l->arg(3));
          }
        }
      }

      return exp;
    }
  };

  inline Expr rewriteSet (Expr exp, ExprSet& defs)
  {
    RW<SetRewriter> rw(new SetRewriter(defs));
    return dagVisit (rw, exp);
  }

  struct EvalReplacer
  {
    ExprSet& defs;

    EvalReplacer (ExprSet& _defs) :  defs(_defs) {};

    Expr operator() (Expr exp)
    {
      if (isOpX<FAPP>(exp))
      {
        auto str = lexical_cast<string>(exp->left()->left());
        auto pos = str.find("evaluate");
        if (pos != string::npos)
        {
          auto obj = exp->arg(2);
          assert(isOpX<FAPP>(obj) &&
                 lexical_cast<string>(obj->left()->left()) == "object-address");
          str = lexical_cast<string>(obj->last());
          str = str.substr(1, str.length() - 2);
          auto name = mkTerm<string>(str, exp->getFactory());
          auto var = mk<FAPP>(mk<FDECL>(name, exp->left()->last()));
          typeSafeInsert(defs, var);
          return var;
        }
      }
      return exp;
    }
  };

  inline static Expr evalReplace (Expr exp, ExprSet& defs)
  {
    RW<EvalReplacer> rw(new EvalReplacer(defs));
    return dagVisit (rw, exp);
  }
}

#endif
