	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.intel_syntax noprefix
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception0
## BB#0:
	push	rbp
Ltmp15:
	.cfi_def_cfa_offset 16
Ltmp16:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp17:
	.cfi_def_cfa_register rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 88
Ltmp18:
	.cfi_offset rbx, -56
Ltmp19:
	.cfi_offset r12, -48
Ltmp20:
	.cfi_offset r13, -40
Ltmp21:
	.cfi_offset r14, -32
Ltmp22:
	.cfi_offset r15, -24
	mov	r13, qword ptr [rip + __ZTVN17basic_cpp_runtime5PListIiEE@GOTPCREL]
	add	r13, 16
	mov	qword ptr [rbp - 80], r13
	mov	dword ptr [rbp - 72], 0
	mov	qword ptr [rbp - 48], 0
	mov	qword ptr [rbp - 56], 0
	mov	qword ptr [rbp - 64], 0
Ltmp0:
	lea	rdi, [rbp - 120]
	lea	rsi, [rbp - 80]
	call	__ZN17basic_cpp_runtime11CastFunctorINS_5PListINS_4PAnyEEENS1_IiEEE4implERKS4_
Ltmp1:
## BB#1:
Ltmp3:
	mov	edi, 40
	call	__Znwm
	mov	r15, rax
Ltmp4:
## BB#2:
Ltmp6:
	lea	rsi, [rbp - 120]
	mov	rdi, r15
	call	__ZN17basic_cpp_runtime5PListINS_4PAnyEEC2ERKS2_
Ltmp7:
## BB#3:
Ltmp9:
	mov	edi, 32
	call	__Znwm
	mov	r12, rax
Ltmp10:
## BB#4:
	mov	qword ptr [r12 + 16], 0
	mov	qword ptr [r12 + 8], 0
	mov	rax, qword ptr [rip + __ZTVNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE@GOTPCREL]
	add	rax, 16
	mov	qword ptr [r12], rax
	mov	qword ptr [r12 + 24], r15
	mov	edi, 8
	call	___cxa_allocate_exception
	mov	rbx, rax
	mov	rdi, rbx
	call	__ZNSt8bad_castC1Ev
Ltmp12:
	mov	rsi, qword ptr [rip + __ZTISt8bad_cast@GOTPCREL]
	mov	rdx, qword ptr [rip + __ZNSt8bad_castD1Ev@GOTPCREL]
	mov	rdi, rbx
	call	___cxa_throw
Ltmp13:
## BB#7:
LBB0_10:
Ltmp14:
	mov	r14, rax
	mov	rdi, r12
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
	jmp	LBB0_11
LBB0_5:
Ltmp11:
	mov	r14, rax
	mov	rax, qword ptr [r15]
	mov	rdi, r15
	call	qword ptr [rax + 8]
	jmp	LBB0_11
LBB0_6:
Ltmp8:
	mov	r14, rax
	mov	rdi, r15
	call	__ZdlPv
	jmp	LBB0_11
LBB0_9:
Ltmp5:
	mov	r14, rax
LBB0_11:
	mov	rax, qword ptr [rip + __ZTVN17basic_cpp_runtime5PListINS_4PAnyEEE@GOTPCREL]
	add	rax, 16
	mov	qword ptr [rbp - 120], rax
	mov	rbx, qword ptr [rbp - 104]
	test	rbx, rbx
	je	LBB0_17
## BB#12:
	mov	rax, qword ptr [rbp - 96]
	cmp	rax, rbx
	je	LBB0_16
	.p2align	4, 0x90
LBB0_13:                                ## =>This Inner Loop Header: Depth=1
	lea	rcx, [rax - 40]
	mov	qword ptr [rbp - 96], rcx
	mov	rdi, qword ptr [rax - 8]
	test	rdi, rdi
	je	LBB0_14
## BB#22:                               ##   in Loop: Header=BB0_13 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
	mov	rcx, qword ptr [rbp - 96]
LBB0_14:                                ##   in Loop: Header=BB0_13 Depth=1
	cmp	rcx, rbx
	mov	rax, rcx
	jne	LBB0_13
## BB#15:
	mov	rbx, qword ptr [rbp - 104]
LBB0_16:
	mov	rdi, rbx
	call	__ZdlPv
	jmp	LBB0_17
LBB0_8:
Ltmp2:
	mov	r14, rax
LBB0_17:
	mov	qword ptr [rbp - 80], r13
	mov	rdi, qword ptr [rbp - 64]
	test	rdi, rdi
	je	LBB0_21
## BB#18:
	mov	rax, qword ptr [rbp - 56]
	cmp	rax, rdi
	je	LBB0_20
## BB#19:
	lea	rcx, [rax - 4]
	sub	rcx, rdi
	not	rcx
	and	rcx, -4
	add	rcx, rax
	mov	qword ptr [rbp - 56], rcx
LBB0_20:
	call	__ZdlPv
LBB0_21:
	mov	rdi, r14
	call	__Unwind_Resume
Lfunc_end0:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table0:
Lexception0:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	155                     ## @TType Encoding = indirect pcrel sdata4
	.byte	93                      ## @TType base offset
	.byte	3                       ## Call site Encoding = udata4
	.byte	91                      ## Call site table length
Lset0 = Ltmp0-Lfunc_begin0              ## >> Call Site 1 <<
	.long	Lset0
Lset1 = Ltmp1-Ltmp0                     ##   Call between Ltmp0 and Ltmp1
	.long	Lset1
Lset2 = Ltmp2-Lfunc_begin0              ##     jumps to Ltmp2
	.long	Lset2
	.byte	0                       ##   On action: cleanup
Lset3 = Ltmp3-Lfunc_begin0              ## >> Call Site 2 <<
	.long	Lset3
Lset4 = Ltmp4-Ltmp3                     ##   Call between Ltmp3 and Ltmp4
	.long	Lset4
Lset5 = Ltmp5-Lfunc_begin0              ##     jumps to Ltmp5
	.long	Lset5
	.byte	0                       ##   On action: cleanup
Lset6 = Ltmp6-Lfunc_begin0              ## >> Call Site 3 <<
	.long	Lset6
Lset7 = Ltmp7-Ltmp6                     ##   Call between Ltmp6 and Ltmp7
	.long	Lset7
Lset8 = Ltmp8-Lfunc_begin0              ##     jumps to Ltmp8
	.long	Lset8
	.byte	0                       ##   On action: cleanup
Lset9 = Ltmp9-Lfunc_begin0              ## >> Call Site 4 <<
	.long	Lset9
Lset10 = Ltmp10-Ltmp9                   ##   Call between Ltmp9 and Ltmp10
	.long	Lset10
Lset11 = Ltmp11-Lfunc_begin0            ##     jumps to Ltmp11
	.long	Lset11
	.byte	0                       ##   On action: cleanup
Lset12 = Ltmp10-Lfunc_begin0            ## >> Call Site 5 <<
	.long	Lset12
