	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_add                    ## -- Begin function add
	.p2align	4, 0x90
_add:                                   ## @add
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
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rsi
	addq	-16(%rbp), %rsi
	movq	%rsi, %rax
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
	subq	$48, %rsp
	leaq	L_.str(%rip), %rdi
	movl	$0, -4(%rbp)
	movb	$0, %al
	callq	_printf
	leaq	L_.str.1(%rip), %rdi
	leaq	-16(%rbp), %rsi
	movl	%eax, -36(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_scanf
	leaq	L_.str.1(%rip), %rdi
	leaq	-24(%rbp), %rsi
	movl	%eax, -40(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_scanf
	movq	-16(%rbp), %rdi
	movq	-24(%rbp), %rsi
	movl	%eax, -44(%rbp)         ## 4-byte Spill
	callq	_add
	leaq	L_.str.2(%rip), %rdi
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rsi
	movb	$0, %al
	callq	_printf
	xorl	%ecx, %ecx
	movl	%eax, -48(%rbp)         ## 4-byte Spill
	movl	%ecx, %eax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Enter two numbers: "

L_.str.1:                               ## @.str.1
	.asciz	"%ld"

L_.str.2:                               ## @.str.2
	.asciz	"Result is: %ld\n"


.subsections_via_symbols
