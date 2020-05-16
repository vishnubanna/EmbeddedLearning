.cpu cortex-m0
.thumb
.syntax unified
.fpu softvfp

.equ RCC,       0x40021000
.equ GPIOA,     0x48000000
.equ GPIOC,     0x48000800
.equ AHBENR,    0x14
.equ APB2ENR,   0x18
.equ APB1ENR,   0x1c
.equ IOPAEN,    0x20000
.equ IOPCEN,    0x80000
.equ SYSCFGCOMPEN, 1
.equ TIM3EN,    2
.equ MODER,     0x00
.equ OSPEEDR,   0x08
.equ PUPDR,     0x0c
.equ IDR,       0x10
.equ ODR,       0x14
.equ BSRR,      0x18
.equ BRR,       0x28
.equ PC8,       0x100

// SYSCFG control registers
.equ SYSCFG,    0x40010000
.equ EXTICR2,   0x0c

// NVIC control registers
.equ NVIC,      0xe000e000
.equ ISER,      0x100

// External interrupt control registers
.equ EXTI,      0x40010400
.equ IMR,       0
.equ RTSR,      0x8
.equ PR,        0x14

.equ TIM3,      0x40000400
.equ TIMCR1,    0x00
.equ DIER,      0x0c
.equ TIMSR,     0x10
.equ PSC,       0x28
.equ ARR,       0x2c

// Iinterrupt number for EXTI4_15 is 7.
.equ EXTI4_15_IRQn,  7
// Interrupt number for Timer 3 is 16.
.equ TIM3_IRQn,  16

//=====================================================================
// Q1
//=====================================================================
.global subdiv
subdiv:
	push {r4-r7, lr}
	//r0 int x

	if1:
		cmp r0, #0
		bne endif1
	then1:
		movs r0, #0
		pop {r4-r7, pc}
	endif1:

	if2:
		movs r2, #1
		ands r2, r0
		cmp r2, #1
		bne endif2
	then2:
		subs r0, #1
		bl subdiv
		adds r0, #1
		pop {r4-r7, pc}
	endif2:

	lsrs r0, r0, #1
	bl subdiv
	adds r0, #1
	pop {r4-r7, pc}
