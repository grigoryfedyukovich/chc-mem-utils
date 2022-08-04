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
(assert (forall ((|ς| state)) (=> (S19 |ς|) (S20 (enter-scope-state-p64 |ς| (object-address "main::1::length1") (_ bv4 64))))))

; find_symbols
(declare-fun S21 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S20 |ς|) (S21 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int") (_ bv4 64))))))

; find_symbols
(declare-fun S22 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S22-0| (_ BitVec 32))) (=> (S21 |ς|) (S22 (update-state-s32 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int") |nondet::S22-0|)))))

; find_symbols
(declare-fun S23 (state) Bool)
(declare-fun evaluate-s32 (state (_ BitVec 64)) (_ BitVec 32))
; set_to true
(assert (forall ((|ς| state)) (=> (S22 |ς|) (S23 (update-state-s32 |ς| (object-address "main::1::length1") (evaluate-s32 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int")))))))

; find_symbols
(declare-fun S24 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S23 |ς|) (S24 (enter-scope-state-p64 |ς| (object-address "main::1::length2") (_ bv4 64))))))

; find_symbols
(declare-fun S25 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S24 |ς|) (S25 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int$0") (_ bv4 64))))))

; find_symbols
(declare-fun S26 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S26-0| (_ BitVec 32))) (=> (S25 |ς|) (S26 (update-state-s32 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int$0") |nondet::S26-0|)))))

; find_symbols
(declare-fun S27 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S26 |ς|) (S27 (update-state-s32 |ς| (object-address "main::1::length2") (evaluate-s32 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int$0")))))))

; find_symbols
(declare-fun S28T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S27 |ς|) (not (bvslt (evaluate-s32 |ς| (object-address "main::1::length1")) (_ bv1 32)))) (S28T |ς|))))

; find_symbols
(declare-fun S28 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S27 |ς|) (bvslt (evaluate-s32 |ς| (object-address "main::1::length1")) (_ bv1 32))) (S28 |ς|))))

; find_symbols
(declare-fun S29 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S28 |ς|) (S29 (update-state-s32 |ς| (object-address "main::1::length1") (_ bv1 32))))))

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
(assert (forall ((|ς| state)) (=> (and (S30 |ς|) (not (bvslt (evaluate-s32 |ς| (object-address "main::1::length2")) (_ bv1 32)))) (S31T |ς|))))

; find_symbols
(declare-fun S31 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S30 |ς|) (bvslt (evaluate-s32 |ς| (object-address "main::1::length2")) (_ bv1 32))) (S31 |ς|))))

; find_symbols
(declare-fun S32 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (update-state-s32 |ς| (object-address "main::1::length2") (_ bv1 32))))))

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
(declare-fun S34T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S33 |ς|) (not (bvslt (evaluate-s32 |ς| (object-address "main::1::length1")) (evaluate-s32 |ς| (object-address "main::1::length2"))))) (S34T |ς|))))

; find_symbols
(declare-fun S34 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S33 |ς|) (bvslt (evaluate-s32 |ς| (object-address "main::1::length1")) (evaluate-s32 |ς| (object-address "main::1::length2")))) (S34 |ς|))))

; find_symbols
(declare-fun S35 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S34 |ς|) (S35 (update-state-s32 |ς| (object-address "return'") (_ bv0 32))))))

; find_symbols
(declare-fun S36 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (exit-scope-state-p64 |ς| (object-address "main::1::length2"))))))

; find_symbols
(declare-fun S37 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int$0"))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (exit-scope-state-p64 |ς| (object-address "main::1::length1"))))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int"))))))

; set_to true (equal)
(declare-fun S40 (state) Bool)
(assert (= S40 S39))

; set_to true (equal)
(declare-fun S41 (state) Bool)
(assert (= S41 S34T))

; find_symbols
(declare-fun S42 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S41 |ς|) (S42 (enter-scope-state-p64 |ς| (object-address "main::1::nondetArea") (_ bv8 64))))))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S44 (state) Bool)
(declare-fun allocate (state (_ BitVec 64)) (_ BitVec 64))
(declare-fun update-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S43 |ς|) (S44 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 32) (evaluate-s32 |ς| (object-address "main::1::length1"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S45 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45 (update-state-p64 |ς| (object-address "main::1::nondetArea") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (enter-scope-state-p64 |ς| (object-address "main::1::nondetString") (_ bv8 64))))))

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S46 |ς|) (S47 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S48 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0") (allocate |ς| (bvmul ((_ sign_extend 32) (evaluate-s32 |ς| (object-address "main::1::length2"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S49 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S49 (update-state-p64 |ς| (object-address "main::1::nondetString") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc$0")))))))

; set_to true

; set_to true (equal)
(declare-fun S50 (state) Bool)
(assert (= S50 S49))