Lset13 = Ltmp12-Ltmp10                  ##   Call between Ltmp10 and Ltmp12
	.long	Lset13
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lset14 = Ltmp12-Lfunc_begin0            ## >> Call Site 6 <<
	.long	Lset14
Lset15 = Ltmp13-Ltmp12                  ##   Call between Ltmp12 and Ltmp13
	.long	Lset15
Lset16 = Ltmp14-Lfunc_begin0            ##     jumps to Ltmp14
	.long	Lset16
	.byte	0                       ##   On action: cleanup
Lset17 = Ltmp13-Lfunc_begin0            ## >> Call Site 7 <<
	.long	Lset17
Lset18 = Lfunc_end0-Ltmp13              ##   Call between Ltmp13 and Lfunc_end0
	.long	Lset18
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.p2align	2

	.section	__TEXT,__text,regular,pure_instructions
	.globl	__ZN17basic_cpp_runtime5PListINS_4PAnyEED1Ev
	.weak_def_can_be_hidden	__ZN17basic_cpp_runtime5PListINS_4PAnyEED1Ev
	.p2align	4, 0x90
__ZN17basic_cpp_runtime5PListINS_4PAnyEED1Ev: ## @_ZN17basic_cpp_runtime5PListINS_4PAnyEED1Ev
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp23:
	.cfi_def_cfa_offset 16
Ltmp24:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp25:
	.cfi_def_cfa_register rbp
	push	r14
	push	rbx
Ltmp26:
	.cfi_offset rbx, -32
Ltmp27:
	.cfi_offset r14, -24
	mov	rbx, rdi
	mov	rax, qword ptr [rip + __ZTVN17basic_cpp_runtime5PListINS_4PAnyEEE@GOTPCREL]
	add	rax, 16
	mov	qword ptr [rbx], rax
	mov	r14, qword ptr [rbx + 16]
	test	r14, r14
	je	LBB1_6
## BB#1:
	mov	rax, qword ptr [rbx + 24]
	cmp	rax, r14
	je	LBB1_5
	.p2align	4, 0x90
LBB1_2:                                 ## =>This Inner Loop Header: Depth=1
	lea	rcx, [rax - 40]
	mov	qword ptr [rbx + 24], rcx
	mov	rdi, qword ptr [rax - 8]
	test	rdi, rdi
	je	LBB1_3
## BB#7:                                ##   in Loop: Header=BB1_2 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
	mov	rcx, qword ptr [rbx + 24]
LBB1_3:                                 ##   in Loop: Header=BB1_2 Depth=1
	cmp	rcx, r14
	mov	rax, rcx
	jne	LBB1_2
## BB#4:
	mov	r14, qword ptr [rbx + 16]
LBB1_5:
	mov	rdi, r14
	pop	rbx
	pop	r14
	pop	rbp
	jmp	__ZdlPv                 ## TAILCALL
LBB1_6:
	pop	rbx
	pop	r14
	pop	rbp
	ret
	.cfi_endproc

	.globl	__ZN17basic_cpp_runtime5PListIiED1Ev
	.weak_def_can_be_hidden	__ZN17basic_cpp_runtime5PListIiED1Ev
	.p2align	4, 0x90
__ZN17basic_cpp_runtime5PListIiED1Ev:   ## @_ZN17basic_cpp_runtime5PListIiED1Ev
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp28:
	.cfi_def_cfa_offset 16
Ltmp29:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp30:
	.cfi_def_cfa_register rbp
	mov	rax, qword ptr [rip + __ZTVN17basic_cpp_runtime5PListIiEE@GOTPCREL]
	add	rax, 16
	mov	qword ptr [rdi], rax
	mov	rax, qword ptr [rdi + 16]
	test	rax, rax
	je	LBB2_4
## BB#1:
	mov	rcx, qword ptr [rdi + 24]
	cmp	rcx, rax
	je	LBB2_3
## BB#2:
	lea	rdx, [rcx - 4]
	sub	rdx, rax
	not	rdx
	and	rdx, -4
	add	rdx, rcx
	mov	qword ptr [rdi + 24], rdx
LBB2_3:
	mov	rdi, rax
	pop	rbp
	jmp	__ZdlPv                 ## TAILCALL
LBB2_4:
	pop	rbp
	ret
	.cfi_endproc

	.globl	__ZN17basic_cpp_runtime5PListIiED0Ev
	.weak_def_can_be_hidden	__ZN17basic_cpp_runtime5PListIiED0Ev
	.p2align	4, 0x90
__ZN17basic_cpp_runtime5PListIiED0Ev:   ## @_ZN17basic_cpp_runtime5PListIiED0Ev
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp31:
	.cfi_def_cfa_offset 16
Ltmp32:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp33:
	.cfi_def_cfa_register rbp
	push	rbx
	push	rax
Ltmp34:
	.cfi_offset rbx, -24
	mov	rbx, rdi
	mov	rax, qword ptr [rip + __ZTVN17basic_cpp_runtime5PListIiEE@GOTPCREL]
	add	rax, 16
	mov	qword ptr [rbx], rax
	mov	rdi, qword ptr [rbx + 16]
	test	rdi, rdi
	je	LBB3_4
## BB#1:
	mov	rax, qword ptr [rbx + 24]
	cmp	rax, rdi
	je	LBB3_3
## BB#2:
	lea	rcx, [rax - 4]
	sub	rcx, rdi
	not	rcx
	and	rcx, -4
	add	rcx, rax
	mov	qword ptr [rbx + 24], rcx
LBB3_3:
	call	__ZdlPv
LBB3_4:
	mov	rdi, rbx
	add	rsp, 8
	pop	rbx
	pop	rbp
	jmp	__ZdlPv                 ## TAILCALL
	.cfi_endproc

	.globl	__ZN17basic_cpp_runtime5PListINS_4PAnyEED0Ev
	.weak_def_can_be_hidden	__ZN17basic_cpp_runtime5PListINS_4PAnyEED0Ev
	.p2align	4, 0x90
__ZN17basic_cpp_runtime5PListINS_4PAnyEED0Ev: ## @_ZN17basic_cpp_runtime5PListINS_4PAnyEED0Ev
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp35:
	.cfi_def_cfa_offset 16
Ltmp36:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp37:
	.cfi_def_cfa_register rbp
	push	r14
	push	rbx
Ltmp38:
	.cfi_offset rbx, -32
Ltmp39:
	.cfi_offset r14, -24
	mov	rbx, rdi
	mov	rax, qword ptr [rip + __ZTVN17basic_cpp_runtime5PListINS_4PAnyEEE@GOTPCREL]
	add	rax, 16
	mov	qword ptr [rbx], rax
	mov	r14, qword ptr [rbx + 16]
	test	r14, r14
	je	LBB4_6
## BB#1:
	mov	rax, qword ptr [rbx + 24]
	cmp	rax, r14
	je	LBB4_5
	.p2align	4, 0x90
