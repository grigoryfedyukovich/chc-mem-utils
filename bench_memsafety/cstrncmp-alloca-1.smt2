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
(declare-fun S28T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S27 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 32)))) (S28T |ς|))))

; find_symbols
(declare-fun S28 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S27 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 32))) (S28 |ς|))))

; find_symbols
(declare-fun S29 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S28 |ς|) (S29 (update-state-s8 |ς| (object-address "main::1::length1") ((_ extract 7 0) (_ bv1 32)))))))

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
(assert (forall ((|ς| state)) (=> (and (S30 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length2"))) (_ bv1 32)))) (S31T |ς|))))

; find_symbols
(declare-fun S31 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S30 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length2"))) (_ bv1 32))) (S31 |ς|))))

; find_symbols
(declare-fun S32 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (update-state-s8 |ς| (object-address "main::1::length2") ((_ extract 7 0) (_ bv1 32)))))))

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
(assert (forall ((|ς| state)) (=> (S33 |ς|) (S34 (enter-scope-state-p64 |ς| (object-address "main::1::nondetString1") (_ bv8 64))))))

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
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S37 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (update-state-p64 |ς| (object-address "main::1::nondetString1") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (enter-scope-state-p64 |ς| (object-address "main::1::nondetString2") (_ bv8 64))))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S40 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length2"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (update-state-p64 |ς| (object-address "main::1::nondetString2") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc$0")))))))

; set_to true

; set_to true (equal)
(declare-fun S42 (state) Bool)
(assert (= S42 S41))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::nondetString1")) ((_ sign_extend 32) (bvsub ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length1"))) (_ bv1 32)))) ((_ extract 7 0) (_ bv0 32)))))))

; set_to true

; set_to true (equal)
(declare-fun S44 (state) Bool)
(assert (= S44 S43))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::nondetString2")) ((_ sign_extend 32) (bvsub ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length2"))) (_ bv1 32)))) ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$1") (_ bv1 64))))))

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S47-0| (_ BitVec 8))) (=> (S46 |ς|) (S47 (update-state-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$1") |nondet::S47-0|)))))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S48 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_cstrncmp") (_ bv1 64))))))

; set_to true (equal)
(declare-fun S49 (state) Bool)
(assert (= S49 S48))

; find_symbols
(declare-fun S50 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50 (enter-scope-state-p64 |ς| (object-address "cstrncmp::s1") (_ bv8 64))))))

; find_symbols
(declare-fun S51 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S50 |ς|) (S51 (update-state-p64 |ς| (object-address "cstrncmp::s1") (evaluate-p64 |ς| (object-address "main::1::nondetString1")))))))

; find_symbols
(declare-fun S52 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S52 (enter-scope-state-p64 |ς| (object-address "cstrncmp::s2") (_ bv8 64))))))

; find_symbols
(declare-fun S53 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (update-state-p64 |ς| (object-address "cstrncmp::s2") (evaluate-p64 |ς| (object-address "main::1::nondetString2")))))))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (enter-scope-state-p64 |ς| (object-address "cstrncmp::n") (_ bv1 64))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (update-state-s8 |ς| (object-address "cstrncmp::n") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$1")))))))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56 (enter-scope-state-p64 |ς| (object-address "cstrncmp::1::uc1") (_ bv1 64))))))

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (enter-scope-state-p64 |ς| (object-address "cstrncmp::1::uc2") (_ bv1 64))))))

; find_symbols
(declare-fun S58T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S57 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncmp::n"))) (_ bv0 32)))) (S58T |ς|))))

; find_symbols
(declare-fun S58 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S57 |ς|) (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncmp::n"))) (_ bv0 32))) (S58 |ς|))))

; find_symbols
(declare-fun S59 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S59 (update-state-s8 |ς| (object-address "main::$tmp::return_value_cstrncmp") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S60 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60 (exit-scope-state-p64 |ς| (object-address "cstrncmp::1::uc2"))))))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61 (exit-scope-state-p64 |ς| (object-address "cstrncmp::1::uc1"))))))

; set_to true (equal)
(declare-fun S62 (state) Bool)
(assert (= S62 S61))

; set_to true (equal)
(declare-fun S63 (state) Bool)
(assert (= S63 S58T))

