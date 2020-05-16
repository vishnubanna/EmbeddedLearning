.cpu cortex-m0
.thumb
.syntax unified
.fpu softvfp

//============================================================================
// Q1: hello
//============================================================================
.global hello
hello:
	push {lr}
	ldr r0,=text1
	bl printf
	pop {pc}

//============================================================================
// Q2: add2
//============================================================================
.global add2
add2:
	push {r4-r7, lr}
	movs r0, r2// r2= a
	movs r4, r1// r4 = b
	movs r3, r1// r1 =b
	adds r3, r2// r3 = b + a
	ldr r0,=form1
	movs r1, r2
	movs r2, r4
	bl printf
	pop {r4-r7, pc}

//============================================================================
// Q3: add3
//============================================================================
.global add3
add3:
	push {r4-r7, lr}
	movs r3, r0 // a
	adds r3, r1 // a + b
	adds r3, r2 // a + b + c

	//movs r4, r3 // sum
	push {r3}
	movs r3, r2 // c
	movs r2, r1 // b
	movs r1, r0 // a
	ldr r0,=form2 // format

	bl printf
	pop {r3}
	pop {r4-r7, pc}
//============================================================================
// Q4: rotate6
//============================================================================
.global rotate6
rotate6:
	// idk if this works
	push {r4-r7, lr}
	//r0 = a
	//r1 = b
	//r2 = c
	//r3 = d
	//pop{r4} = e
	//pop{r5} = f
	//pop {r4}
	//pop {r5}
	ldr r5, [sp, #24]
	ldr r4, [sp, #20]
	if1:
	cmp r0, #0
	beq endif1
	then1:
	movs r6, r5
	movs r5, r4
	movs r4, r3
	movs r3, r2
	movs r2, r1
	movs r1, r0
	movs r0, r6
	push {r4-r5}
	bl rotate6
	add sp, #8
	pop {r4-r7, pc}
	endif1:
	subs r5, r4
	subs r5, r3
	subs r5, r2
	subs r5, r1
	subs r5, r0
	movs r0, r5
	pop {r4-r7, pc}
//============================================================================
// Q5: low_pattern
//============================================================================
.type compare, %function  // You knew you needed this line.  Of course you did!
compare:
        ldr  r0,[r0]
        ldr  r1,[r1]
        subs r0,r1
        bx lr

.global low_pattern
low_pattern:
	push {r4-r7, lr}
	movs r4, r0
	ldr r0,=intarr1

	for1:
	movs r1, #0 //x
	movs r5, #200
	movs r6, #255
	ldr r7,=0xff
	fcmp1:
	cmp r1,r5
	bge endfor1
	fexec1:
	lsls r2, r1, #2
	movs r3, r1
	adds r3, #1
	muls r3, r6
	ands r3, r7
	str r3, [r0, r2]
	finc1:
	adds r1, #1
	b fcmp1
	endfor1:

	movs r1, r5
	ldr r2,=4
	ldr r3,=compare

	bl qsort
	lsls r5, r4, #2
	ldr r0,=intarr1
	ldr r0,[r0, r5]
	pop {r4-r7, pc}
//============================================================================
// Q6: get_name
//============================================================================
.global get_name
get_name:
	push {r4-r7, lr}

	ldr r0,=text1

	bl printf

	ldr r0,=buffer1
	bl gets

	movs r1, r0
	ldr r0,=form3
	bl printf

	pop {r4-r7, pc}
//============================================================================
// Q7: random_sum
//============================================================================
.global random_sum
random_sum:
	push {r4-r7, lr}
	ldr r4,=intarr2 //*arr
	movs r5, #0 //sum

	bl random
	str r0,[r4]

	for2:
	movs r6, #1 //x
	forcmp2:
	movs r7, #20
	cmp r6, r7
	bge endfor2
	forexec2:
	lsls r7, r6, #2
	subs r7, #4
	bl random
	ldr r1,[r4,r7]
	movs r2, r0
	subs r1, r0
	adds r7, #4
	str r1,[r4,r7]
	forincerm2:
	adds r6, #1
	b forcmp2
	endfor2:

	for3:
	movs r6, #0 //x
	forcmp3:
	movs r7, #20
	cmp r6, r7
	bge endfor3
	forexec3:
	lsls r7, r6, #2
	ldr r1, [r4, r7]
	adds r5, r1
	forincerm3:
	adds r6, #1
	b forcmp3
	endfor3:
	movs r0, r5
	pop {r4-r7, pc}
//============================================================================
// Q8: fibn
//============================================================================
.global fibn
fibn:
	push {r4-r7, lr}
	ldr r1,=intarr3

	movs r2,#0
	str r2,[r1]

	movs r2,#1
	str r2,[r1,#4]

	for4:
	movs r2, #2
	forcmp4:
	movs r3, #120
	cmp r2, r3
	bge endfor4
	forexec4:
	lsls r3, r2, #2

	subs r3, #4
	ldr r4,[r1,r3]

	subs r3, #4
	ldr r5,[r1,r3]

	adds r5,r4
	adds r3,#8

	str r5,[r1,r3]
	forincr4:
	adds r2,#1
	b forcmp4
	endfor4:

	lsls r0, r0, #2
	ldr r0,[r1, r0]

	pop {r4-r7, pc}
//============================================================================
// Q9: fun
//============================================================================
.global fun
fun:
	push {r4-r7, lr}
	movs r6, r0 //a
	movs r7, r1 //b
	movs r0, #0 //sum
	ldr r1,=intarr4 // *arr
	str r0,[r1] // arr[0]=0

	for5:
	movs r2, #1
	fcmp5:
	cmp r2, #100
	bge endfor5
	fexec5:
	lsls r3, r2, #2
	subs r3, #4
	movs r4, r2
	adds r4, #7
	movs r5, #37
	muls r4, r5
	ldr	 r5, [r1,r3]
	adds r5, r4
	adds r3, #4
	str r5, [r1,r3]
	finc5:
	adds r2, #1
	b fcmp5
	endfor5:

	for6:
	movs r2, r6
	fcmp6:
	cmp r2, r7
	bgt endfor6
	fexec6:
	lsls r3,r2,#2
	ldr r4,[r1, r3]
	adds r0, r4
	finc6:
	adds r2, #1
	b fcmp6
	endfor6:

	pop {r4-r7, pc}
//============================================================================
// Q10: sick
//============================================================================
.global sick
sick:
	push {r4-r7, lr}

	push {r0,r1} //start, end
	movs r6, r2 //add
	movs r7, r3 //mull

	movs r0, #0 //sum
	ldr r1,=intarr5 // *arr
	str r0,[r1] // arr[0]=0

	for7:
	movs r2, #1
	fcmp7:
	cmp r2, #100
	bge endfor7
	fexec7:
	lsls r3, r2, #2
	subs r3, #4
	movs r4, r2
	adds r4, r6
	muls r4, r7
	ldr	 r5, [r1,r3]
	adds r5, r4
	adds r3, #4
	str r5, [r1,r3]
	finc7:
	adds r2, #1
	b fcmp7
	endfor7:

	for8:
	pop {r2,r6} // start, end,
	ldr r7,[sp,#20] // step
	fcmp8:
	cmp r2,r6
	bgt endfor8
	fexec8:
	lsls r3,r2,#2
	ldr r4,[r1, r3]
	adds r0, r4
	finc8:
	adds r2, r7
	b fcmp8
	endfor8:

	pop {r4-r7, pc}


.data //literal pool
.align 2
text1: .string "Hello, world. \n"
form1: .string "%d + %d = %d\n"
form2: .string "%d + %d + %d = %d\n"
text2: .string "Enter your name: "
form3: .string "Hello, %s\n"

.align 4
intarr1: .space 800
intarr2: .space 20*4
intarr3: .space 120*4
intarr4: .space 100*4
intarr5: .space 100*4

.align 1
buffer1: .space 77






//intarr2: .space 400



