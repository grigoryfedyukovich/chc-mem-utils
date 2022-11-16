; SMT 2
(set-info :source "cprover")
(set-option :produce-models true)
; function __CPROVER__start

(declare-sort state 0)
(declare-fun initial-state (state) Bool)
; find_symbols
(declare-fun SInitial (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> true (SInitial |ς|))))

; set_to true (equal)
(declare-fun S0 (state) Bool)
(assert (= S0 SInitial))

; set_to true (equal)
(declare-fun S1 (state) Bool)
(assert (= S1 S0))

; set_to true (equal)
(declare-fun S2 (state) Bool)
(assert (= S2 S1))

; set_to true (equal)
(declare-fun S3 (state) Bool)
(assert (= S3 S2))

; set_to true (equal)
(declare-fun S4 (state) Bool)
(assert (= S4 S3))

; set_to true (equal)
(declare-fun S5 (state) Bool)
(assert (= S5 S4))

; set_to true (equal)
(declare-fun S6 (state) Bool)
(assert (= S6 S5))

; set_to true (equal)
(declare-fun S7 (state) Bool)
(assert (= S7 S6))

; set_to true (equal)
(declare-fun S8 (state) Bool)
(assert (= S8 S7))

; set_to true (equal)
(declare-fun S9 (state) Bool)
(assert (= S9 S8))

; set_to true (equal)
(declare-fun S10 (state) Bool)
(assert (= S10 S9))

; set_to true (equal)
(declare-fun S11 (state) Bool)
(assert (= S11 S10))

; set_to true (equal)
(declare-fun S12 (state) Bool)
(assert (= S12 S11))

; find_symbols
(declare-fun S13 (state) Bool)
(declare-fun object-address (String) (_ BitVec 64))
(declare-fun update-state-s32 (state (_ BitVec 64) (_ BitVec 32)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S12 |ς|) (S13 (update-state-s32 |ς| (object-address "__CPROVER_rounding_mode") (_ bv0 32))))))

; set_to true (equal)
(declare-fun S14 (state) Bool)
(assert (= S14 S13))

; set_to true (equal)
(declare-fun S15 (state) Bool)
(assert (= S15 S14))

; set_to true (equal)
(declare-fun S16 (state) Bool)
(assert (= S16 S15))

; set_to true (equal)
(declare-fun S17 (state) Bool)
(assert (= S17 S16))

; set_to true (equal)
(declare-fun S18 (state) Bool)
(assert (= S18 S17))

; set_to true (equal)
(declare-fun S19 (state) Bool)
(assert (= S19 S18))

; find_symbols
(declare-fun S20 (state) Bool)
(declare-fun enter-scope-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S19 |ς|) (S20 (enter-scope-state-p64 |ς| (object-address "main::1::length1") (_ bv1 64))))))

; find_symbols
(declare-fun S21 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S20 |ς|) (S21 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int") (_ bv1 64))))))

; find_symbols
(declare-fun S22 (state) Bool)
(declare-fun update-state-s8 (state (_ BitVec 64) (_ BitVec 8)) state)
; set_to true
(assert (forall ((|ς| state) (|nondet::S22-0| (_ BitVec 8))) (=> (S21 |ς|) (S22 (update-state-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int") |nondet::S22-0|)))))

; find_symbols
(declare-fun S23 (state) Bool)
(declare-fun evaluate-s8 (state (_ BitVec 64)) (_ BitVec 8))
; set_to true
(assert (forall ((|ς| state)) (=> (S22 |ς|) (S23 (update-state-s8 |ς| (object-address "main::1::length1") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int")))))))

; find_symbols
(declare-fun S24T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S23 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 32)))) (S24T |ς|))))

; find_symbols
(declare-fun S24 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S23 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 32))) (S24 |ς|))))

; find_symbols
(declare-fun S25 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S24 |ς|) (S25 (update-state-s8 |ς| (object-address "main::1::length1") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S26in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S24T |ς|) (S26in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S25 |ς|) (S26in |ς|))))

; set_to true (equal)
(declare-fun S26 (state) Bool)
(assert (= S26 S26in))

; find_symbols
(declare-fun S27 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S26 |ς|) (S27 (enter-scope-state-p64 |ς| (object-address "main::1::nondetString1") (_ bv8 64))))))

