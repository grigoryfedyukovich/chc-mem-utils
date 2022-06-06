#ifndef SETENCODING__HPP__
#define SETENCODING__HPP__

#include "../ae/ExprSimpl.hpp"

using namespace std;
using namespace expr::op::bind;
using namespace boost;
using namespace boost::multiprecision;

namespace ufo
{
  inline static void varToDefs(ExprSet& defs, Expr var)
  {
    if (isOpX<FAPP>(var) && var->left()->arity() == 2)
      typeSafeInsert(defs, var);
  }

  struct SetDecoder
  {
    ExprFactory& efac;
    ExprSet& defs;
    ExprVector& names;
    Expr alloc, mem, off, siz, allocP, memP, offP, sizP, aux, auxP, o, ty;
         // alloc and aux should be initially zeroes
         // then aux (v) == 1 if v is cstring,
         //      aux (v) == 2 if v is read-only
         //      aux (v) == 3 if v is write-only
         //      aux (v) == 4 if v is read and write

    SetDecoder (ExprSet& _defs,
         Expr _alloc, Expr _mem, Expr _off, Expr _siz, Expr _aux,
         Expr _allocP, Expr _memP, Expr _offP, Expr _sizP, Expr _auxP,
         ExprVector &_names) :
      efac(_alloc->getFactory()),
      defs(_defs), alloc(_alloc), mem(_mem), off(_off), siz(_siz), aux(_aux),
      allocP(_allocP), memP(_memP), offP(_offP), sizP(_sizP), auxP(_auxP),
      names(_names) {
        ty = alloc->left()->right()->right();
        o = bvnum(mkMPZ(0, efac), ty);
      };

    Expr getVarId (Expr var)
    {
      int i;
      for (i = 0; i < names.size(); i++)
        if (names[i] == var) break;

      if (i == names.size())
        names.push_back(var);

      return bvnum(mkMPZ(i + 1, efac), ty);
    }
  };

  static Expr evalReplace (Expr exp, ExprSet& defs);
  struct SetRewriter : SetDecoder
  {
    SetRewriter (ExprSet& _defs,
      Expr _alloc, Expr _mem, Expr _off, Expr _siz, Expr _aux,
      Expr _allocP, Expr _memP, Expr _offP, Expr _sizP, Expr _auxP,
      ExprVector &_names) : SetDecoder(_defs,
        _alloc, _mem, _off, _siz, _aux,
        _allocP, _memP, _offP, _sizP, _auxP, _names) {};

