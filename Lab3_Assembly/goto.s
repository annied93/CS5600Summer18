	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_checkEvenOrNot         ## -- Begin function checkEvenOrNot
	.p2align	4, 0x90
_checkEvenOrNot:                        ## @checkEvenOrNot
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
	subq	$16, %rsp
	movl	$2, %eax
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %edi
	movl	%eax, -8(%rbp)          ## 4-byte Spill
	movl	%edi, %eax
	cltd
	movl	-8(%rbp), %edi          ## 4-byte Reload
	idivl	%edi
	cmpl	$0, %edx
	jne	LBB0_2
## BB#1:
	jmp	LBB0_3
LBB0_2:
	jmp	LBB0_4
LBB0_3:
	leaq	L_.str(%rip), %rdi
	movl	-4(%rbp), %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	jmp	LBB0_5
LBB0_4:
	leaq	L_.str.1(%rip), %rdi
	movl	-4(%rbp), %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -16(%rbp)         ## 4-byte Spill
LBB0_5:
	addq	$16, %rsp
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
	callq	_checkEvenOrNot
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
