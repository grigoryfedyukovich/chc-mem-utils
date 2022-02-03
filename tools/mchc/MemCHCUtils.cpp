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
  process(string(argv[argc-1]), debug);
  return 0;
}
