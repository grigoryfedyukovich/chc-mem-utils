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
(assert (forall ((|ς| state)) (=> (S20 |ς|) (S21 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char") (_ bv1 64))))))

; find_symbols
(declare-fun S22 (state) Bool)
(declare-fun update-state-s8 (state (_ BitVec 64) (_ BitVec 8)) state)
; set_to true
(assert (forall ((|ς| state) (|nondet::S22-0| (_ BitVec 8))) (=> (S21 |ς|) (S22 (update-state-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char") |nondet::S22-0|)))))

; find_symbols
(declare-fun S23 (state) Bool)
(declare-fun evaluate-s8 (state (_ BitVec 64)) (_ BitVec 8))
; set_to true
(assert (forall ((|ς| state)) (=> (S22 |ς|) (S23 (update-state-s8 |ς| (object-address "main::1::length1") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char")))))))

; find_symbols
(declare-fun S24 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S23 |ς|) (S24 (enter-scope-state-p64 |ς| (object-address "main::1::length2") (_ bv1 64))))))

; find_symbols
(declare-fun S25 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S24 |ς|) (S25 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0") (_ bv1 64))))))

; find_symbols
(declare-fun S26 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S26-0| (_ BitVec 8))) (=> (S25 |ς|) (S26 (update-state-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0") |nondet::S26-0|)))))

; find_symbols
(declare-fun S27 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S26 |ς|) (S27 (update-state-s8 |ς| (object-address "main::1::length2") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0")))))))

; find_symbols
(declare-fun S28 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S27 |ς|) (S28 (enter-scope-state-p64 |ς| (object-address "main::1::n") (_ bv1 64))))))

; find_symbols
(declare-fun S29 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S28 |ς|) (S29 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$1") (_ bv1 64))))))

; find_symbols
(declare-fun S30 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S30-0| (_ BitVec 8))) (=> (S29 |ς|) (S30 (update-state-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$1") |nondet::S30-0|)))))

; find_symbols
(declare-fun S31 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S30 |ς|) (S31 (update-state-s8 |ς| (object-address "main::1::n") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$1")))))))

; find_symbols
(declare-fun S32T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S31 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 32)))) (S32T |ς|))))

; find_symbols
(declare-fun S32 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S31 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 32))) (S32 |ς|))))

; find_symbols
(declare-fun S33 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33 (update-state-s8 |ς| (object-address "main::1::length1") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S34in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S32T |ς|) (S34in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S33 |ς|) (S34in |ς|))))

; set_to true (equal)
(declare-fun S34 (state) Bool)
(assert (= S34 S34in))

; find_symbols
(declare-fun S35T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S34 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length2"))) (_ bv1 32)))) (S35T |ς|))))

; find_symbols
(declare-fun S35 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S34 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length2"))) (_ bv1 32))) (S35 |ς|))))

; find_symbols
(declare-fun S36 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (update-state-s8 |ς| (object-address "main::1::length2") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S37in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S35T |ς|) (S37in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37in |ς|))))

; set_to true (equal)
(declare-fun S37 (state) Bool)
(assert (= S37 S37in))

; find_symbols
(declare-fun S38T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S37 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::n"))) (_ bv1 32)))) (S38T |ς|))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S37 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::n"))) (_ bv1 32))) (S38 |ς|))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (update-state-s8 |ς| (object-address "main::1::n") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S40in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38T |ς|) (S40in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40in |ς|))))

; set_to true (equal)
(declare-fun S40 (state) Bool)
(assert (= S40 S40in))

; find_symbols
(declare-fun S41T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S40 |ς|) (not (or (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::n"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1")))) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::n"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length2"))))))) (S41T |ς|))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S40 |ς|) (or (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::n"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1")))) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::n"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length2")))))) (S41 |ς|))))

; find_symbols
(declare-fun S42 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S41 |ς|) (S42 (update-state-s8 |ς| (object-address "return'") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S43 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (exit-scope-state-p64 |ς| (object-address "main::1::n"))))))

; find_symbols
(declare-fun S44 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S43 |ς|) (S44 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$1"))))))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45 (exit-scope-state-p64 |ς| (object-address "main::1::length2"))))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0"))))))

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S46 |ς|) (S47 (exit-scope-state-p64 |ς| (object-address "main::1::length1"))))))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S48 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S49 (state) Bool)
(assert (= S49 S48))

; set_to true (equal)
(declare-fun S50 (state) Bool)
(assert (= S50 S41T))

; find_symbols
(declare-fun S51 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S50 |ς|) (S51 (enter-scope-state-p64 |ς| (object-address "main::1::nondetString1") (_ bv8 64))))))

