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

; find_symbols
(declare-fun S18 (state) Bool)
(declare-fun update-state-s8 (state (_ BitVec 64) (_ BitVec 8)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S17 |ς|) (S18 (update-state-s8 |ς| (object-address "MAX") ((_ extract 7 0) (_ bv100000 32)))))))

; set_to true (equal)
(declare-fun S19 (state) Bool)
(assert (= S19 S18))

; set_to true (equal)
(declare-fun S20 (state) Bool)
(assert (= S20 S19))

; find_symbols
(declare-fun S21 (state) Bool)
(declare-fun enter-scope-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S20 |ς|) (S21 (enter-scope-state-p64 |ς| (object-address "main::1::length") (_ bv1 64))))))

; find_symbols
(declare-fun S22 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S21 |ς|) (S22 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char") (_ bv1 64))))))

; find_symbols
(declare-fun S23 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S23-0| (_ BitVec 8))) (=> (S22 |ς|) (S23 (update-state-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char") |nondet::S23-0|)))))

; find_symbols
(declare-fun S24 (state) Bool)
(declare-fun evaluate-s8 (state (_ BitVec 64)) (_ BitVec 8))
; set_to true
(assert (forall ((|ς| state)) (=> (S23 |ς|) (S24 (update-state-s8 |ς| (object-address "main::1::length") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char")))))))

; find_symbols
(declare-fun S25 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S24 |ς|) (S25 (enter-scope-state-p64 |ς| (object-address "main::1::c") (_ bv1 64))))))

; find_symbols
(declare-fun S26 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S25 |ς|) (S26 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0") (_ bv1 64))))))

; find_symbols
(declare-fun S27 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S27-0| (_ BitVec 8))) (=> (S26 |ς|) (S27 (update-state-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0") |nondet::S27-0|)))))

; find_symbols
(declare-fun S28 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S27 |ς|) (S28 (update-state-s8 |ς| (object-address "main::1::c") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0")))))))

; find_symbols
(declare-fun S29T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S28 |ς|) (not (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32)) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv10000 32))))) (S29T |ς|))))

; find_symbols
(declare-fun S29 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S28 |ς|) (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32)) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv10000 32)))) (S29 |ς|))))

; find_symbols
(declare-fun S30 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S29 |ς|) (S30 (update-state-s8 |ς| (object-address "main::1::length") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S31in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S29T |ς|) (S31in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S30 |ς|) (S31in |ς|))))

; set_to true (equal)
(declare-fun S31 (state) Bool)
(assert (= S31 S31in))

; find_symbols
(declare-fun S32 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (enter-scope-state-p64 |ς| (object-address "main::1::nondetArea") (_ bv8 64))))))

; find_symbols
(declare-fun S33 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S34 (state) Bool)
(declare-fun allocate (state (_ BitVec 64)) (_ BitVec 64))
(declare-fun update-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S33 |ς|) (S34 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (bvmul ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 64))))))))

; find_symbols
(declare-fun S35 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S34 |ς|) (S35 (update-state-p64 |ς| (object-address "main::1::nondetArea") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; set_to true (equal)
(declare-fun S36 (state) Bool)
(assert (= S36 S35))

; find_symbols
(declare-fun S37 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (enter-scope-state-p64 |ς| (object-address "cmemrchr::s") (_ bv8 64))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (update-state-p64 |ς| (object-address "cmemrchr::s") (evaluate-p64 |ς| (object-address "main::1::nondetArea")))))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (enter-scope-state-p64 |ς| (object-address "cmemrchr::c") (_ bv1 64))))))

; find_symbols
(declare-fun S40 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40 (update-state-s8 |ς| (object-address "cmemrchr::c") (evaluate-s8 |ς| (object-address "main::1::c")))))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (enter-scope-state-p64 |ς| (object-address "cmemrchr::n") (_ bv8 64))))))

; find_symbols
(declare-fun S42 (state) Bool)
(declare-fun update-state-u64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S41 |ς|) (S42 (update-state-u64 |ς| (object-address "cmemrchr::n") ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length"))))))))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (enter-scope-state-p64 |ς| (object-address "cmemrchr::1::cp") (_ bv8 64))))))

(declare-fun evaluate-u64 (state (_ BitVec 64)) (_ BitVec 64))
; find_symbols
(declare-fun S44T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S43 |ς|) (not (not (= (evaluate-u64 |ς| (object-address "cmemrchr::n")) ((_ sign_extend 32) (_ bv0 32)))))) (S44T |ς|))))

