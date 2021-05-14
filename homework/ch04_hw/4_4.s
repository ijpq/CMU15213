	.file	"4_4.c"
	.text
	.globl	rsum
	.type	rsum, @function
rsum:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	cmpq	$0, -32(%rbp)
	jg	.L2
	movl	$0, %eax
	jmp	.L3
.L2:
	movq	-24(%rbp), %rax
	movq	(%rax), %rbx
	movq	-32(%rbp), %rax
	leaq	-1(%rax), %rdx
	movq	-24(%rbp), %rax
	addq	$8, %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	rsum
	addq	%rbx, %rax
.L3:
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	rsum, .-rsum
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
