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
(assert (forall ((|ς| state)) (=> (S19 |ς|) (S20 (enter-scope-state-p64 |ς| (object-address "main::1::i") (_ bv1 64))))))

; find_symbols
(declare-fun S21 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S20 |ς|) (S21 (enter-scope-state-p64 |ς| (object-address "main::1::j") (_ bv1 64))))))

; find_symbols
(declare-fun S22 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S21 |ς|) (S22 (enter-scope-state-p64 |ς| (object-address "main::1::val") (_ bv1 64))))))

; find_symbols
(declare-fun S23 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S22 |ς|) (S23 (enter-scope-state-p64 |ς| (object-address "main::1::length") (_ bv1 64))))))

; find_symbols
(declare-fun S24 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S23 |ς|) (S24 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char") (_ bv1 64))))))

; find_symbols
(declare-fun S25 (state) Bool)
(declare-fun update-state-s8 (state (_ BitVec 64) (_ BitVec 8)) state)
; set_to true
(assert (forall ((|ς| state) (|nondet::S25-0| (_ BitVec 8))) (=> (S24 |ς|) (S25 (update-state-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char") |nondet::S25-0|)))))

; find_symbols
(declare-fun S26 (state) Bool)
(declare-fun evaluate-s8 (state (_ BitVec 64)) (_ BitVec 8))
; set_to true
(assert (forall ((|ς| state)) (=> (S25 |ς|) (S26 (update-state-s8 |ς| (object-address "main::1::length") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char")))))))

; find_symbols
(declare-fun S27T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S26 |ς|) (not (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32)) (bvuge ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length"))) (bvudiv ((_ sign_extend 32) (_ bv2147483647 32)) (_ bv1 64)))))) (S27T |ς|))))

; find_symbols
(declare-fun S27 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S26 |ς|) (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32)) (bvuge ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length"))) (bvudiv ((_ sign_extend 32) (_ bv2147483647 32)) (_ bv1 64))))) (S27 |ς|))))

; find_symbols
(declare-fun S28 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S27 |ς|) (S28 (update-state-s8 |ς| (object-address "main::1::length") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S29in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S27T |ς|) (S29in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S28 |ς|) (S29in |ς|))))

; set_to true (equal)
(declare-fun S29 (state) Bool)
(assert (= S29 S29in))

; find_symbols
(declare-fun S30 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S29 |ς|) (S30 (enter-scope-state-p64 |ς| (object-address "main::1::arr") (_ bv8 64))))))

; find_symbols
(declare-fun S31 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S30 |ς|) (S31 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S32 (state) Bool)
(declare-fun allocate (state (_ BitVec 64)) (_ BitVec 64))
(declare-fun update-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S33 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33 (update-state-p64 |ς| (object-address "main::1::arr") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; find_symbols
(declare-fun S34T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S33 |ς|) (not (= (evaluate-p64 |ς| (object-address "main::1::arr")) (_ bv0 64)))) (S34T |ς|))))

; find_symbols
(declare-fun S34 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S33 |ς|) (not (not (= (evaluate-p64 |ς| (object-address "main::1::arr")) (_ bv0 64))))) (S34 |ς|))))

; find_symbols
(declare-fun S35 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S34 |ς|) (S35 (update-state-s8 |ς| (object-address "return'") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S36 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (exit-scope-state-p64 |ς| (object-address "main::1::arr"))))))

; find_symbols
(declare-fun S37 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (exit-scope-state-p64 |ς| (object-address "main::1::length"))))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; find_symbols
(declare-fun S40 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40 (exit-scope-state-p64 |ς| (object-address "main::1::val"))))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (exit-scope-state-p64 |ς| (object-address "main::1::j"))))))

; find_symbols
(declare-fun S42 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S41 |ς|) (S42 (exit-scope-state-p64 |ς| (object-address "main::1::i"))))))

; set_to true (equal)
(declare-fun S43 (state) Bool)
(assert (= S43 S42))

; set_to true (equal)
(declare-fun S44 (state) Bool)
(assert (= S44 S34T))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45 (update-state-s8 |ς| (object-address "main::1::i") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S54 (state) Bool)
; find_symbols
(declare-fun S46in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S46in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46in |ς|))))