; find_symbols
(declare-fun S44 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S43 |ς|) (not (= (evaluate-u64 |ς| (object-address "cmemrchr::n")) ((_ sign_extend 32) (_ bv0 32))))) (S44 |ς|))))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45 (update-state-p64 |ς| (object-address "cmemrchr::1::cp") (bvadd (evaluate-p64 |ς| (object-address "cmemrchr::s")) (evaluate-u64 |ς| (object-address "cmemrchr::n"))))))))

; find_symbols
(declare-fun S54T (state) Bool)
; find_symbols
(declare-fun S46in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54T |ς|) (S46in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46in |ς|))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S46in |ς|) (S46 (update-state-p64 |ς| (object-address "cmemrchr::1::cp") (bvadd (evaluate-p64 |ς| (object-address "cmemrchr::1::cp")) (bvneg (_ bv1 64))))))))

; set_to true

; set_to true (equal)
(declare-fun S47 (state) Bool)
(assert (= S47 S46))

(declare-fun evaluate-u8 (state (_ BitVec 64)) (_ BitVec 8))
; find_symbols
(declare-fun S48T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S47 |ς|) (not (= (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "cmemrchr::1::cp"))) (evaluate-s8 |ς| (object-address "cmemrchr::c"))))) (S48T |ς|))))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S47 |ς|) (= (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "cmemrchr::1::cp"))) (evaluate-s8 |ς| (object-address "cmemrchr::c")))) (S48 |ς|))))

; set_to true (equal)
(declare-fun S49 (state) Bool)
(assert (= S49 S48))

; find_symbols
(declare-fun S50 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50 (exit-scope-state-p64 |ς| (object-address "cmemrchr::1::cp"))))))

; set_to true (equal)
(declare-fun S51 (state) Bool)
(assert (= S51 S50))

; set_to true (equal)
(declare-fun S52 (state) Bool)
(assert (= S52 S48T))

; find_symbols
(declare-fun S53 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (update-state-u64 |ς| (object-address "cmemrchr::n") (bvsub (evaluate-u64 |ς| (object-address "cmemrchr::n")) (_ bv1 64)))))))

; set_to true
(assert (forall ((|ς| state)) (=> (and (S53 |ς|) (not (= (evaluate-u64 |ς| (object-address "cmemrchr::n")) ((_ sign_extend 32) (_ bv0 32))))) (S54T |ς|))))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S53 |ς|) (not (not (= (evaluate-u64 |ς| (object-address "cmemrchr::n")) ((_ sign_extend 32) (_ bv0 32)))))) (S54 |ς|))))

; set_to true (equal)
(declare-fun S55 (state) Bool)
(assert (= S55 S54))

; find_symbols
(declare-fun S56in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S44T |ς|) (S56in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56in |ς|))))

; set_to true (equal)
(declare-fun S56 (state) Bool)
(assert (= S56 S56in))

; set_to true (equal)
(declare-fun S57 (state) Bool)
(assert (= S57 S56))

; find_symbols
(declare-fun S58 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S57 |ς|) (S58 (exit-scope-state-p64 |ς| (object-address "cmemrchr::1::cp"))))))

; set_to true (equal)
(declare-fun S59 (state) Bool)
(assert (= S59 S58))

; find_symbols
(declare-fun S60in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S60in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S59 |ς|) (S60in |ς|))))

; set_to true (equal)
(declare-fun S60 (state) Bool)
(assert (= S60 S60in))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61 (exit-scope-state-p64 |ς| (object-address "cmemrchr::s"))))))

; find_symbols
(declare-fun S62 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62 (exit-scope-state-p64 |ς| (object-address "cmemrchr::c"))))))

; find_symbols
(declare-fun S63 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S62 |ς|) (S63 (exit-scope-state-p64 |ς| (object-address "cmemrchr::n"))))))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (update-state-s8 |ς| (object-address "return'") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65 (exit-scope-state-p64 |ς| (object-address "main::1::nondetArea"))))))

; find_symbols
(declare-fun S66 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S65 |ς|) (S66 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S66 |ς|) (S67 (exit-scope-state-p64 |ς| (object-address "main::1::c"))))))

; find_symbols
(declare-fun S68 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67 |ς|) (S68 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0"))))))

; find_symbols
(declare-fun S69 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69 (exit-scope-state-p64 |ς| (object-address "main::1::length"))))))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S70 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S71 (state) Bool)
(assert (= S71 S70))

; set_to true (equal)
(declare-fun S72 (state) Bool)
(assert (= S72 S71))

; set_to true (equal)
(declare-fun S73 (state) Bool)
(assert (= S73 S72))

; set_to true (equal)
(declare-fun S74 (state) Bool)
(assert (= S74 S73))

(check-sat)


; end of SMT2 file
