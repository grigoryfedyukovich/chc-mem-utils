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
; set_to true
(assert (forall ((|ς| state)) (=> (S17 |ς|) (S18 (update-state-s32 |ς| (object-address "i") (_ bv1 32))))))

; find_symbols
(declare-fun S19 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S18 |ς|) (S19 (update-state-s32 |ς| (object-address "ind") (_ bv0 32))))))

; find_symbols
(declare-fun S20 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S19 |ς|) (S20 (update-state-s32 |ς| (object-address "newsize") (_ bv0 32))))))

; find_symbols
(declare-fun S21 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S20 |ς|) (S21 (update-state-s32 |ς| (object-address "num") (_ bv0 32))))))

; set_to true (equal)
(declare-fun S22 (state) Bool)
(assert (= S22 S21))

; set_to true (equal)
(declare-fun S23 (state) Bool)
(assert (= S23 S22))

; find_symbols
(declare-fun S24 (state) Bool)
; set_to true
(assert (forall ((|ς| state) (|nondet::S24-0| (_ BitVec 32))) (=> (S23 |ς|) (S24 (update-state-s32 |ς| (object-address "num") |nondet::S24-0|)))))

(declare-fun evaluate-s32 (state (_ BitVec 64)) (_ BitVec 32))
; find_symbols
(declare-fun S25T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S24 |ς|) (and (bvsgt (evaluate-s32 |ς| (object-address "num")) (_ bv0 32)) (bvslt (evaluate-s32 |ς| (object-address "num")) (_ bv100 32)))) (S25T |ς|))))

; find_symbols
(declare-fun S25 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S24 |ς|) (not (and (bvsgt (evaluate-s32 |ς| (object-address "num")) (_ bv0 32)) (bvslt (evaluate-s32 |ς| (object-address "num")) (_ bv100 32))))) (S25 |ς|))))

; find_symbols
(declare-fun S26 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S25 |ς|) (S26 (update-state-s32 |ς| (object-address "return'") (_ bv0 32))))))

; set_to true (equal)
(declare-fun S27 (state) Bool)
(assert (= S27 S26))

; set_to true (equal)
(declare-fun S28 (state) Bool)
(assert (= S28 S25T))

; find_symbols
(declare-fun S29 (state) Bool)
(declare-fun enter-scope-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S28 |ς|) (S29 (enter-scope-state-p64 |ς| (object-address "main::1::a") (_ bv8 64))))))

; find_symbols
(declare-fun S30 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S29 |ς|) (S30 (enter-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (_ bv8 64))))))
; __CPROVER__start → malloc

; find_symbols
(declare-fun S31 (state) Bool)
(declare-fun allocate (state (_ BitVec 64)) (_ BitVec 64))
(declare-fun update-state-p64 (state (_ BitVec 64) (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S30 |ς|) (S31 (update-state-p64 |ς| (object-address "main::$tmp::return_value_malloc") (allocate |ς| (_ bv4 64)))))))

; find_symbols
(declare-fun S32 (state) Bool)
(declare-fun evaluate-p64 (state (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S31 |ς|) (S32 (update-state-p64 |ς| (object-address "main::1::a") (evaluate-p64 |ς| (object-address "main::$tmp::return_value_malloc")))))))

; find_symbols
(declare-fun S33T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S32 |ς|) (not (= (evaluate-p64 |ς| (object-address "main::1::a")) (_ bv0 64)))) (S33T |ς|))))

; find_symbols
(declare-fun S33 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S32 |ς|) (= (evaluate-p64 |ς| (object-address "main::1::a")) (_ bv0 64))) (S33 |ς|))))

; find_symbols
(declare-fun S34 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S33 |ς|) (S34 (update-state-s32 |ς| (object-address "return'") (_ bv0 32))))))

; find_symbols
(declare-fun S35 (state) Bool)
(declare-fun exit-scope-state-p64 (state (_ BitVec 64)) state)
; set_to true
(assert (forall ((|ς| state)) (=> (S34 |ς|) (S35 (exit-scope-state-p64 |ς| (object-address "main::1::a"))))))

; find_symbols
(declare-fun S36 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S35 |ς|) (S36 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; set_to true (equal)
(declare-fun S37 (state) Bool)
(assert (= S37 S36))

; set_to true (equal)
(declare-fun S38 (state) Bool)
(assert (= S38 S33T))

; find_symbols
(declare-fun S39 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S38 |ς|) (S39 (update-state-s32 |ς| (object-address "newsize") (_ bv0 32))))))

; set_to true (equal)
(declare-fun S40 (state) Bool)
(assert (= S40 S39))

; find_symbols
(declare-fun S41 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S40 |ς|) (S41 (enter-scope-state-p64 |ς| (object-address "expandArray::arg") (_ bv8 64))))))

; find_symbols
(declare-fun S42 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S41 |ς|) (S42 (update-state-p64 |ς| (object-address "expandArray::arg") (evaluate-p64 |ς| (object-address "main::1::a")))))))

; find_symbols
(declare-fun S43 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S42 |ς|) (S43 (enter-scope-state-p64 |ς| (object-address "expandArray::1::a") (_ bv8 64))))))

; find_symbols
(declare-fun S44 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S43 |ς|) (S44 (update-state-p64 |ς| (object-address "expandArray::1::a") (evaluate-p64 |ς| (object-address "expandArray::arg")))))))