; find_symbols
(declare-fun S46T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S46in |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length")))))) (S46T |ς|))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S46in |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))))) (S46 |ς|))))

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S47-0| (_ BitVec 8))) (=> (S46 |ς|) (S47 (update-state-s8 |ς| (object-address "main::1::val") |nondet::S47-0|)))))

; find_symbols
(declare-fun S48T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S47 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::val"))) (_ bv0 32)))) (S48T |ς|))))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S47 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::val"))) (_ bv0 32))) (S48 |ς|))))

; find_symbols
(declare-fun S49 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S49 (update-state-s8 |ς| (object-address "main::1::val") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S50in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48T |ς|) (S50in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50in |ς|))))

; set_to true (equal)
(declare-fun S50 (state) Bool)
(assert (= S50 S50in))

; set_to true

; set_to true (equal)
(declare-fun S51 (state) Bool)
(assert (= S51 S50))

; find_symbols
(declare-fun S52 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S52 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::arr")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::i")))) (evaluate-s8 |ς| (object-address "main::1::val")))))))

; find_symbols
(declare-fun S53 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (update-state-s8 |ς| (object-address "main::1::i") (bvadd (evaluate-s8 |ς| (object-address "main::1::i")) (_ bv1 8)))))))

; set_to true
(assert (= S54 S53))

; set_to true (equal)
(declare-fun S55 (state) Bool)
(assert (= S55 S46T))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56 (update-state-s8 |ς| (object-address "main::1::j") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S66 (state) Bool)
; find_symbols
(declare-fun S57in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S66 |ς|) (S57in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57in |ς|))))

; find_symbols
(declare-fun S57T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S57in |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::j"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length")))))) (S57T |ς|))))

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S57in |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::j"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))))) (S57 |ς|))))

; find_symbols
(declare-fun S63 (state) Bool)
; find_symbols
(declare-fun S58in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S58in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S57 |ς|) (S58in |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S58 (state) Bool)
(assert (= S58 S58in))

; find_symbols
(declare-fun S59T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S58 |ς|) (not (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::arr")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::j")))))) (_ bv0 32)))) (S59T |ς|))))

; find_symbols
(declare-fun S59 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S58 |ς|) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::arr")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::j")))))) (_ bv0 32))) (S59 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S60 (state) Bool)
(assert (= S60 S59))

; set_to true

; set_to true (equal)
(declare-fun S61 (state) Bool)
(assert (= S61 S60))

; find_symbols
(declare-fun S62 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::arr")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::j")))) (bvsub (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::arr")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::j"))))) (_ bv1 8)))))))

; set_to true
(assert (= S63 S62))

; set_to true (equal)
(declare-fun S64 (state) Bool)
(assert (= S64 S59T))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65 (update-state-s8 |ς| (object-address "main::1::j") (bvadd (evaluate-s8 |ς| (object-address "main::1::j")) (_ bv1 8)))))))

; set_to true
(assert (= S66 S65))

; set_to true (equal)
(declare-fun S67 (state) Bool)
(assert (= S67 S57T))

; find_symbols
(declare-fun S68 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67 |ς|) (S68 (update-state-s8 |ς| (object-address "return'") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S69 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69 (exit-scope-state-p64 |ς| (object-address "main::1::arr"))))))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S70 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S71 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S71 (exit-scope-state-p64 |ς| (object-address "main::1::length"))))))

; find_symbols
(declare-fun S72 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S71 |ς|) (S72 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; find_symbols
(declare-fun S73 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73 (exit-scope-state-p64 |ς| (object-address "main::1::val"))))))

; find_symbols
(declare-fun S74 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S73 |ς|) (S74 (exit-scope-state-p64 |ς| (object-address "main::1::j"))))))

; find_symbols
(declare-fun S75 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S74 |ς|) (S75 (exit-scope-state-p64 |ς| (object-address "main::1::i"))))))

; set_to true (equal)
(declare-fun S76 (state) Bool)
(assert (= S76 S75))

; find_symbols
(declare-fun S77in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S43 |ς|) (S77in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S77in |ς|))))

; set_to true (equal)
(declare-fun S77 (state) Bool)
(assert (= S77 S77in))

; set_to true (equal)
(declare-fun S78 (state) Bool)
(assert (= S78 S77))

; set_to true (equal)
(declare-fun S79 (state) Bool)
(assert (= S79 S78))

(check-sat)


; end of SMT2 file
