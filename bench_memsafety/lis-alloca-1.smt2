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

; find_symbols
(declare-fun S31 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S30 |ς|) (S31 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_lis") (_ bv1 64))))))

; set_to true (equal)
(declare-fun S32 (state) Bool)
(assert (= S32 S31))

; find_symbols
(declare-fun S33 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33 (enter-scope-state-p64 |ς| (object-address "lis::a") (_ bv8 64))))))

; find_symbols
(declare-fun S34 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S33 |ς|) (S34 (update-state-p64 |ς| (object-address "lis::a") (evaluate-p64 |ς| (object-address "main::1::numbers")))))))

; find_symbols
(declare-fun S35 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S34 |ς|) (S35 (enter-scope-state-p64 |ς| (object-address "lis::N") (_ bv1 64))))))

; find_symbols
(declare-fun S36 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (update-state-s8 |ς| (object-address "lis::N") (evaluate-s8 |ς| (object-address "main::1::array_size")))))))

; find_symbols
(declare-fun S37 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (enter-scope-state-p64 |ς| (object-address "lis::1::best") (_ bv8 64))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (enter-scope-state-p64 |ς| (object-address "lis::1::prev") (_ bv8 64))))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (enter-scope-state-p64 |ς| (object-address "lis::1::i") (_ bv1 64))))))

; find_symbols
(declare-fun S40 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40 (enter-scope-state-p64 |ς| (object-address "lis::1::j") (_ bv1 64))))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (enter-scope-state-p64 |ς| (object-address "lis::1::max") (_ bv1 64))))))

; find_symbols
(declare-fun S42 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S41 |ς|) (S42 (update-state-s8 |ς| (object-address "lis::1::max") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (enter-scope-state-p64 |ς| (object-address "lis::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S44 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S43 |ς|) (S44 (update-state-p64 |ς| (object-address "lis::$tmp::return_value_malloc") (allocate |ς| (bvmul (_ bv1 64) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::N"))))))))))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45 (update-state-p64 |ς| (object-address "lis::1::best") (evaluate-p64 |ς| (object-address "lis::$tmp::return_value_malloc")))))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (enter-scope-state-p64 |ς| (object-address "lis::$tmp::return_value_malloc$0") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S46 |ς|) (S47 (update-state-p64 |ς| (object-address "lis::$tmp::return_value_malloc$0") (allocate |ς| (bvmul (_ bv1 64) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::N"))))))))))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S48 (update-state-p64 |ς| (object-address "lis::1::prev") (evaluate-p64 |ς| (object-address "lis::$tmp::return_value_malloc$0")))))))

; find_symbols
(declare-fun S49 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S49 (update-state-s8 |ς| (object-address "lis::1::i") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S56 (state) Bool)
; find_symbols
(declare-fun S50in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S50in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50in |ς|))))

; find_symbols
(declare-fun S50T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S50in |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::N")))))) (S50T |ς|))))

; find_symbols
(declare-fun S50 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S50in |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::N"))))) (S50 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S51 (state) Bool)
(assert (= S51 S50))

; find_symbols
(declare-fun S52 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S52 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i")))) ((_ extract 7 0) (_ bv1 32)))))))

; set_to true

; set_to true (equal)
(declare-fun S53 (state) Bool)
(assert (= S53 S52))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::prev")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i")))) (evaluate-s8 |ς| (object-address "lis::1::i")))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (update-state-s8 |ς| (object-address "lis::1::i") (bvadd (evaluate-s8 |ς| (object-address "lis::1::i")) (_ bv1 8)))))))

; set_to true
(assert (= S56 S55))

; set_to true (equal)
(declare-fun S57 (state) Bool)
(assert (= S57 S50T))

; find_symbols
(declare-fun S58 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S57 |ς|) (S58 (update-state-s8 |ς| (object-address "lis::1::i") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S79 (state) Bool)
; find_symbols
(declare-fun S59in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S79 |ς|) (S59in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S59in |ς|))))

