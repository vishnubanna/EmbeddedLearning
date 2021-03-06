#include "game.h"

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

//extern uint8_t midifile[]; // arbitrary define after all initializations
uint16_t queue[200];



// sine wave
//void init_sine(void) {
//    int x;
//    for(x=0; x<N; x++) {
//    	//wavetable[x] = 32767 * sin(2 * M_PI * x / N);
//        sine[x] = 32767 * sin(2 * M_PI * x / N);
//    }
//}

//// sawtooth wave
//void init_sawtooth(void) {
//    int x;
//    for(x=0; x<N; x++)
//        //wavetable[x] = 32767.0 * (x - N/2) / (1.0*N);
//    	sawtooth[x] = 32767.0 * (x - N/2) / (1.0*N);
//}

// square wave
//void init_square(void) {
//    int x;
//    for(x=0; x<N; x++)
//        if (x < N/2){
//            //wavetable[x] = -32767;
//        	square[x] = -32767;
//        }else{
//            //wavetable[x] = 32767;
//        	square[x] = 32767;
//        }
//}

// 1/2 amplitude sine wave added to 1/2 amplitude sawtooth wave
void init_hybrid(void) {
    int x;
    for(x=0; x<N; x++)
        wavetable[x] = 16383 * sin(2 * M_PI * x / N) + 16383.0 * (x - N/2) / (1.0*N);
    	//hybrid[x] = 16383 * sin(2 * M_PI * x / N) + 16383.0 * (x - N/2) / (1.0*N);
}

//// 3/4 amplitude sine wave added to 1/4 amplitude sawtooth wave
//void init_hybrid2(void) {
//    int x;
//    for(x=0; x<N; x++)
//        //wavetable[x] = 3*8191 * sin(2 * M_PI * x / N) + 8191.0 * (x - N/2) / (1.0*N);
//    	hybrid2[x] = 3*8191 * sin(2 * M_PI * x / N) + 8191.0 * (x - N/2) / (1.0*N);
//}

/***********************************************************************
 * initialize GPIO PA4 for analog
 * Configure DAC to automatically transfer data on the next clock cycle
 * enable the trigger based data transfer
 * enable the DAC
 ************************************************************************/
void init_DAC(void) {
	RCC -> APB1ENR |= RCC_APB1ENR_DACEN;
	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER |= (3<<(4 * 2));

	DAC -> CR &= ~DAC_CR_EN1;
	DAC -> CR &= ~DAC_CR_TSEL1;
	//DAC -> CR &= ~DAC_CR_BOFF1;
	DAC -> CR |= DAC_CR_TEN1;
	DAC -> CR |= DAC_CR_EN1;
}

/***********************************************************************
 * initialize TIM6 -> Timer 6 will be use for DMA
 * set a low PSC to allow operation at high RATE
 * configure the clock to run at RATE times per second
 * enable TIM6 to trigger an interrupt for DMA
 * Do not set interrupt priority for timer 6 because the TIM6_IRQ should
 * 		be triggered, DMA_IRQ is triggered instead
 ************************************************************************/
void init_TIM6(void) {
	// ENABLE CLOCK
	RCC -> APB1ENR |= RCC_APB1ENR_TIM6EN;
	// INIT TIMER 6
	TIM6 -> PSC = 1 - 1;
	TIM6 -> ARR = (48e6/((TIM6 -> PSC + 1) * RATE)) - 1;

	// ENABLE THE INTERUPT FOR DMA
	TIM6 -> DIER |= TIM_DIER_UDE;
	TIM6 -> CR2 |= TIM_CR2_MMS_1;
	TIM6 -> CR2 &= ~TIM_CR2_MMS_0;
	TIM6 -> CR2 &= ~TIM_CR2_MMS_2;

	// ENABLE THE TIMER
	TIM6 -> CR1 |= TIM_CR1_CEN;
}

/*************************************************************************************
 * initialize TIM3 -> Timer 3 will be used to compute everything needed to play a song
 * input: speed, the system will play a song at 1e6/speed times per second
 *
 * enable ARPE to allow value of ARR to change as externally
 *************************************************************************************/
void init_TIM3(int count){

	RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN;

	// INIT TIMER 2
	TIM3 -> PSC = 48 - 1;
	TIM3 -> ARR = count - 1; //make it 480 times faster//(48/((TIM3 -> PSC + 1)/((1)/count))) - 1  = count;
	TIM3 -> CR1 |= TIM_CR1_ARPE; // ARR is pre-loaded, so writing to or reading from this reg will mean that on the next
								 // clock cycle, the value of the preload register  will be updated
								 // allows you to change clock rate, i.e set the tempo

	// ENABLE THE INTERUPT
	TIM3 -> DIER |= TIM_DIER_UIE;

	// ENABLE THE TIMER
	TIM3 -> CR1 |= TIM_CR1_CEN;

	// TODO: you fill this in.
//	NVIC -> ISER[0] |= (1<<TIM3_IRQn);
//	NVIC_SetPriority(TIM3_IRQn, 3);
}

void set_tempo(int time, int value, const MIDI_Header *hdr){
	if (value/hdr -> divisions - 1 <= 65535)
		TIM3 -> ARR = value/hdr -> divisions - 1;
	else
		TIM3 -> ARR = 65535;
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


//void program_change(int time, int chan, int prog)
//{
//	// prog will range from 0 ... 127
//	// The standard MIDI sound are numbered 1 ... 128
//	// (prog + 1) is the sound number.
//    channel_wavetable[chan] = hybrid; // Use this by default.
//    if (prog >= 71 && prog <= 79) // flutes, recorders, etc.
//        channel_wavetable[chan] = sine;
//    if (prog == 29) // overdriven guitar
//        channel_wavetable[chan] = square;
//    if (prog >= 52 && prog <= 54) // choir voices
//        channel_wavetable[chan] = sine;
//}

int time = 0;
int n = 0;

void TIM3_IRQHandler(void)
{
    // TODO: Remember to acknowledge the interrupt here!
	TIM3 -> SR &= ~TIM_SR_UIF;
	midi_play();
//    if (mp->nexttick >= MAXTICKS)
//        mp = midi_init(midifile);
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
//		if (channel_wavetable[chan] == 0){
//    		voice[n].wavetable = hybrid;
//    	}else{
//    		voice[n].wavetable = channel_wavetable[chan];
//    	}
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


 // the circular buffer
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
	            //voice[x].last_sample = voice[x].wavetable[voice[x].offset>>16];
	            voice[x].last_sample = wavetable[voice[x].offset>>16];
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

//	NVIC -> ISER[0] |= 1 << DMA1_Channel2_3_IRQn; //un-mask the interrupt
//	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 2); // set to highest priority

	DMA1_Channel3 ->CCR |= DMA_CCR_EN; // enable
	return;
}


void init_music(){
    //init_sawtooth();
          // pick one wavetable form
    //init_hybrid2();
//    init_square();
//    init_sine();
	init_hybrid();
    init_DAC();   // initialize the DAC
    init_DMA();
    init_TIM6();  // initialize TIM6
}
//
//void end_of_track(int time){
//	if (mp -> nexttick >= MAXTICKS)
//		mp = midi_init(midifile);
//	return;
//}