    Expr updAllocs(Expr arg, Expr ty, Expr obj, Expr allSz)
    {
      auto str = lexical_cast<string>(obj);
      if (str.find("\"") != string::npos)
        str = str.substr(1, str.length() - 2);
      auto name = mkTerm<string>(str, efac);
      Expr var = mk<FAPP>(mk<FDECL>(name, ty));

      if (allSz != NULL)
      {
        typeSafeInsert(defs, alloc);
        typeSafeInsert(defs, siz);
        Expr h = getVarId(var);
        return mk<AND>(
            mk<AND>(mk<EQ>(allocP, mk<STORE>(alloc, h, h)),
                    mk<EQ>(sizP, mk<STORE>(siz, h, allSz))),
            mk<EQ>(offP, mk<STORE>(off, h, o))
          );
      }
      else
      {
        Expr offs;
        Expr argTmp = isOpX<BADD>(arg) ? arg->left() : arg;
        if (contains(names, argTmp) || contains(names, var))
        {
          Expr k = getVarId(argTmp);
          Expr h = getVarId(var);
          if (isOpX<BADD>(arg))
          {
            Expr offVal;
            if (isOpX<BMUL>(arg->right()))
              offVal = arg->right()->left();
            else
              offVal = arg->right();
            // TODO: compute proper offset based on BMUL;

            offs = mk<EQ>(offP, mk<STORE>(off, h,
                                mk<BADD>(mk<SELECT>(off, k), offVal)));
          }
          else
          {
            offs = mk<EQ>(offP, mk<STORE>(off, h, mk<SELECT>(off, k)));
          }

          typeSafeInsert(defs, alloc);
          typeSafeInsert(defs, off);
          return mk<AND>(offs,
            mk<EQ>(allocP, mk<STORE>(alloc, h, mk<SELECT>(alloc, k))));
        }

        varToDefs(defs, var);
        name = mkTerm<string>(lexical_cast<string>(name) + "'", efac);
        var = mk<FAPP>(mk<FDECL>(name, ty));
        return mk<EQ>(var, arg);
      }
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
          if (pos == string::npos) pos = str.find("allocate");   // TODO: split
          if (pos == string::npos) pos = str.find("reallocate"); // TODO: split
          if (pos != string::npos)
          {
            Expr obj = l->arg(2);
            Expr allSz = NULL;
            if (l->arity() > 3 && isOpX<FAPP>(l->arg(3)) &&
                (lexical_cast<string>(l->arg(3)->left()->left()) == "allocate" ||
                 lexical_cast<string>(l->arg(3)->left()->left()) == "reallocate"))
            {
              allSz = l->arg(3)->last();
            }
            if (isOpX<FAPP>(obj) && obj->left()->arity() > 2)
            {
              auto name = lexical_cast<string>(obj->left()->left());
              if (name == "object-address"){
                return updAllocs(l->arg(3), l->left()->arg(3), obj->last(), allSz);
              }
              else if (name.find("field-address") != string::npos)
              {
                str = lexical_cast<string>(obj->last());
                str = str.substr(1, str.length() - 2);
                auto name = mkTerm<string>(str, exp->getFactory());
                      // TODO: check type
                auto field = mk<FAPP>(mk<FDECL>(name, ty));

                Expr objj;
                if (isOpX<FAPP>(obj->right()) && obj->right()->left()->arity() == 2)
                  objj = obj->right();
                else
                {
                  str = lexical_cast<string>(obj->right()->last());
                  str = str.substr(1, str.length() - 2);
                  name = mkTerm<string>(str, exp->getFactory());
                        // TODO: check type
                  objj = mk<FAPP>(mk<FDECL>(name, ty));
                }

                Expr h = getVarId(objj);
                Expr k = getVarId(field);

                Expr toWrite = l->arg(3);
                if (contains(names, toWrite))
                {
                  toWrite = getVarId(toWrite);
                }
                typeSafeInsert(defs, mem);
                typeSafeInsert(defs, objj);

                h = mk<SELECT>(alloc, h);

                return mk<EQ>(memP, mk<STORE>(mem,
                        h, mk<STORE>(mk<SELECT>(mem, h), k, toWrite)));
              }
              else
              {
                outs () << "Not supported function: \"" << name << "\"\n";
                assert(0);
              }
            }
            else if (isOpX<BADD>(obj))
            {
              if(isOpX<BMUL>(obj->right()))
              {
                Expr h = obj->left();
                if (!isOpX<SELECT>(h)) h = getVarId(h);

                Expr t = mk<BADD>(mk<SELECT>(off, h), obj->right()->left());
                Expr b =mk<SELECT>(mem, mk<SELECT>(alloc, h));
                typeSafeInsert(defs, mem);
                return mk<EQ>(memP, mk<STORE>(mem,
                  mk<SELECT>(alloc, h),
                  mk<STORE>(b, t, l->arg(3))));
              }
              else
              {
                // unsupported for now
                outs () << "Ignoring the \"" << obj << "\" constraint\n";
              }
            }
            else
            {
              Expr h = obj;
              if (!isOpX<SELECT>(h)) h = getVarId(h);
              Expr t = mk<SELECT>(off, h);
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
                Expr alloc, Expr mem, Expr off, Expr siz, Expr aux,
                Expr allocP, Expr memP, Expr offP, Expr sizP, Expr auxP,
                ExprVector& names)

  {
    RW<SetRewriter> rw(new SetRewriter(defs, alloc, mem, off, siz, aux,
                                allocP, memP, offP, sizP, auxP, names));
    return dagVisit (rw, exp);
  }

  struct EvalReplacer : SetDecoder
  {
    EvalReplacer (ExprSet& _defs,
      Expr _alloc, Expr _mem, Expr _off, Expr _siz, Expr _aux,
      Expr _allocP, Expr _memP, Expr _offP, Expr _sizP, Expr _auxP,
      ExprVector &_names) : SetDecoder(_defs,
        _alloc, _mem, _off, _siz, _aux,
        _allocP, _memP, _offP, _sizP, _auxP, _names) {};

    Expr operator() (Expr exp)
    {
      if (isOpX<FAPP>(exp))
      {
        auto str = lexical_cast<string>(exp->left()->left());
        auto pos = str.find("evaluate");
        if (pos != string::npos)
        {
          auto obj = exp->arg(2);
          if (find(names.begin(), names.end(), obj) != names.end())
          {
            // second iter: already added to vars, need to use offset
            Expr h = getVarId(obj);
            Expr t = mk<SELECT>(off, h);
            typeSafeInsert(defs, mem);
            typeSafeInsert(defs, off);
            return mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, h)), t);
          }
          else if(isOpX<FAPP>(obj) &&
                 lexical_cast<string>(obj->left()->left()) == "object-address")
          {
            str = lexical_cast<string>(obj->last());
            str = str.substr(1, str.length() - 2);
            auto name = mkTerm<string>(str, exp->getFactory());
            auto var = mk<FAPP>(mk<FDECL>(name, exp->left()->last()));
            varToDefs(defs, var);
            return var;
          }
          else if(isOpX<FAPP>(obj) &&
                 lexical_cast<string>(obj->left()->left()).
                    find("field-address") != string::npos)
          {
            str = lexical_cast<string>(obj->last());
            str = str.substr(1, str.length() - 2);
            auto name = mkTerm<string>(str, exp->getFactory());
                  // TODO: check type
            auto field = mk<FAPP>(mk<FDECL>(name, ty));

            Expr objj;
            if (isOpX<SELECT>(obj->right())) // nested
            {
              objj = obj->right();
              Expr k = getVarId(field);
              return mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, objj)), k);
            }
            if (isOpX<FAPP>(obj->right()) && obj->right()->left()->arity() == 2)
              objj = obj->right();
            else
            {
              str = lexical_cast<string>(obj->right()->last());
              str = str.substr(1, str.length() - 2);
              name = mkTerm<string>(str, exp->getFactory());
                    // TODO: check type
              objj = mk<FAPP>(mk<FDECL>(name, ty));
            }

            Expr h = getVarId(objj);
            Expr k = getVarId(field);
            typeSafeInsert(defs, mem);
            typeSafeInsert(defs, objj);
            return mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, h)), k);
          }
          else if (isOpX<BADD>(obj))
          {
            Expr toRead = obj->left();

            if (contains(names, toRead)) toRead = getVarId(toRead);

            typeSafeInsert(defs, mem);
            Expr t;
            if(isOpX<BMUL>(obj->right())) t = obj->right()->left();
            else t = obj->right();

            return mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, toRead)), t);
          }
          else if (isOpX<SELECT>(obj))
          {
            Expr toRead = obj;

            if (contains(names, toRead)) toRead = getVarId(toRead);
            return mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, toRead)),
                                              mk<SELECT>(off, toRead));
          }
        }
      }
      return exp;
    }
  };

  inline static Expr evalReplace (Expr exp, ExprSet& defs,
                Expr alloc, Expr mem, Expr off, Expr siz, Expr aux,
                Expr allocP, Expr memP, Expr offP, Expr sizP, Expr auxP,
                ExprVector& names)
  {
    RW<EvalReplacer> rw(new EvalReplacer(defs, alloc, mem, off, siz, aux,
                                allocP, memP, offP, sizP, auxP, names));
    return dagVisit (rw, exp);
  }


  struct TypeRep
  {
    TypeRep () {}
    Expr operator() (Expr exp)
    {
      // TODO: extend
      if (isOpX<EQ>(exp) && typeOf(exp->left()) != typeOf(exp->right()))
      {
        int w1 = width(typeOf(exp->left()));
        int w2 = width(typeOf(exp->right()));
        if (w1 > w2)
          exp = mk<EQ>(exp->left(), sext(exp->right(), w1));
        else
          exp = mk<EQ>(sext(exp->left(), w2), exp->right());
      }

      if (isOpX<NEQ>(exp) && typeOf(exp->left()) != typeOf(exp->right()))
      {
        int w1 = width(typeOf(exp->left()));
        int w2 = width(typeOf(exp->right()));
        if (w1 > w2)
          exp = mk<NEQ>(exp->left(), sext(exp->right(), w1));
        else
          exp = mk<NEQ>(sext(exp->left(), w2), exp->right());
      }

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
