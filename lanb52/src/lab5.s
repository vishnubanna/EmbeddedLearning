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

.equ  TIM_DIER_UIE, 1
.equ  TIM_CR1_CEN, 1
.equ  EXTI, 0x40010400
.equ  PR, 0x14


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
	orrs r1, r2
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
	movs r1, #0
	str r1,[r0]

	ldr r0,=GPIOC
	ldr r1,[r0,#ODR]
	ldr r2,=0x100
	eors r1,r2
	str r2,[r0,#ODR]
	endif1:

	pop  {r4-r7, pc}




//=======================================================
// 6.5 Debounce keypad
//=======================================================
.global getKeyPressed
getKeyPressed:
	push {r4-r7, lr}

	pop {r4-r7, pc}

.global getKeyReleased
getKeyReleased:
	push {r4-r7, lr}

	pop {r4-r7, pc}
