#include "ae/ExprSimplBv.hpp"
#include "ae/ExprSimpl.hpp"
#include "ae/SMTUtils.hpp"
#include "ufo/Smt/EZ3.hh"

using namespace ufo;
using namespace boost;

int getIntValue(const char * opt, int defValue, int argc, char ** argv)
{
  for (int i = 1; i < argc-1; i++)
  {
    if (strcmp(argv[i], opt) == 0)
    {
      char* p;
      int num = strtol(argv[i+1], &p, 10);
      if (*p) return 1;      // if used w/o arg, return boolean
      else return num;
    }
  }
  return defValue;
}

static inline bool insertSubexpr(Expr e, ExprSet & s)
{
  if (isOpX<FDECL>(e) || is_bvnum(e) || isOpX<BVSORT>(e)) return false;
  if (!containsOp<FDECL>(e)) return false;
  if (contains(s, e)) return false;
  s.insert(e);
  return true;
}

static inline void getSubExprs(Expr e, ExprSet & s)
{
  if (isOpX<BEXTRACT>(e) || isOpX<BROTATE_LEFT>(e) || isOpX<BROTATE_RIGHT>(e))
  {
    auto f = e->last();
    if (insertSubexpr(f, s)) getSubExprs(f, s);
  }
  else if (isOpX<BSEXT>(e) || isOpX<BZEXT>(e))
  {
    auto f = e->left();
    if (insertSubexpr(f, s)) getSubExprs(f, s);
  }
  else for (int i = 0; i < e->arity(); i++)
  {
    auto f = e->arg(i);
    if (insertSubexpr(f, s)) getSubExprs(f, s);
  }
}

static inline bool containsPair(set<pair<Expr, Expr>>& pairs, pair<Expr, Expr> p)
{
  for (auto & q : pairs)
    if (p.first == q.first && p.second == q.second)
      return true;
  return false;
}

// for debug:
void somePrint(Expr sb, int lvl)
{
  if (sb == NULL)
  {
    outs () << "  NULL ";
    return;
  }
  if (lvl == 0)
  {
    outs () << "  ... ";
    return;
  }
  if (isOpX<FDECL>(sb))
  {
    return;
  }

  if (dagSize(sb) < 20) // heuri
  {
    outs () << sb << " ";
    return;
  }

  if (isOpX<BCONCAT>(sb)) outs () << "concat ";
  else if (isOpX<ITE>(sb)) outs () << "ite ";
  else if (isOpX<BEXTRACT>(sb)) outs () << "extract ";
  else if (isOpX<BXOR>(sb)) outs () << "bvxor ";
  else if (isOpX<BOR>(sb)) outs () << "bor ";
  else if (isOpX<BADD>(sb)) outs () << "badd ";
  else if (isOpX<FAPP>(sb)) outs () << sb->left()->left() << " ";
  else if (isOpX<BNOT>(sb)) outs () << "bnot ";
  else if (isOpX<BAND>(sb)) outs () << "band ";
  else if (isOpX<EQ>(sb)) outs () << "= ";
  else if (isOpX<BLSHR>(sb)) outs () << "bvlshr ";
  else if (isOpX<BSUB>(sb)) outs () << "bvsub ";
  else if (isOpX<BROTATE_LEFT>(sb)) outs () << "rotate_left ";
  else if (isOpX<BROTATE_RIGHT>(sb)) outs () << "rotate_right ";
  else outs () << " ... ";
  if (sb->arity() > 0)
  {
    outs () << "(";
    for (int i = 0; i < sb->arity(); i++)
      somePrint(sb->arg(i), lvl - 1);
    outs () << ") ";
  }
}

bool findInl(Expr sb, map<Expr, set<pair<Expr, Expr>>>& in, int lvl, string name = "")
{
  bool f = false;
  for (auto & a : in)
  {
    for (auto & b : a.second)
    {
      if (sb == b.first &&
          lexical_cast<string>(b.second->left()).find(name) != string::npos)
      {
        f = true;
        break;
      }
    }
    if (f) break;
  }
  if (f) return true;

  if (lvl > 0)
  {
    f = true;
    for (int i = 0; i < sb->arity(); i++)
    {
      auto nest = !containsOp<FDECL> || findInl(sb->arg(i), in, lvl - 1, name);
      f &= nest;
    }
  }
  return f;
}

// TODO: remove global vars and flags:
map<Expr, set<pair<Expr, Expr>>> inls;
map<Expr, map<Expr, set<pair<Expr, Expr>>>> inlDefs;