; find_symbols
(declare-fun S51 (state) Bool)
(declare-fun update-state-s8 (state (_ BitVec 64) (_ BitVec 8)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S50 |ς|) (S51 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::nondetString")) ((_ sign_extend 32) (bvsub (evaluate-s32 |ς| (object-address "main::1::length2")) (_ bv1 32)))) ((_ extract 7 0) (_ bv0 32)))))))

; set_to true (equal)
(declare-fun S52 (state) Bool)
(assert (= S52 S51))

; find_symbols
(declare-fun S53 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (enter-scope-state-p64 |ς| (object-address "cstrcpy::to") (_ bv8 64))))))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (update-state-p64 |ς| (object-address "cstrcpy::to") (evaluate-p64 |ς| (object-address "main::1::nondetArea")))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (enter-scope-state-p64 |ς| (object-address "cstrcpy::from") (_ bv8 64))))))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56 (update-state-p64 |ς| (object-address "cstrcpy::from") (evaluate-p64 |ς| (object-address "main::1::nondetString")))))))

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (enter-scope-state-p64 |ς| (object-address "cstrcpy::1::save") (_ bv8 64))))))

; find_symbols
(declare-fun S58 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S57 |ς|) (S58 (update-state-p64 |ς| (object-address "cstrcpy::1::save") (evaluate-p64 |ς| (object-address "cstrcpy::to")))))))

; set_to true (equal)
(declare-fun S59 (state) Bool)
(assert (= S59 S58))

; find_symbols
(declare-fun S69 (state) Bool)
; find_symbols
(declare-fun S60in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S60in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60in |ς|))))

; find_symbols
(declare-fun S60 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60in |ς|) (S60 (enter-scope-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_assign") (_ bv1 64))))))

; set_to true

; set_to true (equal)
(declare-fun S61 (state) Bool)
(assert (= S61 S60))

; find_symbols
(declare-fun S62 (state) Bool)
(declare-fun evaluate-s8 (state (_ BitVec 64)) (_ BitVec 8))
; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62 (update-state-s8 |ς| (object-address "cstrcpy::$tmp::tmp_assign") (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrcpy::from"))))))))

; set_to true

; set_to true (equal)
(declare-fun S63 (state) Bool)
(assert (= S63 S62))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (update-state-s8 |ς| (evaluate-p64 |ς| (object-address "cstrcpy::to")) (evaluate-s8 |ς| (object-address "cstrcpy::$tmp::tmp_assign")))))))

; find_symbols
(declare-fun S65T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S64 |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrcpy::$tmp::tmp_assign"))) (_ bv0 32))))) (S65T |ς|))))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S64 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cstrcpy::$tmp::tmp_assign"))) (_ bv0 32)))) (S65 |ς|))))

; set_to true (equal)
(declare-fun S66 (state) Bool)
(assert (= S66 S65))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S66 |ς|) (S67 (update-state-p64 |ς| (object-address "cstrcpy::from") (bvadd (evaluate-p64 |ς| (object-address "cstrcpy::from")) (_ bv1 64)))))))

; find_symbols
(declare-fun S68 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67 |ς|) (S68 (update-state-p64 |ς| (object-address "cstrcpy::to") (bvadd (evaluate-p64 |ς| (object-address "cstrcpy::to")) (_ bv1 64)))))))

; set_to true
(assert (= S69 S68))

; set_to true (equal)
(declare-fun S70 (state) Bool)
(assert (= S70 S65T))

; find_symbols
(declare-fun S71 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S71 (exit-scope-state-p64 |ς| (object-address "cstrcpy::$tmp::tmp_assign"))))))

; set_to true (equal)
(declare-fun S72 (state) Bool)
(assert (= S72 S71))

; find_symbols
(declare-fun S73 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S73 (exit-scope-state-p64 |ς| (object-address "cstrcpy::1::save"))))))

; set_to true (equal)
(declare-fun S74 (state) Bool)
(assert (= S74 S73))

; set_to true (equal)
(declare-fun S75 (state) Bool)
(assert (= S75 S74))

; find_symbols
(declare-fun S76 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S75 |ς|) (S76 (exit-scope-state-p64 |ς| (object-address "cstrcpy::to"))))))

; find_symbols
(declare-fun S77 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S77 (exit-scope-state-p64 |ς| (object-address "cstrcpy::from"))))))

; find_symbols
(declare-fun S78 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S77 |ς|) (S78 (update-state-s32 |ς| (object-address "return'") (_ bv0 32))))))

; find_symbols
(declare-fun S79 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S78 |ς|) (S79 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString"))))))

; find_symbols
(declare-fun S80 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S79 |ς|) (S80 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0"))))))

; find_symbols
(declare-fun S81 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S80 |ς|) (S81 (exit-scope-state-p64 |ς| (object-address "main::1::nondetArea"))))))

; find_symbols
(declare-fun S82 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S81 |ς|) (S82 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S83 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S82 |ς|) (S83 (exit-scope-state-p64 |ς| (object-address "main::1::length2"))))))

; find_symbols
(declare-fun S84 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S83 |ς|) (S84 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int$0"))))))

; find_symbols
(declare-fun S85 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S84 |ς|) (S85 (exit-scope-state-p64 |ς| (object-address "main::1::length1"))))))

; find_symbols
(declare-fun S86 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S85 |ς|) (S86 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int"))))))

; set_to true (equal)
(declare-fun S87 (state) Bool)
(assert (= S87 S86))

; find_symbols
(declare-fun S88in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S88in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S87 |ς|) (S88in |ς|))))

; set_to true (equal)
(declare-fun S88 (state) Bool)
(assert (= S88 S88in))

; set_to true (equal)
(declare-fun S89 (state) Bool)
(assert (= S89 S88))

; set_to true (equal)
(declare-fun S90 (state) Bool)
(assert (= S90 S89))

(check-sat)


; end of SMT2 file