; find_symbols
(declare-fun S28 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S27 |ς|) (S28 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S29 (state) Bool)
(declare-fun allocate (state (_ BitVec 64)) (_ BitVec 64))
(declare-fun update-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S28 |ς|) (S29 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S30 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S29 |ς|) (S30 (update-state-p64 |ς| (object-address "main::1::nondetString1") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; set_to true

; set_to true (equal)
(declare-fun S31 (state) Bool)
(assert (= S31 S30))

; find_symbols
(declare-fun S32 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::nondetString1")) ((_ sign_extend 32) (bvsub ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 32)))) ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S33 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_strdup") (_ bv8 64))))))

; set_to true (equal)
(declare-fun S34 (state) Bool)
(assert (= S34 S33))

; find_symbols
(declare-fun S35 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S34 |ς|) (S35 (enter-scope-state-p64 |ς| (object-address "strdup::str") (_ bv8 64))))))

; find_symbols
(declare-fun S36 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (update-state-p64 |ς| (object-address "strdup::str") (evaluate-p64 |ς| (object-address "main::1::nondetString1")))))))

; find_symbols
(declare-fun S37 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (enter-scope-state-p64 |ς| (object-address "strdup::1::bufsz") (_ bv1 64))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (enter-scope-state-p64 |ς| (object-address "strdup::$tmp::return_value_cstrlen") (_ bv1 64))))))

; set_to true (equal)
(declare-fun S39 (state) Bool)
(assert (= S39 S38))

; find_symbols
(declare-fun S40 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40 (enter-scope-state-p64 |ς| (object-address "cstrlen::s") (_ bv8 64))))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (update-state-p64 |ς| (object-address "cstrlen::s") (evaluate-p64 |ς| (object-address "strdup::str")))))))

; find_symbols
(declare-fun S42 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S41 |ς|) (S42 (enter-scope-state-p64 |ς| (object-address "cstrlen::1::p") (_ bv8 64))))))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (update-state-p64 |ς| (object-address "cstrlen::1::p") (evaluate-p64 |ς| (object-address "cstrlen::s")))))))

; find_symbols
(declare-fun S47 (state) Bool)
; find_symbols
(declare-fun S44in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S44in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S43 |ς|) (S44in |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S44 (state) Bool)
(assert (= S44 S44in))

; find_symbols
(declare-fun S45T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S44 |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrlen::1::p")))) (_ bv0 32))))) (S45T |ς|))))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S44 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrlen::1::p")))) (_ bv0 32)))) (S45 |ς|))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (update-state-p64 |ς| (object-address "cstrlen::1::p") (bvadd (evaluate-p64 |ς| (object-address "cstrlen::1::p")) (_ bv1 64)))))))

; set_to true
(assert (= S47 S46))

; set_to true (equal)
(declare-fun S48 (state) Bool)
(assert (= S48 S45T))

; find_symbols
(declare-fun S49 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S49 (update-state-s8 |ς| (object-address "strdup::$tmp::return_value_cstrlen") ((_ extract 7 0) (bvsub (evaluate-p64 |ς| (object-address "cstrlen::1::p")) (evaluate-p64 |ς| (object-address "cstrlen::s")))))))))

; find_symbols
(declare-fun S50 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50 (exit-scope-state-p64 |ς| (object-address "cstrlen::1::p"))))))

; set_to true (equal)
(declare-fun S51 (state) Bool)
(assert (= S51 S50))

; set_to true (equal)
(declare-fun S52 (state) Bool)
(assert (= S52 S51))

; find_symbols
(declare-fun S53 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (exit-scope-state-p64 |ς| (object-address "cstrlen::s"))))))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (update-state-s8 |ς| (object-address "strdup::1::bufsz") ((_ extract 7 0) (bvadd ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "strdup::$tmp::return_value_cstrlen"))) (_ bv1 32))))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (enter-scope-state-p64 |ς| (object-address "strdup::1::cpy") (_ bv8 64))))))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56 (enter-scope-state-p64 |ς| (object-address "strdup::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (update-state-p64 |ς| (object-address "strdup::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "strdup::1::bufsz"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S58 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S57 |ς|) (S58 (update-state-p64 |ς| (object-address "strdup::1::cpy") (evaluate-p64 |ς| (object-address "strdup::$tmp::return_value_malloc")))))))

; find_symbols
(declare-fun S59T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S58 |ς|) (not (= (evaluate-p64 |ς| (object-address "strdup::1::cpy")) (_ bv0 64)))) (S59T |ς|))))

; find_symbols
(declare-fun S59 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S58 |ς|) (= (evaluate-p64 |ς| (object-address "strdup::1::cpy")) (_ bv0 64))) (S59 |ς|))))