; find_symbols
(declare-fun S90 (state) Bool)
; find_symbols
(declare-fun S64in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S90 |ς|) (S64in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64in |ς|))))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64in |ς|) (S64 (enter-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_n") (_ bv1 64))))))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65 (update-state-s8 |ς| (object-address "cstrncmp::$tmp::tmp_post_n") (evaluate-s8 |ς| (object-address "cstrncmp::n")))))))

; find_symbols
(declare-fun S66 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S65 |ς|) (S66 (update-state-s8 |ς| (object-address "cstrncmp::n") (bvsub (evaluate-s8 |ς| (object-address "cstrncmp::n")) (_ bv1 8)))))))

; find_symbols
(declare-fun S67T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S66 |ς|) (not (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncmp::$tmp::tmp_post_n"))) (_ bv0 32)))) (S67T |ς|))))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S66 |ς|) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncmp::$tmp::tmp_post_n"))) (_ bv0 32))) (S67 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S68 (state) Bool)
(assert (= S68 S67))

; set_to true

; set_to true (equal)
(declare-fun S69 (state) Bool)
(assert (= S69 S68))

; find_symbols
(declare-fun S70T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S69 |ς|) (not (= (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s1"))) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s2")))))) (S70T |ς|))))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S69 |ς|) (= (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s1"))) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s2"))))) (S70 |ς|))))

; set_to true (equal)
(declare-fun S71 (state) Bool)
(assert (= S71 S70))

; set_to true (equal)
(declare-fun S72 (state) Bool)
(assert (= S72 S71))

; find_symbols
(declare-fun S73 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73 (enter-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_if_expr") (_ bv1 64))))))

; find_symbols
(declare-fun S74T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S73 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncmp::n"))) (_ bv0 32)))) (S74T |ς|))))

; find_symbols
(declare-fun S74 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S73 |ς|) (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrncmp::n"))) (_ bv0 32))) (S74 |ς|))))

; find_symbols
(declare-fun S75 (state) Bool)
(declare-fun update-state-b (state (_ BitVec 64) Bool) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S74 |ς|) (S75 (update-state-b |ς| (object-address "cstrncmp::$tmp::tmp_if_expr") true)))))

; set_to true (equal)
(declare-fun S76 (state) Bool)
(assert (= S76 S75))

; set_to true

; set_to true (equal)
(declare-fun S77 (state) Bool)
(assert (= S77 S74T))

; find_symbols
(declare-fun S78 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S77 |ς|) (S78 (update-state-b |ς| (object-address "cstrncmp::$tmp::tmp_if_expr") (ite (= ((_ sign_extend 24) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s1")))) (_ bv0 32)) true false))))))

; find_symbols
(declare-fun S79in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S79in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S78 |ς|) (S79in |ς|))))

; set_to true (equal)
(declare-fun S79 (state) Bool)
(assert (= S79 S79in))

(declare-fun evaluate-b (state (_ BitVec 64)) Bool)
; find_symbols
(declare-fun S80T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S79 |ς|) (not (evaluate-b |ς| (object-address "cstrncmp::$tmp::tmp_if_expr")))) (S80T |ς|))))

; find_symbols
(declare-fun S80 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S79 |ς|) (evaluate-b |ς| (object-address "cstrncmp::$tmp::tmp_if_expr"))) (S80 |ς|))))

; find_symbols
(declare-fun S81 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S80 |ς|) (S81 (update-state-s8 |ς| (object-address "main::$tmp::return_value_cstrncmp") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S82 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S81 |ς|) (S82 (exit-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_n"))))))

; find_symbols
(declare-fun S83 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S82 |ς|) (S83 (exit-scope-state-p64 |ς| (object-address "cstrncmp::1::uc2"))))))

; find_symbols
(declare-fun S84 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S83 |ς|) (S84 (exit-scope-state-p64 |ς| (object-address "cstrncmp::1::uc1"))))))

; set_to true (equal)
(declare-fun S85 (state) Bool)
(assert (= S85 S84))

; set_to true (equal)
(declare-fun S86 (state) Bool)
(assert (= S86 S80T))

; find_symbols
(declare-fun S87 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S86 |ς|) (S87 (update-state-p64 |ς| (object-address "cstrncmp::s1") (bvadd (evaluate-p64 |ς| (object-address "cstrncmp::s1")) (_ bv1 64)))))))

; find_symbols
(declare-fun S88 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S87 |ς|) (S88 (update-state-p64 |ς| (object-address "cstrncmp::s2") (bvadd (evaluate-p64 |ς| (object-address "cstrncmp::s2")) (_ bv1 64)))))))

