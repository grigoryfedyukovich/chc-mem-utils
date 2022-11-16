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
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_strcasecmp") (_ bv1 64))))))

; set_to true (equal)
(declare-fun S47 (state) Bool)
(assert (= S47 S46))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S48 (enter-scope-state-p64 |ς| (object-address "strcasecmp::s1") (_ bv8 64))))))

; find_symbols
(declare-fun S49 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S49 (update-state-p64 |ς| (object-address "strcasecmp::s1") (evaluate-p64 |ς| (object-address "main::1::nondetString1")))))))

; find_symbols
(declare-fun S50 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S49 |ς|) (S50 (enter-scope-state-p64 |ς| (object-address "strcasecmp::s2") (_ bv8 64))))))

; find_symbols
(declare-fun S51 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S50 |ς|) (S51 (update-state-p64 |ς| (object-address "strcasecmp::s2") (evaluate-p64 |ς| (object-address "main::1::nondetString2")))))))

; find_symbols
(declare-fun S52 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S52 (enter-scope-state-p64 |ς| (object-address "strcasecmp::1::i") (_ bv1 64))))))

; find_symbols
(declare-fun S53 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S52 |ς|) (S53 (update-state-s8 |ς| (object-address "strcasecmp::1::i") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (enter-scope-state-p64 |ς| (object-address "strcasecmp::1::ch1") (_ bv1 64))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (enter-scope-state-p64 |ς| (object-address "strcasecmp::1::ch2") (_ bv1 64))))))

; find_symbols
(declare-fun S82T (state) Bool)
; find_symbols
(declare-fun S56in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S82T |ς|) (S56in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56in |ς|))))

; set_to true

; set_to true (equal)
(declare-fun S56 (state) Bool)
(assert (= S56 S56in))

; find_symbols
(declare-fun S57 (state) Bool)
(declare-fun update-state-u8 (state (_ BitVec 64) (_ BitVec 8)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (update-state-u8 |ς| (object-address "strcasecmp::1::ch1") (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "strcasecmp::s1")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "strcasecmp::1::i"))))))))))

; set_to true

; set_to true (equal)
(declare-fun S58 (state) Bool)
(assert (= S58 S57))

; find_symbols
(declare-fun S59 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S59 (update-state-u8 |ς| (object-address "strcasecmp::1::ch2") (evaluate-s8 |ς| (bvadd (evaluate-p64 |ς| (object-address "strcasecmp::s2")) ((_ sign_extend 56) (evaluate-s8 |ς| (object-address "strcasecmp::1::i"))))))))))

(declare-fun evaluate-u8 (state (_ BitVec 64)) (_ BitVec 8))
; find_symbols
(declare-fun S60T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S59 |ς|) (not (and (bvsge ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1"))) (_ bv65 32)) (bvsle ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1"))) (_ bv90 32))))) (S60T |ς|))))

; find_symbols
(declare-fun S60 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S59 |ς|) (and (bvsge ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1"))) (_ bv65 32)) (bvsle ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1"))) (_ bv90 32)))) (S60 |ς|))))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61 (update-state-u8 |ς| (object-address "strcasecmp::1::ch1") ((_ extract 7 0) (bvadd ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1"))) (bvsub (_ bv97 32) (_ bv65 32)))))))))

; find_symbols
(declare-fun S62in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60T |ς|) (S62in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S61 |ς|) (S62in |ς|))))

; set_to true (equal)
(declare-fun S62 (state) Bool)
(assert (= S62 S62in))

; find_symbols
(declare-fun S63T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S62 |ς|) (not (and (bvsge ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2"))) (_ bv65 32)) (bvsle ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2"))) (_ bv90 32))))) (S63T |ς|))))

; find_symbols
(declare-fun S63 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S62 |ς|) (and (bvsge ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2"))) (_ bv65 32)) (bvsle ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2"))) (_ bv90 32)))) (S63 |ς|))))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (update-state-u8 |ς| (object-address "strcasecmp::1::ch2") ((_ extract 7 0) (bvadd ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2"))) (bvsub (_ bv97 32) (_ bv65 32)))))))))

; find_symbols
(declare-fun S65in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63T |ς|) (S65in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65in |ς|))))

; set_to true (equal)
(declare-fun S65 (state) Bool)
(assert (= S65 S65in))

; find_symbols
(declare-fun S66T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S65 |ς|) (= (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1")) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2")))) (S66T |ς|))))

; find_symbols
(declare-fun S66 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S65 |ς|) (not (= (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1")) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2"))))) (S66 |ς|))))

; find_symbols
(declare-fun S67T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S66 |ς|) (not (bvslt ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1"))) ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2")))))) (S67T |ς|))))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S66 |ς|) (bvslt ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1"))) ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2"))))) (S67 |ς|))))

; find_symbols
(declare-fun S68 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67 |ς|) (S68 (update-state-s8 |ς| (object-address "main::$tmp::return_value_strcasecmp") ((_ extract 7 0) (bvneg (_ bv1 32))))))))

; find_symbols
(declare-fun S69 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69 (exit-scope-state-p64 |ς| (object-address "strcasecmp::1::ch2"))))))

; find_symbols
(declare-fun S70 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S69 |ς|) (S70 (exit-scope-state-p64 |ς| (object-address "strcasecmp::1::ch1"))))))

; find_symbols
(declare-fun S71 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S70 |ς|) (S71 (exit-scope-state-p64 |ς| (object-address "strcasecmp::1::i"))))))

