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
(assert (forall ((|ς| state)) (=> (S19 |ς|) (S20 (enter-scope-state-p64 |ς| (object-address "main::1::array_size") (_ bv1 64))))))

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
(assert (forall ((|ς| state)) (=> (S22 |ς|) (S23 (update-state-s8 |ς| (object-address "main::1::array_size") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char")))))))

; find_symbols
(declare-fun S24T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S23 |ς|) (not (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (_ bv2 32)) (bvuge ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (bvudiv ((_ sign_extend 32) (_ bv2147483647 32)) (_ bv1 64)))))) (S24T |ς|))))

; find_symbols
(declare-fun S24 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S23 |ς|) (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (_ bv2 32)) (bvuge ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (bvudiv ((_ sign_extend 32) (_ bv2147483647 32)) (_ bv1 64))))) (S24 |ς|))))

; find_symbols
(declare-fun S25 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S24 |ς|) (S25 (update-state-s8 |ς| (object-address "main::1::array_size") ((_ extract 7 0) (_ bv2 32)))))))

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
(assert (forall ((|ς| state)) (=> (S26 |ς|) (S27 (enter-scope-state-p64 |ς| (object-address "main::1::numbers") (_ bv8 64))))))

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
(assert (forall ((|ς| state)) (=> (S28 |ς|) (S29 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S30 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S29 |ς|) (S30 (update-state-p64 |ς| (object-address "main::1::numbers") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; find_symbols
(declare-fun S31 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S30 |ς|) (S31 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_sumOfThirdBytes") (_ bv1 64))))))

; set_to true (equal)
(declare-fun S32 (state) Bool)
(assert (= S32 S31))

; find_symbols
(declare-fun S33 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33 (enter-scope-state-p64 |ς| (object-address "sumOfThirdBytes::numbers") (_ bv8 64))))))

; find_symbols
(declare-fun S34 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S33 |ς|) (S34 (update-state-p64 |ς| (object-address "sumOfThirdBytes::numbers") (evaluate-p64 |ς| (object-address "main::1::numbers")))))))

; find_symbols
(declare-fun S35 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S34 |ς|) (S35 (enter-scope-state-p64 |ς| (object-address "sumOfThirdBytes::array_size") (_ bv1 64))))))

; find_symbols
(declare-fun S36 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (update-state-s8 |ς| (object-address "sumOfThirdBytes::array_size") ((_ extract 7 0) (bvsub ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (_ bv2 32))))))))

; find_symbols
(declare-fun S37 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (enter-scope-state-p64 |ς| (object-address "sumOfThirdBytes::1::i") (_ bv1 64))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (enter-scope-state-p64 |ς| (object-address "sumOfThirdBytes::1::sum") (_ bv1 64))))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (enter-scope-state-p64 |ς| (object-address "sumOfThirdBytes::1::p") (_ bv8 64))))))

; find_symbols
(declare-fun S40 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40 (update-state-s8 |ς| (object-address "sumOfThirdBytes::1::sum") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (update-state-s8 |ς| (object-address "sumOfThirdBytes::1::i") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S62 (state) Bool)
; find_symbols
(declare-fun S42in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S62 |ς|) (S42in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S41 |ς|) (S42in |ς|))))

; find_symbols
(declare-fun S42T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S42in |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "sumOfThirdBytes::array_size")))))) (S42T |ς|))))

; find_symbols
(declare-fun S42 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S42in |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "sumOfThirdBytes::array_size"))))) (S42 |ς|))))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (enter-scope-state-p64 |ς| (object-address "sumOfThirdBytes::1::1::1::j") (_ bv1 64))))))