; find_symbols
(declare-fun S58 (state) Bool)
; find_symbols
(declare-fun S45in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S58 |ς|) (S45in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S44 |ς|) (S45in |ς|))))

; find_symbols
(declare-fun S45T (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S45in |ς|) (not (bvslt (evaluate-s32 |ς| (object-address "newsize")) (evaluate-s32 |ς| (object-address "num"))))) (S45T |ς|))))

; find_symbols
(declare-fun S45 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (and (S45in |ς|) (bvslt (evaluate-s32 |ς| (object-address "newsize")) (evaluate-s32 |ς| (object-address "num")))) (S45 |ς|))))

; find_symbols
(declare-fun S46 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S45 |ς|) (S46 (update-state-s32 |ς| (object-address "newsize") (bvadd (evaluate-s32 |ς| (object-address "newsize")) (_ bv1 32)))))))

; find_symbols
(declare-fun S47 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S46 |ς|) (S47 (enter-scope-state-p64 |ς| (object-address "expandArray::1::1::b") (_ bv8 64))))))

; find_symbols
(declare-fun S48 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S47 |ς|) (S48 (update-state-p64 |ς| (object-address "expandArray::1::1::b") ((_ sign_extend 32) (_ bv0 32)))))))

; find_symbols
(declare-fun S49 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S48 |ς|) (S49 (enter-scope-state-p64 |ς| (object-address "expandArray::$tmp::return_value_realloc") (_ bv8 64))))))

; set_to true

; set_to true (equal)
(declare-fun S50 (state) Bool)
(assert (= S50 S49))
; __CPROVER__start → realloc

; find_symbols
(declare-fun S51 (state) Bool)
(declare-fun reallocate (state (_ BitVec 64) (_ BitVec 64)) (_ BitVec 64))
; set_to true
(assert (forall ((|ς| state)) (=> (S50 |ς|) (S51 (update-state-p64 |ς| (object-address "expandArray::$tmp::return_value_realloc") (reallocate |ς| (evaluate-p64 |ς| (object-address "expandArray::1::a")) (bvmul (_ bv4 64) ((_ sign_extend 32) (evaluate-s32 |ς| (object-address "newsize"))))))))))

; find_symbols
(declare-fun S52 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S51 |ς|) (S52 (update-state-p64 |ς| (object-address "expandArray::1::1::b") (evaluate-p64 |ς| (object-address "expandArray::$tmp::return_value_realloc")))))))

; set_to true

; set_to true (equal)
(declare-fun S53 (state) Bool)
(assert (= S53 S52))

; find_symbols
(declare-fun S54 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S53 |ς|) (S54 (update-state-s32 |ς| (bvadd (evaluate-p64 |ς| (object-address "expandArray::1::1::b")) (bvmul ((_ sign_extend 32) (bvsub (evaluate-s32 |ς| (object-address "newsize")) (_ bv1 32))) (_ bv4 64))) (evaluate-s32 |ς| (object-address "i")))))))

; find_symbols
(declare-fun S55 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S54 |ς|) (S55 (update-state-p64 |ς| (object-address "expandArray::1::a") (evaluate-p64 |ς| (object-address "expandArray::1::1::b")))))))

; find_symbols
(declare-fun S56 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S55 |ς|) (S56 (exit-scope-state-p64 |ς| (object-address "expandArray::$tmp::return_value_realloc"))))))

; find_symbols
(declare-fun S57 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S56 |ς|) (S57 (exit-scope-state-p64 |ς| (object-address "expandArray::1::1::b"))))))

; set_to true
(assert (= S58 S57))

; set_to true (equal)
(declare-fun S59 (state) Bool)
(assert (= S59 S45T))

; set_to true (equal)
(declare-fun S60 (state) Bool)
(assert (= S60 S59))

; find_symbols
(declare-fun S61 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S60 |ς|) (S61 (exit-scope-state-p64 |ς| (object-address "expandArray::1::a"))))))

; set_to true (equal)
(declare-fun S62 (state) Bool)
(assert (= S62 S61))

; set_to true (equal)
(declare-fun S63 (state) Bool)
(assert (= S63 S62))

; find_symbols
(declare-fun S64 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S63 |ς|) (S64 (exit-scope-state-p64 |ς| (object-address "expandArray::arg"))))))

; find_symbols
(declare-fun S65 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S64 |ς|) (S65 (update-state-s32 |ς| (object-address "return'") (_ bv0 32))))))

; find_symbols
(declare-fun S66 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S65 |ς|) (S66 (exit-scope-state-p64 |ς| (object-address "main::1::a"))))))

; find_symbols
(declare-fun S67 (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S66 |ς|) (S67 (exit-scope-state-p64 |ς| (object-address "main::$tmp::return_value_malloc"))))))

; set_to true (equal)
(declare-fun S68 (state) Bool)
(assert (= S68 S67))

; find_symbols
(declare-fun S69in (state) Bool)
; set_to true
(assert (forall ((|ς| state)) (=> (S27 |ς|) (S69in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S37 |ς|) (S69in |ς|))))

; set_to true
(assert (forall ((|ς| state)) (=> (S68 |ς|) (S69in |ς|))))

; set_to true (equal)
(declare-fun S69 (state) Bool)
(assert (= S69 S69in))

; set_to true (equal)
(declare-fun S70 (state) Bool)
(assert (= S70 S69))

; set_to true (equal)
(declare-fun S71 (state) Bool)
(assert (= S71 S70))

(check-sat)


; end of SMT2 file
