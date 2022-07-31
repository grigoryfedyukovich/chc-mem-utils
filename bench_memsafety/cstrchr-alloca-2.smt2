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
(assert (forall ((|ς| state)) (=> (S19 |ς|) (S20 (enter-scope-state-p64 |ς| (object-address "main::1::length") (_ bv4 64))))))

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
(assert (forall ((|ς| state)) (=> (S22 |ς|) (S23 (update-state-s32 |ς| (object-address "main::1::length") (evaluate-s32 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int")))))))

; find_symbols
(declare-fun S24T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S23 |ς|) (not (bvslt (evaluate-s32 |ς| (object-address "main::1::length")) (_ bv1 32)))) (S24T |ς|))))

; find_symbols
(declare-fun S24 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S23 |ς|) (bvslt (evaluate-s32 |ς| (object-address "main::1::length")) (_ bv1 32))) (S24 |ς|))))

; find_symbols
(declare-fun S25 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S24 |ς|) (S25 (update-state-s32 |ς| (object-address "main::1::length") (_ bv1 32))))))

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
(assert (forall ((|ς| state)) (=> (S26 |ς|) (S27 (enter-scope-state-p64 |ς| (object-address "main::1::nondetString") (_ bv8 64))))))

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
(assert (forall ((|ς| state)) (=> (S28 |ς|) (S29 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 32) (evaluate-s32 |ς| (object-address "main::1::length"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S30 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S29 |ς|) (S30 (update-state-p64 |ς| (object-address "main::1::nondetString") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; set_to true

; set_to true (equal)
(declare-fun S31 (state) Bool)
(assert (= S31 S30))

; find_symbols
(declare-fun S32 (state) Bool)
(declare-fun update-state-s8 (state (_ BitVec 64) (_ BitVec 8)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (update-state-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "main::1::nondetString")) ((_ sign_extend 32) (bvsub (evaluate-s32 |ς| (object-address "main::1::length")) (_ bv1 32)))) ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S33 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int$0") (_ bv4 64))))))

; find_symbols
(declare-fun S34 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S34-0| (_ BitVec 32))) (=> (S33 |ς|) (S34 (update-state-s32 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int$0") |nondet::S34-0|)))))

; set_to true (equal)
(declare-fun S35 (state) Bool)
(assert (= S35 S34))

; find_symbols
(declare-fun S36 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (enter-scope-state-p64 |ς| (object-address "cstrchr::s") (_ bv8 64))))))

; find_symbols
(declare-fun S37 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (update-state-p64 |ς| (object-address "cstrchr::s") (evaluate-p64 |ς| (object-address "main::1::nondetString")))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (enter-scope-state-p64 |ς| (object-address "cstrchr::c") (_ bv4 64))))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (update-state-s32 |ς| (object-address "cstrchr::c") (evaluate-s32 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int$0")))))))

; find_symbols
(declare-fun S47 (state) Bool)
; find_symbols
(declare-fun S40in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S40in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40in |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S40 (state) Bool)
(assert (= S40 S40in))

(declare-fun evaluate-s8 (state (_ BitVec 64)) (_ BitVec 8))
; find_symbols
(declare-fun S41T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S40 |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrchr::s")))) (_ bv0 32))))) (S41T |ς|))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S40 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrchr::s")))) (_ bv0 32)))) (S41 |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S42 (state) Bool)
(assert (= S42 S41))

; find_symbols
(declare-fun S43T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S42 |ς|) (not (not (= (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrchr::s"))) ((_ extract 7 0) (evaluate-s32 |ς| (object-address "cstrchr::c"))))))) (S43T |ς|))))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S42 |ς|) (not (= (evaluate-s8 |ς| (evaluate-p64 |ς| (object-address "cstrchr::s"))) ((_ extract 7 0) (evaluate-s32 |ς| (object-address "cstrchr::c")))))) (S43 |ς|))))

; set_to true (equal)
(declare-fun S44 (state) Bool)
(assert (= S44 S43))

; set_to true (equal)
(declare-fun S45 (state) Bool)
(assert (= S45 S44))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (update-state-p64 |ς| (object-address "cstrchr::s") (bvadd (evaluate-p64 |ς| (object-address "cstrchr::s")) (_ bv1 64)))))))

; set_to true
(assert (= S47 S46))

; find_symbols
(declare-fun S48in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S41T |ς|) (S48in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S43T |ς|) (S48in |ς|))))

; set_to true (equal)
(declare-fun S48 (state) Bool)
(assert (= S48 S48in))

; set_to true

; set_to true (equal)
(declare-fun S49 (state) Bool)
(assert (= S49 S48))

; set_to true (equal)
(declare-fun S50 (state) Bool)
(assert (= S50 S49))

; set_to true (equal)
(declare-fun S51 (state) Bool)
(assert (= S51 S50))

; set_to true (equal)
(declare-fun S52 (state) Bool)
(assert (= S52 S51))

; find_symbols
(declare-fun S53 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (exit-scope-state-p64 |ς| (object-address "cstrchr::s"))))))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (exit-scope-state-p64 |ς| (object-address "cstrchr::c"))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (update-state-s32 |ς| (object-address "return'") (_ bv0 32))))))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int$0"))))))

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString"))))))

; find_symbols
(declare-fun S58 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S57 |ς|) (S58 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S59 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S59 (exit-scope-state-p64 |ς| (object-address "main::1::length"))))))

; find_symbols
(declare-fun S60 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_int"))))))

; set_to true (equal)
(declare-fun S61 (state) Bool)
(assert (= S61 S60))

; set_to true (equal)
(declare-fun S62 (state) Bool)
(assert (= S62 S61))

; set_to true (equal)
(declare-fun S63 (state) Bool)
(assert (= S63 S62))

; set_to true (equal)
(declare-fun S64 (state) Bool)
(assert (= S64 S63))

(check-sat)


; end of SMT2 file