LBB4_2:                                 ## =>This Inner Loop Header: Depth=1
	lea	rcx, [rax - 40]
	mov	qword ptr [rbx + 24], rcx
	mov	rdi, qword ptr [rax - 8]
	test	rdi, rdi
	je	LBB4_3
## BB#7:                                ##   in Loop: Header=BB4_2 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
	mov	rcx, qword ptr [rbx + 24]
LBB4_3:                                 ##   in Loop: Header=BB4_2 Depth=1
	cmp	rcx, r14
	mov	rax, rcx
	jne	LBB4_2
## BB#4:
	mov	r14, qword ptr [rbx + 16]
LBB4_5:
	mov	rdi, r14
	call	__ZdlPv
LBB4_6:
	mov	rdi, rbx
	pop	rbx
	pop	r14
	pop	rbp
	jmp	__ZdlPv                 ## TAILCALL
	.cfi_endproc

	.globl	__ZN17basic_cpp_runtime11CastFunctorINS_5PListINS_4PAnyEEENS1_IiEEE4implERKS4_
	.weak_def_can_be_hidden	__ZN17basic_cpp_runtime11CastFunctorINS_5PListINS_4PAnyEEENS1_IiEEE4implERKS4_
	.p2align	4, 0x90
__ZN17basic_cpp_runtime11CastFunctorINS_5PListINS_4PAnyEEENS1_IiEEE4implERKS4_: ## @_ZN17basic_cpp_runtime11CastFunctorINS_5PListINS_4PAnyEEENS1_IiEEE4implERKS4_
Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception1
## BB#0:
	push	rbp
Ltmp52:
	.cfi_def_cfa_offset 16
Ltmp53:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp54:
	.cfi_def_cfa_register rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 56
Ltmp55:
	.cfi_offset rbx, -56
Ltmp56:
	.cfi_offset r12, -48
Ltmp57:
	.cfi_offset r13, -40
Ltmp58:
	.cfi_offset r14, -32
Ltmp59:
	.cfi_offset r15, -24
	mov	r14, rsi
	mov	r12, rdi
	mov	rax, qword ptr [rip + __ZTVN17basic_cpp_runtime5PListINS_4PAnyEEE@GOTPCREL]
	add	rax, 16
	mov	qword ptr [rbp - 48], rax ## 8-byte Spill
	mov	qword ptr [r12], rax
	mov	dword ptr [r12 + 8], 0
	mov	qword ptr [r12 + 32], 0
	mov	qword ptr [r12 + 24], 0
	mov	qword ptr [r12 + 16], 0
	mov	eax, dword ptr [r14 + 8]
	test	eax, eax
	jle	LBB5_8
## BB#1:
	lea	rcx, [r12 + 16]
	mov	qword ptr [rbp - 56], rcx ## 8-byte Spill
	lea	r13, [rbp - 80]
	xor	ebx, ebx
	mov	r15, qword ptr [rip + __ZTVNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE@GOTPCREL]
	add	r15, 16
	.p2align	4, 0x90
LBB5_2:                                 ## =>This Inner Loop Header: Depth=1
	cdqe
	cmp	rax, rbx
	jle	LBB5_9
## BB#3:                                ##   in Loop: Header=BB5_2 Depth=1
	mov	rax, qword ptr [r14 + 16]
	mov	eax, dword ptr [rax + 4*rbx]
	mov	rcx, qword ptr [rip + __ZTIi@GOTPCREL]
	mov	qword ptr [rbp - 96], rcx
	mov	dword ptr [rbp - 88], eax
	mov	byte ptr [rbp - 84], 0
	mov	qword ptr [r13 + 8], 0
	mov	qword ptr [r13], 0
Ltmp46:
	mov	edi, 32
	call	__Znwm
Ltmp47:
## BB#4:                                ##   in Loop: Header=BB5_2 Depth=1
	mov	qword ptr [rax + 16], 0
	mov	qword ptr [rax + 8], 0
	mov	qword ptr [rax], r15
	mov	qword ptr [rax + 24], 0
	mov	qword ptr [rbp - 64], rax
Ltmp49:
	mov	rdi, r12
	lea	rsi, [rbp - 96]
	call	__ZN17basic_cpp_runtime5PListINS_4PAnyEE3addERKS1_
Ltmp50:
## BB#5:                                ##   in Loop: Header=BB5_2 Depth=1
	mov	rdi, qword ptr [rbp - 64]
	test	rdi, rdi
	je	LBB5_7
## BB#6:                                ##   in Loop: Header=BB5_2 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
LBB5_7:                                 ##   in Loop: Header=BB5_2 Depth=1
	inc	rbx
	movsxd	rax, dword ptr [r14 + 8]
	cmp	rbx, rax
	jl	LBB5_2
LBB5_8:
	mov	rax, r12
	add	rsp, 56
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	ret
LBB5_9:
	mov	edi, 16
	call	___cxa_allocate_exception
	mov	r15, rax
Ltmp40:
	lea	rsi, [rip + L_.str]
	mov	rdi, r15
	call	__ZNSt11logic_errorC2EPKc
Ltmp41:
## BB#10:
	mov	rax, qword ptr [rip + __ZTVSt12out_of_range@GOTPCREL]
	add	rax, 16
	mov	qword ptr [r15], rax
Ltmp43:
	mov	rsi, qword ptr [rip + __ZTISt12out_of_range@GOTPCREL]
	mov	rdx, qword ptr [rip + __ZNSt12out_of_rangeD1Ev@GOTPCREL]
	mov	rdi, r15
	call	___cxa_throw
Ltmp44:
## BB#11:
LBB5_12:
Ltmp45:
	jmp	LBB5_17
LBB5_13:
Ltmp42:
	mov	r14, rax
	mov	rdi, r15
	call	___cxa_free_exception
	jmp	LBB5_18
LBB5_14:
Ltmp51:
	mov	r14, rax
	mov	rdi, qword ptr [rbp - 64]
	test	rdi, rdi
	je	LBB5_18
## BB#15:
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
	jmp	LBB5_18
LBB5_16:
Ltmp48:
LBB5_17:
	mov	r14, rax
LBB5_18:
	mov	rax, qword ptr [rbp - 48] ## 8-byte Reload
	mov	qword ptr [r12], rax
	mov	rbx, qword ptr [r12 + 16]
	test	rbx, rbx
	je	LBB5_25
## BB#19:
	mov	rax, qword ptr [r12 + 24]
	cmp	rax, rbx
	je	LBB5_24
	.p2align	4, 0x90
LBB5_20:                                ## =>This Inner Loop Header: Depth=1
	lea	rcx, [rax - 40]
	mov	qword ptr [r12 + 24], rcx
	mov	rdi, qword ptr [rax - 8]
	test	rdi, rdi
	je	LBB5_22
## BB#21:                               ##   in Loop: Header=BB5_20 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
	mov	rcx, qword ptr [r12 + 24]
LBB5_22:                                ##   in Loop: Header=BB5_20 Depth=1
	cmp	rcx, rbx
	mov	rax, rcx
	jne	LBB5_20
