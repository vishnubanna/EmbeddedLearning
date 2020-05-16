#ifndef _GAME_H
#define _GAME_H
#include <stdint.h>

struct fifo {
    char buffer[128]; // circular buffer for the fifo
    volatile uint8_t head; // the first thing to remove from the fifo
    volatile uint8_t tail; // the next place to insert a new character
    volatile uint8_t newline; // offset of last-entered newline
};

int fifo_empty(const struct fifo *f);
int fifo_full(const struct fifo *f);
void fifo_insert(struct fifo *f, char ch);
char fifo_uninsert(struct fifo *f);
int fifo_newline(const struct fifo *f);
char fifo_remove(struct fifo *f);

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdarg.h>

#define UNK -1
#define NON_INTR 0
#define INTR 1

void tty_init(void);
void tty_uninit(void);
int  available(void);
int  getkey(void);
void raw_mode(void);
void cooked_mode(void);
int __io_putchar(int ch);


// Put the cursor in the upper left.
void home(void);

// Clear the screen.
void clear(void);

// Set the cursor position to x,y (zero-based indexing)
void setpos(int x, int y);

// Turn the cursor off.
void cursor_off(void);

// Turn the cursor on.
void cursor_on(void);

// Set the foreground color to rgb.
// Each component must have the value 0 ... 5.
void fg(int r, int g, int b);

// Set the foreground color to rgb.
// Each component must have the value 0 ... 5.
void bg(int r, int g, int b);

// Set the foreground and background to the byte specified by n.
// The most significant nibble is the foreground.
// The least significant nibble is the background.
// The bits in each nibble specifie:
//  IBGR
//  I    High intensity
//   B   Blue
//    G  Green
//     R Red
//
void fgbg(int n);


typedef struct {
  const uint8_t *file_start;
  int length;
  uint8_t format;
  uint8_t tracks;
  uint16_t divisions;
} MIDI_Header;

//#define MAXTICKS 0x10000000
#define MAXTICKS 0x1000000
typedef struct {
  int ticks;
  int nexttick;
} MIDI_Player;

// The core functions to interpret and play MIDI file contents...
MIDI_Player *midi_init(const uint8_t *array);
void midi_play(void);

// Functions you may implement to handle events in the real world.
void set_tempo(int time, int value, const MIDI_Header *hdr);
void note_off(int time, int chan, int key, int velo);
void note_on(int time, int chan, int key, int velo);

void key_pressure(int time, int chan, int key, int val);
void control_change(int time, int chan, int ctrl, int val);
void program_change(int time, int chan, int prog);
void channel_pressure(int time, int chan, int val);
void pitch_wheel_change(int time, int chan, int val);
void text_event(const char *str, int len);
void copyright_notice(const char *str, int len);
void track_name(const char *str, int len);
void lyric(int time, const char *str, int len);
void marker(int time, const char *str, int len);
void end_of_track(int time);
void smpte_offset(int time, int hr, int mn, int se, int fr, int ff);
void time_signature(int time, int n, int d, int b, int c);
void key_signature(int time, int8_t sf, int mi);

// Debugging.
void midi_error(int time, const uint8_t *p, const uint8_t *file_start, const char *msg);
void unknown_meta(int time, int offset, int type, int len, const uint8_t *buf);
void header_info(MIDI_Header *hdr);
void file_offset_info(int time);
void track_info(int num, const uint8_t *p, int len, const MIDI_Header *hdr);
void sysex_info(int time, const uint8_t *p, const uint8_t *buf, int len,
                const MIDI_Header *hdr);



#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h> // for random()
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

/********************
 * Game requirements
 * memory-screen: 2080/8000
 * memory-player: 14/8000
 *******************/

#define BOUNDX 80
#define BOUNDY 13
#define FLOOR BOUNDY-2-1
#define PSIZEX 6
#define PSIZEY 3
#define RUNNING 1
#define SPLASH 0


#define PH "(*_*)"
#define B1 "r)+)v"
#define L1 " / > "

#define B2 "r)x)v"
#define L2 " > \\ "

#define DUCK 1
#define RUN 0



/******************************
 * total alocated memory this file
 * memory: 1858 bytes allocated + 124 static for pointer and definitions
 ******************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <math.h>

#define RATE 20000
#define N 100

/********************
 * DMA requirements
 * memory: 400/8000
 *******************/

/******************************
 * music play back requirements
 * memory: 1458/8000
 ******************************/
// allowing 3 instruments may reduce memory use
//short int square[N];
//short int sine[N];
//short int hybrid[N];
//short int hybrid2[N];
//short int sawtooth[N];
short int wavetable[N];

#define STEP1 1.05946309436
#define STEP2 (STEP1*STEP1)
#define STEP3 (STEP2*STEP1)
#define STEP4 (STEP3*STEP1)
#define STEP5 (STEP4*STEP1)
#define STEP6 (STEP5*STEP1)
#define STEP7 (STEP6*STEP1)
#define STEP8 (STEP7*STEP1)
#define STEP9 (STEP8*STEP1)

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
#define VOICES 15

void init_square();
void init_sin();
void init_hybrid();
void init_TIM6(void);
void init_TIM3(int sec);
void init_DMA();
void init_DAC(void);
void init_music();
void re_song();


void init(void);
void render(void);
void player_act();
void update(char action);
void print_obs();
void gencure();
void genobs();


/***********************
 * total memory
 * allocated 274 bytes
 * *********************/







#endif

