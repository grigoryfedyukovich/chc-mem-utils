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
(assert (forall ((|ς| state)) (=> (S19 |ς|) (S20 (enter-scope-state-p64 |ς| (object-address "main::1::length") (_ bv1 64))))))

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
(assert (forall ((|ς| state)) (=> (S22 |ς|) (S23 (update-state-s8 |ς| (object-address "main::1::length") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char")))))))

; find_symbols
(declare-fun S24 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S23 |ς|) (S24 (enter-scope-state-p64 |ς| (object-address "main::1::n") (_ bv1 64))))))

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
(assert (forall ((|ς| state)) (=> (S26 |ς|) (S27 (update-state-s8 |ς| (object-address "main::1::n") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0")))))))

; find_symbols
(declare-fun S28T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S27 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32)))) (S28T |ς|))))

; find_symbols
(declare-fun S28 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S27 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32))) (S28 |ς|))))

; find_symbols
(declare-fun S29 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S28 |ς|) (S29 (update-state-s8 |ς| (object-address "main::1::length") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S30in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S28T |ς|) (S30in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S29 |ς|) (S30in |ς|))))

; set_to true (equal)
(declare-fun S30 (state) Bool)
(assert (= S30 S30in))

; find_symbols
(declare-fun S31T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S30 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::n"))) (_ bv1 32)))) (S31T |ς|))))

; find_symbols
(declare-fun S31 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S30 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::n"))) (_ bv1 32))) (S31 |ς|))))

; find_symbols
(declare-fun S32 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (update-state-s8 |ς| (object-address "main::1::n") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S33in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S31T |ς|) (S33in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33in |ς|))))

; set_to true (equal)
(declare-fun S33 (state) Bool)
(assert (= S33 S33in))

; find_symbols
(declare-fun S34 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S33 |ς|) (S34 (enter-scope-state-p64 |ς| (object-address "main::1::nondetArea") (_ bv8 64))))))

; find_symbols
(declare-fun S35 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S34 |ς|) (S35 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S36 (state) Bool)
(declare-fun allocate (state (_ BitVec 64)) (_ BitVec 64))
(declare-fun update-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::n"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S37 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (update-state-p64 |ς| (object-address "main::1::nondetArea") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (enter-scope-state-p64 |ς| (object-address "main::1::nondetString") (_ bv8 64))))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S40 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (update-state-p64 |ς| (object-address "main::1::nondetString") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc$0")))))))

; set_to true

; set_to true (equal)
(declare-fun S42 (state) Bool)
(assert (= S42 S41))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::nondetString")) ((_ sign_extend 32) (bvsub ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32)))) ((_ extract 7 0) (_ bv0 32)))))))

; set_to true (equal)
(declare-fun S44 (state) Bool)
(assert (= S44 S43))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45 (enter-scope-state-p64 |ς| (object-address "cstrncpy::s1") (_ bv8 64))))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (update-state-p64 |ς| (object-address "cstrncpy::s1") (evaluate-p64 |ς| (object-address "main::1::nondetArea")))))))

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S46 |ς|) (S47 (enter-scope-state-p64 |ς| (object-address "cstrncpy::s2") (_ bv8 64))))))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S48 (update-state-p64 |ς| (object-address "cstrncpy::s2") (evaluate-p64 |ς| (object-address "main::1::nondetString")))))))

; find_symbols
(declare-fun S49 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S49 (enter-scope-state-p64 |ς| (object-address "cstrncpy::n") (_ bv1 64))))))

; find_symbols
(declare-fun S50 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50 (update-state-s8 |ς| (object-address "cstrncpy::n") (evaluate-s8 |ς| (object-address "main::1::n")))))))

; find_symbols
(declare-fun S51 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S50 |ς|) (S51 (enter-scope-state-p64 |ς| (object-address "cstrncpy::1::dst") (_ bv8 64))))))

; find_symbols
(declare-fun S52 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S52 (update-state-p64 |ς| (object-address "cstrncpy::1::dst") (evaluate-p64 |ς| (object-address "cstrncpy::s1")))))))

; find_symbols
(declare-fun S53 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (enter-scope-state-p64 |ς| (object-address "cstrncpy::1::src") (_ bv8 64))))))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (update-state-p64 |ς| (object-address "cstrncpy::1::src") (evaluate-p64 |ς| (object-address "cstrncpy::s2")))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (enter-scope-state-p64 |ς| (object-address "cstrncpy::1::us") (_ bv8 64))))))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56 (enter-scope-state-p64 |ς| (object-address "cstrncpy::1::n2") (_ bv1 64))))))

