	.file	"insertionsort.c"
	.text
	.globl	sort
	.type	sort, @function
sort:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$1, -4(%rbp)
	jmp	.L2
.L6:
	movl	-4(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, -12(%rbp)
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -8(%rbp)
	jmp	.L3
.L5:
	movl	-8(%rbp), %eax
	cltq
	addq	$1, %rax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rcx
	movq	-24(%rbp), %rax
	addq	%rcx, %rax
	movl	(%rax), %eax
	movl	%eax, (%rdx)
	subl	$1, -8(%rbp)
.L3:
	cmpl	$0, -8(%rbp)
	js	.L4
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	(%rax), %eax
	cmpl	-12(%rbp), %eax
	jg	.L5
.L4:
	movl	-8(%rbp), %eax
	cltq
	addq	$1, %rax
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rax, %rdx
	movl	-12(%rbp), %eax
	movl	%eax, (%rdx)
	addl	$1, -4(%rbp)
.L2:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L6
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	sort, .-sort
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	$116, -48(%rbp)
	movl	$67, -44(%rbp)
	movl	$69, -40(%rbp)
	movl	$102, -36(%rbp)
	movl	$101, -32(%rbp)
	movl	$102, -28(%rbp)
	movl	$111, -24(%rbp)
	movl	$114, -20(%rbp)
	movl	$65, -16(%rbp)
	leaq	-48(%rbp), %rax
	movl	$9, %esi
	movq	%rax, %rdi
	call	sort
	movl	$0, -4(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L8
.L9:
	leaq	-48(%rbp), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	$4, %edx
	movq	%rax, %rsi
	movl	$1, %edi
	call	write
	addl	$1, -4(%rbp)
.L8:
	cmpl	$8, -4(%rbp)
	jle	.L9
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-16)"
	.section	.note.GNU-stack,"",@progbits
