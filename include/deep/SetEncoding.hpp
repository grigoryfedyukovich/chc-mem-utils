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

  static Expr selRewrite (Expr exp, ExprVector& names, Expr alloc,
                              function<Expr(void*, Expr)> f, void* obj);

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
         //      aux (v) == 2 if v is read-ok
         //      aux (v) == 3 if v is write-ok
         //      aux (v) == 4 if v is read-and-write-ok
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

    Expr getVarByName (Expr name, Expr type)
    {
      auto str = lexical_cast<string>(name);
      if (str.find("\"") != string::npos)
        str = str.substr(1, str.length() - 2);
      auto varname = mkTerm<string>(str, efac);
      return mk<FAPP>(mk<FDECL>(varname, type));
    }

    Expr addNoname()
    {
      int i = names.size() + 1;
      auto var = mkConst(mkTerm<string> ("noname_alloc" +
                 lexical_cast<string>(i), efac), ty);
      names.push_back(var);
      return var;
    }

    Expr getVarId (Expr var)
    {
      if (!containsOp<FAPP>(var))
      {
        return mkConst(mkTerm<string> ("nondet", efac), ty);
      }
      outs().flush();
      if (isOpX<FAPP>(var))
      {
        auto str = lexical_cast<string>(var->left());
        if (var->arity() > 2)
        {
          if (str.find("element-address") != string::npos &&
              isOpX<BCONCAT>(var->right()))
            return mkConst(mkTerm<string> ("nondet", efac), ty);
          assert(0 && "Cannot get an ID of non-variable expr");
        }
        if (str.find("object-address") != string::npos)
          assert(0 && "Cannot get an ID of object-address");
      }

      if (isOpX<SELECT>(var)) // not a var at all
        return var;

      assert(isOpX<FAPP>(var) && var->arity() == 1);

      int i;
      for (i = 0; i < names.size(); i++)
        if (names[i] == var) break;

      if (i == names.size())
        names.push_back(var);

      return bvnum(mkMPZ(i + 1, efac), ty);
    }

    static Expr getVarIdWr (void* arg, Expr e)
    {
      SetDecoder* that = (SetDecoder*)arg;
      return that->getVarId(e);
    }

    Expr concatFieldNames(Expr obj)
    {
      string name = isOpX<FAPP>(obj) ?
        lexical_cast<string>(obj->left()->left()) : "";
      if (name.find("field-address") != string::npos)
      {
        auto var = getVarByName(obj->last(), ty);
        Expr res = concatFieldNames(obj->right());
        if (res == NULL) // just var name
          return var;
        else if (isOpX<FAPP>(res))
        {
          auto tmp = mkTerm<string>(lexical_cast<string>(res) + "::" +
                                    lexical_cast<string>(var), efac);
          return replaceAll(res, res->left()->left(),
                  mkTerm<string>(lexical_cast<string>(res) + "::" +
                 lexical_cast<string>(var), efac));
        }
        else assert(0);
      }
      else
        return NULL;
      assert(0 && "Unsupported field type");
      return NULL;
    }

    Expr getObj(Expr obj)
    {
      string name = isOpX<FAPP>(obj) ?
        lexical_cast<string>(obj->left()->left()) : "";
      if (name.find("field-address") != string::npos)
        return getObj(obj->right());
      else if (isOpX<SELECT>(obj))
      {
        assert(isOpX<SELECT>(obj->left()) && mem == obj->left()->left());
        return mk<SELECT>(mem, obj);
      }
      else
        return obj;
    }

    Expr getMemField(Expr exp)
    {
      if (isOpX<BEXTRACT>(exp))
        return getMemField(exp->last());
      if (mem == exp->left()->left())
        return exp->left()->right();
      outs () << "Unable to detect mem field: " << exp << "\n";
      return NULL;
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
      Expr var, offs, h;
      ExprVector assms;
      if (isOpX<SELECT>(obj))
      {
        var = obj;
        offs = nums[0];
        assms = {mk<EQ>(var, getVarId(addNoname()))};
      }
      else if (isOpX<FAPP>(obj) && lexical_cast<string>(
          obj->left()->left()).find("field-address") != string::npos)
      {
        auto field = concatFieldNames(obj);
        Expr objj = getObj(obj);
        assert(0 && "Assume over fields not supported");
      }
      else if (isOpX<BADD>(obj))
      {
        assert(isOpX<BMUL>(obj->right()));
        var = obj->left();
        offs = obj->right()->left();
      }
      else
      {
        var = getVarByName(obj, ty);
        offs = nums[0];
      }

      h = getVarId(var);
      if (fname == "state-r-ok")
        assms.push_back(mk<EQ>(auxP, mk<STORE>(aux, h, nums[2])));
      else if (fname == "state-w-ok")
        assms.push_back(mk<EQ>(auxP, mk<STORE>(aux, h, nums[3])));
      else if (fname == "state-rw-ok")
        assms.push_back(mk<EQ>(auxP, mk<STORE>(aux, h, nums[4])));

      typeSafeInsert(defs, alloc);
      typeSafeInsert(defs, siz);
      typeSafeInsert(defs, off);
      typeSafeInsert(defs, aux);
      assms.push_back(mk<EQ>(allocP, mk<STORE>(alloc, h, h)));
      assms.push_back(mk<EQ>(sizP, mk<STORE>(siz, h, allSz)));
      assms.push_back(mk<EQ>(offP, mk<STORE>(off, h, offs)));
      return mknary<AND>(assms);
    }

    Expr updAllocs(Expr arg, Expr ty, Expr obj, Expr allSz, bool ptr)
    {
      Expr var = getVarByName(obj, ty);
      if (allSz != NULL)
      {
        // TODO: check types if isOpX<BMUL>(allSz)
        Expr h = getVarId(var);

        typeSafeInsert(defs, alloc);
        typeSafeInsert(defs, siz);
        typeSafeInsert(defs, off);
        typeSafeInsert(defs, aux);
        return mknary<AND>(ExprVector{
           mk<EQ>(allocP, mk<STORE>(alloc, h, h)),
           mk<EQ>(sizP, mk<STORE>(siz, h, allSz)),
           mk<EQ>(offP, mk<STORE>(off, h, nums[0])),
           mk<EQ>(auxP, mk<STORE>(aux, h, nums[4]))});
      }
      else
      {
        Expr offs;
        Expr argTmp = isOpX<BADD>(arg) ? arg->left() : arg;
        if (ptr)
        {
          Expr h = getVarId(var);
          if (isOpX<FAPP>(argTmp) &&
              lexical_cast<string>(argTmp->left()->left()) == "object-address")
          {
            typeSafeInsert(defs, alloc);
            Expr k = getVarId(getVarByName(argTmp->last(), ty));
            Expr m = mk<SELECT>(alloc, k);
            return
              mk<EQ>(allocP, mk<STORE>(alloc, h, m));
          }
          else if (isOpX<FAPP>(argTmp) &&
              lexical_cast<string>(argTmp->left()->left()).find(
                    "field-address") != string::npos) // field copy here
          {
            assert(0 && "Field pointers not supported");
          }

          Expr k = getVarId(argTmp);
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
            offs = mk<EQ>(offP, mk<STORE>(off, h, mk<SELECT>(off, k)));

          typeSafeInsert(defs, off);

          if (h == k) return offs;
          else
          {
            typeSafeInsert(defs, alloc);
            return mk<AND>(
              offs,
              mk<EQ>(allocP, mk<STORE>(alloc, h, mk<SELECT>(alloc, k))));
          }
        }

        ExprVector av;
        filter (arg, IsConst (), inserter(av, av.begin()));
        for (auto & a : av)
        {
          Expr var = getVarByName(a, ty);
          if (find(names.begin(), names.end(), a) != names.end())
          {
            Expr h = getVarId(a);
            arg = replaceAll(arg, a, mk<SELECT>(off, h));
            //TODO: make sure it's always offset
          }
        }
        varToDefs(defs, var);
        Expr name = mkTerm<string>(lexical_cast<string>(var) + "'", efac);
        var = mk<FAPP>(mk<FDECL>(name, ty));
        return mk<EQ>(var, arg);
      }
    }

    Expr assumeCstring(Expr ty, Expr obj)
    {
      Expr var = getVarByName(obj, ty);
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
      else if (isOpX<FAPP>(obj) && lexical_cast<string>
                        (obj->left()->left()) == "object-address")
      {
        var = getVarByName(obj->last(), ty);
        s = nums[0];
      }
      else if (isOpX<FAPP>(obj) && lexical_cast<string>
                   (obj->left()->left()).find("field-address") != string::npos)
      {
        auto field = concatFieldNames(obj);
        Expr objj = getObj(obj);
        assert(0 && "assert over fields not supported");
        var = getVarByName(obj->last(), ty);
        s = nums[0];
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
          auto posa = str.find("allocate");
          auto posd = str.find("deallocate");

          if ((pos != string::npos || posa != string::npos) &&
               posd == string::npos)
          {
            Expr obj = l->arg(2);
            Expr allSz = NULL;
            string fname = "";
            if (l->arity() > 3 && isOpX<FAPP>(l->arg(3)))
            {
              fname = lexical_cast<string>(l->arg(3)->left()->left());
              if (fname == "allocate" || fname == "reallocate")
              {
                allSz = l->arg(3)->last();
              }
            }
            if (isOpX<FAPP>(obj) && obj->left()->arity() > 2)
            {
              auto name = lexical_cast<string>(obj->left()->left());
              if (fname == "reallocate")
              {
                assert(name == "object-address");
                auto var = getVarId(l->arg(3)->arg(2));
                auto objj = getVarByName(obj->last(), ty);
                auto varr = getVarId(objj);

                typeSafeInsert(defs, siz);
                typeSafeInsert(defs, alloc);
                return mk<AND>(
                  mk<EQ>(sizP, mk<STORE>(siz, mk<SELECT>(alloc, var), allSz)),
                  mk<EQ>(allocP, mk<STORE>(alloc, varr, mk<SELECT>(alloc, var)))
                );
              }
              else if (name == "object-address" && fname != "reallocate")
              {
                // TODO: make sure that l->arg(3) also comes with "-p"
                return updAllocs(l->arg(3), l->left()->arg(3),
                      obj->last(), allSz,
                      str.find("update-state-p") != string::npos);
              }
              else if (name.find("field-address") != string::npos)
              {
                Expr wr = l->arg(3);
                wr = selRewrite(wr, names, alloc, getVarIdWr, this);

                if (isOpX<FAPP>(wr) && lexical_cast<string>  // struct copy
                                  (wr->left()->left()) == "object-address")
                {
                  wr = getVarByName(wr->last(), ty);
                  wr = getVarId(wr);
                  wr = mk<SELECT>(mk<SELECT>(mem,
                                    mk<SELECT>(alloc, wr)), nums[0]);
                }
                auto field = concatFieldNames(obj);
                Expr objj = getObj(obj);

                Expr addNew = mk<TRUE>(efac);
                Expr k = getVarId(field);
                typeSafeInsert(defs, mem);

                if (isOpX<SELECT>(objj))
                {
                  assert(!isOpX<SELECT>(field));
                  assert(objj->left() == mem);

                  return mk<EQ>(memP, mk<STORE>(mem,
                              objj->last(), mk<STORE>(objj, k, wr)));
                }
                Expr h;
                if (isOpX<FAPP>(objj) &&
                    lexical_cast<string>(objj->left()->left())
                                    == "object-address")
                {
                  Expr fmem = getMemField(wr);
                  objj = getVarByName(objj->last(), ty);
                  h = getVarId(objj);
                  typeSafeInsert(defs, alloc);
                  typeSafeInsert(defs, off);
                  ExprVector eqs = {mk<EQ>(allocP, mk<STORE>(alloc, h, h)),
                         mk<EQ>(offP, mk<STORE>(off, h, nums[0]))};
                  if (fmem != NULL)
                  {
                    // uncomment to add the r/w-ok preservation:
                    // typeSafeInsert(defs, aux);

                    eqs.push_back(mk<EQ>(sizP,
                                    mk<STORE>(siz, h, mk<SELECT>(siz, fmem))));
                    // mk<EQ>(auxP, mk<STORE>(aux, h, mk<SELECT>(aux, fmem))),
                    typeSafeInsert(defs, siz);
                  }

                  addNew = mknary<AND>(eqs);
                }
                else
                {
                  h = getVarId(objj);
                  h = mk<SELECT>(alloc, h);
                }

                return mk<AND>(addNew,
                        mk<EQ>(memP, mk<STORE>(mem, h,
                                     mk<STORE>(mk<SELECT>(mem, h), k, wr))));
              }
              else
              {
                outs () << "Not supported function: \"" << name << "\"\n";
                assert(0);
              }
            }
            else if (isOpX<BADD>(obj))
            {
              Expr ind = isOpX<BMUL>(obj->right()) ?
                              obj->right()->left() : obj->right();
              Expr h = obj->left();

              // TODO: add similar asserts everywhere
              assert(width(typeOf(l->arg(3))) == isOpX<BMUL>(obj->right()) ?
                     lexical_cast<int>(toMpz(obj->right()->right())) * 8 : 8);

              if (!isOpX<SELECT>(h)) h = getVarId(h);

              Expr t = mk<BADD>(mk<SELECT>(off, h), ind);
              Expr b =mk<SELECT>(mem, mk<SELECT>(alloc, h));
              typeSafeInsert(defs, mem);
              return mk<EQ>(memP, mk<STORE>(mem,
                mk<SELECT>(alloc, h),
                mk<STORE>(b, t, l->arg(3))));
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
          if (posd != string::npos)
          {
            // dealloc
            auto h = getVarId(l->last());
            typeSafeInsert(defs, mem);
            auto nondetMem = mkConst(mkTerm<string> ("nondetMem", efac),
                  typeOf(mem)->last());
            return mk<EQ>(memP,
              mk<STORE>(mem, mk<SELECT>(alloc, h), nondetMem));
          }
        }
      }
      else if (isOpX<NEQ>(exp))
      {
        auto lhs = exp->left();
        auto rhs = exp->right();

        if (find(names.begin(), names.end(), lhs) != names.end())
        {
          if (is_bvnum(rhs) && lexical_cast<string>(rhs->left()) == "0")
          {
            // lhs is not a nullpointer
            return mk<NEQ>(mk<SELECT>(alloc, getVarId(lhs)), nums[0]);
          }
        }
        if (find(names.begin(), names.end(), rhs) != names.end())
        {
          if (is_bvnum(lhs) && lexical_cast<string>(lhs->left()) == "0")
          {
            // rhs is not a nullpointer
            return mk<NEQ>(mk<SELECT>(alloc, getVarId(rhs)), nums[0]);
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

    Expr extract(Expr t, Expr e)
    {
      if (t == ty) return e;
      return bv::extract (width(t)-1, 0, e);
    }

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
          Expr type = exp->left()->last();
          if (find(names.begin(), names.end(), obj) != names.end())
          {
            // second iter: already added to vars, need to use offset
            Expr h = getVarId(obj);
            Expr t = mk<SELECT>(off, h);
            typeSafeInsert(defs, mem);
            typeSafeInsert(defs, off);
            return extract(type,
              mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, h)), t));
          }
          else if(isOpX<FAPP>(obj) &&
                 lexical_cast<string>(obj->left()->left()) == "object-address")
          {
            auto var = getVarByName(obj->last(), type);
            if (find(names.begin(), names.end(), var) == names.end())
              varToDefs(defs, var);
            if (isOpX<BOOL_TY>(type))
            {
              return var;
            }
            if (cell == NULL)
              return extract(type, var);
            else
              return extract(type, mk<BADD>(var, cell));
          }
          else if(isOpX<FAPP>(obj) &&
                 lexical_cast<string>(obj->left()->left()).
                    find("field-address") != string::npos)
          {
            Expr field = concatFieldNames(obj);
            Expr objj = getObj(obj);

            if (isOpX<FAPP>(objj) &&
                lexical_cast<string>(objj->left()->left())
                                == "object-address")
            {
              objj = getVarByName(objj->last(), ty);
            }

            if (isOpX<SELECT>(objj))
            {
              assert(!isOpX<SELECT>(field));
              assert(objj->left() == mem);

              auto h = getVarId(field);
              return mk<SELECT>(objj, h);
            }

            Expr h = getVarId(objj);
            Expr k = getVarId(field);
            typeSafeInsert(defs, mem);
            return extract(type,
              mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, h)), k));
          }
          else if (isOpX<BADD>(obj))
          {
            Expr toRead = obj->left();

            if (contains(names, toRead)) toRead = getVarId(toRead);

            typeSafeInsert(defs, mem);
            Expr t;
            if (isOpX<BMUL>(obj->right())) t = obj->right()->left();
            else t = obj->right();

            Expr res = mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, toRead)), t);
            return extract(type, res);
          }
          else if (isOpX<SELECT>(obj))
          {
            Expr toRead = obj;

            if (contains(names, toRead)) toRead = getVarId(toRead);
            return extract(type,
              mk<SELECT>(mk<SELECT>(mem, mk<SELECT>(alloc, toRead)),
                                              mk<SELECT>(off, toRead)));
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

  struct SelRewr
  {
    function<Expr(void*, Expr)> f;
    ExprVector& names;
    Expr alloc;
    void* obj;

    SelRewr (function<Expr(void*, Expr)> _f, ExprVector& _names, Expr _alloc,
              void* _obj): f(_f), names(_names), alloc(_alloc), obj(_obj) {}
    Expr operator() (Expr exp)
    {
      if (contains(names, exp))
        return mk<SELECT>(alloc, f(obj, exp));
      return exp;
    }
  };

  inline static Expr selRewrite (Expr exp, ExprVector& names, Expr alloc,
                    function<Expr(void*, Expr)> f, void* obj)
  {
    RW<SelRewr> rw(new SelRewr(f, names, alloc, obj));
    return dagVisit (rw, exp);
  }

  struct updSeq
  {
    Expr& res;
    bool& nested;
    updSeq (Expr& _res, bool& _nested) : res(_res), nested(_nested) {}
    Expr operator() (Expr exp)
    {
      if (!isOpX<FAPP>(exp)) return exp;
      auto name = lexical_cast<string>(exp->left()->left());
      if (name.find("update-state") == string::npos) return exp;
      if (res == NULL) res = exp;
      else nested = true;
      return exp;
    }
  };

  inline static void findUpd (Expr exp, Expr& res, bool& nested)
  {
    RW<updSeq> rw(new updSeq(res, nested));
    dagVisit (rw, exp);
  }
}

#endif
