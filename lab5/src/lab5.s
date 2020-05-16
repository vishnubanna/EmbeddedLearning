.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

//=============================================================================
// ECE 362 Lab Experiment 5
// Timers
//
//=============================================================================

.equ  RCC,      0x40021000
.equ  APB1ENR,  0x1C
.equ  AHBENR,   0x14
.equ  TIM6EN,	0x10
.equ  GPIOCEN,  0x00080000
.equ  GPIOBEN,  0x00040000
.equ  GPIOAEN,  0x00020000
.equ  GPIOC,    0x48000800
.equ  GPIOB,    0x48000400
.equ  GPIOA,    0x48000000
.equ  MODER,    0x00
.equ  PUPDR,    0x0c
.equ  IDR,      0x10
.equ  ODR,      0x14
.equ  PC0,      0x01
.equ  PC1,      0x04
.equ  PC2,      0x10
.equ  PC3,      0x40
.equ  PIN8,     0x00000100

// NVIC control registers...
.equ NVIC,		0xe000e000
.equ ISER, 		0x100
.equ ICER, 		0x180
.equ ISPR, 		0x200
.equ ICPR, 		0x280

// TIM6 control registers
.equ  TIM6, 	0x40001000
.equ  CR1,		0x00
.equ  DIER,		0x0C
.equ  PSC,		0x28
.equ  ARR,		0x2C
.equ  TIM6_DAC_IRQn, 17
.equ  SR,		0x10