## BB#23:
	mov	rax, qword ptr [rbp - 56] ## 8-byte Reload
	mov	rbx, qword ptr [rax]
LBB5_24:
	mov	rdi, rbx
	call	__ZdlPv
LBB5_25:
	mov	rdi, r14
	call	__Unwind_Resume
Lfunc_end1:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table5:
Lexception1:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	155                     ## @TType Encoding = indirect pcrel sdata4
	.asciz	"\320"                  ## @TType base offset
	.byte	3                       ## Call site Encoding = udata4
	.byte	78                      ## Call site table length
Lset19 = Ltmp46-Lfunc_begin1            ## >> Call Site 1 <<
	.long	Lset19
Lset20 = Ltmp47-Ltmp46                  ##   Call between Ltmp46 and Ltmp47
	.long	Lset20
Lset21 = Ltmp48-Lfunc_begin1            ##     jumps to Ltmp48
	.long	Lset21
	.byte	0                       ##   On action: cleanup
Lset22 = Ltmp49-Lfunc_begin1            ## >> Call Site 2 <<
	.long	Lset22
Lset23 = Ltmp50-Ltmp49                  ##   Call between Ltmp49 and Ltmp50
	.long	Lset23
Lset24 = Ltmp51-Lfunc_begin1            ##     jumps to Ltmp51
	.long	Lset24
	.byte	0                       ##   On action: cleanup
Lset25 = Ltmp50-Lfunc_begin1            ## >> Call Site 3 <<
	.long	Lset25
Lset26 = Ltmp40-Ltmp50                  ##   Call between Ltmp50 and Ltmp40
	.long	Lset26
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lset27 = Ltmp40-Lfunc_begin1            ## >> Call Site 4 <<
	.long	Lset27
Lset28 = Ltmp41-Ltmp40                  ##   Call between Ltmp40 and Ltmp41
	.long	Lset28
Lset29 = Ltmp42-Lfunc_begin1            ##     jumps to Ltmp42
	.long	Lset29
	.byte	0                       ##   On action: cleanup
Lset30 = Ltmp43-Lfunc_begin1            ## >> Call Site 5 <<
	.long	Lset30
Lset31 = Ltmp44-Ltmp43                  ##   Call between Ltmp43 and Ltmp44
	.long	Lset31
Lset32 = Ltmp45-Lfunc_begin1            ##     jumps to Ltmp45
	.long	Lset32
	.byte	0                       ##   On action: cleanup
Lset33 = Ltmp44-Lfunc_begin1            ## >> Call Site 6 <<
	.long	Lset33
Lset34 = Lfunc_end1-Ltmp44              ##   Call between Ltmp44 and Lfunc_end1
	.long	Lset34
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.p2align	2

	.section	__TEXT,__text,regular,pure_instructions
	.globl	__ZN17basic_cpp_runtime5PListINS_4PAnyEE3addERKS1_
	.weak_def_can_be_hidden	__ZN17basic_cpp_runtime5PListINS_4PAnyEE3addERKS1_
	.p2align	4, 0x90
__ZN17basic_cpp_runtime5PListINS_4PAnyEE3addERKS1_: ## @_ZN17basic_cpp_runtime5PListINS_4PAnyEE3addERKS1_
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp60:
	.cfi_def_cfa_offset 16
Ltmp61:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp62:
	.cfi_def_cfa_register rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	push	rax
Ltmp63:
	.cfi_offset rbx, -56
Ltmp64:
	.cfi_offset r12, -48
Ltmp65:
	.cfi_offset r13, -40
Ltmp66:
	.cfi_offset r14, -32
Ltmp67:
	.cfi_offset r15, -24
	mov	r14, rsi
	mov	r12, rdi
	lea	r15, [r12 + 16]
	movsxd	r13, dword ptr [r12 + 8]
	lea	rsi, [r13 + 1]
	mov	rcx, qword ptr [r12 + 16]
	mov	rax, qword ptr [r12 + 24]
	mov	rdi, rax
	sub	rdi, rcx
	sar	rdi, 3
	movabs	rdx, -3689348814741910323
	imul	rdx, rdi
	cmp	rsi, rdx
	jb	LBB6_7
## BB#1:
	lea	rdi, [r13 + 2]
	mov	rsi, rdi
	sub	rsi, rdx
	jbe	LBB6_3
## BB#2:
	mov	rdi, r15
	call	__ZNSt3__16vectorIN17basic_cpp_runtime4PAnyENS_9allocatorIS2_EEE8__appendEm
	jmp	LBB6_7
LBB6_3:
	jae	LBB6_7
## BB#4:
	lea	rdx, [rdi + 4*rdi]
	lea	rbx, [rcx + 8*rdx]
	cmp	rax, rbx
	je	LBB6_7
	.p2align	4, 0x90
LBB6_5:                                 ## =>This Inner Loop Header: Depth=1
	lea	rcx, [rax - 40]
	mov	qword ptr [r12 + 24], rcx
	mov	rdi, qword ptr [rax - 8]
	test	rdi, rdi
	je	LBB6_6
## BB#12:                               ##   in Loop: Header=BB6_5 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
	mov	rcx, qword ptr [r12 + 24]
LBB6_6:                                 ##   in Loop: Header=BB6_5 Depth=1
	cmp	rcx, rbx
	mov	rax, rcx
	jne	LBB6_5
LBB6_7:
	mov	r15, qword ptr [r15]
	lea	rbx, [r13 + 4*r13]
	mov	rax, qword ptr [r14 + 16]
	mov	qword ptr [r15 + 8*rbx + 16], rax
	mov	rax, qword ptr [r14]
	mov	rcx, qword ptr [r14 + 8]
	mov	qword ptr [r15 + 8*rbx + 8], rcx
	mov	qword ptr [r15 + 8*rbx], rax
	lea	rax, [r15 + 8*rbx + 24]
	mov	r13, qword ptr [r14 + 24]
	mov	r14, qword ptr [r14 + 32]
	test	r14, r14
	je	LBB6_9
## BB#8:
	mov	rdi, r14
	mov	qword ptr [rbp - 48], rax ## 8-byte Spill
	call	__ZNSt3__119__shared_weak_count12__add_sharedEv
	mov	rax, qword ptr [rbp - 48] ## 8-byte Reload
LBB6_9:
	mov	qword ptr [rax], r13
	mov	rdi, qword ptr [r15 + 8*rbx + 32]
	mov	qword ptr [r15 + 8*rbx + 32], r14
	test	rdi, rdi
	je	LBB6_11
## BB#10:
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
LBB6_11:
	inc	dword ptr [r12 + 8]
	add	rsp, 8
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	ret
	.cfi_endproc

	.globl	__ZNSt3__16vectorIN17basic_cpp_runtime4PAnyENS_9allocatorIS2_EEE8__appendEm
	.weak_def_can_be_hidden	__ZNSt3__16vectorIN17basic_cpp_runtime4PAnyENS_9allocatorIS2_EEE8__appendEm
	.p2align	4, 0x90
