#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

void init_lcd(void);
void display1(const char *);
void display2(const char *);
void test_wiring();

int red = 0, blue = 0, grn = 0;
char line1[16] = {"Freq:"};
char line2[16] = {"Duty:"};

int col = 0;
int8_t history[16] = {0};
int8_t lookup[16] = {1,4,7,0xe,2,5,8,0,3,6,9,0xf,0xa,0xb,0xc,0xd};
char char_lookup[16] = {'1','4','7','*','2','5','8','0','3','6','9','#','A','B','C','D'};

int get_key_pressed() {
	int key = get_key_press();
	while(key != get_key_release());
	return key;
}

char get_char_key() {
	int index = get_key_pressed();
	return char_lookup[index];
}

int get_user_freq() {
    int freq = 0;
    int pos = 0;
    while(1) {
        int index = get_key_pressed();
        int key = lookup[index];
        if(key == 0x0d)
            break;
        if(key >= 0 && key <= 9) {
            freq = freq * 10 + key;
            pos++;
            if(pos < 9)
                line1[4+pos] = key + '0';

            display1(line1);
        }
    }

    return freq;
}

void get_pwm_duty() {

    int pos = 0;
    red = 0;
    grn = 0;
    blue = 0;
    while(1) {
        int index = get_key_pressed();
        int key = lookup[index];
        if(key >= 0 && key <= 9) {
            switch(pos) {
            case 0: red = 10 * key;
                    break;
            case 1: red = red + key;
                    break;
            case 2: grn = 10 * key;
                    break;
            case 3: grn = grn + key;
                    break;
            case 4: blue = 10 * key;
                    break;
            case 5: blue = blue + key;
                    break;
            }
            pos++;
            if(pos < 9)
                line2[4+pos] = key + '0';

            display2(line2);
        }

        if(pos == 6)
            break;
    }
}

void prob2() {
    setup_gpio();
    setup_pwm();
    int r, g, b;
    r = g = b = 0;
    int state = 0;
    while(1) {
        if(r == 100) {
            state = 1;
        } if(g == 100) {
            state = 2;
        } if(b == 100) {
            state = 3;
        } if(r == 100) {
            state = 1;
        }

        if(state == 0) {
            r = r + 1;
        }
        if(state == 1) {
            r = r - 1;
            g = g + 1;
        }

        if(state == 2) {
            g = g - 1;
            b = b + 1;
        }

        if(state == 3) {
            r = r + 1;
            b = b - 1;
        }

        update_rgb(r, g, b);
        usleep(10000);
    }
}

void prob3(void)
{
    char keys[16] = {"Key Pressed:"};
    init_lcd();
    init_keypad();
    setup_timer6();
    display1("Problem 3");
    display2(keys);
    while(1) {
        char key = get_char_key();
        if(key != '\0') {
            keys[12] = key;
            display2(keys);
        }
    }
}

// Student Code goes below, do not modify code above this
// -------------------------------------------
// Section 6.2
// -------------------------------------------
// Should enable clock to GPIO port A, configure the modes of the three
// pins corresponding to TIM1_CH1, TIM1_CH2 and TIM1_CH3 as alternate function.
void setup_gpio() {
	/* Student code goes here */
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER &= ~(3 << 8 * 2);
	GPIOA -> MODER &= ~(3 << 9 * 2);
	GPIOA -> MODER &= ~(3 << 10 * 2);
	GPIOA -> MODER |= (2 << 8 * 2);
	GPIOA -> MODER |= (2 << 9 * 2);
	GPIOA -> MODER |= (2 << 10 * 2);
	GPIOA -> AFR[1] &= ~(15);
	GPIOA -> AFR[1] &= ~(15<<4);
	GPIOA -> AFR[1] &= ~(15<<8);
	GPIOA -> AFR[1] |= (0b0010);
	GPIOA -> AFR[1] |= (0b0010 << 4);
	GPIOA -> AFR[1] |= (0b0010 << 8);
	return;
}

// Should use TIM1 to PSC so that the clock is 1 KHz, and choose the
// value of ARR so that the PWM frequency is 10 Hz. The duty cycle of each
// channel will be set by writing a value between 0 and 99 to the CCRx
// registers. Note since we have a common anode configuration,
// CCRx of 100 will result in an off LED and
// a CCRx of 0 will result in maximum brightness.
void setup_pwm() {
	/* Student code goes here */
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1 -> PSC = (1400 - 1);
	TIM1 -> ARR = (100 - 1);
	TIM1 -> CCR1 = 100;
	TIM1 -> CCR2 = 100;
	TIM1 -> CCR3 = 100;

	TIM1 -> CCMR1 |= TIM_CCMR1_OC1M_2|TIM_CCMR1_OC1M_1|TIM_CCMR1_OC2M_2|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC1PE|TIM_CCMR1_OC2PE;
	TIM1 -> CCMR2 |= TIM_CCMR1_OC1M_2|TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1PE;

//	TIM1 -> CCMR1 |= TIM_CCMR1_OC1PE|TIM_CCMR1_OC2PE;
//	TIM1 -> CCMR2 |= TIM_CCMR1_OC1PE;
	TIM1 -> CCER &= ~(TIM_CCER_CC1P|TIM_CCER_CC2P|TIM_CCER_CC3P);
	//TIM1 -> CCER |= TIM_CCER_CC1P|TIM_CCER_CC2P|TIM_CCER_CC3P;
	TIM1 -> CCER |= TIM_CCER_CC1E|TIM_CCER_CC2E|TIM_CCER_CC3E;
	TIM1 -> BDTR |= TIM_BDTR_MOE;
	//TIM1 -> CR1 |= TIM_CR1_ARPE;

	TIM1 -> CR1 |= TIM_CR1_DIR;
	TIM1 -> CR1 &= ~(TIM_CR1_CMS_0|TIM_CR1_CMS_1);//&= ~(3 << 5);

	TIM1 -> CR1 |= 1;
	TIM1 -> EGR |= TIM_EGR_UG;

	return;
}


