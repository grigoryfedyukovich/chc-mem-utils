Automated equivalence checker
==================================================================

The tool implements an algorithm for SMT-based checking of equivalence of given Implementation w.r.t. a given Specification. The tool expects an SMT-LIB-V2 file that has a single proof goal written as `(assert (not (= (SPEC IMPL))))` and may have functions calls. Examples can be found in the `bench` directory. The tool automatically identifies semantically equivalent subexpressions in `SPEC` and `IMPL` and replaces them with uninterpreted functions.

Installation
============

Assumes preinstalled <a href="https://gmplib.org/">GMP</a>, and Boost (libboost-system1.74-dev) packages.

* `cd aeval ; mkdir build ; cd build`
* `cmake ../`
* `make` to build dependencies (Z3)
* `make` (again) to build the tool

The `eq` binary can be found at `build/tools/equiv/eq`. Run `eq <SMT file> [--debug X] [--mdls Y] [--reuse Z]` with `0`, `1` (i.e., use one model), and `1` (i.e., reuse equivalence results) as default values for `X`, `Y`, and `Z`, respectively.
