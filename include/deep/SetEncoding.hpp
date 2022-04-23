#ifndef SETENCODING__HPP__
#define SETENCODING__HPP__

#include "../ae/ExprSimpl.hpp"

using namespace std;
using namespace expr::op::bind;
using namespace boost;
using namespace boost::multiprecision;

namespace ufo
{
  static Expr evalReplace (Expr exp, ExprSet& defs);

  struct SetRewriter
  {
    ExprSet& defs;
    ExprVector& names;
    Expr alloc, mem, allocP, memP;

    SetRewriter (ExprSet& _defs, Expr _alloc, Expr _mem, Expr _allocP,
              Expr _memP, ExprVector &_names) :
      defs(_defs), alloc(_alloc), mem(_mem), allocP(_allocP),
              memP(_memP), names(_names) {};

    int getVarId (Expr var)
    {
      for (int i = 0; i < names.size(); i++)
        if (names[i] == var) return i + 1;

      names.push_back(var);
      return names.size();
    }

    Expr operator() (Expr exp)
    {
      if (isOpX<EQ>(exp))
      {
        auto l = exp->left();
        if (isOpX<FAPP>(l))
        {
          auto str = lexical_cast<string>(l->left()->left());
          auto pos = str.find("update-state");
          if (pos == string::npos) pos = str.find("allocate"); // TODO: split

          if (pos != string::npos)
          {
            Expr obj = l->arg(2);
            auto & efac = l->getFactory();
            if (isOpX<FAPP>(obj) &&
                lexical_cast<string>(obj->left()->left()) == "object-address")
            {
              str = lexical_cast<string>(obj->last());
              str = str.substr(1, str.length() - 2);
              auto name = mkTerm<string>(str, efac);
              Expr var = mk<FAPP>(mk<FDECL>(name, l->left()->arg(3)));

              str = lexical_cast<string>(l->arg(3));
              pos = str.find("return_value_malloc");
              if (pos == string::npos)
              {
                pprint(names, 4);
                if (find(names.begin(), names.end(), l->arg(3)) != names.end() ||
                    find(names.begin(), names.end(), var) != names.end())
                {
                  typeSafeInsert(defs, alloc);
                  Expr h = bvnum(mkMPZ(getVarId(var), efac),
                                      alloc->left()->right()->right());
                  Expr k = bvnum(mkMPZ(getVarId(l->arg(3)), efac),
                                      alloc->left()->right()->right());
                  return mk<EQ>(allocP, mk<STORE>(alloc, h,
                                        mk<SELECT>(alloc, k)));
                }
                typeSafeInsert(defs, var);
                name = mkTerm<string>(lexical_cast<string>(name) + "'", efac);
                var = mk<FAPP>(mk<FDECL>(name, l->left()->arg(3)));
                return mk<EQ>(var, l->arg(3));
              }
              else
              {
                typeSafeInsert(defs, alloc);
                Expr h = bvnum(mkMPZ(getVarId(var), efac),
                                    alloc->left()->right()->right());
                return mk<EQ>(allocP, mk<STORE>(alloc, h, h));
              }
            }
            else if (isOpX<BADD>(obj))
            {
              Expr h = bvnum(mkMPZ(getVarId(obj->left()), efac),
                                  alloc->left()->right()->right());
              assert(isOpX<BMUL>(obj->right()));
              Expr t = obj->right()->left();
              Expr b = mk<SELECT>(mem, mk<SELECT>(alloc, h));
              typeSafeInsert(defs, mem);
              return mk<EQ>(memP, mk<STORE>(mem,
                mk<SELECT>(alloc, h),
                mk<STORE>(b, t, l->arg(3))));
            }
          }
        }
      }

      return exp;
    }
  };

  inline Expr rewriteSet (Expr exp, ExprSet& defs,
                  Expr alloc, Expr mem, Expr allocP, Expr memP, ExprVector& names)
  {
    RW<SetRewriter> rw(new SetRewriter(defs, alloc, mem, allocP, memP, names));
    return dagVisit (rw, exp);
  }

  struct EvalReplacer
  {
    ExprSet& defs;
    ExprVector& names;
    Expr alloc, mem, allocP, memP;

    EvalReplacer (ExprSet& _defs, Expr _alloc, Expr _mem, Expr _allocP,
              Expr _memP, ExprVector &_names) :
      defs(_defs), alloc(_alloc), mem(_mem), allocP(_allocP),
              memP(_memP), names(_names) {}

    int getVarId (Expr var)
    {
      for (int i = 0; i < names.size(); i++)
        if (names[i] == var) return i + 1;
      names.push_back(var);
      return names.size() - 1;
    }

    Expr operator() (Expr exp)
    {
      if (isOpX<FAPP>(exp))
      {
        auto str = lexical_cast<string>(exp->left()->left());
        auto pos = str.find("evaluate");
        if (pos != string::npos)
        {
          auto obj = exp->arg(2);
          if(isOpX<FAPP>(obj) &&
                 lexical_cast<string>(obj->left()->left()) == "object-address")
          {
            str = lexical_cast<string>(obj->last());
            str = str.substr(1, str.length() - 2);
            auto name = mkTerm<string>(str, exp->getFactory());
            auto var = mk<FAPP>(mk<FDECL>(name, exp->left()->last()));
            typeSafeInsert(defs, var);
            return var;
          }
          else if (isOpX<BADD>(obj))
          {
            Expr h = bvnum(mkMPZ(getVarId(obj->left()), exp->getFactory()),
                                alloc->left()->right()->right());
            assert(isOpX<BMUL>(obj->right()));
            Expr t = obj->right()->left();
            typeSafeInsert(defs, mem);
            return mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, h)), t);
          }
        }
      }
      return exp;
    }
  };

  inline static Expr evalReplace (Expr exp, ExprSet& defs,
                Expr alloc, Expr mem, Expr allocP, Expr memP, ExprVector& names)
  {
    RW<EvalReplacer> rw(new EvalReplacer(defs, alloc, mem, allocP, memP, names));
    return dagVisit (rw, exp);
  }
}

#endif