; find_symbols
(declare-fun S60 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60 (update-state-p64 |ς| (object-address "main::$tmp::return_value_strdup") (_ bv0 64))))))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61 (exit-scope-state-p64 |ς| (object-address "strdup::1::cpy"))))))

; find_symbols
(declare-fun S62 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62 (exit-scope-state-p64 |ς| (object-address "strdup::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S63 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S62 |ς|) (S63 (exit-scope-state-p64 |ς| (object-address "strdup::$tmp::return_value_cstrlen"))))))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (exit-scope-state-p64 |ς| (object-address "strdup::1::bufsz"))))))

; set_to true (equal)
(declare-fun S65 (state) Bool)
(assert (= S65 S64))

; set_to true (equal)
(declare-fun S66 (state) Bool)
(assert (= S66 S59T))

; set_to true (equal)
(declare-fun S67 (state) Bool)
(assert (= S67 S66))

; find_symbols
(declare-fun S68 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67 |ς|) (S68 (enter-scope-state-p64 |ς| (object-address "cstrcpy::s1") (_ bv8 64))))))

; find_symbols
(declare-fun S69 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69 (update-state-p64 |ς| (object-address "cstrcpy::s1") (evaluate-p64 |ς| (object-address "strdup::1::cpy")))))))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S70 (enter-scope-state-p64 |ς| (object-address "cstrcpy::s2") (_ bv8 64))))))

; find_symbols
(declare-fun S71 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S71 (update-state-p64 |ς| (object-address "cstrcpy::s2") (evaluate-p64 |ς| (object-address "strdup::str")))))))

; find_symbols
(declare-fun S72 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S71 |ς|) (S72 (enter-scope-state-p64 |ς| (object-address "cstrcpy::1::dst") (_ bv8 64))))))

; find_symbols
(declare-fun S73 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73 (update-state-p64 |ς| (object-address "cstrcpy::1::dst") (evaluate-p64 |ς| (object-address "cstrcpy::s1")))))))

; find_symbols
(declare-fun S74 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S73 |ς|) (S74 (enter-scope-state-p64 |ς| (object-address "cstrcpy::1::src") (_ bv8 64))))))

; find_symbols
(declare-fun S75 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S74 |ς|) (S75 (update-state-p64 |ς| (object-address "cstrcpy::1::src") (evaluate-p64 |ς| (object-address "cstrcpy::s2")))))))

; find_symbols
(declare-fun S89 (state) Bool)
; find_symbols
(declare-fun S76in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S89 |ς|) (S76in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S75 |ς|) (S76in |ς|))))