struct Inl
{
  map<Expr, pair<Expr, ExprVector>>& qdefs;
  Inl(map<Expr, pair<Expr, ExprVector>>& _qdefs) : qdefs(_qdefs) {};

  Expr operator() (Expr exp)
  {
    for (auto & b : qdefs)
    {
      Expr decl = b.first->left(), body = b.second.first;
      ExprVector& args = b.second.second;
      if (isOpX<FAPP>(exp) && exp->left() == decl)
      {
        assert(exp->arity() - 1 == args.size());
        ExprMap args2;
        for (int i = 1; i < exp->arity(); i++)
          args2[args[i-1]] = exp->arg(i);
        auto res = replaceAll(body, args2, 0);
        refreshMap(rewrRepls, res, exp);
        inls[decl].insert({res, exp});

        for (auto & t : inlDefs)
        {
          if (t.first != decl) continue;
          for (auto & d : t.second)
          {
            for (auto & b : d.second)
            {
              cacheRewrs = false;  // to refactor
              auto e1 = replaceAll(b.first, args2, 0);
              auto e2 = replaceAll(b.second, args2, 0);
              cacheRewrs = true;   // to refactor
              inls[d.first].insert({e1, e2});
              refreshMap(rewrRepls, e1, e2);
            }
          }
        }
        return res;
      }
    }
    return exp;
  }
};

Expr inlBody (Expr exp, map<Expr, pair<Expr, ExprVector>>& qdefs)
{
  ufo::RW<Inl> rw(new Inl(qdefs));
  return dagVisit (rw, exp);
}

void inlineAll(Expr& a, map<Expr, pair<Expr, ExprVector>>& qdefs, ExprMap& m)
{
  rewrRepls.clear();
  cacheRewrs = true;  // to refactor
  a = inlBody(a, qdefs);

  for (auto & r : rewrRepls)
    if (isOpX<FAPP>(r.second))
      m.insert(r);
  cacheRewrs = false;  // to refactor
}

bool parse(Expr a, Expr& l, Expr& r, ExprMap& defs,
   map<Expr, pair<Expr, ExprVector>>& qdefs)
{
  ExprSet e;
  getConj(a, e);
  Expr eq = NULL;
  for (auto c : e)
  {
    if (isOpX<NEG>(c) && isOpX<EQ>(c->left()))
    {
      if (eq != NULL) return false;
      eq = c->left();
      l = eq->left();
      r = eq->right();
    }
    if (isOpX<EQ>(c))
    {
      defs[c->left()] = c->right();
    }
    if (isOpX<FORALL>(c))
    {
      c = regularizeQF(c);
      ExprVector args;
      for (int i = 0; i < c->arity()-1; i++)
        args.push_back(bind::fapp(c->arg(i)));
      auto def = c->last();
      assert(isOpX<EQ>(def));
      Expr fapp = def->left();
      def = def->right();
      qdefs[fapp] = {def, args};
    }
  }
  return l != NULL;
}

void inlineDefs(map<Expr, pair<Expr, ExprVector>>& qdefs, ExprSet& leaves,
                map<Expr, pair<Expr, ExprVector>>& qdefsLeaves, ExprMap& funrepl)
{
  while (true)
  {
    for (auto & a : qdefs)     // find leaves first
    {
      bool isLeaf = true;
      for (auto & b : qdefs)
        if (contains((Expr)a.second.first, (Expr)b.first->left()))
          isLeaf = false;
      if (isLeaf)
      {
        leaves.insert(a.first->left());
        qdefsLeaves.insert(a);
      }
    }

    map<Expr, pair<Expr, ExprVector>> inlinedQdefs;

    for (auto it = qdefs.begin(); it != qdefs.end(); )
    {
      inls.clear();
      auto q = *it;
      auto inlined = q.second.first;
      inlined = inlBody(inlined, qdefsLeaves);

      if (q.second.first == inlined) ++it;
      else
      {
        inlDefs[q.first->left()] = inls;
        Expr nFun = cloneVar(q.first,  mkTerm<string>("_inl_" +
          lexical_cast<string>(q.first->left()->left()), inlined->getFactory()));
        funrepl[q.first->left()] = nFun->left();
        inlDefs[nFun->left()] = inls;
        inlinedQdefs[nFun] = {inlined, q.second.second};
        it = qdefs.erase(it);
      }
    }

    if (inlinedQdefs.empty()) break;

    for (auto & a : qdefs) a.second.first = replaceAll(a.second.first, funrepl);
    qdefs.insert(inlinedQdefs.begin(), inlinedQdefs.end());
  }
}