; find_symbols
(declare-fun S90 (state) Bool)
; find_symbols
(declare-fun S57in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S90 |ς|) (S57in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57in |ς|))))

; find_symbols
(declare-fun S57T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S57in |ς|) (not (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncpy::n"))) (_ bv0 32)))) (S57T |ς|))))

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S57in |ς|) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncpy::n"))) (_ bv0 32))) (S57 |ς|))))

; find_symbols
(declare-fun S58 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S57 |ς|) (S58 (update-state-s8 |ς| (object-address "cstrncpy::n") (bvsub (evaluate-s8 |ς| (object-address "cstrncpy::n")) (_ bv1 8)))))))

; find_symbols
(declare-fun S59 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S59 (enter-scope-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_dst") (_ bv8 64))))))

; find_symbols
(declare-fun S60 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60 (update-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_dst") (evaluate-p64 |ς| (object-address "cstrncpy::1::dst")))))))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61 (update-state-p64 |ς| (object-address "cstrncpy::1::dst") (bvadd (evaluate-p64 |ς| (object-address "cstrncpy::1::dst")) (_ bv1 64)))))))

; find_symbols
(declare-fun S62 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62 (enter-scope-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_src") (_ bv8 64))))))

; find_symbols
(declare-fun S63 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S62 |ς|) (S63 (update-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_src") (evaluate-p64 |ς| (object-address "cstrncpy::1::src")))))))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (update-state-p64 |ς| (object-address "cstrncpy::1::src") (bvadd (evaluate-p64 |ς| (object-address "cstrncpy::1::src")) (_ bv1 64)))))))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65 (enter-scope-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_assign") (_ bv1 64))))))

; set_to true

; set_to true (equal)
(declare-fun S66 (state) Bool)
(assert (= S66 S65))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S66 |ς|) (S67 (update-state-s8 |ς| (object-address "cstrncpy::$tmp::tmp_assign") (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_src"))))))))

; set_to true

; set_to true (equal)
(declare-fun S68 (state) Bool)
(assert (= S68 S67))

; find_symbols
(declare-fun S69 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69 (update-state-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_dst")) (evaluate-s8 |ς| (object-address "cstrncpy::$tmp::tmp_assign")))))))

; find_symbols
(declare-fun S70T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S69 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncpy::$tmp::tmp_assign"))) (_ bv0 32)))) (S70T |ς|))))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S69 |ς|) (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncpy::$tmp::tmp_assign"))) (_ bv0 32))) (S70 |ς|))))

; find_symbols
(declare-fun S71 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S71 (update-state-p64 |ς| (object-address "cstrncpy::1::us") (evaluate-p64 |ς| (object-address "cstrncpy::1::dst")))))))

; find_symbols
(declare-fun S72 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S71 |ς|) (S72 (update-state-s8 |ς| (object-address "cstrncpy::1::n2") (evaluate-s8 |ς| (object-address "cstrncpy::n")))))))

; find_symbols
(declare-fun S82 (state) Bool)
; find_symbols
(declare-fun S73in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S82 |ς|) (S73in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73in |ς|))))

; find_symbols
(declare-fun S73 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S73in |ς|) (S73 (enter-scope-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_n2") (_ bv1 64))))))

; find_symbols
(declare-fun S74 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S73 |ς|) (S74 (update-state-s8 |ς| (object-address "cstrncpy::$tmp::tmp_post_n2") (evaluate-s8 |ς| (object-address "cstrncpy::1::n2")))))))

; find_symbols
(declare-fun S75 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S74 |ς|) (S75 (update-state-s8 |ς| (object-address "cstrncpy::1::n2") (bvsub (evaluate-s8 |ς| (object-address "cstrncpy::1::n2")) (_ bv1 8)))))))