__ZNSt3__16vectorIN17basic_cpp_runtime4PAnyENS_9allocatorIS2_EEE8__appendEm: ## @_ZNSt3__16vectorIN17basic_cpp_runtime4PAnyENS_9allocatorIS2_EEE8__appendEm
Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception2
## BB#0:
	push	rbp
Ltmp71:
	.cfi_def_cfa_offset 16
Ltmp72:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp73:
	.cfi_def_cfa_register rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 24
Ltmp74:
	.cfi_offset rbx, -56
Ltmp75:
	.cfi_offset r12, -48
Ltmp76:
	.cfi_offset r13, -40
Ltmp77:
	.cfi_offset r14, -32
Ltmp78:
	.cfi_offset r15, -24
	mov	r12, rsi
	mov	r14, rdi
	mov	rbx, qword ptr [r14 + 8]
	mov	rax, qword ptr [r14 + 16]
	mov	rcx, rax
	sub	rcx, rbx
	sar	rcx, 3
	movabs	r15, -3689348814741910323
	imul	rcx, r15
	cmp	rcx, r12
	jae	LBB7_1
## BB#4:
	movabs	r13, 461168601842738790
	mov	rcx, qword ptr [r14]
	sub	rbx, rcx
	sar	rbx, 3
	imul	rbx, r15
	add	rbx, r12
	cmp	rbx, r13
	jbe	LBB7_6
## BB#5:
	mov	rdi, r14
	call	__ZNKSt3__120__vector_base_commonILb1EE20__throw_length_errorEv
	mov	rcx, qword ptr [r14]
	mov	rax, qword ptr [r14 + 16]
LBB7_6:
	sub	rax, rcx
	sar	rax, 3
	imul	rax, r15
	movabs	rdx, 230584300921369395
	cmp	rax, rdx
	jae	LBB7_7
## BB#8:
	add	rax, rax
	cmp	rax, rbx
	cmovb	rax, rbx
	mov	rbx, qword ptr [r14 + 8]
	sub	rbx, rcx
	sar	rbx, 3
	imul	rbx, r15
	xor	ecx, ecx
	test	rax, rax
	mov	r13, rax
	mov	eax, 0
	jne	LBB7_9
	jmp	LBB7_10
LBB7_1:
	mov	r15, qword ptr [rip + __ZTIi@GOTPCREL]
	mov	r13, qword ptr [rip + __ZTVNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE@GOTPCREL]
	add	r13, 16
	.p2align	4, 0x90
LBB7_2:                                 ## =>This Inner Loop Header: Depth=1
	mov	qword ptr [rbx], r15
	mov	dword ptr [rbx + 8], 0
	mov	byte ptr [rbx + 12], 0
	mov	qword ptr [rbx + 24], 0
	mov	qword ptr [rbx + 16], 0
	mov	edi, 32
	call	__Znwm
	mov	qword ptr [rax + 16], 0
	mov	qword ptr [rax + 8], 0
	mov	qword ptr [rax], r13
	mov	qword ptr [rax + 24], 0
	mov	qword ptr [rbx + 32], rax
	mov	rbx, qword ptr [r14 + 8]
	add	rbx, 40
	mov	qword ptr [r14 + 8], rbx
	dec	r12
	jne	LBB7_2
LBB7_3:
	add	rsp, 24
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	ret
LBB7_7:
	mov	rbx, qword ptr [r14 + 8]
	sub	rbx, rcx
	sar	rbx, 3
	imul	rbx, r15
LBB7_9:
	lea	rax, [8*r13]
	lea	rdi, [rax + 4*rax]
	call	__Znwm
	mov	rcx, r13
LBB7_10:
	lea	rdx, [rbx + 4*rbx]
	lea	r13, [rax + 8*rdx]
	lea	rcx, [rcx + 4*rcx]
	mov	qword ptr [rbp - 56], rax ## 8-byte Spill
	lea	rax, [rax + 8*rcx]
	mov	qword ptr [rbp - 48], rax ## 8-byte Spill
	mov	r15, qword ptr [rip + __ZTVNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE@GOTPCREL]
	add	r15, 16
	mov	rbx, r13
	.p2align	4, 0x90
LBB7_11:                                ## =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [rip + __ZTIi@GOTPCREL]
	mov	qword ptr [rbx], rax
	mov	dword ptr [rbx + 8], 0
	mov	byte ptr [rbx + 12], 0
	mov	qword ptr [rbx + 24], 0
	mov	qword ptr [rbx + 16], 0
Ltmp68:
	mov	edi, 32
	call	__Znwm
Ltmp69:
## BB#12:                               ##   in Loop: Header=BB7_11 Depth=1
	mov	qword ptr [rax + 16], 0
	mov	qword ptr [rax + 8], 0
	mov	qword ptr [rax], r15
	mov	qword ptr [rax + 24], 0
	mov	qword ptr [rbx + 32], rax
	add	rbx, 40
	dec	r12
	jne	LBB7_11
## BB#13:
	mov	r12, qword ptr [r14]
	mov	rax, qword ptr [r14 + 8]
	cmp	rax, r12
	je	LBB7_14
	.p2align	4, 0x90
LBB7_15:                                ## =>This Inner Loop Header: Depth=1
	mov	rcx, qword ptr [rax - 24]
	mov	qword ptr [r13 - 24], rcx
	mov	rcx, qword ptr [rax - 40]
	mov	rdx, qword ptr [rax - 32]
	mov	qword ptr [r13 - 32], rdx
	mov	qword ptr [r13 - 40], rcx
	movups	xmm0, xmmword ptr [rax - 16]
	movups	xmmword ptr [r13 - 16], xmm0
	mov	qword ptr [rax - 8], 0
	mov	qword ptr [rax - 16], 0
	lea	rax, [rax - 40]
	add	r13, -40
	cmp	r12, rax
	jne	LBB7_15
## BB#16:
	mov	r15, qword ptr [r14]
	mov	r12, qword ptr [r14 + 8]
	jmp	LBB7_17
LBB7_14:
	mov	r15, r12
LBB7_17:
	mov	qword ptr [r14], r13
	mov	qword ptr [r14 + 8], rbx
	mov	rax, qword ptr [rbp - 48] ## 8-byte Reload
	mov	qword ptr [r14 + 16], rax
	cmp	r12, r15
	je	LBB7_20
	.p2align	4, 0x90
LBB7_18:                                ## =>This Inner Loop Header: Depth=1
	mov	rdi, qword ptr [r12 - 8]
	add	r12, -40
	test	rdi, rdi
	je	LBB7_19
## BB#28:                               ##   in Loop: Header=BB7_18 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
LBB7_19:                                ##   in Loop: Header=BB7_18 Depth=1
	cmp	r15, r12
	jne	LBB7_18
LBB7_20:
	test	r15, r15
	je	LBB7_3
