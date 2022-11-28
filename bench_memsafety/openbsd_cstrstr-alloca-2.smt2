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
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_cstrstr") (_ bv8 64))))))

; set_to true (equal)
(declare-fun S47 (state) Bool)
(assert (= S47 S46))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S48 (enter-scope-state-p64 |ς| (object-address "cstrstr::s") (_ bv8 64))))))

; find_symbols
(declare-fun S49 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S49 (update-state-p64 |ς| (object-address "cstrstr::s") (evaluate-p64 |ς| (object-address "main::1::nondetString1")))))))

; find_symbols
(declare-fun S50 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50 (enter-scope-state-p64 |ς| (object-address "cstrstr::find") (_ bv8 64))))))

; find_symbols
(declare-fun S51 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S50 |ς|) (S51 (update-state-p64 |ς| (object-address "cstrstr::find") (evaluate-p64 |ς| (object-address "main::1::nondetString2")))))))

; find_symbols
(declare-fun S52 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S52 (enter-scope-state-p64 |ς| (object-address "cstrstr::1::c") (_ bv1 64))))))

; find_symbols
(declare-fun S53 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (enter-scope-state-p64 |ς| (object-address "cstrstr::1::sc") (_ bv1 64))))))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (enter-scope-state-p64 |ς| (object-address "cstrstr::1::len") (_ bv8 64))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (enter-scope-state-p64 |ς| (object-address "cstrstr::$tmp::tmp_post_find") (_ bv8 64))))))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56 (update-state-p64 |ς| (object-address "cstrstr::$tmp::tmp_post_find") (evaluate-p64 |ς| (object-address "cstrstr::find")))))))

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (update-state-p64 |ς| (object-address "cstrstr::find") (bvadd (evaluate-p64 |ς| (object-address "cstrstr::find")) (_ bv1 64)))))))

; set_to true

; set_to true (equal)
(declare-fun S58 (state) Bool)
(assert (= S58 S57))

; find_symbols
(declare-fun S59 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S59 (update-state-s8 |ς| (object-address "cstrstr::1::c") (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrstr::$tmp::tmp_post_find"))))))))

; find_symbols
(declare-fun S60T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S59 |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrstr::1::c"))) (_ bv0 32))))) (S60T |ς|))))

; find_symbols
(declare-fun S60 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S59 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrstr::1::c"))) (_ bv0 32)))) (S60 |ς|))))

; set_to true (equal)
(declare-fun S61 (state) Bool)
(assert (= S61 S60))

; find_symbols
(declare-fun S62 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62 (enter-scope-state-p64 |ς| (object-address "cstrlen::str") (_ bv8 64))))))

; find_symbols
(declare-fun S63 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S62 |ς|) (S63 (update-state-p64 |ς| (object-address "cstrlen::str") (evaluate-p64 |ς| (object-address "cstrstr::find")))))))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (enter-scope-state-p64 |ς| (object-address "cstrlen::1::s") (_ bv8 64))))))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65 (update-state-p64 |ς| (object-address "cstrlen::1::s") (evaluate-p64 |ς| (object-address "cstrlen::str")))))))

; find_symbols
(declare-fun S70 (state) Bool)
; find_symbols
(declare-fun S66in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S66in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S65 |ς|) (S66in |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S66 (state) Bool)
(assert (= S66 S66in))

; find_symbols
(declare-fun S67T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S66 |ς|) (not (not (= (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrlen::1::s"))) (_ bv0 8))))) (S67T |ς|))))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S66 |ς|) (not (= (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrlen::1::s"))) (_ bv0 8)))) (S67 |ς|))))

; set_to true (equal)
(declare-fun S68 (state) Bool)
(assert (= S68 S67))

; find_symbols
(declare-fun S69 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69 (update-state-p64 |ς| (object-address "cstrlen::1::s") (bvadd (evaluate-p64 |ς| (object-address "cstrlen::1::s")) (_ bv1 64)))))))

; set_to true
(assert (= S70 S69))

; set_to true (equal)
(declare-fun S71 (state) Bool)
(assert (= S71 S67T))