Expr useDefs(Expr fla, ExprMap& defs)
{
  ExprSet v;
  for (int i = 0; i != defs.size(); i++)
  {
    filter(fla, bind::IsConst(), inserter (v, v.begin ()));
    auto fla1 = replaceAll(fla, defs, 0);
    filter(fla1, bind::IsConst(), inserter (v, v.begin ()));
    if (fla == fla1) break;
    fla = fla1;
  }
  return fla;
}

int fillChecks(SMTUtils& u, Expr l, Expr r,
  map<Expr, set<pair<Expr, Expr>>>& inlsL,
  map<Expr, set<pair<Expr, Expr>>>& inlsR,
  map<int,set<pair<Expr, Expr>>>& toCheck, int modls, bool debug)
{
  ExprSet subExprsL, subExprsR;
  getSubExprs(l, subExprsL);
  getSubExprs(r, subExprsR);

  ExprSet varsL, varsR;
  filter(l, bind::IsConst(), inserter (varsL, varsL.begin ()));
  filter(r, bind::IsConst(), inserter (varsR, varsR.begin ()));

  auto & efac = l->getFactory();
  auto ty = typeOf(l);
  auto outL = mkConst(mkTerm<string>("_fv1", efac), ty);
  auto outR = mkConst(mkTerm<string>("_fv2", efac), ty);
  auto fla = mk<AND>(mk<EQ>(outL, l), mk<EQ>(outR, r));
  ExprSet toSolve = {fla};
  for (auto & v : varsL)
    for (auto & w : varsR)
      if (v != w && typeOf(v) == typeOf(w))
        toSolve.insert(mk<NEQ>(v, w));

  vector<map <Expr, ExprSet>> smdls1, smdls2;

  int totNum = 0;

  for (int i = 0; i < modls; i++)
  {
    if (i > 0)
      for (auto & v : varsL)
        toSolve.insert(mk<NEQ>(v, u.getModel(v)));

    u.isSat(toSolve);
    map <Expr, ExprSet> tmp;
    smdls1.push_back(tmp);
    smdls2.push_back(tmp);

    for (auto & a : subExprsL)
    {
      if (!findInl(a, inlsL, 0)) continue;
      auto m = u.getModel(a);
      smdls1[i][m].insert(a);
    }

    for (auto & a : subExprsR)
    {
      if (!findInl(a, inlsR, 0)) continue;
      auto m = u.getModel(a);
      smdls2[i][m].insert(a);
    }

    if (i == 0)
    {
      for (auto & m1 : smdls1[i])
      {
        for (auto & m2 : smdls2[i])
        {
          if (m1.first == m2.first)
          {
            for (auto e1 : m1.second)
            {
              ExprSet v1;
              filter(e1, bind::IsConst(), inserter (v1, v1.begin ()));
              for (auto e2 : m2.second)
              {
                if (e1 == e2) continue;
                ExprSet v2;
                filter(e2, bind::IsConst(), inserter (v2, v2.begin ()));

                bool canCheck = true;
                for (auto & v : v1)
                {
                  bool found = false;
                  for (auto & w : v2)
                  {
                    if (v == w) found = true;
                  }
                  if (!found) canCheck = false;
                }

                if (canCheck)
                {
                  int id = dagSize(e1) + dagSize(e2);
                  toCheck[id].insert({e1, e2});
                  totNum++;
                }
              }
            }
          }
        }
      }
    }
    else
    {
      for (auto & p : toCheck)
        for (auto it = p.second.begin(); it != p.second.end();)
        {
          if (u.getModel((*it).first) != u.getModel((*it).second))
          {
            it = p.second.erase(it);
            totNum--;
          }
          else ++it;
        }
    }
  }
  outs () << "total number of equivalence checks: " << totNum << "\n";
  return totNum;
}

