.globl	_wordpointer

.globl	_bitindex

.globl	_getbits
_getbits:
	cmpl	$0,4(%esp)
	jne	.L1
	xorl	%eax,%eax
	ret

.L1:
	movl	_wordpointer,%ecx
	movzbl	(%ecx),%eax
	shll	$16,%eax
	movb	1(%ecx),%ah
	movb	2(%ecx),%al
	movl	_bitindex,%ecx
	shll	$8,%eax
	shll	%cl,%eax
	movl 	4(%esp),%ecx
	addl	%ecx,_bitindex
	negl	%ecx
	addl	$32,%ecx
	shrl	%cl,%eax
	movl	_bitindex,%ecx
	sarl	$3,%ecx
	addl	%ecx,_wordpointer
	andl	$7,_bitindex
	ret



.globl	_getbits_fast
_getbits_fast:
	movl	_wordpointer,%ecx
	movzbl	1(%ecx),%eax
	movb	(%ecx),%ah
	movl	_bitindex,%ecx
	shlw	%cl,%ax
	movl 	4(%esp),%ecx
	addl	%ecx,_bitindex
	negl	%ecx
	addl	$16,%ecx
	shrl	%cl,%eax
	movl	_bitindex,%ecx
	sarl	$3,%ecx
	addl	%ecx,_wordpointer
	andl	$7,_bitindex
	ret



.globl	_get1bit
_get1bit:
	movl	_wordpointer,%ecx
	movzbl	(%ecx),%eax
	movl	_bitindex,%ecx
	incl	%ecx
	rolb	%cl,%al
	andb	$1,%al
	movl	%ecx,_bitindex
	andl	$7,_bitindex
	sarl	$3,%ecx
	addl	%ecx,_wordpointer
	ret
