
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <math.h>
#include <stdint.h>
#include "midi.h"

// The DAC rate
#define RATE 30000
// The wavetable size
// #define N 1000
#define N 100
//short int wavetable[N];
short int sine[N];
short int sawtooth[N];
short int square[N];
short int hybrid[N];
short int hybrid2[N];

// The number of simultaneous voices to support.
#define VOICES 15

// N powers of the the 12th-root of 2.
#define STEP1 1.05946309436
#define STEP2 (STEP1*STEP1)
#define STEP3 (STEP2*STEP1)
#define STEP4 (STEP3*STEP1)
#define STEP5 (STEP4*STEP1)
#define STEP6 (STEP5*STEP1)
#define STEP7 (STEP6*STEP1)
#define STEP8 (STEP7*STEP1)
#define STEP9 (STEP8*STEP1)

// Macros for computing the fixed point representation of
// the step size used for traversing a wavetable of size N
// at a rate of RATE to produce tones of various doublings
// and halvings of 440 Hz.  (The "A" above middle "C".)
#define A14    ((13.75   * N/RATE) * (1<<16)) /* A0 */
#define A27    ((27.5    * N/RATE) * (1<<16)) /* A1 */
#define A55    ((55.0    * N/RATE) * (1<<16)) /* A2 */
#define A110   ((110.0   * N/RATE) * (1<<16)) /* A3 */
#define A220   ((220.0   * N/RATE) * (1<<16)) /* A4 */
#define A440   ((440.0   * N/RATE) * (1<<16)) /* A5 */
#define A880   ((880.0   * N/RATE) * (1<<16)) /* A6 */
#define A1760  ((1760.0  * N/RATE) * (1<<16)) /* A7 */
#define A3520  ((3520.0  * N/RATE) * (1<<16)) /* A8 */
#define A7040  ((7040.0  * N/RATE) * (1<<16)) /* A9 */
#define A14080 ((14080.0 * N/RATE) * (1<<16)) /* A10 */