; find_symbols
(declare-fun S52 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S52 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S53 (state) Bool)
(declare-fun allocate (state (_ BitVec 64)) (_ BitVec 64))
(declare-fun update-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S54 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (update-state-p64 |ς| (object-address "main::1::nondetString1") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (enter-scope-state-p64 |ς| (object-address "main::1::nondetString2") (_ bv8 64))))))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length2"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S58 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S57 |ς|) (S58 (update-state-p64 |ς| (object-address "main::1::nondetString2") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc$0")))))))

; set_to true (equal)
(declare-fun S59 (state) Bool)
(assert (= S59 S58))

; find_symbols
(declare-fun S60 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60 (enter-scope-state-p64 |ς| (object-address "memcmp::s1") (_ bv8 64))))))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61 (update-state-p64 |ς| (object-address "memcmp::s1") (evaluate-p64 |ς| (object-address "main::1::nondetString1")))))))

; find_symbols
(declare-fun S62 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62 (enter-scope-state-p64 |ς| (object-address "memcmp::s2") (_ bv8 64))))))

; find_symbols
(declare-fun S63 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S62 |ς|) (S63 (update-state-p64 |ς| (object-address "memcmp::s2") (evaluate-p64 |ς| (object-address "main::1::nondetString2")))))))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (enter-scope-state-p64 |ς| (object-address "memcmp::n") (_ bv1 64))))))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65 (update-state-s8 |ς| (object-address "memcmp::n") (evaluate-s8 |ς| (object-address "main::1::n")))))))

; find_symbols
(declare-fun S66 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S65 |ς|) (S66 (enter-scope-state-p64 |ς| (object-address "memcmp::1::res") (_ bv4 64))))))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S66 |ς|) (S67 (update-state-s32 |ς| (object-address "memcmp::1::res") (_ bv0 32))))))

; find_symbols
(declare-fun S68 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67 |ς|) (S68 (enter-scope-state-p64 |ς| (object-address "memcmp::1::sc1") (_ bv8 64))))))

; find_symbols
(declare-fun S69 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69 (update-state-p64 |ς| (object-address "memcmp::1::sc1") (evaluate-p64 |ς| (object-address "memcmp::s1")))))))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S70 (enter-scope-state-p64 |ς| (object-address "memcmp::1::sc2") (_ bv8 64))))))

; find_symbols
(declare-fun S71 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S71 (update-state-p64 |ς| (object-address "memcmp::1::sc2") (evaluate-p64 |ς| (object-address "memcmp::s2")))))))

; set_to true (equal)
(declare-fun S72 (state) Bool)
(assert (= S72 S71))

; find_symbols
(declare-fun S95 (state) Bool)
; find_symbols
(declare-fun S73in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S95 |ς|) (S73in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73in |ς|))))

; find_symbols
(declare-fun S73T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S73in |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "memcmp::n"))) (_ bv0 32))))) (S73T |ς|))))

; find_symbols
(declare-fun S73 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S73in |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "memcmp::n"))) (_ bv0 32)))) (S73 |ς|))))

; find_symbols
(declare-fun S74 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S73 |ς|) (S74 (enter-scope-state-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc1") (_ bv8 64))))))

; find_symbols
(declare-fun S75 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S74 |ς|) (S75 (update-state-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc1") (evaluate-p64 |ς| (object-address "memcmp::1::sc1")))))))

; find_symbols
(declare-fun S76 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S75 |ς|) (S76 (update-state-p64 |ς| (object-address "memcmp::1::sc1") (bvadd (evaluate-p64 |ς| (object-address "memcmp::1::sc1")) (_ bv1 64)))))))

; find_symbols
(declare-fun S77 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S77 (enter-scope-state-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc2") (_ bv8 64))))))

; find_symbols
(declare-fun S78 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S77 |ς|) (S78 (update-state-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc2") (evaluate-p64 |ς| (object-address "memcmp::1::sc2")))))))

; find_symbols
(declare-fun S79 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S78 |ς|) (S79 (update-state-p64 |ς| (object-address "memcmp::1::sc2") (bvadd (evaluate-p64 |ς| (object-address "memcmp::1::sc2")) (_ bv1 64)))))))

; set_to true

; set_to true (equal)
(declare-fun S80 (state) Bool)
(assert (= S80 S79))

; set_to true

; set_to true (equal)
(declare-fun S81 (state) Bool)
(assert (= S81 S80))

; find_symbols
(declare-fun S82 (state) Bool)
(declare-fun evaluate-u8 (state (_ BitVec 64)) (_ BitVec 8))
; set_to true
(assert (forall ((|ς| state)) (=> (S81 |ς|) (S82 (update-state-s32 |ς| (object-address "memcmp::1::res") (bvsub ((_ zero_extend 24) (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc1")))) ((_ zero_extend 24) (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc2"))))))))))

(declare-fun evaluate-s32 (state (_ BitVec 64)) (_ BitVec 32))
; find_symbols
(declare-fun S83T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S82 |ς|) (not (not (= (evaluate-s32 |ς| (object-address "memcmp::1::res")) (_ bv0 32))))) (S83T |ς|))))