## BB#21:
	mov	rdi, r15
	add	rsp, 24
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	jmp	__ZdlPv                 ## TAILCALL
LBB7_22:
Ltmp70:
	mov	r14, rax
	cmp	rbx, r13
	je	LBB7_25
	.p2align	4, 0x90
LBB7_23:                                ## =>This Inner Loop Header: Depth=1
	mov	rdi, qword ptr [rbx - 8]
	add	rbx, -40
	test	rdi, rdi
	je	LBB7_24
## BB#29:                               ##   in Loop: Header=BB7_23 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
LBB7_24:                                ##   in Loop: Header=BB7_23 Depth=1
	cmp	r13, rbx
	jne	LBB7_23
LBB7_25:
	mov	rdi, qword ptr [rbp - 56] ## 8-byte Reload
	test	rdi, rdi
	je	LBB7_27
## BB#26:
	call	__ZdlPv
LBB7_27:
	mov	rdi, r14
	call	__Unwind_Resume
Lfunc_end2:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table7:
Lexception2:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	155                     ## @TType Encoding = indirect pcrel sdata4
	.byte	41                      ## @TType base offset
	.byte	3                       ## Call site Encoding = udata4
	.byte	39                      ## Call site table length
Lset35 = Lfunc_begin2-Lfunc_begin2      ## >> Call Site 1 <<
	.long	Lset35
Lset36 = Ltmp68-Lfunc_begin2            ##   Call between Lfunc_begin2 and Ltmp68
	.long	Lset36
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
Lset37 = Ltmp68-Lfunc_begin2            ## >> Call Site 2 <<
	.long	Lset37
Lset38 = Ltmp69-Ltmp68                  ##   Call between Ltmp68 and Ltmp69
	.long	Lset38
Lset39 = Ltmp70-Lfunc_begin2            ##     jumps to Ltmp70
	.long	Lset39
	.byte	0                       ##   On action: cleanup
Lset40 = Ltmp69-Lfunc_begin2            ## >> Call Site 3 <<
	.long	Lset40
Lset41 = Lfunc_end2-Ltmp69              ##   Call between Ltmp69 and Lfunc_end2
	.long	Lset41
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.p2align	2

	.section	__TEXT,__text,regular,pure_instructions
	.globl	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED1Ev
	.weak_def_can_be_hidden	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED1Ev
	.p2align	4, 0x90
__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED1Ev: ## @_ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED1Ev
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp79:
	.cfi_def_cfa_offset 16
Ltmp80:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp81:
	.cfi_def_cfa_register rbp
	pop	rbp
	jmp	__ZNSt3__119__shared_weak_countD2Ev ## TAILCALL
	.cfi_endproc

	.globl	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED0Ev
	.weak_def_can_be_hidden	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED0Ev
	.p2align	4, 0x90
__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED0Ev: ## @_ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED0Ev
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp82:
	.cfi_def_cfa_offset 16
Ltmp83:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp84:
	.cfi_def_cfa_register rbp
	push	rbx
	push	rax
Ltmp85:
	.cfi_offset rbx, -24
	mov	rbx, rdi
	call	__ZNSt3__119__shared_weak_countD2Ev
	mov	rdi, rbx
	add	rsp, 8
	pop	rbx
	pop	rbp
	jmp	__ZdlPv                 ## TAILCALL
	.cfi_endproc

	.globl	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE16__on_zero_sharedEv
	.weak_def_can_be_hidden	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE16__on_zero_sharedEv
	.p2align	4, 0x90
__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE16__on_zero_sharedEv: ## @_ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE16__on_zero_sharedEv
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp86:
	.cfi_def_cfa_offset 16
Ltmp87:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp88:
	.cfi_def_cfa_register rbp
	mov	rdi, qword ptr [rdi + 24]
	test	rdi, rdi
	je	LBB10_1
## BB#2:
	mov	rax, qword ptr [rdi]
	pop	rbp
	jmp	qword ptr [rax + 8]     ## TAILCALL
LBB10_1:
	pop	rbp
	ret
	.cfi_endproc

	.globl	__ZNKSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE13__get_deleterERKSt9type_info
	.weak_def_can_be_hidden	__ZNKSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE13__get_deleterERKSt9type_info
	.p2align	4, 0x90
__ZNKSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE13__get_deleterERKSt9type_info: ## @_ZNKSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE13__get_deleterERKSt9type_info
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp89:
	.cfi_def_cfa_offset 16
Ltmp90:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp91:
	.cfi_def_cfa_register rbp
	mov	rcx, qword ptr [rsi + 8]
	add	rdi, 24
	xor	eax, eax
	cmp	rcx, qword ptr [rip + __ZTSNSt3__114default_deleteIN17basic_cpp_runtime8PTypePtrEEE@GOTPCREL]
	cmove	rax, rdi
	pop	rbp
	ret
	.cfi_endproc

	.globl	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE21__on_zero_shared_weakEv
	.weak_def_can_be_hidden	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE21__on_zero_shared_weakEv
	.p2align	4, 0x90
__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE21__on_zero_shared_weakEv: ## @_ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE21__on_zero_shared_weakEv
	.cfi_startproc
## BB#0:
	push	rbp
Ltmp92:
	.cfi_def_cfa_offset 16
Ltmp93:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp94:
	.cfi_def_cfa_register rbp
	pop	rbp
	jmp	__ZdlPv                 ## TAILCALL
	.cfi_endproc

	.globl	__ZN17basic_cpp_runtime5PListINS_4PAnyEEC2ERKS2_
	.weak_def_can_be_hidden	__ZN17basic_cpp_runtime5PListINS_4PAnyEEC2ERKS2_
	.p2align	4, 0x90
__ZN17basic_cpp_runtime5PListINS_4PAnyEEC2ERKS2_: ## @_ZN17basic_cpp_runtime5PListINS_4PAnyEEC2ERKS2_
Lfunc_begin3:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception3
## BB#0:
	push	rbp
Ltmp98:
	.cfi_def_cfa_offset 16
Ltmp99:
	.cfi_offset rbp, -16
	mov	rbp, rsp
Ltmp100:
	.cfi_def_cfa_register rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 56
Ltmp101:
	.cfi_offset rbx, -56
Ltmp102:
	.cfi_offset r12, -48
Ltmp103:
	.cfi_offset r13, -40
Ltmp104:
	.cfi_offset r14, -32
Ltmp105:
	.cfi_offset r15, -24
	mov	r14, rsi
	mov	r13, rdi
	mov	rax, qword ptr [rip + __ZTVN17basic_cpp_runtime5PListINS_4PAnyEEE@GOTPCREL]
	add	rax, 16
	mov	qword ptr [r13], rax
	mov	eax, dword ptr [r14 + 8]
	mov	dword ptr [r13 + 8], eax
	mov	qword ptr [r13 + 32], 0
	mov	qword ptr [r13 + 24], 0
	mov	qword ptr [r13 + 16], 0
	cmp	dword ptr [r14 + 8], 0
	jle	LBB13_25