; find_symbols
(declare-fun S72 (state) Bool)
(declare-fun update-state-u64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S71 |ς|) (S72 (update-state-u64 |ς| (object-address "cstrstr::1::len") (bvsub (evaluate-p64 |ς| (object-address "cstrlen::1::s")) (evaluate-p64 |ς| (object-address "cstrlen::str"))))))))

; find_symbols
(declare-fun S73 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73 (exit-scope-state-p64 |ς| (object-address "cstrlen::1::s"))))))

; set_to true (equal)
(declare-fun S74 (state) Bool)
(assert (= S74 S73))

; set_to true (equal)
(declare-fun S75 (state) Bool)
(assert (= S75 S74))

; find_symbols
(declare-fun S76 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S75 |ς|) (S76 (exit-scope-state-p64 |ς| (object-address "cstrlen::str"))))))

; find_symbols
(declare-fun S91T (state) Bool)
; find_symbols
(declare-fun S77in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S91T |ς|) (S77in |ς|))))

; find_symbols
(declare-fun S136T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S136T |ς|) (S77in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S77in |ς|))))

; find_symbols
(declare-fun S77 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S77in |ς|) (S77 (enter-scope-state-p64 |ς| (object-address "cstrstr::$tmp::tmp_post_s") (_ bv8 64))))))

; find_symbols
(declare-fun S78 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S77 |ς|) (S78 (update-state-p64 |ς| (object-address "cstrstr::$tmp::tmp_post_s") (evaluate-p64 |ς| (object-address "cstrstr::s")))))))

; find_symbols
(declare-fun S79 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S78 |ς|) (S79 (update-state-p64 |ς| (object-address "cstrstr::s") (bvadd (evaluate-p64 |ς| (object-address "cstrstr::s")) (_ bv1 64)))))))

; set_to true

; set_to true (equal)
(declare-fun S80 (state) Bool)
(assert (= S80 S79))

; find_symbols
(declare-fun S81 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S80 |ς|) (S81 (update-state-s8 |ς| (object-address "cstrstr::1::sc") (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrstr::$tmp::tmp_post_s"))))))))

; find_symbols
(declare-fun S82T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S81 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrstr::1::sc"))) (_ bv0 32)))) (S82T |ς|))))

; find_symbols
(declare-fun S82 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S81 |ς|) (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrstr::1::sc"))) (_ bv0 32))) (S82 |ς|))))

; find_symbols
(declare-fun S83 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S82 |ς|) (S83 (update-state-p64 |ς| (object-address "main::$tmp::return_value_cstrstr") ((_ sign_extend 32) (_ bv0 32)))))))

; find_symbols
(declare-fun S84 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S83 |ς|) (S84 (exit-scope-state-p64 |ς| (object-address "cstrstr::$tmp::return_value_cstrncmp"))))))

; find_symbols
(declare-fun S85 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S84 |ς|) (S85 (exit-scope-state-p64 |ς| (object-address "cstrstr::1::len"))))))

; find_symbols
(declare-fun S86 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S85 |ς|) (S86 (exit-scope-state-p64 |ς| (object-address "cstrstr::1::sc"))))))

; find_symbols
(declare-fun S87 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S86 |ς|) (S87 (exit-scope-state-p64 |ς| (object-address "cstrstr::1::c"))))))

; set_to true (equal)
(declare-fun S88 (state) Bool)
(assert (= S88 S87))

; set_to true (equal)
(declare-fun S89 (state) Bool)
(assert (= S89 S82T))

; find_symbols
(declare-fun S90 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S89 |ς|) (S90 (exit-scope-state-p64 |ς| (object-address "cstrstr::$tmp::tmp_post_s"))))))

; set_to true
(assert (forall ((|ς| state)) (=> (and (S90 |ς|) (not (= (evaluate-s8 |ς| (object-address "cstrstr::1::sc")) (evaluate-s8 |ς| (object-address "cstrstr::1::c"))))) (S91T |ς|))))

; find_symbols
(declare-fun S91 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S90 |ς|) (not (not (= (evaluate-s8 |ς| (object-address "cstrstr::1::sc")) (evaluate-s8 |ς| (object-address "cstrstr::1::c")))))) (S91 |ς|))))

; set_to true (equal)
(declare-fun S92 (state) Bool)
(assert (= S92 S91))