; find_symbols
(declare-fun S83 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S82 |ς|) (not (= (evaluate-s32 |ς| (object-address "memcmp::1::res")) (_ bv0 32)))) (S83 |ς|))))

; set_to true (equal)
(declare-fun S84 (state) Bool)
(assert (= S84 S83))

; find_symbols
(declare-fun S85 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S84 |ς|) (S85 (exit-scope-state-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc2"))))))

; find_symbols
(declare-fun S86 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S85 |ς|) (S86 (exit-scope-state-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc1"))))))

; find_symbols
(declare-fun S87 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S86 |ς|) (S87 (exit-scope-state-p64 |ς| (object-address "memcmp::1::sc2"))))))

; find_symbols
(declare-fun S88 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S87 |ς|) (S88 (exit-scope-state-p64 |ς| (object-address "memcmp::1::sc1"))))))

; find_symbols
(declare-fun S89 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S88 |ς|) (S89 (exit-scope-state-p64 |ς| (object-address "memcmp::1::res"))))))

; set_to true (equal)
(declare-fun S90 (state) Bool)
(assert (= S90 S89))

; set_to true (equal)
(declare-fun S91 (state) Bool)
(assert (= S91 S83T))

; find_symbols
(declare-fun S92 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S91 |ς|) (S92 (exit-scope-state-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc2"))))))

; find_symbols
(declare-fun S93 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S92 |ς|) (S93 (exit-scope-state-p64 |ς| (object-address "memcmp::$tmp::tmp_post_sc1"))))))

; find_symbols
(declare-fun S94 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S93 |ς|) (S94 (update-state-s8 |ς| (object-address "memcmp::n") (bvsub (evaluate-s8 |ς| (object-address "memcmp::n")) (_ bv1 8)))))))

; set_to true
(assert (= S95 S94))

; set_to true (equal)
(declare-fun S96 (state) Bool)
(assert (= S96 S73T))

; set_to true (equal)
(declare-fun S97 (state) Bool)
(assert (= S97 S96))

; find_symbols
(declare-fun S98 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S97 |ς|) (S98 (exit-scope-state-p64 |ς| (object-address "memcmp::1::sc2"))))))

; find_symbols
(declare-fun S99 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S98 |ς|) (S99 (exit-scope-state-p64 |ς| (object-address "memcmp::1::sc1"))))))

; find_symbols
(declare-fun S100 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S99 |ς|) (S100 (exit-scope-state-p64 |ς| (object-address "memcmp::1::res"))))))

; set_to true (equal)
(declare-fun S101 (state) Bool)
(assert (= S101 S100))

; find_symbols
(declare-fun S102in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S90 |ς|) (S102in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S101 |ς|) (S102in |ς|))))

; set_to true (equal)
(declare-fun S102 (state) Bool)
(assert (= S102 S102in))

; find_symbols
(declare-fun S103 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S102 |ς|) (S103 (exit-scope-state-p64 |ς| (object-address "memcmp::s1"))))))

; find_symbols
(declare-fun S104 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S103 |ς|) (S104 (exit-scope-state-p64 |ς| (object-address "memcmp::s2"))))))

; find_symbols
(declare-fun S105 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S104 |ς|) (S105 (exit-scope-state-p64 |ς| (object-address "memcmp::n"))))))

; find_symbols
(declare-fun S106 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S105 |ς|) (S106 (update-state-s8 |ς| (object-address "return'") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S107 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S106 |ς|) (S107 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString2"))))))

; find_symbols
(declare-fun S108 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S107 |ς|) (S108 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0"))))))

; find_symbols
(declare-fun S109 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S108 |ς|) (S109 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString1"))))))

; find_symbols
(declare-fun S110 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S109 |ς|) (S110 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S111 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S110 |ς|) (S111 (exit-scope-state-p64 |ς| (object-address "main::1::n"))))))

; find_symbols
(declare-fun S112 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S111 |ς|) (S112 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$1"))))))

; find_symbols
(declare-fun S113 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S112 |ς|) (S113 (exit-scope-state-p64 |ς| (object-address "main::1::length2"))))))

; find_symbols
(declare-fun S114 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S113 |ς|) (S114 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0"))))))

; find_symbols
(declare-fun S115 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S114 |ς|) (S115 (exit-scope-state-p64 |ς| (object-address "main::1::length1"))))))

; find_symbols
(declare-fun S116 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S115 |ς|) (S116 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S117 (state) Bool)
(assert (= S117 S116))

; find_symbols
(declare-fun S118in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S118in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S117 |ς|) (S118in |ς|))))

; set_to true (equal)
(declare-fun S118 (state) Bool)
(assert (= S118 S118in))

; set_to true (equal)
(declare-fun S119 (state) Bool)
(assert (= S119 S118))

; set_to true (equal)
(declare-fun S120 (state) Bool)
(assert (= S120 S119))

(check-sat)


; end of SMT2 file