; find_symbols
(declare-fun S59T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S59in |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::N")))))) (S59T |ς|))))

; find_symbols
(declare-fun S59 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S59in |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::N"))))) (S59 |ς|))))

; find_symbols
(declare-fun S60 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60 (update-state-s8 |ς| (object-address "lis::1::j") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S76 (state) Bool)
; find_symbols
(declare-fun S61in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S61in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61in |ς|))))

; find_symbols
(declare-fun S61T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S61in |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::j"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::i")))))) (S61T |ς|))))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S61in |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::j"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::i"))))) (S61 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S62 (state) Bool)
(assert (= S62 S61))

; set_to true

; set_to true (equal)
(declare-fun S63 (state) Bool)
(assert (= S63 S62))

; find_symbols
(declare-fun S64T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S63 |ς|) (not (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i")))))) ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::j"))))))))) (S64T |ς|))))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S63 |ς|) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i")))))) ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::a")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::j")))))))) (S64 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S65 (state) Bool)
(assert (= S65 S64))

; set_to true

; set_to true (equal)
(declare-fun S66 (state) Bool)
(assert (= S66 S65))

; find_symbols
(declare-fun S67T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S66 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i")))))) (bvadd ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::j")))))) (_ bv1 32))))) (S67T |ς|))))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S66 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i")))))) (bvadd ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::j")))))) (_ bv1 32)))) (S67 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S68 (state) Bool)
(assert (= S68 S67))

; set_to true

; set_to true (equal)
(declare-fun S69 (state) Bool)
(assert (= S69 S68))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S70 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i")))) ((_ extract 7 0) (bvadd ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::j")))))) (_ bv1 32))))))))

; set_to true

; set_to true (equal)
(declare-fun S71 (state) Bool)
(assert (= S71 S70))

; find_symbols
(declare-fun S72 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S71 |ς|) (S72 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::prev")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i")))) (evaluate-s8 |ς| (object-address "lis::1::j")))))))

; find_symbols
(declare-fun S73in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67T |ς|) (S73in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73in |ς|))))

; set_to true (equal)
(declare-fun S73 (state) Bool)
(assert (= S73 S73in))

; find_symbols
(declare-fun S74in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64T |ς|) (S74in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S73 |ς|) (S74in |ς|))))

; set_to true (equal)
(declare-fun S74 (state) Bool)
(assert (= S74 S74in))

; find_symbols
(declare-fun S75 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S74 |ς|) (S75 (update-state-s8 |ς| (object-address "lis::1::j") (bvadd (evaluate-s8 |ς| (object-address "lis::1::j")) (_ bv1 8)))))))

; set_to true
(assert (= S76 S75))

; set_to true (equal)
(declare-fun S77 (state) Bool)
(assert (= S77 S61T))

; find_symbols
(declare-fun S78 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S77 |ς|) (S78 (update-state-s8 |ς| (object-address "lis::1::i") (bvadd (evaluate-s8 |ς| (object-address "lis::1::i")) (_ bv1 8)))))))

; set_to true
(assert (= S79 S78))

; set_to true (equal)
(declare-fun S80 (state) Bool)
(assert (= S80 S59T))

; find_symbols
(declare-fun S81 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S80 |ς|) (S81 (update-state-s8 |ς| (object-address "lis::1::i") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S89 (state) Bool)
; find_symbols
(declare-fun S82in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S89 |ς|) (S82in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S81 |ς|) (S82in |ς|))))

; find_symbols
(declare-fun S82T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S82in |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::N")))))) (S82T |ς|))))

; find_symbols
(declare-fun S82 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S82in |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::i"))) ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::N"))))) (S82 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S83 (state) Bool)
(assert (= S83 S82))

; find_symbols
(declare-fun S84T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S83 |ς|) (not (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::max"))) ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i"))))))))) (S84T |ς|))))

