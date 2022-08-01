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
(assert (forall ((|ς| state)) (=> (S23 |ς|) (S24 (enter-scope-state-p64 |ς| (object-address "main::1::c") (_ bv1 64))))))

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
(assert (forall ((|ς| state)) (=> (S26 |ς|) (S27 (update-state-s8 |ς| (object-address "main::1::c") (evaluate-s8 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0")))))))

; find_symbols
(declare-fun S28T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S27 |ς|) (not (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32)) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv10000 32))))) (S28T |ς|))))

; find_symbols
(declare-fun S28 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S27 |ς|) (or (bvslt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32)) (bvsgt ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv10000 32)))) (S28 |ς|))))

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
(declare-fun S31 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S30 |ς|) (S31 (enter-scope-state-p64 |ς| (object-address "main::1::nondetArea") (_ bv8 64))))))

; find_symbols
(declare-fun S32 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S33 (state) Bool)
(declare-fun allocate (state (_ BitVec 64)) (_ BitVec 64))
(declare-fun update-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S32 |ς|) (S33 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "main::1::length")))))))))

; find_symbols
(declare-fun S34 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S33 |ς|) (S34 (update-state-p64 |ς| (object-address "main::1::nondetArea") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; set_to true (equal)
(declare-fun S35 (state) Bool)
(assert (= S35 S34))

; find_symbols
(declare-fun S36 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (enter-scope-state-p64 |ς| (object-address "cmemchr::s") (_ bv8 64))))))

; find_symbols
(declare-fun S37 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S36 |ς|) (S37 (update-state-p64 |ς| (object-address "cmemchr::s") (evaluate-p64 |ς| (object-address "main::1::nondetArea")))))))

; find_symbols
(declare-fun S38 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S38 (enter-scope-state-p64 |ς| (object-address "cmemchr::c") (_ bv1 64))))))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (update-state-s8 |ς| (object-address "cmemchr::c") (evaluate-s8 |ς| (object-address "main::1::c")))))))

; find_symbols
(declare-fun S40 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S39 |ς|) (S40 (enter-scope-state-p64 |ς| (object-address "cmemchr::n") (_ bv1 64))))))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (update-state-s8 |ς| (object-address "cmemchr::n") ((_ extract 7 0) (bvsub ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "main::1::length"))) (_ bv1 32))))))))

; find_symbols
(declare-fun S42T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S41 |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cmemchr::n"))) (_ bv0 32))))) (S42T |ς|))))

; find_symbols
(declare-fun S42 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S41 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cmemchr::n"))) (_ bv0 32)))) (S42 |ς|))))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (enter-scope-state-p64 |ς| (object-address "cmemchr::1::1::p") (_ bv8 64))))))

; find_symbols
(declare-fun S44 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S43 |ς|) (S44 (update-state-p64 |ς| (object-address "cmemchr::1::1::p") (evaluate-p64 |ς| (object-address "cmemchr::s")))))))

; find_symbols
(declare-fun S56T (state) Bool)
; find_symbols
(declare-fun S45in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56T |ς|) (S45in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45in |ς|))))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45in |ς|) (S45 (enter-scope-state-p64 |ς| (object-address "cmemchr::$tmp::tmp_post_p") (_ bv8 64))))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (update-state-p64 |ς| (object-address "cmemchr::$tmp::tmp_post_p") (evaluate-p64 |ς| (object-address "cmemchr::1::1::p")))))))

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S46 |ς|) (S47 (update-state-p64 |ς| (object-address "cmemchr::1::1::p") (bvadd (evaluate-p64 |ς| (object-address "cmemchr::1::1::p")) (_ bv1 64)))))))

; set_to true

; set_to true (equal)
(declare-fun S48 (state) Bool)
(assert (= S48 S47))

(declare-fun evaluate-u8 (state (_ BitVec 64)) (_ BitVec 8))
; find_symbols
(declare-fun S49T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S48 |ς|) (not (= (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "cmemchr::$tmp::tmp_post_p"))) (evaluate-s8 |ς| (object-address "cmemchr::c"))))) (S49T |ς|))))

; find_symbols
(declare-fun S49 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S48 |ς|) (= (evaluate-u8 |ς| (evaluate-p64 |ς| (object-address "cmemchr::$tmp::tmp_post_p"))) (evaluate-s8 |ς| (object-address "cmemchr::c")))) (S49 |ς|))))

; set_to true (equal)
(declare-fun S50 (state) Bool)
(assert (= S50 S49))

; find_symbols
(declare-fun S51 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S50 |ς|) (S51 (exit-scope-state-p64 |ς| (object-address "cmemchr::1::1::p"))))))

; set_to true (equal)
(declare-fun S52 (state) Bool)
(assert (= S52 S51))

; set_to true (equal)
(declare-fun S53 (state) Bool)
(assert (= S53 S49T))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (exit-scope-state-p64 |ς| (object-address "cmemchr::$tmp::tmp_post_p"))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (update-state-s8 |ς| (object-address "cmemchr::n") (bvsub (evaluate-s8 |ς| (object-address "cmemchr::n")) (_ bv1 8)))))))

; set_to true
(assert (forall ((|ς| state)) (=> (and (S55 |ς|) (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cmemchr::n"))) (_ bv0 32)))) (S56T |ς|))))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S55 |ς|) (not (not (= ((_ sign_extend 24) (evaluate-s8 |ς| (object-address "cmemchr::n"))) (_ bv0 32))))) (S56 |ς|))))

; set_to true (equal)
(declare-fun S57 (state) Bool)
(assert (= S57 S56))

; find_symbols
(declare-fun S58 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S57 |ς|) (S58 (exit-scope-state-p64 |ς| (object-address "cmemchr::1::1::p"))))))

; find_symbols
(declare-fun S59in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42T |ς|) (S59in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S59in |ς|))))

; set_to true (equal)
(declare-fun S59 (state) Bool)
(assert (= S59 S59in))

; set_to true (equal)
(declare-fun S60 (state) Bool)
(assert (= S60 S59))

; set_to true (equal)
(declare-fun S61 (state) Bool)
(assert (= S61 S60))

; find_symbols
(declare-fun S62in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S62in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62in |ς|))))

; set_to true (equal)
(declare-fun S62 (state) Bool)
(assert (= S62 S62in))

; find_symbols
(declare-fun S63 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S62 |ς|) (S63 (exit-scope-state-p64 |ς| (object-address "cmemchr::s"))))))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (exit-scope-state-p64 |ς| (object-address "cmemchr::c"))))))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65 (exit-scope-state-p64 |ς| (object-address "cmemchr::n"))))))

; find_symbols
(declare-fun S66 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S65 |ς|) (S66 (update-state-s32 |ς| (object-address "return'") (_ bv0 32))))))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S66 |ς|) (S67 (exit-scope-state-p64 |ς| (object-address "main::1::nondetArea"))))))

; find_symbols
(declare-fun S68 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67 |ς|) (S68 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S69 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69 (exit-scope-state-p64 |ς| (object-address "main::1::c"))))))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S70 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0"))))))

; find_symbols
(declare-fun S71 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S71 (exit-scope-state-p64 |ς| (object-address "main::1::length"))))))

; find_symbols
(declare-fun S72 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S71 |ς|) (S72 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S73 (state) Bool)
(assert (= S73 S72))

; set_to true (equal)
(declare-fun S74 (state) Bool)
(assert (= S74 S73))

; set_to true (equal)
(declare-fun S75 (state) Bool)
(assert (= S75 S74))

; set_to true (equal)
(declare-fun S76 (state) Bool)
(assert (= S76 S75))

(check-sat)


; end of SMT2 file