// This function accepts an integer argument that is used to update the
// TIM1_PSC to produce the requested frequency (as close as possible) on
// the output pins. Remember that the output frequency will be 100 times
// slower than the TIM1_PSC due to TIM1_ARR always being 100-1 (99).
// The formula for calculating the output frequency is then:
//          freq = 48,000,000.0 / (TIM1_PSC + 1) / 100.0
// You should determine the formula to use to put the proper value
// into TIM1_PSC given the frequency
void update_freq(int freq) {
	/* Student code goes here */
	int npsc = (48000000/(freq * 100)) - 1;
	//if (48000000/(npsc * 100) < 100){
	TIM1 -> PSC = npsc;
	//}
	return;
}

// This function accepts three arguments---the red, green, and blue values used
// to set the CCRx registers. The values should never be smaller than zero or
// larger than 100. The value can be assigned directly to the appropriate
// CCR registers. E.g. the red LED is connected to channel 1.
void update_rgb(int r, int g, int b) {
	/* Student code goes here */
	int base = 1;
	int range = 100;
	if (r > range || r < base){
		r = base + 1;
	}
	if (g > range || g < base){
		g = base + 1;
	}
	if (b > range || b < base){
		b = base + 1;
	}
	TIM1 -> CCR1 = r;
	TIM1 -> CCR2 = g;
	TIM1 -> CCR3 = b;
	return;
}

// -------------------------------------------
// Section 6.3
// -------------------------------------------
// This function should enable the clock to port A, configure pins 0, 1, 2 and
// 3 as outputs (we will use these to drive the columns of the keypad).
// Configure pins 4, 5, 6 and 7 to have a pull down resistor
// (these four pins connected to the rows will being scanned
// to determine the row of a button press).
void init_keypad() {
	/* Student code goes here */
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER &= ~(3 << 0 * 2);
	GPIOA -> MODER &= ~(3 << 1 * 2);
	GPIOA -> MODER &= ~(3 << 2 * 2);
	GPIOA -> MODER &= ~(3 << 3 * 2);
	GPIOA -> MODER &= ~(3 << 4 * 2);
	GPIOA -> MODER &= ~(3 << 5 * 2);
	GPIOA -> MODER &= ~(3 << 6 * 2);
	GPIOA -> MODER &= ~(3 << 7 * 2);

	GPIOA -> MODER |= (1 << 0 * 2);
	GPIOA -> MODER |= (1 << 1 * 2);
	GPIOA -> MODER |= (1 << 2 * 2);
	GPIOA -> MODER |= (1 << 3 * 2);

	GPIOA -> PUPDR &= ~(3 << 4 * 2);
	GPIOA -> PUPDR &= ~(3 << 5 * 2);
	GPIOA -> PUPDR &= ~(3 << 6 * 2);
	GPIOA -> PUPDR &= ~(3 << 7 * 2);
	GPIOA -> PUPDR |= (2 << 4 * 2);
	GPIOA -> PUPDR |= (2 << 5 * 2);
	GPIOA -> PUPDR |= (2 << 6 * 2);
	GPIOA -> PUPDR |= (2 << 7 * 2);
	return;
}

// This function should,
// enable clock to timer6,
// setup pre scalar and arr so that the interrupt is triggered every
// 1ms, enable the timer 6 interrupt, and start the timer.
void setup_timer6() {
    /* Student code goes here */
	RCC -> APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6 -> PSC = 140 - 1;
	TIM6 -> ARR = 100 - 1;
	TIM6 -> DIER |= TIM_DIER_UIE;
	NVIC -> ISER[0] |= (1 << 17);
	TIM6 -> CR1 |= TIM_CR1_CEN;
	return;
}

// The functionality of this subroutine is described in the lab document
int get_key_press() {
	/* Student code goes here */
	while(1){
		for (int i = 0; i < 16; i++){
			if (history[i] == 1){
				return i;
			}
		}
	}
	return 0;
}

// The functionality of this subroutine is described in the lab document
int get_key_release() {
	/* Student code goes here */
	while(1){
		for (int i = 0; i < 16; i++){
			if (history[i] == -2){
				return i;
			}
		}
	}
	return 0;
}


// See lab document for the instructions as to how to fill this
void TIM6_DAC_IRQHandler() {
	/* Student code goes here */
	TIM6 -> SR &= ~TIM_SR_UIF;
	int row = (GPIOA -> IDR >> 4) & 0xf;
	int index = col << 2;
//	history[index] = history[index] << 1;
//	history[index] |= row & 0x1;

	for (int i = 0; i <= 3; i++){
		history[index + i] = history[index + i] << 1;
		history[index + i] |= (row >> i) & 0x1;
	}

	col = col + 1;
	if (col > 3){
		col = 0;
	}
	GPIOA -> ODR = 1 << col;
	return;
}

// -------------------------------------------
// Section 6.4
// -------------------------------------------
void prob4(void)
{
    init_lcd();
    init_keypad();
    setup_gpio();
    setup_pwm();
    setup_timer6();
    display1(line1);
    display2(line2);
    int freq;
    while(1) {
    	char key = get_char_key();
        /* Student code goes here*/
    	if (key == '*'){
    		freq = get_user_freq();
    		update_freq(freq);
    	}
    	if (key == '#'){
    		get_pwm_duty();
    		update_rgb(red, grn, blue);
    	}
        /* End of student code*/
    }
}


int main(void)
{
    //test_wiring();
    //prob2();
    prob3();
    //prob4();
}
