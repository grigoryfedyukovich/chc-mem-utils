#ifndef SETENCODING__HPP__
#define SETENCODING__HPP__

#include "../ae/ExprSimpl.hpp"

using namespace std;
using namespace expr::op::bind;
using namespace boost;
using namespace boost::multiprecision;

namespace ufo
{
  static vector<string> supportedPreds = {
              "state-r-ok",  "state-rw-ok",
              "state-w-ok", "state-is-cstring",
              "evaluate-b"   // to support, actually
            };

  // GF: to generate automatically
  static vector<string> supportedPredsAss = {
            "state-r-ok-ass", "state-rw-ok-ass",
            "state-w-ok-ass", "state-is-cstring-ass"
            };

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
    Expr alloc, mem, off, siz, allocP, memP, offP, sizP, aux, auxP, ty;
         // alloc and aux should be initially zeroes
         // then aux (v) == 1 if v is cstring,
         //      aux (v) == 2 if v is read-only
         //      aux (v) == 3 if v is write-only
         //      aux (v) == 4 if v is read and write
    ExprVector nums;

    SetDecoder (ExprSet& _defs,
         Expr _alloc, Expr _mem, Expr _off, Expr _siz, Expr _aux,
         Expr _allocP, Expr _memP, Expr _offP, Expr _sizP, Expr _auxP,
         ExprVector &_names) :
      efac(_alloc->getFactory()),
      defs(_defs), alloc(_alloc), mem(_mem), off(_off), siz(_siz), aux(_aux),
      allocP(_allocP), memP(_memP), offP(_offP), sizP(_sizP), auxP(_auxP),
      names(_names) {
        ty = alloc->left()->right()->right();
        for (int i = 0; i <= 4; i++) nums.push_back(bvnum(mkMPZ(i, efac), ty));
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

    Expr assumeOks(Expr obj, Expr allSz, string fname)
    {
      Expr var;
      Expr offs;
      if (isOpX<BADD>(obj))
      {
        var = obj->left();
        assert(isOpX<BMUL>(obj->right()));
        offs = obj->right()->left();
      }
      else
      {
        auto str = lexical_cast<string>(obj);
        if (str.find("\"") != string::npos)
          str = str.substr(1, str.length() - 2);
        auto name = mkTerm<string>(str, efac);
        var = mk<FAPP>(mk<FDECL>(name, ty));
        offs = nums[0];
      }

      Expr h = getVarId(var);
      Expr auxUpd;
      if (fname == "state-r-ok")
        auxUpd = mk<EQ>(auxP, mk<STORE>(aux, h, nums[2]));
      else if (fname == "state-w-ok")
        auxUpd = mk<EQ>(auxP, mk<STORE>(aux, h, nums[3]));
      else if (fname == "state-rw-ok")
        auxUpd = mk<EQ>(auxP, mk<STORE>(aux, h, nums[4]));
      else
        auxUpd = mk<TRUE>(efac);

      typeSafeInsert(defs, alloc);
      typeSafeInsert(defs, siz);
      typeSafeInsert(defs, aux);
      return mk<AND>(
          auxUpd,
          mk<AND>(mk<EQ>(allocP, mk<STORE>(alloc, h, h)),
                  mk<EQ>(sizP, mk<STORE>(siz, h, allSz))),
          mk<EQ>(offP, mk<STORE>(off, h, offs))
        );
    }

    Expr updAllocs(Expr arg, Expr ty, Expr obj, Expr allSz, bool ptr)
    {
      auto str = lexical_cast<string>(obj);
      if (str.find("\"") != string::npos)
        str = str.substr(1, str.length() - 2);
      auto name = mkTerm<string>(str, efac);
      Expr var = mk<FAPP>(mk<FDECL>(name, ty));

      if (allSz != NULL)
      {
        Expr h = getVarId(var);

        typeSafeInsert(defs, alloc);
        typeSafeInsert(defs, siz);
        typeSafeInsert(defs, off);
        return mk<AND>(
            mk<AND>(mk<EQ>(allocP, mk<STORE>(alloc, h, h)),
                    mk<EQ>(sizP, mk<STORE>(siz, h, allSz))),
            mk<EQ>(offP, mk<STORE>(off, h, nums[0]))
          );
      }
      else
      {
        Expr offs;
        Expr argTmp = isOpX<BADD>(arg) ? arg->left() : arg;
        if (ptr)
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

    Expr assumeCstring(Expr ty, Expr obj)
    {
      auto str = lexical_cast<string>(obj);
      if (str.find("\"") != string::npos)
      {
        str = str.substr(1, str.length() - 2);
      }
      auto name = mkTerm<string>(str, efac);
      Expr var = mk<FAPP>(mk<FDECL>(name, ty));
      typeSafeInsert(defs, alloc);
      typeSafeInsert(defs, aux);
      typeSafeInsert(defs, off);
      Expr h = getVarId(var);

      auto key = mkConst(mkTerm<string> ("k", efac), ty);
      ExprVector assm = {
        mk<BULE>(nums[0], key),
        mk<BULT>(key, mk<SELECT>(siz, mk<SELECT>(allocP, h))),
        mk<EQ>(mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(allocP, h)), key), nums[0])
      };

      return mknary<AND>(ExprVector{
          mk<EQ>(allocP, mk<STORE>(alloc, h, h)),
          mk<EQ>(auxP, mk<STORE>(aux, h, nums[1])),
          mk<EQ>(offP, mk<STORE>(off, h, nums[0])),
          mknary<EXISTS>(ExprVector{key->left(), conjoin(assm, efac)})}
        );
    }

    Expr assertCstring(Expr obj)
    {
      Expr var, s;
      if (isOpX<BADD>(obj))
      {
        var = obj->left();
        s = obj->right();
      }
      else
      {
        var = obj;
        s = nums[0];
      }

      typeSafeInsert(defs, alloc);
      typeSafeInsert(defs, siz);
      typeSafeInsert(defs, aux);
      Expr h = getVarId(var);
      s = mk<BADD>(s, mk<SELECT>(off, h));
      Expr ss = mk<SELECT>(siz, mk<SELECT>(alloc, h));

      auto key = mkConst(mkTerm<string> ("k", efac), ty);
      ExprVector tmp = {
        mk<BULE>(s, key),
        mk<BULT>(key, mk<SELECT>(siz, mk<SELECT>(alloc, h))),
        mk<EQ>(mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, h)), key), nums[0])
      };

      // don't require `mk<EQ>(mk<SELECT>(aux, mk<SELECT>(alloc, h)), nums[1])`
      // to allow finding "implicit" cstrings
      return mk<AND>(
        mknary<EXISTS>(ExprVector{key->left(), conjoin(tmp, efac)}),
        mk<NEQ>(nums[0], mk<SELECT>(alloc, h))
      );
    }

    Expr assertAllocs(Expr obj, Expr allSz, string fname)
    {
      assert (allSz != NULL);
      Expr var, s;
      if (isOpX<BADD>(obj))
      {
        var = obj->left();
        s = obj->right();
      }
      else
      {
        var = obj;
        s = nums[0];
      }

      Expr h = getVarId(var);
      Expr auxAss;
      Expr al = mk<SELECT>(alloc, h);
      Expr rwacc = mk<EQ>(mk<SELECT>(aux, al), nums[4]);

      if (fname == "state-r-ok-ass")
        auxAss = mk<OR>(
          mk<EQ>(mk<SELECT>(aux, al), nums[2]), rwacc);
      else if (fname == "state-w-ok-ass")
        auxAss = mk<OR>(
          mk<EQ>(mk<SELECT>(aux, al), nums[3]), rwacc);
      else if (fname == "state-rw-ok-ass")
        auxAss = rwacc;
      else
        auxAss = mk<TRUE>(efac);

      typeSafeInsert(defs, alloc);
      typeSafeInsert(defs, siz);
      typeSafeInsert(defs, aux);
      s = mk<BADD>(s, mk<SELECT>(off, h));
      Expr ss = mk<SELECT>(siz, al);

      auto key = mkConst(mkTerm<string> ("k", efac), ty);
      ExprVector tmp = {
        mk<BULE>(mk<BSUB>(mk<BADD>(s, allSz), nums[1]), key),
        mk<BULT>(key, mk<SELECT>(siz, al)),
        mk<EQ>(mk<SELECT>(mk<SELECT>(mem, al), key), nums[0])
      };

      Expr sizeConstraint = mk<ITE>(
        mk<EQ>(mk<SELECT>(aux, al), nums[1]),
        mknary<EXISTS>(ExprVector{key->left(), conjoin(tmp, efac)}), // if cstring
        mk<AND>(auxAss, mk<BULE>(mk<BADD>(s, allSz), ss)) // else
      );

      return mk<AND>(
        mk<NEQ>(nums[0], al),
        //mk<BSGE>(allSz, nums[0]),  // GF: to add when we get adequate types
        sizeConstraint);
    }

    Expr operator() (Expr exp)
    {
      if (isOpX<FAPP>(exp))
      {
        auto name = lexical_cast<string>(exp->left()->left());
        if (name == "state-r-ok" || name == "state-w-ok" ||
            name == "state-rw-ok")
          // TODO: update to use \exists
          return assumeOks(exp->arg(2), exp->last(), name);
        else if (name == "state-is-cstring")
          return assumeCstring(exp->left()->arg(2), exp->arg(2));
        else if (name == "state-r-ok-ass" || name == "state-w-ok-ass" ||
                 name == "state-rw-ok-ass")
          return assertAllocs(exp->arg(2), exp->last(), name);
        else if (name == "state-is-cstring-ass")
          return assertCstring(exp->arg(2));
      }
      else if (isOpX<EQ>(exp))
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
              if (name == "object-address")
              {
                // TODO: make sure that l->arg(3) also comes with "-p"
                return updAllocs(l->arg(3), l->left()->arg(3),
                      obj->last(), allSz,
                      str.find("update-state-p") != string::npos);
              }
              else if (name.find("field-address") != string::npos)
              {
                str = lexical_cast<string>(obj->last());
                str = str.substr(1, str.length() - 2);
                auto name = mkTerm<string>(str, efac);
                      // TODO: check type
                auto field = mk<FAPP>(mk<FDECL>(name, ty));

                Expr objj;
                if (isOpX<FAPP>(obj->right()) && obj->right()->left()->arity() == 2)
                  objj = obj->right();
                else
                {
                  str = lexical_cast<string>(obj->right()->last());
                  str = str.substr(1, str.length() - 2);
                  name = mkTerm<string>(str, efac);
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
        auto ela = str.find("element-address");

        Expr cell = NULL;
        if (ela != string::npos)
        {
          if (exp->arity() == 4)
            cell = mk<BMUL>(exp->arg(2), exp->arg(3));
          else      // hack: the last arg should not be used
            cell = mk<BMUL>(exp->arg(2), exp->arg(2));
        }

        if (pos != string::npos || ela != string::npos)
        {
          auto obj = exp->arg(pos != string::npos ? 2 : 1);
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
            auto name = mkTerm<string>(str, efac);
            auto var = mk<FAPP>(mk<FDECL>(name, exp->left()->last()));
            varToDefs(defs, var);
            if (cell == NULL)
              return var;
            else
              return mk<BADD>(var, cell);
          }
          else if(isOpX<FAPP>(obj) &&
                 lexical_cast<string>(obj->left()->left()).
                    find("field-address") != string::npos)
          {
            str = lexical_cast<string>(obj->last());
            str = str.substr(1, str.length() - 2);
            auto name = mkTerm<string>(str, efac);
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
              name = mkTerm<string>(str, efac);
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
