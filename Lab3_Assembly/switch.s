	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_getString              ## -- Begin function getString
	.p2align	4, 0x90
_getString:                             ## @getString
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi0:
	.cfi_def_cfa_offset 16
Lcfi1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi2:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %edi
	movl	%edi, %eax
	shrl	$31, %eax
	movl	%edi, %ecx
	addl	%eax, %ecx
	andl	$-2, %ecx
	subl	%ecx, %edi
	movl	%edi, -8(%rbp)
	movl	-8(%rbp), %eax
	testl	%eax, %eax
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	je	LBB0_1
	jmp	LBB0_5
LBB0_5:
	movl	-12(%rbp), %eax         ## 4-byte Reload
	subl	$1, %eax
	movl	%eax, -16(%rbp)         ## 4-byte Spill
	je	LBB0_2
	jmp	LBB0_3
LBB0_1:
	leaq	L_.str(%rip), %rdi
	movl	-4(%rbp), %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -20(%rbp)         ## 4-byte Spill
	jmp	LBB0_4
LBB0_2:
	leaq	L_.str.1(%rip), %rdi
	movl	-4(%rbp), %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -24(%rbp)         ## 4-byte Spill
	jmp	LBB0_4
LBB0_3:
	jmp	LBB0_4
LBB0_4:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi3:
	.cfi_def_cfa_offset 16
Lcfi4:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi5:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	leaq	L_.str.2(%rip), %rdi
	movl	$0, -4(%rbp)
	movb	$0, %al
	callq	_printf
	leaq	L_.str.3(%rip), %rdi
	leaq	-8(%rbp), %rsi
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_scanf
	movl	-8(%rbp), %edi
	movl	%eax, -16(%rbp)         ## 4-byte Spill
	callq	_getString
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%d is even\n"

L_.str.1:                               ## @.str.1
	.asciz	"%d is odd\n"

L_.str.2:                               ## @.str.2
	.asciz	"Enter a number: "

L_.str.3:                               ## @.str.3
	.asciz	"%d"


.subsections_via_symbols
