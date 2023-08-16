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
void somePrint(Expr sb, int lvl = 3, bool top = true)
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
    outs () << sb << (top ? "\n" : " ");
    return;
  }

  if (isOpX<BCONCAT>(sb)) outs () << "concat ";
  else if (isOpX<ITE>(sb)) outs () << "ite ";
  else if (isOpX<BEXTRACT>(sb)) outs () << "extract ";
  else if (isOpX<BXOR>(sb)) outs () << "bvxor ";
  else if (isOpX<BOR>(sb)) outs () << "bvor ";
  else if (isOpX<BADD>(sb)) outs () << "bvadd ";
  else if (isOpX<FAPP>(sb)) outs () << sb->left()->left() << " ";
  else if (isOpX<BNOT>(sb)) outs () << "bvnot ";
  else if (isOpX<BAND>(sb)) outs () << "bvand ";
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
      somePrint(sb->arg(i), lvl - 1, false);
    outs () << ") ";
  }
  if (top) outs () << "\n";
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
vector<pair<Expr, Expr>> inlsOrder;

inline static void sortedInsert(Expr e1, Expr e2)
{
  int i = 0;
  for (; i < inlsOrder.size(); i++)
    if (!contains(inlsOrder[i].first, e1)) break;
  inlsOrder.insert(inlsOrder.begin() + i, {e1, e2});
}

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
        sortedInsert(res, exp);
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
              sortedInsert(e1, e2);
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
  inlsOrder.clear();
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