; find_symbols
(declare-fun S93 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S92 |ς|) (S93 (enter-scope-state-p64 |ς| (object-address "cstrstr::$tmp::return_value_cstrncmp") (_ bv1 64))))))

; set_to true (equal)
(declare-fun S94 (state) Bool)
(assert (= S94 S93))

; find_symbols
(declare-fun S95 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S94 |ς|) (S95 (enter-scope-state-p64 |ς| (object-address "cstrncmp::s1") (_ bv8 64))))))

; find_symbols
(declare-fun S96 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S95 |ς|) (S96 (update-state-p64 |ς| (object-address "cstrncmp::s1") (evaluate-p64 |ς| (object-address "cstrstr::s")))))))

; find_symbols
(declare-fun S97 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S96 |ς|) (S97 (enter-scope-state-p64 |ς| (object-address "cstrncmp::s2") (_ bv8 64))))))

; find_symbols
(declare-fun S98 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S97 |ς|) (S98 (update-state-p64 |ς| (object-address "cstrncmp::s2") (evaluate-p64 |ς| (object-address "cstrstr::find")))))))

; find_symbols
(declare-fun S99 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S98 |ς|) (S99 (enter-scope-state-p64 |ς| (object-address "cstrncmp::n") (_ bv8 64))))))

; find_symbols
(declare-fun S100 (state) Bool)
(declare-fun evaluate-u64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S99 |ς|) (S100 (update-state-u64 |ς| (object-address "cstrncmp::n") (evaluate-u64 |ς| (object-address "cstrstr::1::len")))))))

; find_symbols
(declare-fun S101T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S100 |ς|) (not (= (evaluate-u64 |ς| (object-address "cstrncmp::n")) ((_ sign_extend 32) (_ bv0 32))))) (S101T |ς|))))

; find_symbols
(declare-fun S101 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S100 |ς|) (= (evaluate-u64 |ς| (object-address "cstrncmp::n")) ((_ sign_extend 32) (_ bv0 32)))) (S101 |ς|))))

; find_symbols
(declare-fun S102 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S101 |ς|) (S102 (update-state-s8 |ς| (object-address "cstrstr::$tmp::return_value_cstrncmp") ((_ extract 7 0) (_ bv0 32)))))))

; set_to true (equal)
(declare-fun S103 (state) Bool)
(assert (= S103 S102))

; set_to true (equal)
(declare-fun S104 (state) Bool)
(assert (= S104 S101T))

; find_symbols
(declare-fun S128T (state) Bool)
; find_symbols
(declare-fun S105in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S128T |ς|) (S105in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S104 |ς|) (S105in |ς|))))

; find_symbols
(declare-fun S105 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S105in |ς|) (S105 (enter-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s2") (_ bv8 64))))))

; find_symbols
(declare-fun S106 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S105 |ς|) (S106 (update-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s2") (evaluate-p64 |ς| (object-address "cstrncmp::s2")))))))

; find_symbols
(declare-fun S107 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S106 |ς|) (S107 (update-state-p64 |ς| (object-address "cstrncmp::s2") (bvadd (evaluate-p64 |ς| (object-address "cstrncmp::s2")) (_ bv1 64)))))))

; set_to true

; set_to true (equal)
(declare-fun S108 (state) Bool)
(assert (= S108 S107))

; set_to true

; set_to true (equal)
(declare-fun S109 (state) Bool)
(assert (= S109 S108))

; find_symbols
(declare-fun S110T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S109 |ς|) (not (not (= (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s1"))) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s2"))))))) (S110T |ς|))))

; find_symbols
(declare-fun S110 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S109 |ς|) (not (= (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s1"))) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s2")))))) (S110 |ς|))))

; find_symbols
(declare-fun S111 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S110 |ς|) (S111 (update-state-p64 |ς| (object-address "cstrncmp::s2") (bvadd (evaluate-p64 |ς| (object-address "cstrncmp::s2")) (bvneg (_ bv1 64))))))))

; set_to true

; set_to true (equal)
(declare-fun S112 (state) Bool)
(assert (= S112 S111))

; set_to true

; set_to true (equal)
(declare-fun S113 (state) Bool)
(assert (= S113 S112))