; find_symbols
(declare-fun S76 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S76in |ς|) (S76 (enter-scope-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_post_dst") (_ bv8 64))))))

; find_symbols
(declare-fun S77 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S77 (update-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_post_dst") (evaluate-p64 |ς| (object-address "cstrcpy::1::dst")))))))

; find_symbols
(declare-fun S78 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S77 |ς|) (S78 (update-state-p64 |ς| (object-address "cstrcpy::1::dst") (bvadd (evaluate-p64 |ς| (object-address "cstrcpy::1::dst")) (_ bv1 64)))))))

; find_symbols
(declare-fun S79 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S78 |ς|) (S79 (enter-scope-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_post_src") (_ bv8 64))))))

; find_symbols
(declare-fun S80 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S79 |ς|) (S80 (update-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_post_src") (evaluate-p64 |ς| (object-address "cstrcpy::1::src")))))))

; find_symbols
(declare-fun S81 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S80 |ς|) (S81 (update-state-p64 |ς| (object-address "cstrcpy::1::src") (bvadd (evaluate-p64 |ς| (object-address "cstrcpy::1::src")) (_ bv1 64)))))))

; find_symbols
(declare-fun S82 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S81 |ς|) (S82 (enter-scope-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_assign") (_ bv1 64))))))

; set_to true

; set_to true (equal)
(declare-fun S83 (state) Bool)
(assert (= S83 S82))

; find_symbols
(declare-fun S84 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S83 |ς|) (S84 (update-state-s8 |ς| (object-address "cstrcpy::$tmp::tmp_assign") (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrcpy::$tmp::tmp_post_src"))))))))

; set_to true

; set_to true (equal)
(declare-fun S85 (state) Bool)
(assert (= S85 S84))

; find_symbols
(declare-fun S86 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S85 |ς|) (S86 (update-state-s8 |ς| (evaluate-p64 |ς| (object-address "cstrcpy::$tmp::tmp_post_dst")) (evaluate-s8 |ς| (object-address "cstrcpy::$tmp::tmp_assign")))))))

; find_symbols
(declare-fun S87T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S86 |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrcpy::$tmp::tmp_assign"))) (_ bv0 32))))) (S87T |ς|))))

; find_symbols
(declare-fun S87 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S86 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrcpy::$tmp::tmp_assign"))) (_ bv0 32)))) (S87 |ς|))))

; set_to true (equal)
(declare-fun S88 (state) Bool)
(assert (= S88 S87))

; set_to true
(assert (= S89 S88))

; set_to true (equal)
(declare-fun S90 (state) Bool)
(assert (= S90 S87T))

; set_to true (equal)
(declare-fun S91 (state) Bool)
(assert (= S91 S90))

; find_symbols
(declare-fun S92 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S91 |ς|) (S92 (exit-scope-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_assign"))))))

; find_symbols
(declare-fun S93 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S92 |ς|) (S93 (exit-scope-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_post_src"))))))

; find_symbols
(declare-fun S94 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S93 |ς|) (S94 (exit-scope-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_post_dst"))))))

; find_symbols
(declare-fun S95 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S94 |ς|) (S95 (exit-scope-state-p64 |ς| (object-address "cstrcpy::1::src"))))))

; find_symbols
(declare-fun S96 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S95 |ς|) (S96 (exit-scope-state-p64 |ς| (object-address "cstrcpy::1::dst"))))))

; set_to true (equal)
(declare-fun S97 (state) Bool)
(assert (= S97 S96))

; set_to true (equal)
(declare-fun S98 (state) Bool)
(assert (= S98 S97))

; find_symbols
(declare-fun S99 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S98 |ς|) (S99 (exit-scope-state-p64 |ς| (object-address "cstrcpy::s1"))))))

; find_symbols
(declare-fun S100 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S99 |ς|) (S100 (exit-scope-state-p64 |ς| (object-address "cstrcpy::s2"))))))

; find_symbols
(declare-fun S101 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S100 |ς|) (S101 (update-state-p64 |ς| (object-address "main::$tmp::return_value_strdup") (evaluate-p64 |ς| (object-address "strdup::1::cpy")))))))

; find_symbols
(declare-fun S102 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S101 |ς|) (S102 (exit-scope-state-p64 |ς| (object-address "strdup::1::cpy"))))))

; find_symbols
(declare-fun S103 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S102 |ς|) (S103 (exit-scope-state-p64 |ς| (object-address "strdup::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S104 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S103 |ς|) (S104 (exit-scope-state-p64 |ς| (object-address "strdup::$tmp::return_value_cstrlen"))))))

; find_symbols
(declare-fun S105 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S104 |ς|) (S105 (exit-scope-state-p64 |ς| (object-address "strdup::1::bufsz"))))))

; set_to true (equal)
(declare-fun S106 (state) Bool)
(assert (= S106 S105))

; find_symbols
(declare-fun S107in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S65 |ς|) (S107in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S106 |ς|) (S107in |ς|))))

; set_to true (equal)
(declare-fun S107 (state) Bool)
(assert (= S107 S107in))

; find_symbols
(declare-fun S108 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S107 |ς|) (S108 (exit-scope-state-p64 |ς| (object-address "strdup::str"))))))

; find_symbols
(declare-fun S109 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S108 |ς|) (S109 (update-state-s8 |ς| (object-address "return'") ((_ extract 7 0) (evaluate-p64 |ς| (object-address "main::$tmp::return_value_strdup"))))))))

; find_symbols
(declare-fun S110 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S109 |ς|) (S110 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_strdup"))))))

; find_symbols
(declare-fun S111 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S110 |ς|) (S111 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString1"))))))

; find_symbols
(declare-fun S112 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S111 |ς|) (S112 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S113 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S112 |ς|) (S113 (exit-scope-state-p64 |ς| (object-address "main::1::length1"))))))

; find_symbols
(declare-fun S114 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S113 |ς|) (S114 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int"))))))

; set_to true (equal)
(declare-fun S115 (state) Bool)
(assert (= S115 S114))

; set_to true (equal)
(declare-fun S116 (state) Bool)
(assert (= S116 S115))

; set_to true (equal)
(declare-fun S117 (state) Bool)
(assert (= S117 S116))

; set_to true (equal)
(declare-fun S118 (state) Bool)
(assert (= S118 S117))

(check-sat)


; end of SMT2 file
