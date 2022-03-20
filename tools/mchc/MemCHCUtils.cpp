#include "deep/MemProcessor.hpp"

using namespace ufo;
using namespace std;

bool getBoolValue(const char * opt, bool defValue, int argc, char ** argv)
{
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], opt) == 0) return true;
  }
  return defValue;
}

char * getStrValue(const char * opt, char * defValue, int argc, char ** argv)
{
  for (int i = 1; i < argc-1; i++)
  {
    if (strcmp(argv[i], opt) == 0)
    {
      return argv[i+1];
    }
  }
  return defValue;
}

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

void getStrValues(const char * opt, vector<string> & values, int argc, char ** argv)
{
  for (int i = 1; i < argc-1; i++)
  {
    if (strcmp(argv[i], opt) == 0)
    {
      values.push_back(string(argv[i+1]));
    }
  }
}

int main (int argc, char ** argv)
{
  const char *OPT_DEBUG = "--debug";
  const char *OPT_HELP = "--help";

  if (getBoolValue(OPT_HELP, false, argc, argv) || argc == 1)
  {
    outs () <<
        "* * *                                 MemCHCUtils v.0.1 - Copyright (C) 2022.                                 * * *\n" <<
        "                                    Based on FreqHorn. Grigory Fedyukovich et al.                                   \n\n" <<
        "Usage:                          Purpose:\n" <<
        "  mchc [options] <file.smt2>      simplify CHCs and serialize to `chc.smt2`\n\n" <<
        "Options:\n" <<
        "  " << OPT_DEBUG << " <LVL>                   print debugging information during run (default level: 0)\n\n";

    return 0;
  }

  int debug = getIntValue(OPT_DEBUG, 0, argc, argv);

  //get names
  const string pref("; var_id: ");
  map<int, string> var_ids;
  ifstream fi;
  string line;
  fi.open(argv[argc-1]);
  while(getline(fi, line))
  {
    int pos1 = line.find(pref, 0);
    if (pos1 != string::npos)
    {
      pos1 += pref.length();
      int pos2 = line.find("; ", 1);
      auto str1 = line.substr(pos1, pos2 - pos1);
      auto str2 = stoi(line.substr(pos2 + 2));
      var_ids[str2] = str1;
      if (debug) outs() << "comment parsed: `" << str1 << "` `" << str2 << "`\n";
    }
  }

  process(string(argv[argc-1]), var_ids, debug);
  return 0;
}