## BB#1:
	lea	rax, [r13 + 16]
	mov	qword ptr [rbp - 48], rax ## 8-byte Spill
	xor	ecx, ecx
	xor	eax, eax
	xor	r15d, r15d
	mov	qword ptr [rbp - 80], r14 ## 8-byte Spill
	jmp	LBB13_2
	.p2align	4, 0x90
LBB13_17:                               ##   in Loop: Header=BB13_2 Depth=1
	mov	rcx, qword ptr [r13 + 16]
	mov	rax, qword ptr [r13 + 24]
	mov	r15, rdx
LBB13_2:                                ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB13_7 Depth 2
	lea	r12, [r15 + 1]
	mov	rdx, rax
	sub	rdx, rcx
	sar	rdx, 3
	movabs	rsi, -3689348814741910323
	imul	rdx, rsi
	cmp	r12, rdx
	jb	LBB13_9
## BB#3:                                ##   in Loop: Header=BB13_2 Depth=1
	lea	rdi, [r15 + 2]
	mov	rsi, rdi
	sub	rsi, rdx
	jbe	LBB13_5
## BB#4:                                ##   in Loop: Header=BB13_2 Depth=1
Ltmp95:
	mov	rdi, qword ptr [rbp - 48] ## 8-byte Reload
	call	__ZNSt3__16vectorIN17basic_cpp_runtime4PAnyENS_9allocatorIS2_EEE8__appendEm
Ltmp96:
	jmp	LBB13_9
	.p2align	4, 0x90
LBB13_5:                                ##   in Loop: Header=BB13_2 Depth=1
	jae	LBB13_9
## BB#6:                                ##   in Loop: Header=BB13_2 Depth=1
	lea	rdx, [rdi + 4*rdi]
	lea	rbx, [rcx + 8*rdx]
	cmp	rax, rbx
	je	LBB13_9
	.p2align	4, 0x90
LBB13_7:                                ##   Parent Loop BB13_2 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	lea	rcx, [rax - 40]
	mov	qword ptr [r13 + 24], rcx
	mov	rdi, qword ptr [rax - 8]
	test	rdi, rdi
	je	LBB13_8
## BB#26:                               ##   in Loop: Header=BB13_7 Depth=2
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
	mov	rcx, qword ptr [r13 + 24]
LBB13_8:                                ##   in Loop: Header=BB13_7 Depth=2
	cmp	rcx, rbx
	mov	rax, rcx
	jne	LBB13_7
	.p2align	4, 0x90
LBB13_9:                                ##   in Loop: Header=BB13_2 Depth=1
	mov	qword ptr [rbp - 88], r12 ## 8-byte Spill
	mov	rax, qword ptr [r14 + 16]
	lea	rbx, [r15 + 4*r15]
	mov	rcx, qword ptr [rax + 8*rbx + 16]
	mov	qword ptr [rbp - 56], rcx
	mov	rcx, qword ptr [rax + 8*rbx + 8]
	mov	qword ptr [rbp - 64], rcx
	mov	rcx, qword ptr [rax + 8*rbx]
	mov	qword ptr [rbp - 72], rcx
	mov	r15, qword ptr [rax + 8*rbx + 24]
	mov	r12, qword ptr [rax + 8*rbx + 32]
	test	r12, r12
	je	LBB13_10
## BB#11:                               ##   in Loop: Header=BB13_2 Depth=1
	mov	rdi, r12
	call	__ZNSt3__119__shared_weak_count12__add_sharedEv
	mov	rax, qword ptr [rbp - 48] ## 8-byte Reload
	mov	r14, qword ptr [rax]
	mov	rax, qword ptr [rbp - 56]
	mov	qword ptr [r14 + 8*rbx + 16], rax
	mov	rax, qword ptr [rbp - 72]
	mov	rcx, qword ptr [rbp - 64]
	mov	qword ptr [r14 + 8*rbx + 8], rcx
	mov	qword ptr [r14 + 8*rbx], rax
	mov	rdi, r12
	call	__ZNSt3__119__shared_weak_count12__add_sharedEv
	jmp	LBB13_12
	.p2align	4, 0x90
LBB13_10:                               ##   in Loop: Header=BB13_2 Depth=1
	mov	rax, qword ptr [rbp - 48] ## 8-byte Reload
	mov	r14, qword ptr [rax]
	mov	rax, qword ptr [rbp - 56]
	mov	qword ptr [r14 + 8*rbx + 16], rax
	mov	rax, qword ptr [rbp - 72]
	mov	rcx, qword ptr [rbp - 64]
	mov	qword ptr [r14 + 8*rbx + 8], rcx
	mov	qword ptr [r14 + 8*rbx], rax
LBB13_12:                               ##   in Loop: Header=BB13_2 Depth=1
	mov	qword ptr [r14 + 8*rbx + 24], r15
	mov	rdi, qword ptr [r14 + 8*rbx + 32]
	mov	qword ptr [r14 + 8*rbx + 32], r12
	test	rdi, rdi
	je	LBB13_14
## BB#13:                               ##   in Loop: Header=BB13_2 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
LBB13_14:                               ##   in Loop: Header=BB13_2 Depth=1
	test	r12, r12
	mov	r14, qword ptr [rbp - 80] ## 8-byte Reload
	je	LBB13_16
## BB#15:                               ##   in Loop: Header=BB13_2 Depth=1
	mov	rdi, r12
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
LBB13_16:                               ##   in Loop: Header=BB13_2 Depth=1
	movsxd	rax, dword ptr [r14 + 8]
	mov	rdx, qword ptr [rbp - 88] ## 8-byte Reload
	cmp	rdx, rax
	jl	LBB13_17
LBB13_25:
	add	rsp, 56
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	ret
LBB13_18:
Ltmp97:
	mov	r15, rax
	mov	rax, qword ptr [rbp - 48] ## 8-byte Reload
	mov	rbx, qword ptr [rax]
	test	rbx, rbx
	je	LBB13_24
## BB#19:
	mov	rax, qword ptr [r13 + 24]
	cmp	rax, rbx
	je	LBB13_23
	.p2align	4, 0x90
LBB13_20:                               ## =>This Inner Loop Header: Depth=1
	lea	rcx, [rax - 40]
	mov	qword ptr [r13 + 24], rcx
	mov	rdi, qword ptr [rax - 8]
	test	rdi, rdi
	je	LBB13_21
## BB#27:                               ##   in Loop: Header=BB13_20 Depth=1
	call	__ZNSt3__119__shared_weak_count16__release_sharedEv
	mov	rcx, qword ptr [r13 + 24]
LBB13_21:                               ##   in Loop: Header=BB13_20 Depth=1
	cmp	rcx, rbx
	mov	rax, rcx
	jne	LBB13_20
## BB#22:
	mov	rax, qword ptr [rbp - 48] ## 8-byte Reload
	mov	rbx, qword ptr [rax]
LBB13_23:
	mov	rdi, rbx
	call	__ZdlPv