void inlineDefs(ExprFactory& efac,
              map<Expr, pair<Expr, ExprVector>>& qdefs, ExprSet& leaves,
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
      if (q.second.first == inlined)
      {
        ++it;
        continue;
      }
      inlDefs[q.first->left()] = inls;
      Expr nFun = cloneVar(q.first,  mkTerm<string>("_inl_" +
                    lexical_cast<string>(q.first->left()->left()), efac));
      funrepl[q.first->left()] = nFun->left();
      inlDefs[nFun->left()] = inls;
      inlinedQdefs[nFun] = {inlined, q.second.second};
      it = qdefs.erase(it);
    }

    if (inlinedQdefs.empty()) break;

    qdefs.insert(inlinedQdefs.begin(), inlinedQdefs.end());

    for (auto & a : qdefs)
      a.second.first = replaceAll(a.second.first, funrepl);
  }

  // transitive clausure now:
  bool toCont = true;
  while (toCont)
  {
    toCont = false;
    for (auto it = funrepl.begin(); it != funrepl.end(); ++it)
    {
      for (auto it2 = funrepl.begin(); it2 != funrepl.end(); ++it2)
      {
        if (it == it2) continue;
        if (it->second == it2->first)
        {
          it->second = it2->second;
          funrepl.erase(it2);
          toCont = true;
          break;
        }
      }
      if (toCont) break;
    }
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
  map<int,set<pair<Expr, Expr>>>& toCheck,
  int mdls, int inlDist, bool debug)
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

  outs () << "total number of considered pairs: "
          << (subExprsL.size() * subExprsR.size()) << "\n";

  int totNum = 0;
  for (int i = 0; i < mdls; i++)
  {
    if (i > 0)
      for (auto & v : varsL)
        toSolve.insert(mk<NEQ>(v, u.getModel(v)));

    u.isSat(toSolve);
    map <Expr, ExprSet> tmp;
    smdls1.push_back(tmp);
    smdls2.push_back(tmp);
    int w = 0, v = 0;

    for (auto & a : subExprsL)
    {
      if (!findInl(a, inlsL, inlDist)) continue;
      auto m = u.getModel(a);
      smdls1[i][m].insert(a);
      w++;
    }

    for (auto & a : subExprsR)
    {
      if (!findInl(a, inlsR, inlDist)) continue;
      auto m = u.getModel(a);
      smdls2[i][m].insert(a);
      v++;
    }

    if (i == 0)
    {
      outs () << "  after inlining-based filtering: " << (w*v) << "\n";
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
  outs () << "  after model-based filtering: " << totNum << "\n";
  return totNum;
}

Expr prepareSubsts (vector<pair<Expr, Expr>>& inlsOrder,
                    ExprMap& funrepl, Expr subexpr)
{
  Expr repl = subexpr;
  for (auto & a : inlsOrder)
  {
    repl = replaceAll(repl, a.first, a.second);
    // GF: to consider skipping some of the replacements
  }
  return replaceAllRev(repl, funrepl);
}

int findEquivs(ExprFactory& efac,
      vector<pair<Expr, Expr>>& inlsOrderL,
      vector<pair<Expr, Expr>>& inlsOrderR,
      map<int,set<pair<Expr, Expr>>>& toCheck,
      ExprMap& curRepls1, ExprMap& curRepls2,
      ExprMap& rewrReplsL, ExprMap& rewrReplsR,
      ExprVector& newdefs, ExprMap funrepl,
      int batch, bool toRepl, bool debug, int to, int dump)
{
  SMTUtils u100 (efac, to);
  set<pair<Expr, Expr>> checked;
  int indets = 0, unsats = 0, sats = 0, skips = 0, oldIndets = 0;
  bool doublebreak = false;
  for (auto & p : toCheck)
  {
    for (auto & c : p.second)
    {
      if (containsPair(checked, c)) continue;
      Expr e1 = toRepl ? replaceAll(c.first, curRepls1, 0) : c.first;
      Expr e2 = toRepl ? replaceAll(c.second, curRepls2, 0) : c.second;
      if (dump)
      {
        u100.dumpToFile(mk<NEQ>(e1, e2));
        continue;
      }

      tribool res = true, skipped = false;

      for (auto it = checked.begin(); res && it != checked.end(); ++it)
        if (replaceAll(e1, it->first, it->second) == e2)
          res = false;

      if (res == true) res = u100.isSat(mk<NEQ>(e1, e2));
      else skipped = true;

      if (false == res)
      {
        checked.insert(c);
        Expr r1 = prepareSubsts (inlsOrderL, funrepl, c.first);
        Expr r2 = prepareSubsts (inlsOrderR, funrepl, c.second);
        Expr app = dagSize(r1) < dagSize(r2) ? r1 : r2;
        assert(app != NULL);
        curRepls1[c.first] = app;
        curRepls2[c.second] = app;
        if (debug)
        {
          outs () << "\n  found eq (" << (skipped ? "w/o" : "with")
                  << " SMT) of common size " << p.first << ":\n";
          outs () << "    spec (size " << dagSize (r1) << "): "; somePrint(r1);
          outs () << "    impl (size " << dagSize (r2) << "): "; somePrint(r2);
        }
        else
        {
          outs() << "🟢";
          outs().flush();
        }
        newdefs.push_back(mk<EQ>(e1, app));
        if (skipped) skips++;
        else unsats++;
      }
      else
      {
        if (res == true) sats++;
        else indets++;
        if (!debug) outs() << "🔴";
      }

      if ((unsats + skips + sats + indets) % batch == 0)
      {
        if (debug)
        {
          outs () << "\ncurrent stats on checks:\n  " <<
            "unsat: " << unsats << ", " <<
            "skipped: " << skips << ", " <<
            "sat: " << sats << ", " <<
            "TO: " << indets << "\n";
        }
        else outs () << "\n";
        if (indets - oldIndets == batch)
        {
          if (debug) outs() << "\n";
          doublebreak = true;
          break;
        }
        oldIndets = indets;
      }
    }
    if (doublebreak) break;
  }
  outs () << "\nfinal stats on checks:\n  " <<
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
  int inlDist = getIntValue("--inl", 0, argc, argv);
  int batch = getIntValue("--batch", 10, argc, argv);
  int repls = getIntValue("--reuse", 1, argc, argv);
  int to = getIntValue("--to", 250, argc, argv);
  int dump = getIntValue("--dump", 0, argc, argv);

  ExprFactory efac;
  EZ3 z3(efac);
  SMTUtils u (efac, to*1000);
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

  inlineDefs(efac, qdefs, leaves, qdefsLeaves, funrepl);

  l = replaceAll(useDefs(l, defs), funrepl);
  r = replaceAll(useDefs(r, defs), funrepl);

  inlineAll(l, qdefs, rewrReplsL);
  auto inlsL = inls;
  auto inlsOrderL = inlsOrder;
  inls.clear();
  inlineAll(r, qdefs, rewrReplsR);
  auto inlsR = inls;
  auto inlsOrderR = inlsOrder;

  int st1 = fillChecks(u, l, r, inlsL, inlsR, toCheck, mdls, inlDist, debug);
  int st2 = findEquivs(efac, inlsOrderL, inlsOrder, toCheck, curRepls1, curRepls2,
    rewrReplsL, rewrReplsR, newdefs, funrepl, batch, repls, debug, to, dump);
  if (dump)
  {
    assert (st1 == st2);
    outs () << " dumped to " << st1
            << " files\nrun `ls -lt " << SMTUtils::filename << "*.smt2`\n";
    exit(0);
  }

  l = replaceAll(l, curRepls1, 0);
  r = replaceAll(r, curRepls2, 0);

  ExprSet eqCheck = {mk<NEQ>(l, r)};
  for (int i = 0; i < newdefs.size() + 1; i ++)
  {
    auto res = u.isSat(eqCheck);
    if (false == res)
    {
      errs () << "unsat\n";
      exit(0);
    }
    if (indeterminate(res))
    {
      errs () << "unknown\n";
      exit(0);
    }
    if (i < newdefs.size()) eqCheck.insert(newdefs[i]);
    outs () << "refined\n";
  }
  outs () << "unknown\n";
  exit(0);
  return 0;
}