// A table of steps for each of 128 notes.
// step[60] is the step size for middle C.
// step[69] is the step size for 440 Hz.
const int step[] = {
        A14 / STEP9,    // C                         C-1
        A14 / STEP8,    // C# / Db
        A14 / STEP7,    // D
        A14 / STEP6,    // D# / Eb
        A14 / STEP5,    // E
        A14 / STEP4,    // F
        A14 / STEP3,    // F# / Gb
        A14 / STEP2,    // G
        A14 / STEP1,    // G# / Ab
        A14,            // A27                       A0
        A14 * STEP1,    // A# / Bb
        A14 * STEP2,    // B
        A14 * STEP3,    // C                         C0
        A14 * STEP4,    // C# / Db
        A14 * STEP5,    // D
        A27 * STEP6,    // D# / Eb
        A27 / STEP5,    // E
        A27 / STEP4,    // F
        A27 / STEP3,    // F# / Gb
        A27 / STEP2,    // G
        A27 / STEP1,    // G# / Ab
        A27,            // A27                       A1
        A27 * STEP1,    // A# / Bb
        A27 * STEP2,    // B
        A27 * STEP3,    // C                         C1
        A27 * STEP4,    // C# / Db
        A27 * STEP5,    // D
        A27 * STEP6,    // D# / Eb
        A55 / STEP5,    // E
        A55 / STEP4,    // F
        A55 / STEP3,    // F# / Gb
        A55 / STEP2,    // G
        A55 / STEP1,    // G# / Ab
        A55,            // A55                       A2
        A55 * STEP1,    // A# / Bb
        A55 * STEP2,    // B
        A55 * STEP3,    // C                         C2
        A55 * STEP4,    // C# / Db
        A55 * STEP5,    // D
        A55 * STEP6,    // D# / Eb
        A110 / STEP5,   // E
        A110 / STEP4,   // F
        A110 / STEP3,   // F# / Gb
        A110 / STEP2,   // G
        A110 / STEP1,   // G# / Ab
        A110,           // A110                     A3
        A110 * STEP1,   // A# / Bb
        A110 * STEP2,   // B
        A110 * STEP3,   // C                        C3
        A110 * STEP4,   // C# / Db
        A110 * STEP5,   // D
        A110 * STEP6,   // D# / Eb
        A220 / STEP5,   // E
        A220 / STEP4,   // F
        A220 / STEP3,   // F# / Gb
        A220 / STEP2,   // G
        A220 / STEP1,   // G# / Ab
        A220,           // A220                     A4
        A220 * STEP1,   // A# / Bb
        A220 * STEP2,   // B
        A220 * STEP3,   // C (middle C)             C4 (element #60)
        A220 * STEP4,   // C# / Db
        A220 * STEP5,   // D
        A220 * STEP6,   // D# / Eb
        A440 / STEP5,   // E
        A440 / STEP4,   // F
        A440 / STEP3,   // F# / Gb
        A440 / STEP2,   // G
        A440 / STEP1,   // G# / Ab
        A440,           // A440                     A5
        A440 * STEP1,   // A# / Bb
        A440 * STEP2,   // B
        A440 * STEP3,   // C                        C5
        A440 * STEP4,   // C# / Db
        A440 * STEP5,   // D
        A440 * STEP6,   // D# / Eb
        A880 / STEP5,   // E
        A880 / STEP4,   // F
        A880 / STEP3,   // F# / Gb
        A880 / STEP2,   // G
        A880 / STEP1,   // G# / Ab
        A880,           // A880                     A6
        A880 * STEP1,   // A# / Bb
        A880 * STEP2,   // B
        A880 * STEP3,   // C                        C6
        A880 * STEP4,   // C# / Db
        A880 * STEP5,   // D
        A880 * STEP6,   // D# / Eb
        A1760 / STEP5,  // E
        A1760 / STEP4,  // F
        A1760 / STEP3,  // F# / Gb
        A1760 / STEP2,  // G
        A1760 / STEP1,  // G# / Ab
        A1760,          // A1760                   A7
        A1760 * STEP1,  // A# / Bb
        A1760 * STEP2,  // B
        A1760 * STEP3,  // C                       C7
        A1760 * STEP4,  // C# / Db
        A1760 * STEP5,  // D
        A1760 * STEP6,  // D# / Eb
        A3520 / STEP5,  // E
        A3520 / STEP4,  // F
        A3520 / STEP3,  // F# / Gb
        A3520 / STEP2,  // G
        A3520 / STEP1,  // G# / Ab
        A3520,          // A3520                   A8
        A3520 * STEP1,  // A# / Bb
        A3520 * STEP2,  // B
        A3520 * STEP3,  // C                       C8
        A3520 * STEP4,  // C# / Db
        A3520 * STEP5,  // D
        A3520 * STEP6,  // D# / Eb
        A7040 / STEP5,  // E
        A7040 / STEP4,  // F
        A7040 / STEP3,  // F# / Gb
        A7040 / STEP2,  // G
        A7040 / STEP1,  // G# / Ab
        A7040,          // A7040                   A9
        A7040 * STEP1,  // A# / Bb
        A7040 * STEP2,  // B
        A7040 * STEP3,  // C                       C9
        A7040 * STEP4,  // C# / Db
        A7040 * STEP5,  // D
        A7040 * STEP6,  // D# / Eb
        A14080 / STEP5, // E
        A14080 / STEP4, // F
        A14080 / STEP3, // F# / Gb
        A14080 / STEP2, // G
};

// An array of "voices".  Each voice can be used to play a different note.
// Each voice can be associated with a channel (explained later).
// Each voice has a step size and an offset into the wave table.
struct {
    uint8_t in_use;
    uint8_t note;
    uint8_t chan;
    uint8_t volume;
    int     step;
    int     offset;
    int 	last_sample;
    int16_t *wavetable;
} voice[VOICES];