LBB13_24:
	mov	rdi, r15
	call	__Unwind_Resume
Lfunc_end3:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table13:
Lexception3:
	.byte	255                     ## @LPStart Encoding = omit
	.byte	155                     ## @TType Encoding = indirect pcrel sdata4
	.asciz	"\234"                  ## @TType base offset
	.byte	3                       ## Call site Encoding = udata4
	.byte	26                      ## Call site table length
Lset42 = Ltmp95-Lfunc_begin3            ## >> Call Site 1 <<
	.long	Lset42
Lset43 = Ltmp96-Ltmp95                  ##   Call between Ltmp95 and Ltmp96
	.long	Lset43
Lset44 = Ltmp97-Lfunc_begin3            ##     jumps to Ltmp97
	.long	Lset44
	.byte	0                       ##   On action: cleanup
Lset45 = Ltmp96-Lfunc_begin3            ## >> Call Site 2 <<
	.long	Lset45
Lset46 = Lfunc_end3-Ltmp96              ##   Call between Ltmp96 and Lfunc_end3
	.long	Lset46
	.long	0                       ##     has no landing pad
	.byte	0                       ##   On action: cleanup
	.p2align	2

	.section	__DATA,__data
	.globl	__ZTVN17basic_cpp_runtime5PListIiEE ## @_ZTVN17basic_cpp_runtime5PListIiEE
	.weak_def_can_be_hidden	__ZTVN17basic_cpp_runtime5PListIiEE
	.p2align	3
__ZTVN17basic_cpp_runtime5PListIiEE:
	.quad	0
	.quad	__ZTIN17basic_cpp_runtime5PListIiEE
	.quad	__ZN17basic_cpp_runtime5PListIiED1Ev
	.quad	__ZN17basic_cpp_runtime5PListIiED0Ev

	.section	__TEXT,__const
	.globl	__ZTSN17basic_cpp_runtime5PListIiEE ## @_ZTSN17basic_cpp_runtime5PListIiEE
	.weak_definition	__ZTSN17basic_cpp_runtime5PListIiEE
	.p2align	4
__ZTSN17basic_cpp_runtime5PListIiEE:
	.asciz	"N17basic_cpp_runtime5PListIiEE"

	.globl	__ZTSN17basic_cpp_runtime8PTypePtrE ## @_ZTSN17basic_cpp_runtime8PTypePtrE
	.weak_definition	__ZTSN17basic_cpp_runtime8PTypePtrE
	.p2align	4
__ZTSN17basic_cpp_runtime8PTypePtrE:
	.asciz	"N17basic_cpp_runtime8PTypePtrE"

	.section	__DATA,__data
	.globl	__ZTIN17basic_cpp_runtime8PTypePtrE ## @_ZTIN17basic_cpp_runtime8PTypePtrE
	.weak_definition	__ZTIN17basic_cpp_runtime8PTypePtrE
	.p2align	3
__ZTIN17basic_cpp_runtime8PTypePtrE:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTSN17basic_cpp_runtime8PTypePtrE

	.globl	__ZTIN17basic_cpp_runtime5PListIiEE ## @_ZTIN17basic_cpp_runtime5PListIiEE
	.weak_definition	__ZTIN17basic_cpp_runtime5PListIiEE
	.p2align	4
__ZTIN17basic_cpp_runtime5PListIiEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSN17basic_cpp_runtime5PListIiEE
	.quad	__ZTIN17basic_cpp_runtime8PTypePtrE

	.globl	__ZTVN17basic_cpp_runtime5PListINS_4PAnyEEE ## @_ZTVN17basic_cpp_runtime5PListINS_4PAnyEEE
	.weak_def_can_be_hidden	__ZTVN17basic_cpp_runtime5PListINS_4PAnyEEE
	.p2align	3
__ZTVN17basic_cpp_runtime5PListINS_4PAnyEEE:
	.quad	0
	.quad	__ZTIN17basic_cpp_runtime5PListINS_4PAnyEEE
	.quad	__ZN17basic_cpp_runtime5PListINS_4PAnyEED1Ev
	.quad	__ZN17basic_cpp_runtime5PListINS_4PAnyEED0Ev

	.section	__TEXT,__const
	.globl	__ZTSN17basic_cpp_runtime5PListINS_4PAnyEEE ## @_ZTSN17basic_cpp_runtime5PListINS_4PAnyEEE
	.weak_definition	__ZTSN17basic_cpp_runtime5PListINS_4PAnyEEE
	.p2align	4
__ZTSN17basic_cpp_runtime5PListINS_4PAnyEEE:
	.asciz	"N17basic_cpp_runtime5PListINS_4PAnyEEE"

	.section	__DATA,__data
	.globl	__ZTIN17basic_cpp_runtime5PListINS_4PAnyEEE ## @_ZTIN17basic_cpp_runtime5PListINS_4PAnyEEE
	.weak_definition	__ZTIN17basic_cpp_runtime5PListINS_4PAnyEEE
	.p2align	4
__ZTIN17basic_cpp_runtime5PListINS_4PAnyEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSN17basic_cpp_runtime5PListINS_4PAnyEEE
	.quad	__ZTIN17basic_cpp_runtime8PTypePtrE

	.globl	__ZTVNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE ## @_ZTVNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE
	.weak_def_can_be_hidden	__ZTVNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE
	.p2align	3
__ZTVNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE:
	.quad	0
	.quad	__ZTINSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE
	.quad	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED1Ev
	.quad	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEED0Ev
	.quad	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE16__on_zero_sharedEv
	.quad	__ZNKSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE13__get_deleterERKSt9type_info
	.quad	__ZNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEE21__on_zero_shared_weakEv

	.section	__TEXT,__const
	.globl	__ZTSNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE ## @_ZTSNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE
	.weak_definition	__ZTSNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE
	.p2align	4
__ZTSNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE:
	.asciz	"NSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE"

	.section	__DATA,__data
	.globl	__ZTINSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE ## @_ZTINSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE
	.weak_definition	__ZTINSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE
	.p2align	4
__ZTINSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSNSt3__120__shared_ptr_pointerIPN17basic_cpp_runtime8PTypePtrENS_14default_deleteIS2_EENS_9allocatorIS2_EEEE
	.quad	__ZTINSt3__119__shared_weak_countE

	.section	__TEXT,__const
	.globl	__ZTSNSt3__114default_deleteIN17basic_cpp_runtime8PTypePtrEEE ## @_ZTSNSt3__114default_deleteIN17basic_cpp_runtime8PTypePtrEEE
	.weak_definition	__ZTSNSt3__114default_deleteIN17basic_cpp_runtime8PTypePtrEEE
	.p2align	4
__ZTSNSt3__114default_deleteIN17basic_cpp_runtime8PTypePtrEEE:
	.asciz	"NSt3__114default_deleteIN17basic_cpp_runtime8PTypePtrEEE"

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"PList::get"


.subsections_via_symbols