//=====================================================================
// Q2
//=====================================================================
.global enable_porta
enable_porta:
	push {r4-r7, lr}
	ldr r0,=RCC
	ldr r1,[r0,#AHBENR]
	ldr r2,=IOPAEN
	orrs r1, r2
	str r1,[r0,#AHBENR]
	pop {r4-r7, pc}

//=====================================================================
// Q3
//=====================================================================
.global enable_portc
enable_portc:
	push {r4-r7, lr}
	// retype

	ldr r0,=RCC
	ldr r1,[r0,#AHBENR]
	ldr r2,=IOPCEN
	orrs r1, r2
	str r1,[r0,#AHBENR]

	pop {r4-r7, pc}
//=====================================================================
// Q4
//=====================================================================
.global setup_pa4
setup_pa4:
	push {r4-r7, lr}

	ldr r0,=GPIOA
	ldr r1,[r0,#MODER]
	ldr r2,=(3<<(4*2))
	bics r1, r2
	str r1,[r0,#MODER]

	ldr r1,[r0,#PUPDR]
	ldr r2,=(3<<(4*2))
	bics r1,r2
	ldr r2,=(1<<(4*2))
	orrs r1, r2
	str r1,[r0,#PUPDR]

	pop {r4-r7, pc}
//=====================================================================
// Q5
//=====================================================================
.global setup_pa5
setup_pa5:
	push {r4-r7, lr}
	ldr r0,=GPIOA
	ldr r1,[r0,#MODER]
	ldr r2,=(3<<(5 * 2))
	bics r1,r2
	str r1,[r0,#MODER]

	ldr r1,[r0,#PUPDR]
	ldr r2,=(3<<(5*2))
	bics r1,r2
	ldr r2,=(2<<(5*2))
	orrs r1,r2
	str r1,[r0,#PUPDR]
	pop {r4-r7, pc}
//=====================================================================
// Q6
//=====================================================================
.global setup_pc8
setup_pc8:
	push {r4-r7, lr}
	ldr r0,=GPIOC
	ldr r1,[r0,#MODER]
	ldr r2,=(3<<(8*2))
	bics r1,r2
	ldr r2,=(1<<(8*2))
	orrs r1,r2
	str r1,[r0,#MODER]

	ldr r1,[r0,#OSPEEDR]
	ldr r2,=(3<<(8*2))
	//bics r1,r2
	//ldr r2,=(1<<(8*2))
	orrs r1,r2
	str r1,[r0,#OSPEEDR]
	pop {r4-r7, pc}
//=====================================================================
// Q7
//=====================================================================
.global setup_pc9
setup_pc9:
	push {r4-r7, lr}

	ldr r0,=GPIOC
	ldr r1,[r0,#MODER]
	ldr r2,=(3<<(9*2))
	bics r1,r2
	ldr r2,=(1<<(9*2))
	orrs r1,r2
	str r1,[r0,#MODER]

	ldr r1,[r0,#OSPEEDR]
	ldr r2,=(3<<(2*9))
	bics r1,r2
	ldr r2,=(1<<(2*9))
	orrs r1, r2
	str r1,[r0,#OSPEEDR]
	pop {r4-r7, pc}
//=====================================================================
// Q8
//=====================================================================
.global action8
action8:
	push {r4-r7, lr}
	ldr r0,=GPIOA
	ldr r1,=GPIOC

	ldr r2,[r0,#IDR]
	movs r5,r2
	if3:
		casea3:
		lsrs r3,r2,#4
		movs r4,#1
		ands r3,r4
		cmp r3,#1
		bne else3
		caseb3:
		lsrs r3,r2,#5
		movs r4,#1
		ands r3,r4
		cmp r3,#0
		bne else3
	then3:
	ldr r3,=PC8
	/*
	ldr r2,[r1,#BSRR]
	bics r2,r3
	str r2,[r1,#BSRR]

	ldr r2,[r1,#BRR]
	orrs r2,r3
	str r2,[r1,#BRR]
	*/

	ldr r2,[r1,#ODR]
	bics r2,r3
	str r2,[r1,#ODR]
	b endif3
	else3:
	ldr r3,=PC8
	/*
	ldr r2,[r1,#BRR]
	bics r2,r3
	str r2,[r1,#BRR]

	ldr r2,[r1,#BSRR]
	orrs r2,r3
	str r2,[r1,#BSRR]
	*/
	ldr r2,[r1,#ODR]
	orrs r2,r3
	str r2,[r1,#ODR]
	endif3:
	pop {r4-r7, pc}
//=====================================================================
// Q9
//=====================================================================
.global action9
action9:
	push {r4-r7, lr}
	ldr r0,=GPIOA
	ldr r1,=GPIOC

	ldr r2,[r0,#IDR]

	if4:
		casea4:
			lsrs r3,r2,#4
			movs r4,#1
			ands r3,r4
			cmp r3,#0
			bne else4
		caseb4:
			lsrs r3,r2,#5
			movs r4,#1
			ands r3,r4
			cmp r3,#1
			bne else4
	then4:
		ldr r3,=PC8
		lsls r3,r3,#1
		ldr r2,[r1,#ODR]
		bics r2,r3
		str r2,[r1,#ODR]
		b endif4
	else4:
		ldr r3,=PC8
		lsls r3,r3,#1
		ldr r2,[r1,#ODR]
		orrs r2,r3
		str r2,[r1,#ODR]
	endif4:
	pop {r4-r7, pc}
//=====================================================================
// Q10
//=====================================================================
.type EXTI4_15_IRQHandler, %function
.global EXTI4_15_IRQHandler
EXTI4_15_IRQHandler:
	push {r4-r7, lr}
	ldr r0,=EXTI
	ldr r1,[r0,#PR]
	movs r2,#16
	orrs r1,r2
	str r1,[r0,#PR]

	ldr r0,=counter
	ldr r1,[r0]
	adds r1, #1
	str r1,[r0]
	pop {r4-r7, pc}
//=====================================================================
// Q11
//=====================================================================
.global enable_exti4
enable_exti4:
	push {r4-r7, lr}
	// enable clock to sysconfig

	ldr r0,=RCC
	ldr r1,[r0,#APB2ENR]
	movs r2,#1 // write 1 to sysconfig
	orrs r1,r2
	str r1,[r0,#APB2ENR]

	ldr r0,=SYSCFG
	ldr r1,[r0,#EXTICR2]
	ldr r2,=0xf
	bics r1,r2
	//ldr r2,=0xf
	//orrs r1,r2
	str r1,[r0,#EXTICR2]

	// trigger rising edge
	ldr r0,=EXTI
	ldr r1,[r0,#RTSR]
	ldr r2,=(1<<4)
	orrs r1, r2
	str r1,[r0,#RTSR]

	// mask on the EXTI IMR, tells it to stop, mask pin 4 so that it stops ignoring interupts from pin 4
	ldr r1,[r0,#IMR]
	ldr r2,=(1<<4)
	orrs r1,r2
	str r1,[r0,#IMR]

	ldr r0,=NVIC
	ldr r3,=ISER
	ldr r1,[r0,r3]
	//bit7 unmask_EXTI4_15_IRQHandler
	ldr r2,=(1<<EXTI4_15_IRQn)
	orrs r1,r2
	str r1,[r0,r3]

	pop {r4-r7, pc}
//=====================================================================
// Q12
//=====================================================================
.type TIM3_IRQHandler %function
.global TIM3_IRQHandler

TIM3_IRQHandler:
	push {r4-r7, lr}
	ldr r0,=TIM3
	ldr r1,[r0,#TIMSR]
	movs r2, #1
	bics r1,r2
	str r1,[r0,#TIMSR]

	ldr r0,=GPIOC
	ldr r1,[r0,#ODR]
	ldr r2,=PC8<<1
	eors r1,r2
	str r1,[r0,#ODR]
	pop {r4-r7, pc}

//=====================================================================
// Q13
//=====================================================================
.global enable_tim3
enable_tim3:
	push {r4-r7, lr}

	ldr r0,=RCC
	ldr r1,[r0,#APB1ENR]
	ldr r2,=1<<1
	orrs r1, r2
	str r1,[r0,#APB1ENR]

	ldr r0,=TIM3
	ldr r1,[r0,#PSC]
	ldr r2,=0x0000ffff
	bics r1,r2
	ldr r2,=24000-1
	orrs r1,r2
	str r1,[r0,#PSC]

	ldr r1,[r0,#ARR]
	ldr r2,=0x0000ffff
	bics r1,r2
	ldr r2,=1000-1
	orrs r1,r2
	str r1,[r0,#ARR]

	ldr r1,[r0,#DIER]
	ldr r2,=1
	orrs r1,r2
	str r1,[r0,#DIER]

	ldr r0,=NVIC
	ldr r3,=ISER
	ldr r1,[r0,r3]
	ldr r2,=1<<TIM3_IRQn
	orrs r1,r2 // mask the system interupt in NVIC so that the one you wrote takes priority and enable the interupt for the given bit
	str r1,[r0,r3]

	ldr r0,=TIM3
	ldr r1,[r0,#TIMCR1]
	ldr r2,=1
	orrs r1,r2
	str r1,[r0,#TIMCR1]

	pop {r4-r7, pc}