; find_symbols
(declare-fun S89 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S88 |ς|) (S89 (exit-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_if_expr"))))))

; set_to true
(assert (= S90 S89))

; find_symbols
(declare-fun S91in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67T |ς|) (S91in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S70T |ς|) (S91in |ς|))))

; set_to true (equal)
(declare-fun S91 (state) Bool)
(assert (= S91 S91in))

; set_to true

; set_to true (equal)
(declare-fun S92 (state) Bool)
(assert (= S92 S91))

; find_symbols
(declare-fun S93 (state) Bool)
(declare-fun evaluate-u8 (state (_ BitVec 64)) (_ BitVec 8))
(declare-fun update-state-u8 (state (_ BitVec 64) (_ BitVec 8)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S92 |ς|) (S93 (update-state-u8 |ς| (object-address "cstrncmp::1::uc1") (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s1"))))))))

; set_to true

; set_to true (equal)
(declare-fun S94 (state) Bool)
(assert (= S94 S93))

; find_symbols
(declare-fun S95 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S94 |ς|) (S95 (update-state-u8 |ς| (object-address "cstrncmp::1::uc2") (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s2"))))))))

; find_symbols
(declare-fun S96 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S95 |ς|) (S96 (update-state-s8 |ς| (object-address "main::$tmp::return_value_cstrncmp") ((_ extract 7 0) (ite (bvslt ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "cstrncmp::1::uc1"))) ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "cstrncmp::1::uc2")))) (bvneg (_ bv1 32)) (ite (bvsgt ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "cstrncmp::1::uc1"))) ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "cstrncmp::1::uc2")))) (_ bv1 32) (_ bv0 32)))))))))

; find_symbols
(declare-fun S97 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S96 |ς|) (S97 (exit-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_n"))))))

; find_symbols
(declare-fun S98 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S97 |ς|) (S98 (exit-scope-state-p64 |ς| (object-address "cstrncmp::1::uc2"))))))

; find_symbols
(declare-fun S99 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S98 |ς|) (S99 (exit-scope-state-p64 |ς| (object-address "cstrncmp::1::uc1"))))))

; set_to true (equal)
(declare-fun S100 (state) Bool)
(assert (= S100 S99))

; find_symbols
(declare-fun S101in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S62 |ς|) (S101in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S85 |ς|) (S101in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S100 |ς|) (S101in |ς|))))

; set_to true (equal)
(declare-fun S101 (state) Bool)
(assert (= S101 S101in))

; find_symbols
(declare-fun S102 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S101 |ς|) (S102 (exit-scope-state-p64 |ς| (object-address "cstrncmp::s1"))))))

; find_symbols
(declare-fun S103 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S102 |ς|) (S103 (exit-scope-state-p64 |ς| (object-address "cstrncmp::s2"))))))

; find_symbols
(declare-fun S104 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S103 |ς|) (S104 (exit-scope-state-p64 |ς| (object-address "cstrncmp::n"))))))

; find_symbols
(declare-fun S105 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S104 |ς|) (S105 (update-state-s8 |ς| (object-address "return'") (evaluate-s8 |ς| (object-address "main::$tmp::return_value_cstrncmp")))))))

; find_symbols
(declare-fun S106 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S105 |ς|) (S106 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_cstrncmp"))))))

; find_symbols
(declare-fun S107 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S106 |ς|) (S107 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$1"))))))

; find_symbols
(declare-fun S108 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S107 |ς|) (S108 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString2"))))))

; find_symbols
(declare-fun S109 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S108 |ς|) (S109 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0"))))))

; find_symbols
(declare-fun S110 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S109 |ς|) (S110 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString1"))))))

; find_symbols
(declare-fun S111 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S110 |ς|) (S111 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S112 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S111 |ς|) (S112 (exit-scope-state-p64 |ς| (object-address "main::1::length2"))))))

; find_symbols
(declare-fun S113 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S112 |ς|) (S113 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0"))))))

; find_symbols
(declare-fun S114 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S113 |ς|) (S114 (exit-scope-state-p64 |ς| (object-address "main::1::length1"))))))

; find_symbols
(declare-fun S115 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S114 |ς|) (S115 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S116 (state) Bool)
(assert (= S116 S115))

; set_to true (equal)
(declare-fun S117 (state) Bool)
(assert (= S117 S116))

; set_to true (equal)
(declare-fun S118 (state) Bool)
(assert (= S118 S117))

; set_to true (equal)
(declare-fun S119 (state) Bool)
(assert (= S119 S118))

(check-sat)


; end of SMT2 file
