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
	push {lr}

	if1:
	cmp r0,#0
	bne endif1
	then1:
	movs r0,#0
	pop {pc}
	endif1:

	if2:
	movs r1,r0
	movs r2,#1
	ands r1,r2
	cmp r1,#1
	bne endif2
	then2:
	subs r0, #1
	bl subdiv
	adds r0, #1
	pop {pc}
	endif2:

	lsrs r0,r0,#1
	bl subdiv
	adds r0,#1
	pop {pc}

//=====================================================================
// Q2
//=====================================================================
.global enable_porta
enable_porta:
	push {lr}

	ldr r0,=RCC
	ldr r1,[r0,#AHBENR]
	ldr r2,=IOPAEN
	orrs r1,r2
	str r1,[r0,#AHBENR]

	pop {pc}


//=====================================================================
// Q3
//=====================================================================
.global enable_portc
enable_portc:
	push {lr}

	ldr r0,=RCC
	ldr r1,[r0,#AHBENR]
	ldr r2,=IOPCEN
	orrs r1,r2
	str r1,[r0,#AHBENR]

	pop {pc}

//=====================================================================
// Q4
//=====================================================================
.global setup_pa4
setup_pa4:
	push {lr}
	ldr r0,=GPIOA
	ldr r1,[r0,#MODER]
	ldr r2,=(3<<(4*2))
	bics r1,r2
	str r1,[r0,#MODER]

	ldr r1,[r0,#PUPDR]
	ldr r2,=(3<<(4*2))
	bics r1,r2
	ldr r2,=(1<<(4*2))
	orrs r1,r2
	str r1,[r0,#PUPDR]

	pop {pc}

//=====================================================================
// Q5
//=====================================================================
.global setup_pa5
setup_pa5:
	push {lr}

	ldr r0,=GPIOA
	ldr r1,[r0,#MODER]
	ldr r2,=(3<<(5*2))
	bics r1,r2
	str r1,[r0,#MODER]

	ldr r1,[r0,#PUPDR]
	ldr r2,=(3<<(5*2))
	bics r1,r2
	ldr r2,=(2<<(5*2))
	orrs r1,r2
	str r1,[r0,#PUPDR]

	pop {pc}


//=====================================================================
// Q6
//=====================================================================
.global setup_pc8
setup_pc8:
	push {lr}

	ldr r0,=GPIOC
	ldr r1,[r0,#MODER]
	str r1,[r0,#MODER]

	ldr r1,[r0,#OSPEEDR]
	ldr r2,[r0,#OSPEEDR]
	pop {pc}

//=====================================================================
// Q7
//=====================================================================
.global setup_pc9
setup_pc9:
	push {lr}

	pop {pc}

//=====================================================================
// Q8
//=====================================================================
.global action8
action8:
	push {lr}

	pop {pc}
//=====================================================================
// Q9
//=====================================================================
.global action9
action9:
	push {lr}
	pop {pc}

//=====================================================================
// Q10
//=====================================================================
.type EXTI4_15_IRQHandler, %function
.global EXTI4_15_IRQHandler
EXTI4_15_IRQHandler:
	push {lr}
	//acknowledge the interupt

	ldr r0,=EXTI
	ldr r1,[r0,#PR]
	ldr r2,=(1<<4)
	orrs r1,r2
	str r1,[r0,#PR]

	ldr r0,=counter
	ldr r1,[r0]
	adds r1, #1
	str r1,[r0]
	pop {pc}


//=====================================================================
// Q11
//=====================================================================
.global enable_exti4
enable_exti4:
	push {lr}
	//enable clock to nvic
	ldr r0,=RCC
	ldr r1,[r0,#APB2ENR]
	ldr r2,=2
	orrs r1,r2
	str r1,[r0,#APB2ENR]

	//configure pin to create interupts
	ldr r0,=SYSCFG
	ldr r1,[r0,#EXTICR2]
	ldr r2,=0xf
	bics r1,r2
	//ldr r2,=0x1
	//orrs r1,r2
	str r1,[r0,#EXTICR2]

	// config pin4 to edge trigger
	ldr r0,=EXTI
	ldr r1,[r0,#RTSR]
	ldr r2,=(1<<4)
	orrs r1, r2
	str r1,[r0,#RTSR]

	//unmask pin4 to allow/recognise interupts
	ldr r0,=EXTI
	ldr r1,[r0,#IMR]
	ldr r2,=(1<<4)
	orrs r1, r2
	str r1,[r0,#IMR]

	//unmask the interupt in the vector table to allow cpu access to ISR
	ldr r0,=NVIC
	ldr r3,=ISER
	ldr r1,[r0,r3]
	ldr r2,=(1<<EXTI4_15_IRQn)
	orrs r1,r2
	str r1,[r0,r3]

	pop {pc}





//=====================================================================
// Q12
//=====================================================================
.type TIM3_IRQHandler %function
.global TIM3_IRQHandler
TIM3_IRQHandler:
	push {lr}
	pop {pc}


//=====================================================================
// Q13
//=====================================================================
.global enable_tim3
enable_tim3:
	push {lr}
	pop {pc}









