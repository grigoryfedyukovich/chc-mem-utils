Utils for CHC encodings of C programs with pointers using [CBMC](https://github.com/diffblue/cbmc/tree/saswat-chc-rebased)
==================================================================

The tool implements a CHC-to-CHC transformation based on [FreqHorn](https://github.com/freqhorn/freqhorn) and [Z3](https://github.com/Z3Prover/z3).

Installation
============

Compiles with gcc-7 (on Linux) and clang-1001 (on Mac). Assumes preinstalled <a href="https://gmplib.org/">GMP</a>, and Boost (libboost-system1.74-dev) packages. Additionally, armadillo package to get candidates from behaviors.

* `cd aeval ; mkdir build ; cd build`
* `cmake ../`
* `make` to build dependencies (Z3)
* `make` (again) to build MCHC

The `mchc` binary can be found at `build/tools/mchc/mchc`. After the alias analysis, the `chc.smt2` file is generated (make sure to move it to a secure place, otherwise it will be rewritten in the next run).
Run `mchc --help` for the usage info.