; find_symbols
(declare-fun S44 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S43 |ς|) (S44 (update-state-s8 |ς| (object-address "sumOfThirdBytes::1::1::1::j") ((_ extract 7 0) (bvadd ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::i"))) (_ bv2 32))))))))

; find_symbols
(declare-fun S48 (state) Bool)
; find_symbols
(declare-fun S45in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S45in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45in |ς|))))

; find_symbols
(declare-fun S45T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S45in |ς|) (not (bvsge ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::1::1::j"))) (_ bv0 32)))) (S45T |ς|))))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S45in |ς|) (bvsge ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::1::1::j"))) (_ bv0 32))) (S45 |ς|))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (update-state-s8 |ς| (object-address "sumOfThirdBytes::1::sum") (bvadd (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::sum")) (_ bv1 8)))))))

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S46 |ς|) (S47 (update-state-s8 |ς| (object-address "sumOfThirdBytes::1::1::1::j") (bvsub (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::1::1::j")) (_ bv1 8)))))))

; set_to true
(assert (= S48 S47))

; set_to true (equal)
(declare-fun S49 (state) Bool)
(assert (= S49 S45T))

; find_symbols
(declare-fun S50 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50 (update-state-p64 |ς| (object-address "sumOfThirdBytes::1::p") (bvadd (evaluate-p64 |ς| (object-address "sumOfThirdBytes::numbers")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::i")))))))))

; find_symbols
(declare-fun S51 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S50 |ς|) (S51 (update-state-p64 |ς| (object-address "sumOfThirdBytes::1::p") (bvadd (evaluate-p64 |ς| (object-address "sumOfThirdBytes::1::p")) ((_ sign_extend 32) (_ bv2 32))))))))

; find_symbols
(declare-fun S58 (state) Bool)
; find_symbols
(declare-fun S52in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S52in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S52in |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S52 (state) Bool)
(assert (= S52 S52in))

; find_symbols
(declare-fun S53T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S52 |ς|) (not (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "sumOfThirdBytes::1::p")))) (_ bv0 32)))) (S53T |ς|))))

; find_symbols
(declare-fun S53 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S52 |ς|) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "sumOfThirdBytes::1::p")))) (_ bv0 32))) (S53 |ς|))))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (update-state-s8 |ς| (object-address "sumOfThirdBytes::1::sum") (bvadd (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::sum")) (_ bv1 8)))))))

; set_to true

; set_to true (equal)
(declare-fun S55 (state) Bool)
(assert (= S55 S54))

; set_to true

; set_to true (equal)
(declare-fun S56 (state) Bool)
(assert (= S56 S55))

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (update-state-s8 |ς| (evaluate-p64 |ς| (object-address "sumOfThirdBytes::1::p")) (bvsub (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "sumOfThirdBytes::1::p"))) (_ bv1 8)))))))

; set_to true
(assert (= S58 S57))

; set_to true (equal)
(declare-fun S59 (state) Bool)
(assert (= S59 S53T))

; find_symbols
(declare-fun S60 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60 (exit-scope-state-p64 |ς| (object-address "sumOfThirdBytes::1::1::1::j"))))))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61 (update-state-s8 |ς| (object-address "sumOfThirdBytes::1::i") (bvadd (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::i")) (_ bv1 8)))))))

; set_to true
(assert (= S62 S61))

; set_to true (equal)
(declare-fun S63 (state) Bool)
(assert (= S63 S42T))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (update-state-s8 |ς| (object-address "main::$tmp::return_value_sumOfThirdBytes") (evaluate-s8 |ς| (object-address "sumOfThirdBytes::1::sum")))))))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65 (exit-scope-state-p64 |ς| (object-address "sumOfThirdBytes::1::p"))))))

; find_symbols
(declare-fun S66 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S65 |ς|) (S66 (exit-scope-state-p64 |ς| (object-address "sumOfThirdBytes::1::sum"))))))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S66 |ς|) (S67 (exit-scope-state-p64 |ς| (object-address "sumOfThirdBytes::1::i"))))))

; set_to true (equal)
(declare-fun S68 (state) Bool)
(assert (= S68 S67))

; set_to true (equal)
(declare-fun S69 (state) Bool)
(assert (= S69 S68))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S70 (exit-scope-state-p64 |ς| (object-address "sumOfThirdBytes::numbers"))))))

; find_symbols
(declare-fun S71 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S71 (exit-scope-state-p64 |ς| (object-address "sumOfThirdBytes::array_size"))))))

; find_symbols
(declare-fun S72 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S71 |ς|) (S72 (update-state-s8 |ς| (object-address "return'") (evaluate-s8 |ς| (object-address "main::$tmp::return_value_sumOfThirdBytes")))))))

; find_symbols
(declare-fun S73 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_sumOfThirdBytes"))))))

; find_symbols
(declare-fun S74 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S73 |ς|) (S74 (exit-scope-state-p64 |ς| (object-address "main::1::numbers"))))))

; find_symbols
(declare-fun S75 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S74 |ς|) (S75 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S76 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S75 |ς|) (S76 (exit-scope-state-p64 |ς| (object-address "main::1::array_size"))))))

; find_symbols
(declare-fun S77 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S77 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S78 (state) Bool)
(assert (= S78 S77))

; set_to true (equal)
(declare-fun S79 (state) Bool)
(assert (= S79 S78))

; set_to true (equal)
(declare-fun S80 (state) Bool)
(assert (= S80 S79))

; set_to true (equal)
(declare-fun S81 (state) Bool)
(assert (= S81 S80))

(check-sat)


; end of SMT2 file