// We'll use the Timer 6 IRQ to recompute samples and feed those
// samples into the DAC.


// Initialize the DAC so that it can output analog samples
// on PA4.  Configure it to either be software-triggered
// or triggered by TIM6.
void init_DAC(void) {

    // TODO: you fill this in.
	RCC -> APB1ENR |= RCC_APB1ENR_DACEN;
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

	// enable PA4 as Analog
	// set up gpio
	GPIOA -> MODER |= (3<<(4 * 2));

	// precausetion
	DAC -> CR &= ~DAC_CR_EN1;

	// init dac
	//DAC -> CR |= DAC_CR_TSEL1; // option1, config for software trigger
	DAC -> CR &= ~DAC_CR_TSEL1;// option 2 is to trigger the dac to transfer on each clock cycle of (tim6), tim3, or tim2
	//DAC -> CR &= ~DAC_CR_BOFF1; // enable the buff, changes impedence to remove need for op amp btwn speaker and MCU
	DAC -> CR |= DAC_CR_TEN1; // enable the trigger so that the data in holder is only transfered to odr on trigger
							  // default action is to transfer data to odr one clokc cycle after it is placed on the HDR

	//enable channal 1
	DAC -> CR |= DAC_CR_EN1;
}

// Initialize Timer 6 so that it calls TIM6_DAC_IRQHandler
// at exactly RATE times per second.  You'll need to select
// a PSC value and then do some math on the system clock rate
// to determine the value to set for ARR.
void init_TIM6(void) {
	// ENABLE CLOCK

	RCC -> APB1ENR |= RCC_APB1ENR_TIM6EN;

	// INIT TIMER 6
	TIM6 -> PSC = 1 - 1;
	TIM6 -> ARR = (48e6/((TIM6 -> PSC + 1) * RATE)) - 1;
	// psc = small because ARR or RATE has the potential to change
	// you dont want the timer to clip if value get too large... or the division to go to 0
	// becuase of a division error

	// ENABLE THE INTERUPT
	TIM6 -> DIER |= TIM_DIER_UDE; // trigger interrupt for the DMA not the tim6

	// ENABLE THE TIMER
	TIM6 -> CR2 |= TIM_CR2_MMS_1;
	TIM6 -> CR2 &= ~TIM_CR2_MMS_0;
	TIM6 -> CR2 &= ~TIM_CR2_MMS_2;

	TIM6 -> CR1 |= TIM_CR1_CEN;

    // TODO: you fill this in.
//	NVIC -> ISER[0] |= (1<<TIM6_DAC_IRQn);
//	NVIC_SetPriority(TIM6_DAC_IRQn, 0);
}

void init_TIM2(int sec){

	RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;

	// INIT TIMER 6
	TIM2 -> PSC = 48 - 1;
	TIM2 -> ARR = sec - 1; //make it 480 times faster//(48/((TIM2 -> PSC + 1)/((1)/sec))) - 1  = sec;
	TIM2 -> CR1 |= TIM_CR1_ARPE;
	// psc = small because ARR or RATE has the potential to change
	// you dont want the timer to clip if value get too large... or the division to go to 0
	// becuase of a division error

	// ENABLE THE INTERUPT
	TIM2 -> DIER |= TIM_DIER_UIE;

	// ENABLE THE TIMER
	TIM2 -> CR1 |= TIM_CR1_CEN;

	// TODO: you fill this in.
	NVIC -> ISER[0] |= (1<<TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 3);

}

void set_tempo(int time, int value, const MIDI_Header *hdr){
	TIM2 -> ARR = value/hdr -> divisions - 1;
}

void pitch_wheel_change(int time, int chan, int val){
    int x;

	float mult = pow(1.05946309436, (val - 8192.0)/8192.0);
	for(x=0; x < sizeof(voice)/sizeof(voice[0]); x++) {
		if (voice[x].in_use && voice[x].chan == chan) {
			voice[x].step = step[voice[x].note] * mult;
		}
	}
}