; find_symbols
(declare-fun S76T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S75 |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncpy::$tmp::tmp_post_n2"))) (_ bv0 32))))) (S76T |ς|))))

; find_symbols
(declare-fun S76 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S75 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncpy::$tmp::tmp_post_n2"))) (_ bv0 32)))) (S76 |ς|))))

; find_symbols
(declare-fun S77 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S77 (enter-scope-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_us") (_ bv8 64))))))

; find_symbols
(declare-fun S78 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S77 |ς|) (S78 (update-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_us") (evaluate-p64 |ς| (object-address "cstrncpy::1::us")))))))

; find_symbols
(declare-fun S79 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S78 |ς|) (S79 (update-state-p64 |ς| (object-address "cstrncpy::1::us") (bvadd (evaluate-p64 |ς| (object-address "cstrncpy::1::us")) (_ bv1 64)))))))

; set_to true

; set_to true (equal)
(declare-fun S80 (state) Bool)
(assert (= S80 S79))

; find_symbols
(declare-fun S81 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S80 |ς|) (S81 (update-state-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_us")) ((_ extract 7 0) (_ bv0 32)))))))

; set_to true
(assert (= S82 S81))

; set_to true (equal)
(declare-fun S83 (state) Bool)
(assert (= S83 S76T))

; find_symbols
(declare-fun S84 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S83 |ς|) (S84 (exit-scope-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_us"))))))

; set_to true (equal)
(declare-fun S85 (state) Bool)
(assert (= S85 S84))

; set_to true (equal)
(declare-fun S86 (state) Bool)
(assert (= S86 S70T))

; find_symbols
(declare-fun S87 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S86 |ς|) (S87 (exit-scope-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_assign"))))))

; find_symbols
(declare-fun S88 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S87 |ς|) (S88 (exit-scope-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_src"))))))

; find_symbols
(declare-fun S89 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S88 |ς|) (S89 (exit-scope-state-p64 |ς| (object-address "cstrncpy::$tmp::tmp_post_dst"))))))

; set_to true
(assert (= S90 S89))

; find_symbols
(declare-fun S91in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S57T |ς|) (S91in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S85 |ς|) (S91in |ς|))))

; set_to true (equal)
(declare-fun S91 (state) Bool)
(assert (= S91 S91in))

; set_to true (equal)
(declare-fun S92 (state) Bool)
(assert (= S92 S91))

; find_symbols
(declare-fun S93 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S92 |ς|) (S93 (exit-scope-state-p64 |ς| (object-address "cstrncpy::1::n2"))))))

; find_symbols
(declare-fun S94 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S93 |ς|) (S94 (exit-scope-state-p64 |ς| (object-address "cstrncpy::1::us"))))))

; find_symbols
(declare-fun S95 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S94 |ς|) (S95 (exit-scope-state-p64 |ς| (object-address "cstrncpy::1::src"))))))

; find_symbols
(declare-fun S96 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S95 |ς|) (S96 (exit-scope-state-p64 |ς| (object-address "cstrncpy::1::dst"))))))

; set_to true (equal)
(declare-fun S97 (state) Bool)
(assert (= S97 S96))

; set_to true (equal)
(declare-fun S98 (state) Bool)
(assert (= S98 S97))

; find_symbols
(declare-fun S99 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S98 |ς|) (S99 (exit-scope-state-p64 |ς| (object-address "cstrncpy::s1"))))))

; find_symbols
(declare-fun S100 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S99 |ς|) (S100 (exit-scope-state-p64 |ς| (object-address "cstrncpy::s2"))))))

; find_symbols
(declare-fun S101 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S100 |ς|) (S101 (exit-scope-state-p64 |ς| (object-address "cstrncpy::n"))))))

; find_symbols
(declare-fun S102 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S101 |ς|) (S102 (update-state-s8 |ς| (object-address "return'") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S103 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S102 |ς|) (S103 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString"))))))

; find_symbols
(declare-fun S104 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S103 |ς|) (S104 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0"))))))

; find_symbols
(declare-fun S105 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S104 |ς|) (S105 (exit-scope-state-p64 |ς| (object-address "main::1::nondetArea"))))))

; find_symbols
(declare-fun S106 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S105 |ς|) (S106 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S107 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S106 |ς|) (S107 (exit-scope-state-p64 |ς| (object-address "main::1::n"))))))

; find_symbols
(declare-fun S108 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S107 |ς|) (S108 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0"))))))

; find_symbols
(declare-fun S109 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S108 |ς|) (S109 (exit-scope-state-p64 |ς| (object-address "main::1::length"))))))

; find_symbols
(declare-fun S110 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S109 |ς|) (S110 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S111 (state) Bool)
(assert (= S111 S110))

; set_to true (equal)
(declare-fun S112 (state) Bool)
(assert (= S112 S111))

; set_to true (equal)
(declare-fun S113 (state) Bool)
(assert (= S113 S112))

; set_to true (equal)
(declare-fun S114 (state) Bool)
(assert (= S114 S113))

(check-sat)


; end of SMT2 file