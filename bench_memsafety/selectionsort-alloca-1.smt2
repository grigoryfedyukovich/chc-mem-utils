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
(assert (forall ((|ς| state)) (=> (and (S23 |ς|) (not (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (_ bv1 32)) (bvuge ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (bvudiv ((_ sign_extend 32) (_ bv2147483647 32)) (_ bv1 64)))))) (S24T |ς|))))

; find_symbols
(declare-fun S24 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S23 |ς|) (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (_ bv1 32)) (bvuge ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::array_size"))) (bvudiv ((_ sign_extend 32) (_ bv2147483647 32)) (_ bv1 64))))) (S24 |ς|))))

; find_symbols
(declare-fun S25 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S24 |ς|) (S25 (update-state-s8 |ς| (object-address "main::1::array_size") ((_ extract 7 0) (_ bv1 32)))))))

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

; set_to true (equal)
(declare-fun S31 (state) Bool)
(assert (= S31 S30))

; find_symbols
(declare-fun S32 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (enter-scope-state-p64 |ς| (object-address "SelectionSort::a") (_ bv8 64))))))

; find_symbols
(declare-fun S33 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33 (update-state-p64 |ς| (object-address "SelectionSort::a") (evaluate-p64 |ς| (object-address "main::1::numbers")))))))

; find_symbols
(declare-fun S34 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S33 |ς|) (S34 (enter-scope-state-p64 |ς| (object-address "SelectionSort::array_size") (_ bv1 64))))))

; find_symbols
(declare-fun S35 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S34 |ς|) (S35 (update-state-s8 |ς| (object-address "SelectionSort::array_size") (evaluate-s8 |ς| (object-address "main::1::array_size")))))))

; find_symbols
(declare-fun S36 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (enter-scope-state-p64 |ς| (object-address "SelectionSort::1::i") (_ bv1 64))))))

; find_symbols
(declare-fun S37 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (update-state-s8 |ς| (object-address "SelectionSort::1::i") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S64 (state) Bool)
; find_symbols
(declare-fun S38in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S38in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38in |ς|))))

; find_symbols
(declare-fun S38T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S38in |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "SelectionSort::1::i"))) (bvsub ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "SelectionSort::array_size"))) (_ bv1 32))))) (S38T |ς|))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S38in |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "SelectionSort::1::i"))) (bvsub ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "SelectionSort::array_size"))) (_ bv1 32)))) (S38 |ς|))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (enter-scope-state-p64 |ς| (object-address "SelectionSort::1::1::1::j") (_ bv1 64))))))

; find_symbols
(declare-fun S40 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40 (enter-scope-state-p64 |ς| (object-address "SelectionSort::1::1::1::min") (_ bv1 64))))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (enter-scope-state-p64 |ς| (object-address "SelectionSort::1::1::1::temp") (_ bv1 64))))))

; find_symbols
(declare-fun S42 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S41 |ς|) (S42 (update-state-s8 |ς| (object-address "SelectionSort::1::1::1::min") (evaluate-s8 |ς| (object-address "SelectionSort::1::i")))))))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (update-state-s8 |ς| (object-address "SelectionSort::1::1::1::j") ((_ extract 7 0) (bvadd ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "SelectionSort::1::i"))) (_ bv1 32))))))))

; find_symbols
(declare-fun S51 (state) Bool)
; find_symbols
(declare-fun S44in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S44in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S43 |ς|) (S44in |ς|))))

; find_symbols
(declare-fun S44T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S44in |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::j"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "SelectionSort::array_size")))))) (S44T |ς|))))

; find_symbols
(declare-fun S44 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S44in |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::j"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "SelectionSort::array_size"))))) (S44 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S45 (state) Bool)
(assert (= S45 S44))

; set_to true

; set_to true (equal)
(declare-fun S46 (state) Bool)
(assert (= S46 S45))

; find_symbols
(declare-fun S47T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S46 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "SelectionSort::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::j")))))) ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "SelectionSort::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::min"))))))))) (S47T |ς|))))

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S46 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "SelectionSort::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::j")))))) ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "SelectionSort::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::min")))))))) (S47 |ς|))))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S48 (update-state-s8 |ς| (object-address "SelectionSort::1::1::1::min") (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::j")))))))

; find_symbols
(declare-fun S49in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47T |ς|) (S49in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S49in |ς|))))

; set_to true (equal)
(declare-fun S49 (state) Bool)
(assert (= S49 S49in))

; find_symbols
(declare-fun S50 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50 (update-state-s8 |ς| (object-address "SelectionSort::1::1::1::j") (bvadd (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::j")) (_ bv1 8)))))))

; set_to true
(assert (= S51 S50))

; set_to true (equal)
(declare-fun S52 (state) Bool)
(assert (= S52 S44T))

; set_to true

; set_to true (equal)
(declare-fun S53 (state) Bool)
(assert (= S53 S52))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (update-state-s8 |ς| (object-address "SelectionSort::1::1::1::temp") (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "SelectionSort::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "SelectionSort::1::i"))))))))))

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
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "SelectionSort::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "SelectionSort::1::i")))) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "SelectionSort::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::min"))))))))))

; set_to true

; set_to true (equal)
(declare-fun S58 (state) Bool)
(assert (= S58 S57))

; find_symbols
(declare-fun S59 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S59 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "SelectionSort::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::min")))) (evaluate-s8 |ς| (object-address "SelectionSort::1::1::1::temp")))))))

; find_symbols
(declare-fun S60 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60 (exit-scope-state-p64 |ς| (object-address "SelectionSort::1::1::1::temp"))))))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61 (exit-scope-state-p64 |ς| (object-address "SelectionSort::1::1::1::min"))))))

; find_symbols
(declare-fun S62 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62 (exit-scope-state-p64 |ς| (object-address "SelectionSort::1::1::1::j"))))))

; find_symbols
(declare-fun S63 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S62 |ς|) (S63 (update-state-s8 |ς| (object-address "SelectionSort::1::i") (bvadd (evaluate-s8 |ς| (object-address "SelectionSort::1::i")) (_ bv1 8)))))))

; set_to true
(assert (= S64 S63))

; set_to true (equal)
(declare-fun S65 (state) Bool)
(assert (= S65 S38T))

; find_symbols
(declare-fun S66 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S65 |ς|) (S66 (exit-scope-state-p64 |ς| (object-address "SelectionSort::1::i"))))))

; set_to true (equal)
(declare-fun S67 (state) Bool)
(assert (= S67 S66))

; find_symbols
(declare-fun S68 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67 |ς|) (S68 (exit-scope-state-p64 |ς| (object-address "SelectionSort::a"))))))

; find_symbols
(declare-fun S69 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69 (exit-scope-state-p64 |ς| (object-address "SelectionSort::array_size"))))))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S70 (update-state-s8 |ς| (object-address "return'") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S71 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S71 (exit-scope-state-p64 |ς| (object-address "main::1::numbers"))))))

; find_symbols
(declare-fun S72 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S71 |ς|) (S72 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S73 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73 (exit-scope-state-p64 |ς| (object-address "main::1::array_size"))))))

; find_symbols
(declare-fun S74 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S73 |ς|) (S74 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S75 (state) Bool)
(assert (= S75 S74))

; set_to true (equal)
(declare-fun S76 (state) Bool)
(assert (= S76 S75))

; set_to true (equal)
(declare-fun S77 (state) Bool)
(assert (= S77 S76))

; set_to true (equal)
(declare-fun S78 (state) Bool)
(assert (= S78 S77))

(check-sat)


; end of SMT2 file