int16_t *channel_wavetable[20] = {0};
void program_change(int time, int chan, int prog)
{
	// prog will range from 0 ... 127
	// The standard MIDI sound are numbered 1 ... 128
	// (prog + 1) is the sound number.
    channel_wavetable[chan] = hybrid; // Use this by default.
    if (prog >= 71 && prog <= 79) // flutes, recorders, etc.
        channel_wavetable[chan] = sine;
    if (prog == 29) // overdriven guitar
        channel_wavetable[chan] = square;
    if (prog >= 52 && prog <= 54) // choir voices
        channel_wavetable[chan] = sine;
}

int time = 0;
int n = 0;
void TIM2_IRQHandler(void)
{
    // TODO: Remember to acknowledge the interrupt here!
	TIM2 -> SR &= ~TIM_SR_UIF;
	midi_play();
}

// sine wave
void init_sine(void) {
    int x;
    for(x=0; x<N; x++) {
    	//wavetable[x] = 32767 * sin(2 * M_PI * x / N);
        sine[x] = 32767 * sin(2 * M_PI * x / N);
    }
}

// sawtooth wave
void init_sawtooth(void) {
    int x;
    for(x=0; x<N; x++)
        //wavetable[x] = 32767.0 * (x - N/2) / (1.0*N);
    	sawtooth[x] = 32767.0 * (x - N/2) / (1.0*N);
}

// square wave
void init_square(void) {
    int x;
    for(x=0; x<N; x++)
        if (x < N/2){
            //wavetable[x] = -32767;
        	square[x] = -32767;
        }else{
            //wavetable[x] = 32767;
        	square[x] = 32767;
        }
}

// 1/2 amplitude sine wave added to 1/2 amplitude sawtooth wave
void init_hybrid(void) {
    int x;
    for(x=0; x<N; x++)
        //wavetable[x] = 16383 * sin(2 * M_PI * x / N) + 16383.0 * (x - N/2) / (1.0*N);
    	hybrid[x] = 16383 * sin(2 * M_PI * x / N) + 16383.0 * (x - N/2) / (1.0*N);
}

// 3/4 amplitude sine wave added to 1/4 amplitude sawtooth wave
void init_hybrid2(void) {
    int x;
    for(x=0; x<N; x++)
        //wavetable[x] = 3*8191 * sin(2 * M_PI * x / N) + 8191.0 * (x - N/2) / (1.0*N);
    	hybrid2[x] = 3*8191 * sin(2 * M_PI * x / N) + 8191.0 * (x - N/2) / (1.0*N);
}

// Find the voice current playing a note, and turn it off.
void note_off(int time, int chan, int key, int velo)
{
    int n;
    for(n=0; n<sizeof voice / sizeof voice[0]; n++) {
        if (voice[n].in_use && voice[n].note == key) {
            voice[n].in_use = 0; // disable it first...
            voice[n].chan = 0;   // ...then clear its values
            voice[n].note = key;
            voice[n].step = step[key];
//            voice[n].volume = 0;
            return;
        }
    }
}


// Find an unused voice, and use it to play a note.
void note_on(int time, int chan, int key, int velo)
{
    if (velo == 0) {
        note_off(time, chan, key, velo);
        return;
    }
    int n;
    for(n=0; n<sizeof voice / sizeof voice[0]; n++) {
    	if (channel_wavetable[chan] == 0){
    		voice[n].wavetable = hybrid;
    	}else{
    		voice[n].wavetable = channel_wavetable[chan];
    	}
        if (voice[n].in_use == 0) {
            voice[n].note = key;
            voice[n].step = step[key];
            voice[n].offset = 0;
            voice[n].chan = chan;
            voice[n].in_use = 1;
            voice[n].volume = velo;
            return;
        }
    }
}