; find_symbols
(declare-fun S114 (state) Bool)
(declare-fun evaluate-u8 (state (_ BitVec 64)) (_ BitVec 8))
; set_to true
(assert (forall ((|ς| state)) (=> (S113 |ς|) (S114 (update-state-s8 |ς| (object-address "cstrstr::$tmp::return_value_cstrncmp") ((_ extract 7 0) (bvsub ((_ zero_extend 24) (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s1")))) ((_ zero_extend 24) (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::s2")))))))))))

; set_to true (equal)
(declare-fun S115 (state) Bool)
(assert (= S115 S114))

; set_to true (equal)
(declare-fun S116 (state) Bool)
(assert (= S116 S110T))

; find_symbols
(declare-fun S117 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S116 |ς|) (S117 (enter-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s1") (_ bv8 64))))))

; find_symbols
(declare-fun S118 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S117 |ς|) (S118 (update-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s1") (evaluate-p64 |ς| (object-address "cstrncmp::s1")))))))

; find_symbols
(declare-fun S119 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S118 |ς|) (S119 (update-state-p64 |ς| (object-address "cstrncmp::s1") (bvadd (evaluate-p64 |ς| (object-address "cstrncmp::s1")) (_ bv1 64)))))))

; set_to true

; set_to true (equal)
(declare-fun S120 (state) Bool)
(assert (= S120 S119))

; find_symbols
(declare-fun S121T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S120 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s1")))) (_ bv0 32)))) (S121T |ς|))))

; find_symbols
(declare-fun S121 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S120 |ς|) (= ((_ sign_extend 24) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s1")))) (_ bv0 32))) (S121 |ς|))))

; find_symbols
(declare-fun S122 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S121 |ς|) (S122 (exit-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s2"))))))

; set_to true (equal)
(declare-fun S123 (state) Bool)
(assert (= S123 S122))

; set_to true (equal)
(declare-fun S124 (state) Bool)
(assert (= S124 S121T))

; find_symbols
(declare-fun S125 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S124 |ς|) (S125 (exit-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s1"))))))

; find_symbols
(declare-fun S126 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S125 |ς|) (S126 (exit-scope-state-p64 |ς| (object-address "cstrncmp::$tmp::tmp_post_s2"))))))

; find_symbols
(declare-fun S127 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S126 |ς|) (S127 (update-state-u64 |ς| (object-address "cstrncmp::n") (bvsub (evaluate-u64 |ς| (object-address "cstrncmp::n")) (_ bv1 64)))))))

; set_to true
(assert (forall ((|ς| state)) (=> (and (S127 |ς|) (not (= (evaluate-u64 |ς| (object-address "cstrncmp::n")) ((_ sign_extend 32) (_ bv0 32))))) (S128T |ς|))))

; find_symbols
(declare-fun S128 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S127 |ς|) (not (not (= (evaluate-u64 |ς| (object-address "cstrncmp::n")) ((_ sign_extend 32) (_ bv0 32)))))) (S128 |ς|))))

; find_symbols
(declare-fun S129in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S123 |ς|) (S129in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S128 |ς|) (S129in |ς|))))

; set_to true (equal)
(declare-fun S129 (state) Bool)
(assert (= S129 S129in))

; find_symbols
(declare-fun S130 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S129 |ς|) (S130 (update-state-s8 |ς| (object-address "cstrstr::$tmp::return_value_cstrncmp") ((_ extract 7 0) (_ bv0 32)))))))

; set_to true (equal)
(declare-fun S131 (state) Bool)
(assert (= S131 S130))

; find_symbols
(declare-fun S132in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S103 |ς|) (S132in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S115 |ς|) (S132in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S131 |ς|) (S132in |ς|))))

; set_to true (equal)
(declare-fun S132 (state) Bool)
(assert (= S132 S132in))

; find_symbols
(declare-fun S133 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S132 |ς|) (S133 (exit-scope-state-p64 |ς| (object-address "cstrncmp::s1"))))))

; find_symbols
(declare-fun S134 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S133 |ς|) (S134 (exit-scope-state-p64 |ς| (object-address "cstrncmp::s2"))))))

; find_symbols
(declare-fun S135 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S134 |ς|) (S135 (exit-scope-state-p64 |ς| (object-address "cstrncmp::n"))))))

