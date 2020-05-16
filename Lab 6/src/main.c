#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RATE 100000
#define N 1000
#define FREQ 554.365
#define FREQ2 698.456
//extern const
short int wavetable[N];
int offset1 = 0;
int offset2 = 0;
int a1 = 0;
int a2 = 0;

void init_wavetable(void)
{
  int x;
  for(x=0; x<N; x++)
    wavetable[x] = 32767 * sin(2 * M_PI * x / N);

}

// This function
// 1) enables clock to port A,
// 2) sets PA0, PA1, PA2 and PA4 to analog mode
void setup_gpio() {
    /* Student code goes here */
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER |= 3;
	GPIOA -> MODER |= (3<<(1*2));
	GPIOA -> MODER |= (3<<(2*2));
	GPIOA -> MODER |= (3<<(3*2));
	return;
}

// This function should enable the clock to the
// onboard DAC, enable trigger,
// setup software trigger and finally enable the DAC.
void setup_dac() {
    /* Student code goes here */
	RCC -> APB1ENR |= RCC_APB1ENR_DACEN; //Enable System Clock
	DAC -> CR &= ~DAC_CR_EN1; // Turn off channel while configuring it
	DAC -> CR &= ~DAC_CR_BOFF1; // enable buffer/leave it on
	DAC -> CR |= DAC_CR_TEN1; //enable the trigger
	DAC -> CR |= DAC_CR_TSEL1; // choose the software trigger
	DAC -> CR |= DAC_CR_EN1; // turn channel 1 on again
	DAC -> DHR12R1 = 0; // clear the reg
}

// This function should,
// enable clock to timer6,
// setup pre scalar and arr so that the interrupt is triggered every
// 10us, enable the timer 6 interrupt, and start the timer.
void setup_timer6() {
    /* Student code goes here */
	RCC -> APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6 -> PSC = 240 - 1;
	TIM6 -> ARR = 2 - 1;
	TIM6 -> DIER |= 1;
	NVIC -> ISER[0] |= (1<<17);
	TIM6 -> CR1 |= 1;
	return;
}



// This function should enable the clock to ADC,
// turn on the clocks, wait for ADC to be ready.
void setup_adc() {
    /* Student code goes here */
	RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC -> CR2 |= RCC_CR2_HSI14ON;
	while(!(RCC -> CR2 & RCC_CR2_HSI14RDY));
	ADC1 -> CR |= ADC_CR_ADEN;
	while(!(ADC1 -> ISR & ADC_ISR_ADRDY));
	while((ADC1 -> CR & ADC_CR_ADSTART));
	return;
}

// This function should return the value from the ADC
// conversion of the channel specified by the “channel” variable.
// Make sure to set the right bit in channel selection
// register and do not forget to start adc conversion.
int read_adc_channel(unsigned int channel) {
    /* Student code goes here */
	ADC1 -> CHSELR = 0;
	ADC1 -> CHSELR |= 1 << channel;// ADC channel select register
	while(!(ADC1 -> ISR & ADC_ISR_ADRDY));
	ADC1 -> CR |= ADC_CR_ADSTART;
	while(!(ADC1 -> ISR & ADC_ISR_EOC));
	int val = (ADC1 -> DR);
	return val;
}

void TIM6_DAC_IRQHandler() {
    /* Student code goes here */
	// acknwledge the interupt
	TIM6 -> SR &= ~1;

	// wait for dac to finish  converting last one
	DAC -> SWTRIGR |= DAC_SWTRIGR_SWTRIG1;
	while ((DAC -> SWTRIGR & DAC_SWTRIGR_SWTRIG1) == DAC_SWTRIGR_SWTRIG1);
	int val = 0;
	int val2 = 0;

	a1 = read_adc_channel(0);
	a2 = read_adc_channel(1);

	// step tells you how many indexes to move in the array * 1 << 16 to fit the set
	// to the upper - lower bound of the interger, this way upper 16 is whole and lower 16 is fraction
	unsigned int step = ((800 * a1)/4095 + 200) * N/RATE * (1<<16); // the frequncy(sec^-1) * number-of-samples/outputFreq *
	unsigned int step2 = ((800 * a2)/4095 + 200) * N/RATE * (1<<16); // the frequncy(sec^-1) * number-of-samples/outputFreq *

	offset1 += step;
	if (offset1 >> 16 >= N){
		offset1 -= N<<16;
	}
	offset2 += step2;
	if (offset2 >> 16 >= N){
		offset2-= N<<16;
	}
	val = wavetable[offset1 >> 16];
	val2 = wavetable[offset2 >> 16];

	int sample = val + val2;
	sample = sample/32 + 2048;
	if (sample > 4095){
		sample = 4095;
	}
	else if (sample < 0){
		sample = 0;
	}

	//val ^= 0xfff;
	DAC -> DHR12R1 = sample;

}

int main(void)
{
    /* Student code goes here */
	init_wavetable();
    setup_gpio();
	setup_dac();
	setup_adc();
	setup_timer6();
	while(1);//{
//		a1 = read_adc_channel(0);
//		a2 = read_adc_channel(1);
//	}
	return 0;
}