uint16_t queue[200]; // the circular buffer
//uint16_t queue[100]; // the circular buffer
void DMA1_Channel2_3_IRQHandler(void){
	uint16_t *arr;
	if ((DMA1->ISR & DMA_ISR_HTIF3)) {
		// At halfway mark.  Refill the first half of the buffer.
		arr = &queue[0]; // point to beginning of buffer
		// Need to clear both the half-transfer and global flags.
		DMA1->IFCR = DMA_IFCR_CHTIF3|DMA_IFCR_CGIF3;
	} else {
		// Total complete.  Refill the second half of the buffer.
		arr = &queue[100]; // point to second half of buffer
		//arr = &queue[50];
		// Need to clear both the transfer-complete and global flags.
		DMA1->IFCR = DMA_IFCR_CTCIF3|DMA_IFCR_CGIF3;
	}

	uint16_t i;
	for(i = 0; i < 100; i++) {
	//for(i = 0; i < 50; i++) {
// TODO: Copy the sample generation from TIM6_DAC_IRQHandler()
	    uint16_t x;
	    int sample = 0;
	    for(x=0; x < sizeof(voice)/sizeof(voice[0]); x++) {
	    	if (voice[x].in_use) {
	            voice[x].offset += voice[x].step;
	            if (voice[x].offset >= N<<16)
	                voice[x].offset -= N<<16;
	            voice[x].last_sample = voice[x].wavetable[voice[x].offset>>16];
	            sample += (voice[x].last_sample * voice[x].volume) >> 4;
	        }
	    	else if (voice[x].volume != 0){
	    		sample += (voice[x].last_sample * voice[x].volume) >> 4;
	    		voice[x].volume --;
	    	}
	    }
	    //sample = sample / 128 + 2048;
	    sample = sample / 1024 + 2048;
	    if (sample > 4095) sample = 4095;
	    else if (sample < 0) sample = 0;
		arr[i] = sample;
	}
}

void init_DMA(){
	RCC -> AHBENR |= RCC_AHBENR_DMA1EN;

	DMA1_Channel3 ->CCR &= ~DMA_CCR_EN;
	DMA1_Channel3 ->CMAR = (uint32_t)queue;
	DMA1_Channel3 ->CPAR = (uint32_t)&DAC -> DHR12R1;
	DMA1_Channel3 ->CNDTR = sizeof(queue)/sizeof(queue[0]);
	DMA1_Channel3 ->CCR |= DMA_CCR_DIR; // read from memory
	DMA1_Channel3 ->CCR |= DMA_CCR_CIRC; // circular mode
	DMA1_Channel3 ->CCR |= DMA_CCR_MINC; // increment the address of the memory
	DMA1_Channel3 ->CCR |= DMA_CCR_PSIZE_0; // set the size to 16 bits
	DMA1_Channel3 ->CCR &= ~DMA_CCR_PSIZE_1;
	DMA1_Channel3 ->CCR |= DMA_CCR_MSIZE_0; // set the size to 16 bits
	DMA1_Channel3 ->CCR &= ~DMA_CCR_MSIZE_1;
	DMA1_Channel3 ->CCR |= DMA_CCR_HTIE; //half transfer interrupt
	DMA1_Channel3 ->CCR |= DMA_CCR_TCIE; // full transfer interrupt

	NVIC -> ISER[0] |= 1 << DMA1_Channel2_3_IRQn; //un-mask the interrupt
	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0); // set to highest priority

	DMA1_Channel3 ->CCR |= DMA_CCR_EN; // enable
	return;
}


extern uint8_t midifile[];
int main(void)
{
    init_sawtooth();
    init_hybrid();      // pick one wavetable form
    init_hybrid2();
    init_square();
    init_sine();

    init_DAC();   // initialize the DAC
    init_DMA();
    init_TIM6();  // initialize TIM6
    //init_TIM2(500000);

    MIDI_Player * mp = midi_init(midifile);
    init_TIM2(10417);


    for(;;)
        asm("wfi");
    return 0;
}