int findEquivs(ExprFactory& efac, map<int,set<pair<Expr, Expr>>>& toCheck,
      ExprMap& curRepls1, ExprMap& curRepls2,
      ExprMap& rewrReplsL, ExprMap& rewrReplsR,
      ExprVector& newdefs, bool toRepl, bool debug, int to, int dump)
{
  SMTUtils u100 (efac, to);
  set<pair<Expr, Expr>> checkedPairs;
  int indets = 0, unsats = 0, sats = 0, skips = 0;
  for (auto & p : toCheck)
  {
    bool doublebreak = false;
    for (auto & c : p.second)
    {
      if (containsPair(checkedPairs, c))
      {
        continue;
      }

      Expr e1, e2;
      if (toRepl)
      {
        e1 = replaceAll(c.first, curRepls1);
        e2 = replaceAll(c.second, curRepls2);
      }
      else
      {
        e1 = c.first;
        e2 = c.second;
      }

      bool toSkip = false;
      for (auto & d : checkedPairs)
      {
        if (replaceAll(e1, d.first, d.second) == e2)
        {
          if (debug)
          {
            outs () << "\n found eq (w/o SMT) of common size " << p.first << ":\n";
            outs () << "spec: "; somePrint(replaceAll(c.first, rewrReplsL), 4);
            outs () << "\n";
            outs () << "impl: "; somePrint( replaceAll(c.second, rewrReplsR), 4);
            outs () << "\n";
          }
          skips++;
          checkedPairs.insert(c);
          toSkip = true;
          break;
        }
      }

      if (toSkip) continue;

      if (dump)
      {
        u100.dumpToFile(mk<NEQ>(e1, e2));
        continue;
      }

      auto res = u100.isSat(mk<NEQ>(e1, e2));
      if (false == res)
      {
        checkedPairs.insert(c);
        auto r1 = replaceAll(c.first, rewrReplsL), r2 = replaceAll(c.second, rewrReplsR);
        Expr app = dagSize(r1) < dagSize(r2) ? r1 : r2;

        if (debug)
        {
          outs () << "\n found eq (with SMT) of common size " << p.first << ":\n";
          outs () << "spec: "; somePrint(r1, 4);
          outs () << "\n";
          outs () << "impl: "; somePrint(r2, 4);
          outs () << "\n";
        }

        if (app != NULL)
        {
          curRepls1[e1] = app;
          curRepls2[e2] = app;
          curRepls1[c.first] = app;
          curRepls2[c.second] = app;
          newdefs.push_back(mk<EQ>(e1, app));
        }
        unsats++;
      }
      else if (res == true)
        sats++;
      else
        indets++;
    }
    if (doublebreak) break;
  }
  outs () << "intermediate stats on checks:\n " <<
    "unsat: " << unsats << ", " <<
    "skipped: " << skips << ", " <<
    "sat: " << sats << ", " <<
    "TO: " << indets << "\n";
  return u100.getStat();
}

const string SMTUtils::filename = "_dump_formula_";

int main (int argc, char ** argv)
{
  int debug = getIntValue("--debug", 0, argc, argv);
  int mdls = getIntValue("--mdls", 1, argc, argv);
  int repls = getIntValue("--reuse", 1, argc, argv);
  int to = getIntValue("--to", 1000, argc, argv);
  int dump = getIntValue("--dump", 0, argc, argv);

  ExprFactory efac;
  EZ3 z3(efac);
  SMTUtils u (efac);
  Expr a = z3_from_smtlib_file (z3, argv [1]), l = NULL, r = NULL;
  ExprVector newdefs;
  ExprSet leaves;
  map<int,set<pair<Expr, Expr>>> toCheck;
  ExprMap rewrReplsL, rewrReplsR, curRepls1, curRepls2, defs, funrepl;
  map<Expr, pair<Expr, ExprVector>> qdefs, qdefsLeaves;

  if (!parse(a, l, r, defs, qdefs))
  {
    errs() << "Incompatible input format\n";
    return 0;
  }
  inlineDefs(qdefs, leaves, qdefsLeaves, funrepl);

  l = replaceAll(useDefs(l, defs), funrepl);
  r = replaceAll(useDefs(r, defs), funrepl);

  inlineAll(l, qdefs, rewrReplsL);
  auto inlsL = inls;
  inls.clear();

  inlineAll(r, qdefs, rewrReplsR);
  auto inlsR = inls;

  int st1 = fillChecks(u, l, r, inlsL, inlsR, toCheck, mdls, debug);
  int st2 = findEquivs(efac, toCheck, curRepls1, curRepls2, rewrReplsL,
    rewrReplsR, newdefs, repls, debug, to, dump);
  if (dump)
  {
    assert (st1 == st2);
    outs () << " dumped to " << st1
            << " files\nrun `ls -lt " << SMTUtils::filename << "*.smt2`\n";
    exit(0);
  }

  l = replaceAll(l, curRepls1);
  r = replaceAll(r, curRepls2);

  ExprSet eqCheck = {mk<NEQ>(l, r)};
  for (int i = 0; i < newdefs.size() + 1; i ++)
  {
    if (false == u.isSat(eqCheck))
    {
      outs () << "unsat\n";
      exit(0);
    }
    if (i < newdefs.size()) eqCheck.insert(newdefs[i]);
    outs () << "refined\n";
  }
  outs () << "unknown\n";
  exit(0);
  return 0;
}