//=======================================================
// 6.1: Configure timer 6
//=======================================================
.global init_TIM6
init_TIM6:
	push {r4 - r7, lr}

    ldr r0,=RCC
    ldr r1,[r0,#APB1ENR]
    ldr r2,=TIM6EN
    orrs r1,r2
    str r1,[r0,#APB1ENR]

    ldr r0,=TIM6
    ldr r1,=480-1
    str r1,[r0,#PSC]

    ldr r1,=100-1
    str r1,[r0,#ARR]

    ldr r1,[r0,#DIER]
    ldr r2,=0x1
    orrs r1,r2
    str r1,[r0,#DIER]

    ldr r1,[r0,#CR1]
    ldr r2,=0x1
    orrs r1,r2
    str r1,[r0,#CR1]

    ldr r0,=NVIC
    ldr r3,=ISER
    ldr r1,[r0,r3]
    ldr r2,=(1<<TIM6_DAC_IRQn)
    orrs r1, r2
    str r2,[r0,r3]
	pop {r4 - r7, pc}
//=======================================================
// 6.2: Confiure GPIO
//=======================================================
.global init_GPIO
init_GPIO:
	// enable gpio c in RCC
	push {r4-r7, lr}
	ldr r0,=RCC
	ldr r1,[r0,#AHBENR]
	ldr r2,=GPIOCEN
	ldr r3,=GPIOAEN
	orrs r1,r2
	orrs r1,r3
	str r1,[r0,#AHBENR]

	ldr r0,=GPIOC
	ldr r1,[r0,#MODER]
	ldr r2,=0x3
	bics r1, r2
	ldr r2,=(3<<(1 * 2))
	bics r1, r2
	ldr r2,=(3<<(2 * 2))
	bics r1, r2
	ldr r2,=(3<<(3 * 2))
	bics r1, r2
	ldr r2,=(3<<(8 * 2))
	bics r1, r2

	ldr r2,=0x01
	orrs r1, r2
	ldr r2,=(1<<(1 * 2))
	orrs r1, r2
	ldr r2,=(1<<(2 * 2))
	orrs r1, r2
	ldr r2,=(1<<(3 * 2))
	orrs r1, r2
	ldr r2,=(1<<(8 * 2))
	orrs r1, r2
	str r1, [r0,#MODER]


	ldr r0,=GPIOA
	ldr r1,[r0,#MODER]
	ldr r2,=0x3
	bics r1, r2
	ldr r2,=(3<<(1 * 2))
	bics r1, r2
	ldr r2,=(3<<(2 * 2))
	bics r1, r2
	ldr r2,=(3<<(3 * 2))
	bics r1, r2

	ldr r2,=0x01
	orrs r1, r2
	ldr r2,=(1<<(1 * 2))
	orrs r1, r2
	ldr r2,=(1<<(2 * 2))
	orrs r1, r2
	ldr r2,=(1<<(3 * 2))
	orrs r1, r2
	ldr r0,=GPIOA
	str r1, [r0,#MODER]


	ldr r1, [r0, #PUPDR]
	ldr r2,=3<<(4 * 2)
	bics r1, r2
	ldr r2,=3<<(5 * 2)
	bics r1, r2
	ldr r2,=3<<(6 * 2)
	bics r1, r2
	ldr r2,=3<<(7 * 2)
	bics r1, r2

	ldr r2,=2<<(4 * 2)
	orrs r1, r2
	ldr r2,=2<<(5 * 2)
	orrs r1, r2
	ldr r2,=2<<(6 * 2)
	orrs r1, r2
	ldr r2,=2<<(7 * 2)
	orrs r1, r2
	ldr r0,=GPIOA
	str r1,[r0, #PUPDR]

	pop {r4-r7, pc}
//=======================================================
// 6.3 Blink blue LED using Timer 6 interrupt
// Write your interrupt service routine below.
//=======================================================
.type TIM6_DAC_IRQHandler, %function
.global TIM6_DAC_IRQHandler
TIM6_DAC_IRQHandler:
	push {r4-r7, lr}

	//clear TIM6_SR
	ldr r0,=TIM6
	ldr r1,[r0,#SR]
	movs r2, #1
	bics r1, r2
	str r1,[r0,#SR]

	ldr r0,=tick
	ldr r1,[r0]
	adds r1, #1
	str r1,[r0]

	if1:
	ldr r2,=0x3E8
	cmp r1, r2
	bne endif1
	then1:
	ldr r0,=tick
	movs r1, #0
	str r1,[r0]

	ldr r0,=GPIOC
	ldr r1,[r0,#ODR]
	ldr r2,=0x100
	eors r1,r2
	str r1,[r0,#ODR]
	endif1:

	ldr r0,=col // &col
	ldr r1,[r0] // deref col
	adds r1, #1 // col + 1
	movs r2, #3
	ands r1, r2 // (col + 1) & 3
	str r1,[r0] // col = (col + 1) & 3

	ldr r0,=GPIOA // &GPI0A
	movs r2, #1
	lsls r2, r2, r1 // temp = 1 << col
	str r2,[r0, #ODR] // GPIOA_ODR = temp

	lsls r2, r1, #2 // index(r2) = col << 2
	movs r7, r2 // store index
	ldr r0,=history //&history

	ldrb r1,[r0, r2] // history[index(r2)]
	lsls r1, r1, #1 // history[index(r2)] << 1
	strb r1,[r0, r2] // history[index(r2)] = history[index(r2)] << 1

	adds r2, #1 // index = index + 1
	ldrb r1,[r0, r2] // history[index(r2) + 1]
	lsls r1, r1, #1 // history[index(r2) + 1] << 1
	strb r1,[r0, r2] // history[index(r2) + 1] = history[index(r2) + 1] << 1

	adds r2, #1 // index = index + 2
	ldrb r1,[r0, r2] // history[index(r2) + 2]
	lsls r1, r1, #1 // history[index(r2) + 2] << 1
	strb r1,[r0, r2] // history[index(r2) + 2] = history[index(r2) + 2] << 1

	adds r2, #1 // index = index + 3
	ldrb r1,[r0, r2] // history[index(r2) + 2]
	lsls r1, r1, #1 // history[index(r2) + 2] << 1
	strb r1,[r0, r2] // history[index(r2) + 2] = history[index(r2) + 2] << 1

	ldr r0,=GPIOA //GPIOA
	ldr r1,[r0, #IDR] //IDR
	lsrs r2, r1, #4 //IDR >> 4
	ldr r3,=0x0f // temp = (IDR >> 4) & 15
	ands r2, r3 // r2(row) = temp
	ldr r0,=history //&history

	movs r3, #1 //temp = 1
	movs r4, r2 //r4 = row
	movs r2, r7 // r2 = index

	//r4 = row
	//r2 = index
	//r0 = history

	ldrb r1,[r0, r2] // history[index(r2)]
	movs r5, r4 // move row into temp
	ands r5, r3 // and  temp with 1
	orrs r1, r5 // write temp to r1(history[index(r2)])
	strb r1,[r0, r2] // history[index(r2)] = history[index(r2)] << 1

	adds r2, #1 // increment -> index + 1
	ldrb r1,[r0, r2] // history[index(r2) + 1] to r1
	lsrs r5, r4, #1 // shift row by 1 and store in r5
	ands r5, r3 // and temp with #1
	orrs r1, r5 // write r5 into r1(history[index(r2)])
	strb r1,[r0, r2] // history[index(r2) + 1] = history[index(r2) + 1] << 1

	adds r2, #1 // increment -> index + 2
	ldrb r1,[r0, r2] // history[index(r2) + 2]
	lsrs r5, r4, #2 // shift row by 2 and store in r5
	ands r5, r3
	orrs r1, r5 //write r5 into r1(history[index(r2)])
	strb r1,[r0, r2] // history[index(r2) + 2] = history[index(r2) + 2] << 1

	adds r2, #1 // increment -> index + 3
	ldrb r1,[r0, r2] // history[index(r2) + 2]
	lsrs r5, r4, #3 // shift row by 3 and store in r5
	ands r5, r3 // and temp with #1
	orrs r1, r5 //write r5 into r1(history[index(r2)])
	strb r1,[r0, r2] // history[index(r2) + 2] = history[index(r2) + 2] << 1

	pop  {r4-r7, pc}

/*
TIM6_DAC_IRQHandler:
	push {r4-r7, lr}

	// clear exti
	//ldr r0,=EXTI
	//ldr r1,[r0,#PR]
	//movs r2, #1
	//orrs r1, r2
	//str r1,[r0,#PR]

	//clear TIM6_SR
	ldr r0,=TIM6
	ldr r1,[r0,#SR]
	movs r2, #1
	bics r1, r2
	str r1,[r0,#SR]

	ldr r0,=tick
	ldr r1,[r0]
	adds r1, #1
	str r1,[r0]

	if1:
	ldr r2,=0x3E8
	cmp r1, r2
	bne endif1
	then1:
	ldr r0,=tick
	movs r1, #0
	str r1,[r0]

	ldr r0,=GPIOC
	ldr r1,[r0,#ODR]
	ldr r2,=0x100
	eors r1,r2
	str r1,[r0,#ODR]
	endif1:

	pop  {r4-r7, pc}
*/




//=======================================================
// 6.5 Debounce keypad
//=======================================================
.global getKeyPressed
getKeyPressed:
	push {r4-r7, lr}
	while1:
	for1:
	ldr r0,=history // *history
	ldr r1,=tick // *tick
	movs r2, #0// i
	fcomp1:
	cmp r2, #16 // i < 16
	bge endfor1

	if2:
	ldrb r3,[r0, r2] // history[i]
	cmp r3, #1 // history[i] == 1
	bne endif2
	then2:
	movs r3, #0 // tick = 0
	str r3,[r1]
	movs r0, r2 // return i
	pop {r4-r7, pc} // return
	endif2:

	adds r2, #1 // i++
	b fcomp1 // repeat
	endfor1:

	b while1 // repeat nevery end while
	endwhile1:
	//pop {r4-r7, pc} // return

.global getKeyReleased
getKeyReleased:
	push {r4-r7, lr}
	while2:
	for2:
	ldr r0,=history // *history
	ldr r1,=tick // *tick
	movs r2, #0// i
	fcomp2:
	cmp r2, #16 // i < 16
	bge endfor2
	//fexec2:

	if3:
	ldrb r3,[r0, r2] // history[i]
	ldr r5,=0xfe
	cmp r3, r5 // history[i] == -2
	bne endif3
	then3:
	movs r3, #0 // tick = 0
	str r3,[r1]
	movs r0, r2 // return i
	pop {r4-r7, pc} // return
	endif3:

	//finc2:
	adds r2, #1 // i++
	b fcomp2 // repeat
	endfor2:

	b while2 // repeat nevery end while