; set_to true
(assert (forall ((|ς| state)) (=> (and (S135 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrstr::$tmp::return_value_cstrncmp"))) (_ bv0 32)))) (S136T |ς|))))

; find_symbols
(declare-fun S136 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S135 |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrstr::$tmp::return_value_cstrncmp"))) (_ bv0 32))))) (S136 |ς|))))

; set_to true (equal)
(declare-fun S137 (state) Bool)
(assert (= S137 S136))

; find_symbols
(declare-fun S138 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S137 |ς|) (S138 (update-state-p64 |ς| (object-address "cstrstr::s") (bvadd (evaluate-p64 |ς| (object-address "cstrstr::s")) (bvneg (_ bv1 64))))))))

; find_symbols
(declare-fun S139 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S138 |ς|) (S139 (exit-scope-state-p64 |ς| (object-address "cstrstr::$tmp::return_value_cstrncmp"))))))

; find_symbols
(declare-fun S140in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60T |ς|) (S140in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S139 |ς|) (S140in |ς|))))

; set_to true (equal)
(declare-fun S140 (state) Bool)
(assert (= S140 S140in))

; find_symbols
(declare-fun S141 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S140 |ς|) (S141 (update-state-p64 |ς| (object-address "main::$tmp::return_value_cstrstr") (evaluate-p64 |ς| (object-address "cstrstr::s")))))))

; find_symbols
(declare-fun S142 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S141 |ς|) (S142 (exit-scope-state-p64 |ς| (object-address "cstrstr::$tmp::tmp_post_find"))))))

; find_symbols
(declare-fun S143 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S142 |ς|) (S143 (exit-scope-state-p64 |ς| (object-address "cstrstr::1::len"))))))

; find_symbols
(declare-fun S144 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S143 |ς|) (S144 (exit-scope-state-p64 |ς| (object-address "cstrstr::1::sc"))))))

; find_symbols
(declare-fun S145 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S144 |ς|) (S145 (exit-scope-state-p64 |ς| (object-address "cstrstr::1::c"))))))

; set_to true (equal)
(declare-fun S146 (state) Bool)
(assert (= S146 S145))

; find_symbols
(declare-fun S147in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S88 |ς|) (S147in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S146 |ς|) (S147in |ς|))))

; set_to true (equal)
(declare-fun S147 (state) Bool)
(assert (= S147 S147in))

; find_symbols
(declare-fun S148 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S147 |ς|) (S148 (exit-scope-state-p64 |ς| (object-address "cstrstr::s"))))))

; find_symbols
(declare-fun S149 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S148 |ς|) (S149 (exit-scope-state-p64 |ς| (object-address "cstrstr::find"))))))

; find_symbols
(declare-fun S150 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S149 |ς|) (S150 (update-state-s8 |ς| (object-address "return'") ((_ extract 7 0) (evaluate-p64 |ς| (object-address "main::$tmp::return_value_cstrstr"))))))))

; find_symbols
(declare-fun S151 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S150 |ς|) (S151 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_cstrstr"))))))

; find_symbols
(declare-fun S152 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S151 |ς|) (S152 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString2"))))))

; find_symbols
(declare-fun S153 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S152 |ς|) (S153 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0"))))))

; find_symbols
(declare-fun S154 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S153 |ς|) (S154 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString1"))))))

; find_symbols
(declare-fun S155 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S154 |ς|) (S155 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S156 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S155 |ς|) (S156 (exit-scope-state-p64 |ς| (object-address "main::1::length2"))))))

; find_symbols
(declare-fun S157 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S156 |ς|) (S157 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0"))))))

; find_symbols
(declare-fun S158 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S157 |ς|) (S158 (exit-scope-state-p64 |ς| (object-address "main::1::length1"))))))

; find_symbols
(declare-fun S159 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S158 |ς|) (S159 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S160 (state) Bool)
(assert (= S160 S159))

; set_to true (equal)
(declare-fun S161 (state) Bool)
(assert (= S161 S160))

; set_to true (equal)
(declare-fun S162 (state) Bool)
(assert (= S162 S161))

; set_to true (equal)
(declare-fun S163 (state) Bool)
(assert (= S163 S162))

(check-sat)


; end of SMT2 file