; set_to true (equal)
(declare-fun S72 (state) Bool)
(assert (= S72 S71))

; find_symbols
(declare-fun S73in (state) Bool)
; set_to true (equal)
(declare-fun S73 (state) Bool)
(assert (= S73 S73in))

; find_symbols
(declare-fun S74 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S67T |ς|) (S74 (update-state-s8 |ς| (object-address "main::$tmp::return_value_strcasecmp") ((_ extract 7 0) (_ bv1 32)))))))

; find_symbols
(declare-fun S75 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S74 |ς|) (S75 (exit-scope-state-p64 |ς| (object-address "strcasecmp::1::ch2"))))))

; find_symbols
(declare-fun S76 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S75 |ς|) (S76 (exit-scope-state-p64 |ς| (object-address "strcasecmp::1::ch1"))))))

; find_symbols
(declare-fun S77 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S76 |ς|) (S77 (exit-scope-state-p64 |ς| (object-address "strcasecmp::1::i"))))))

; set_to true (equal)
(declare-fun S78 (state) Bool)
(assert (= S78 S77))

; set_to true (equal)
(declare-fun S79 (state) Bool)
(assert (= S79 S73))

; find_symbols
(declare-fun S80in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S66T |ς|) (S80in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S79 |ς|) (S80in |ς|))))

; set_to true (equal)
(declare-fun S80 (state) Bool)
(assert (= S80 S80in))

; find_symbols
(declare-fun S81 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S80 |ς|) (S81 (update-state-s8 |ς| (object-address "strcasecmp::1::i") (bvadd (evaluate-s8 |ς| (object-address "strcasecmp::1::i")) (_ bv1 8)))))))

; set_to true
(assert (forall ((|ς| state)) (=> (and (S81 |ς|) (and (not (= ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1"))) (_ bv0 32))) (not (= ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2"))) (_ bv0 32))))) (S82T |ς|))))

; find_symbols
(declare-fun S82 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S81 |ς|) (not (and (not (= ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch1"))) (_ bv0 32))) (not (= ((_ zero_extend 24) (evaluate-u8 |ς| (object-address "strcasecmp::1::ch2"))) (_ bv0 32)))))) (S82 |ς|))))

; set_to true (equal)
(declare-fun S83 (state) Bool)
(assert (= S83 S82))

; find_symbols
(declare-fun S84 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S83 |ς|) (S84 (update-state-s8 |ς| (object-address "main::$tmp::return_value_strcasecmp") ((_ extract 7 0) (_ bv0 32)))))))

; find_symbols
(declare-fun S85 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S84 |ς|) (S85 (exit-scope-state-p64 |ς| (object-address "strcasecmp::1::ch2"))))))

; find_symbols
(declare-fun S86 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S85 |ς|) (S86 (exit-scope-state-p64 |ς| (object-address "strcasecmp::1::ch1"))))))

; find_symbols
(declare-fun S87 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S86 |ς|) (S87 (exit-scope-state-p64 |ς| (object-address "strcasecmp::1::i"))))))

; set_to true (equal)
(declare-fun S88 (state) Bool)
(assert (= S88 S87))

; find_symbols
(declare-fun S89in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S72 |ς|) (S89in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S78 |ς|) (S89in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S88 |ς|) (S89in |ς|))))

; set_to true (equal)
(declare-fun S89 (state) Bool)
(assert (= S89 S89in))

; find_symbols
(declare-fun S90 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S89 |ς|) (S90 (exit-scope-state-p64 |ς| (object-address "strcasecmp::s1"))))))

; find_symbols
(declare-fun S91 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S90 |ς|) (S91 (exit-scope-state-p64 |ς| (object-address "strcasecmp::s2"))))))

; find_symbols
(declare-fun S92 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S91 |ς|) (S92 (update-state-s8 |ς| (object-address "return'") (evaluate-s8 |ς| (object-address "main::$tmp::return_value_strcasecmp")))))))

; find_symbols
(declare-fun S93 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S92 |ς|) (S93 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_strcasecmp"))))))

; find_symbols
(declare-fun S94 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S93 |ς|) (S94 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString2"))))))

; find_symbols
(declare-fun S95 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S94 |ς|) (S95 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc$0"))))))

; find_symbols
(declare-fun S96 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S95 |ς|) (S96 (exit-scope-state-p64 |ς| (object-address "main::1::nondetString1"))))))

; find_symbols
(declare-fun S97 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S96 |ς|) (S97 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; find_symbols
(declare-fun S98 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S97 |ς|) (S98 (exit-scope-state-p64 |ς| (object-address "main::1::length2"))))))

; find_symbols
(declare-fun S99 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S98 |ς|) (S99 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char$0"))))))

; find_symbols
(declare-fun S100 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S99 |ς|) (S100 (exit-scope-state-p64 |ς| (object-address "main::1::length1"))))))

; find_symbols
(declare-fun S101 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S100 |ς|) (S101 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value___VERIFIER_nondet_char"))))))

; set_to true (equal)
(declare-fun S102 (state) Bool)
(assert (= S102 S101))

; set_to true (equal)
(declare-fun S103 (state) Bool)
(assert (= S103 S102))

; set_to true (equal)
(declare-fun S104 (state) Bool)
(assert (= S104 S103))

; set_to true (equal)
(declare-fun S105 (state) Bool)
(assert (= S105 S104))

(check-sat)


; end of SMT2 file
