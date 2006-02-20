.section        .rodata
	.align 8
.LC48:
	.long 0x80000000,0x42400000
.text
	.align 16
	.globl dct64_1_486_a
	.type    dct64_1_486_a,@function
dct64_1_486_a:
	 pushl %ebx
	 pushl %edi
	 pushl %esi
	 movl 16(%esp),%esi
	 movl 20(%esp),%ebx
	 fldl .LC48

	 fld %st(0)
	 fadds  (%esi)
	 fstpl  (%ebx)

	 fld %st(0)
	 fadds 4(%esi)
	 fstpl 4(%ebx)

	 fld %st(0)
	 fadds 8(%esi)
	 fstpl 8(%ebx)

	 fld %st(0)
	 fadds 12(%esi)
	 fstpl 12(%ebx)

	 fld %st(0)
	 fadds 16(%esi)
	 fstpl 16(%ebx)

	 fld %st(0)
	 fadds 20(%esi)
	 fstpl 20(%ebx)

	 fld %st(0)
	 fadds 24(%esi)
	 fstpl 24(%ebx)

	 fld %st(0)
	 fadds 28(%esi)
	 fstpl 28(%ebx)

	 fld %st(0)
	 fadds 32(%esi)
	 fstpl 32(%ebx)

	 fld %st(0)
	 fadds 36(%esi)
	 fstpl 36(%ebx)

	 fld %st(0)
	 fadds 40(%esi)
	 fstpl 40(%ebx)

	 fld %st(0)
	 fadds 44(%esi)
	 fstpl 44(%ebx)

	 fld %st(0)
	 fadds 48(%esi)
	 fstpl 48(%ebx)

	 fld %st(0)
	 fadds 52(%esi)
	 fstpl 52(%ebx)

	 fld %st(0)
	 fadds 56(%esi)
	 fstpl 56(%ebx)

	 fld %st(0)
	 fadds 60(%esi)
	 fstpl 60(%ebx)

	 fld %st(0)
	 fadds 64(%esi)
	 fstpl 64(%ebx)

	 fld %st(0)
	 fadds 68(%esi)
	 fstpl 68(%ebx)

	 fld %st(0)
	 fadds 72(%esi)
	 fstpl 72(%ebx)

	 fld %st(0)
	 fadds 76(%esi)
	 fstpl 76(%ebx)

	 fld %st(0)
	 fadds 80(%esi)
	 fstpl 80(%ebx)

	 fld %st(0)
	 fadds 84(%esi)
	 fstpl 84(%ebx)

	 fld %st(0)
	 fadds 88(%esi)
	 fstpl 88(%ebx)

	 fld %st(0)
	 fadds 92(%esi)
	 fstpl 92(%ebx)

	 fld %st(0)
	 fadds 96(%esi)
	 fstpl 96(%ebx)

	 fld %st(0)
	 fadds 100(%esi)
	 fstpl 100(%ebx)

	 fld %st(0)
	 fadds 104(%esi)
	 fstpl 104(%ebx)

	 fld %st(0)
	 fadds 108(%esi)
	 fstpl 108(%ebx)

	 fld %st(0)
	 fadds 112(%esi)
	 fstpl 112(%ebx)

	 fld %st(0)
	 fadds 116(%esi)
	 fstpl 116(%ebx)

	 fld %st(0)
	 fadds 120(%esi)
	 fstpl 120(%ebx)

	 fadds 124(%esi)
	 fstpl 124(%ebx)

	 mov  (%ebx),%eax
	 mov 124(%ebx),%edx
	 add %edx, (%ebx)
	 sub %edx,%eax
	 mov $16403,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,124(%ebx)

	 mov 4(%ebx),%eax
	 mov 120(%ebx),%edx
	 add %edx,4(%ebx)
	 sub %edx,%eax
	 mov $16563,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,120(%ebx)

	 mov 8(%ebx),%eax
	 mov 116(%ebx),%edx
	 add %edx,8(%ebx)
	 sub %edx,%eax
	 mov $16890,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,116(%ebx)

	 mov 12(%ebx),%eax
	 mov 112(%ebx),%edx
	 add %edx,12(%ebx)
	 sub %edx,%eax
	 mov $17401,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,112(%ebx)

	 mov 16(%ebx),%eax
	 mov 108(%ebx),%edx
	 add %edx,16(%ebx)
	 sub %edx,%eax
	 mov $18124,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,108(%ebx)

	 mov 20(%ebx),%eax
	 mov 104(%ebx),%edx
	 add %edx,20(%ebx)
	 sub %edx,%eax
	 mov $19101,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,104(%ebx)

	 mov 24(%ebx),%eax
	 mov 100(%ebx),%edx
	 add %edx,24(%ebx)
	 sub %edx,%eax
	 mov $20398,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,100(%ebx)

	 mov 28(%ebx),%eax
	 mov 96(%ebx),%edx
	 add %edx,28(%ebx)
	 sub %edx,%eax
	 mov $22112,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,96(%ebx)

	 mov 32(%ebx),%eax
	 mov 92(%ebx),%edx
	 add %edx,32(%ebx)
	 sub %edx,%eax
	 mov $24396,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,92(%ebx)

	 mov 36(%ebx),%eax
	 mov 88(%ebx),%edx
	 add %edx,36(%ebx)
	 sub %edx,%eax
	 mov $27503,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,88(%ebx)

	 mov 40(%ebx),%eax
	 mov 84(%ebx),%edx
	 add %edx,40(%ebx)
	 sub %edx,%eax
	 mov $31869,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,84(%ebx)

	 mov 44(%ebx),%eax
	 mov 80(%ebx),%edx
	 add %edx,44(%ebx)
	 sub %edx,%eax
	 mov $38320,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,80(%ebx)

	 mov 48(%ebx),%eax
	 mov 76(%ebx),%edx
	 add %edx,48(%ebx)
	 sub %edx,%eax
	 mov $48633,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,76(%ebx)

	 mov 52(%ebx),%eax
	 mov 72(%ebx),%edx
	 add %edx,52(%ebx)
	 sub %edx,%eax
	 mov $67429,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,72(%ebx)

	 mov 56(%ebx),%eax
	 mov 68(%ebx),%edx
	 add %edx,56(%ebx)
	 sub %edx,%eax
	 mov $111660,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,68(%ebx)

	 mov 60(%ebx),%eax
	 mov 64(%ebx),%edx
	 add %edx,60(%ebx)
	 sub %edx,%eax
	 mov $333906,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,64(%ebx)

	 mov  (%ebx),%eax
	 mov 60(%ebx),%edx
	 add %edx, (%ebx)
	 sub %edx,%eax
	 mov $16463,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,60(%ebx)

	 mov 4(%ebx),%eax
	 mov 56(%ebx),%edx
	 add %edx,4(%ebx)
	 sub %edx,%eax
	 mov $17121,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,56(%ebx)

	 mov 8(%ebx),%eax
	 mov 52(%ebx),%edx
	 add %edx,8(%ebx)
	 sub %edx,%eax
	 mov $18577,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,52(%ebx)

	 mov 12(%ebx),%eax
	 mov 48(%ebx),%edx
	 add %edx,12(%ebx)
	 sub %edx,%eax
	 mov $21195,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,48(%ebx)

	 mov 16(%ebx),%eax
	 mov 44(%ebx),%edx
	 add %edx,16(%ebx)
	 sub %edx,%eax
	 mov $25826,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,44(%ebx)

	 mov 20(%ebx),%eax
	 mov 40(%ebx),%edx
	 add %edx,20(%ebx)
	 sub %edx,%eax
	 mov $34756,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,40(%ebx)

	 mov 24(%ebx),%eax
	 mov 36(%ebx),%edx
	 add %edx,24(%ebx)
	 sub %edx,%eax
	 mov $56441,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,36(%ebx)

	 mov 28(%ebx),%eax
	 mov 32(%ebx),%edx
	 add %edx,28(%ebx)
	 sub %edx,%eax
	 mov $167154,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,32(%ebx)

	 mov 124(%ebx),%eax
	 mov 64(%ebx),%edx
	 add %eax,64(%ebx)
	 sub %edx,%eax
	 mov $16463,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,124(%ebx)

	 mov 120(%ebx),%eax
	 mov 68(%ebx),%edx
	 add %eax,68(%ebx)
	 sub %edx,%eax
	 mov $17121,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,120(%ebx)

	 mov 116(%ebx),%eax
	 mov 72(%ebx),%edx
	 add %eax,72(%ebx)
	 sub %edx,%eax
	 mov $18577,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,116(%ebx)

	 mov 112(%ebx),%eax
	 mov 76(%ebx),%edx
	 add %eax,76(%ebx)
	 sub %edx,%eax
	 mov $21195,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,112(%ebx)

	 mov 108(%ebx),%eax
	 mov 80(%ebx),%edx
	 add %eax,80(%ebx)
	 sub %edx,%eax
	 mov $25826,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,108(%ebx)

	 mov 104(%ebx),%eax
	 mov 84(%ebx),%edx
	 add %eax,84(%ebx)
	 sub %edx,%eax
	 mov $34756,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,104(%ebx)

	 mov 100(%ebx),%eax
	 mov 88(%ebx),%edx
	 add %eax,88(%ebx)
	 sub %edx,%eax
	 mov $56441,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,100(%ebx)

	 mov 96(%ebx),%eax
	 mov 92(%ebx),%edx
	 add %eax,92(%ebx)
	 sub %edx,%eax
	 mov $167154,%edx
	 imul %edx
	 shrdl $15,%edx,%eax
	 movl %eax,96(%ebx)

	 mov $16704,%edi
	 mov $19704,%esi

	 mov  (%ebx),%eax
	 mov 28(%ebx),%edx
	 add %edx, (%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,28(%ebx)

	 mov 4(%ebx),%eax
	 mov 24(%ebx),%edx
	 add %edx,4(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,24(%ebx)

	 mov 60(%ebx),%eax
	 mov 32(%ebx),%edx
	 add %eax,32(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,60(%ebx)

	 mov 56(%ebx),%eax
	 mov 36(%ebx),%edx
	 add %eax,36(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,56(%ebx)

	 mov 64(%ebx),%eax
	 mov 92(%ebx),%edx
	 add %edx,64(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,92(%ebx)

	 mov 68(%ebx),%eax
	 mov 88(%ebx),%edx
	 add %edx,68(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,88(%ebx)

	 mov 124(%ebx),%eax
	 mov 96(%ebx),%edx
	 add %eax,96(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,124(%ebx)

	 mov 120(%ebx),%eax
	 mov 100(%ebx),%edx
	 add %eax,100(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,120(%ebx)

	 mov $29490,%edi
	 mov $83981,%esi

	 mov 8(%ebx),%eax
	 mov 20(%ebx),%edx
	 add %edx,8(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,20(%ebx)

	 mov 12(%ebx),%eax
	 mov 16(%ebx),%edx
	 add %edx,12(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,16(%ebx)

	 mov 52(%ebx),%eax
	 mov 40(%ebx),%edx
	 add %eax,40(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,52(%ebx)

	 mov 48(%ebx),%eax
	 mov 44(%ebx),%edx
	 add %eax,44(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,48(%ebx)

	 mov 72(%ebx),%eax
	 mov 84(%ebx),%edx
	 add %edx,72(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,84(%ebx)

	 mov 76(%ebx),%eax
	 mov 80(%ebx),%edx
	 add %edx,76(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,80(%ebx)

	 mov 116(%ebx),%eax
	 mov 104(%ebx),%edx
	 add %eax,104(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,116(%ebx)

	 mov 112(%ebx),%eax
	 mov 108(%ebx),%edx
	 add %eax,108(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,112(%ebx)

	 mov $17733,%edi
	 mov $42813,%esi

	 mov  (%ebx),%eax
	 mov 12(%ebx),%edx
	 add %edx, (%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,12(%ebx)

	 mov 4(%ebx),%eax
	 mov 8(%ebx),%edx
	 add %edx,4(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,8(%ebx)

	 mov 28(%ebx),%eax
	 mov 16(%ebx),%edx
	 add %eax,16(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,28(%ebx)

	 mov 24(%ebx),%eax
	 mov 20(%ebx),%edx
	 add %eax,20(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,24(%ebx)

	 mov 32(%ebx),%eax
	 mov 44(%ebx),%edx
	 add %edx,32(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,44(%ebx)

	 mov 36(%ebx),%eax
	 mov 40(%ebx),%edx
	 add %edx,36(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,40(%ebx)

	 mov 60(%ebx),%eax
	 mov 48(%ebx),%edx
	 add %eax,48(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,60(%ebx)

	 mov 56(%ebx),%eax
	 mov 52(%ebx),%edx
	 add %eax,52(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,56(%ebx)

	 mov 64(%ebx),%eax
	 mov 76(%ebx),%edx
	 add %edx,64(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,76(%ebx)

	 mov 68(%ebx),%eax
	 mov 72(%ebx),%edx
	 add %edx,68(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,72(%ebx)

	 mov 92(%ebx),%eax
	 mov 80(%ebx),%edx
	 add %eax,80(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,92(%ebx)

	 mov 88(%ebx),%eax
	 mov 84(%ebx),%edx
	 add %eax,84(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,88(%ebx)

	 mov 96(%ebx),%eax
	 mov 108(%ebx),%edx
	 add %edx,96(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,108(%ebx)

	 mov 100(%ebx),%eax
	 mov 104(%ebx),%edx
	 add %edx,100(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,104(%ebx)

	 mov 124(%ebx),%eax
	 mov 112(%ebx),%edx
	 add %eax,112(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,124(%ebx)

	 mov 120(%ebx),%eax
	 mov 116(%ebx),%edx
	 add %eax,116(%ebx)
	 sub %edx,%eax
	 imul %esi
	 shrdl $15,%edx,%eax
	 movl %eax,120(%ebx)

	 mov $23170,%edi

	 mov  (%ebx),%eax
	 mov 4(%ebx),%edx
	 add %edx, (%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,4(%ebx)

	 mov 12(%ebx),%eax
	 mov 8(%ebx),%edx
	 add %eax,8(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,12(%ebx)

	 mov 16(%ebx),%eax
	 mov 20(%ebx),%edx
	 add %edx,16(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,20(%ebx)

	 mov 28(%ebx),%eax
	 mov 24(%ebx),%edx
	 add %eax,24(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,28(%ebx)

	 mov 32(%ebx),%eax
	 mov 36(%ebx),%edx
	 add %edx,32(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,36(%ebx)

	 mov 44(%ebx),%eax
	 mov 40(%ebx),%edx
	 add %eax,40(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,44(%ebx)

	 mov 48(%ebx),%eax
	 mov 52(%ebx),%edx
	 add %edx,48(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,52(%ebx)

	 mov 60(%ebx),%eax
	 mov 56(%ebx),%edx
	 add %eax,56(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,60(%ebx)

	 mov 64(%ebx),%eax
	 mov 68(%ebx),%edx
	 add %edx,64(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,68(%ebx)

	 mov 76(%ebx),%eax
	 mov 72(%ebx),%edx
	 add %eax,72(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,76(%ebx)

	 mov 80(%ebx),%eax
	 mov 84(%ebx),%edx
	 add %edx,80(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,84(%ebx)

	 mov 92(%ebx),%eax
	 mov 88(%ebx),%edx
	 add %eax,88(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,92(%ebx)

	 mov 96(%ebx),%eax
	 mov 100(%ebx),%edx
	 add %edx,96(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,100(%ebx)

	 mov 108(%ebx),%eax
	 mov 104(%ebx),%edx
	 add %eax,104(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,108(%ebx)

	 mov 112(%ebx),%eax
	 mov 116(%ebx),%edx
	 add %edx,112(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,116(%ebx)

	 mov 124(%ebx),%eax
	 mov 120(%ebx),%edx
	 add %eax,120(%ebx)
	 sub %edx,%eax
	 imul %edi
	 shrdl $15,%edx,%eax
	 movl %eax,124(%ebx)

	 popl %esi
	 popl %edi
	 popl %ebx
	 ret