; find_symbols
(declare-fun S84 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S83 |ς|) (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "lis::1::max"))) ((_ sign_extend 24) (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i")))))))) (S84 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S85 (state) Bool)
(assert (= S85 S84))

; find_symbols
(declare-fun S86 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S85 |ς|) (S86 (update-state-s8 |ς| (object-address "lis::1::max") (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "lis::1::best")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "lis::1::i"))))))))))

; find_symbols
(declare-fun S87in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S84T |ς|) (S87in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S86 |ς|) (S87in |ς|))))

; set_to true (equal)
(declare-fun S87 (state) Bool)
(assert (= S87 S87in))

; find_symbols
(declare-fun S88 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S87 |ς|) (S88 (update-state-s8 |ς| (object-address "lis::1::i") (bvadd (evaluate-s8 |ς| (object-address "lis::1::i")) (_ bv1 8)))))))

; set_to true
(assert (= S89 S88))

; set_to true (equal)
(declare-fun S90 (state) Bool)
(assert (= S90 S82T))

; find_symbols
(declare-fun S91 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S90 |ς|) (S91 (update-state-s8 |ς| (object-address "main::$tmp::return_value_lis") (evaluate-s8 |ς| (object-address "lis::1::max")))))))

; find_symbols
(declare-fun S92 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S91 |ς|) (S92 (exit-scope-state-p64 |ς| (object-address "lis::$tmp::return_value_malloc$0"))))))

; find_symbols
(declare-fun S93 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S92 |ς|) (S93 (exit-scope-state-p64 |ς| (object-address "lis::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S94 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S93 |ς|) (S94 (exit-scope-state-p64 |ς| (object-address "lis::1::max"))))))

; find_symbols
(declare-fun S95 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S94 |ς|) (S95 (exit-scope-state-p64 |ς| (object-address "lis::1::j"))))))

; find_symbols
(declare-fun S96 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S95 |ς|) (S96 (exit-scope-state-p64 |ς| (object-address "lis::1::i"))))))

; find_symbols
(declare-fun S97 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S96 |ς|) (S97 (exit-scope-state-p64 |ς| (object-address "lis::1::prev"))))))

; find_symbols
(declare-fun S98 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S97 |ς|) (S98 (exit-scope-state-p64 |ς| (object-address "lis::1::best"))))))

; set_to true (equal)
(declare-fun S99 (state) Bool)
(assert (= S99 S98))

; set_to true (equal)
(declare-fun S100 (state) Bool)
(assert (= S100 S99))

; find_symbols
(declare-fun S101 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S100 |ς|) (S101 (exit-scope-state-p64 |ς| (object-address "lis::a"))))))

; find_symbols
(declare-fun S102 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S101 |ς|) (S102 (exit-scope-state-p64 |ς| (object-address "lis::N"))))))

; find_symbols
(declare-fun S103 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S102 |ς|) (S103 (update-state-s8 |ς| (object-address "return'") (evaluate-s8 |ς| (object-address "main::$tmp::return_value_lis")))))))

; find_symbols
(declare-fun S104 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S103 |ς|) (S104 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_lis"))))))

; find_symbols
(declare-fun S105 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S104 |ς|) (S105 (exit-scope-state-p64 |ς| (object-address "main::1::numbers"))))))

; find_symbols
(declare-fun S106 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S105 |ς|) (S106 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S107 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S106 |ς|) (S107 (exit-scope-state-p64 |ς| (object-address "main::1::array_size"))))))

; find_symbols
(declare-fun S108 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S107 |ς|) (S108 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S109 (state) Bool)
(assert (= S109 S108))

; set_to true (equal)
(declare-fun S110 (state) Bool)
(assert (= S110 S109))

; set_to true (equal)
(declare-fun S111 (state) Bool)
(assert (= S111 S110))

; set_to true (equal)
(declare-fun S112 (state) Bool)
(assert (= S112 S111))

(check-sat)


; end of SMT2 file
