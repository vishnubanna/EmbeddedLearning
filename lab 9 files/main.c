#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "fifo.h"

#define FAIL -1
#define SUCCESS 0
#define WR 0
#define RD 1

void serial_init(void);
void test_prob3(void);
void check_config(void);
void test_wiring(void);
void micro_wait(unsigned int);
uint8_t read_EEPROM(uint16_t);

//===========================================================================
// Check wait for the bus to be idle.
void I2C1_waitidle(void) {
    while ((I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY);  // while busy, wait.
}

int I2C1_checknack(void) {
    if (I2C1->ISR & I2C_ISR_NACKF)
        return 1;
    return 0;
}

void I2C1_clearnack(void) {
    I2C1->ICR |= I2C_ICR_NACKCF;
}

//===========================================================================
// Subroutines for step 2.
//===========================================================================
// Initialize I2C1
/*
1.  Enable clock to GPIOB
2.  Configure PB6 and PB7 to alternate functions I2C1_SCL and I2C1_SDA
3.  Enable clock to I2C1
4.  Set I2C1 to 7 bit mode
5.  Enable NACK generation for I2C1
6.  Configure the I2C1 timing register so that PSC is 6, SCLDEL is 3 and SDADEL is 1 and SCLH is 3 and SCLL is 1
7.  Disable own address1 and own address 2 and set the 7 bit own address to 1
8.  Enable I2C1
 */
void init_I2C1() {
    // Student code goes here

    //---------End-----------
}


//===========================================================================
// Subroutines for step 3.
//===========================================================================
// See lab document for description
void I2C1_start(uint8_t addr, uint32_t dir) {
    // Student code goes here

    //---------End-----------
}

// See lab document for description
void I2C1_stop() {
    // Student code goes here

    //---------End-----------
}

// See lab document for description
int I2C1_senddata(uint8_t* data, uint32_t size) {
    // Student code goes here

    //---------End-----------
}

// See lab document for description
int I2C1_readdata(int8_t* data, uint32_t size) {
    // Student code goes here

    //---------End-----------
}

//===========================================================================
// Subroutines for step 4.
//===========================================================================
// See lab document for description
void write_EEPROM(uint16_t wr_addr, uint8_t data) {
    // Student code goes here

    //---------End-----------
}

//===========================================================================
// Subroutines for step 5.
//===========================================================================
// See lab document for description
void set_gpio(int pin) {
    // Student code goes here

    //---------End-----------
}

// See lab document for description
void clear_gpio(int pin) {
    // Student code goes here

    //---------End-----------
}

//===========================================================================
// End of student code.
//===========================================================================

void prob2() {
    init_I2C1();
    check_config();
}

void prob3() {
    test_prob3();
}

int eeprom_addr = 0x00;

// Act on a command read by prob4().
static void action(char **words) {
    if (words[0] != 0) {
        if (strcasecmp(words[0],"mv") == 0) {
            eeprom_addr = strtol(words[1], NULL, 16);
            printf("Address 0x%x\n", eeprom_addr);
            return;
        }
        if (strcasecmp(words[0],"rd") == 0) {
        	printf("Read value is: 0x%x\n", read_EEPROM(eeprom_addr));
            return;
        }
        if (strcasecmp(words[0],"wr") == 0) {
			int data = strtol(words[1], NULL, 16);
			write_EEPROM(eeprom_addr, data);
			printf("Cursor at 0x%x\n", eeprom_addr);
			printf("Writing successful\n");
			return;
        }

        printf("Unrecognized command: %s\n", words[0]);
    }
}

//===========================================================================
// Interact with the hardware.
// This subroutine waits for a line of input, breaks it apart into an
// array of words, and passes that array of words to the action()
// subroutine.
// The "display1" and "display2" are special words that tell it to
// keep everything after the first space together into words[1].
//
void prob4(void) {
	init_I2C1();
    printf("\nSTM32 EEPROM testbench.\n");
    printf("Interactive EEPROM read write tool\n");
    printf("- mv 0x\"address\", to move the cursor to move the cursor to address(hex)\n");
    printf("- rd to read data at cursor address\n");
    printf("- wr 0x\"data\" to write data at cursor\n");
    for(;;) {
        char buf[60];
        printf("> ");
        fgets(buf, sizeof buf - 1, stdin);
        int sz = strlen(buf);
        if (sz > 0)
            buf[sz-1] = '\0';
        char *words[7] = { 0,0,0,0,0,0,0 };
        int i;
        char *cp = buf;
        for(i=0; i<6; i++) {
            // strtok tokenizes a string, splitting it up into words that
            // are divided by any characters in the second argument.
            words[i] = strtok(cp," \t");
            // Once strtok() is initialized with the buffer,
            // subsequent calls should be made with NULL.
            cp = 0;
            if (words[i] == 0)
                break;
            if (i==0 && strcasecmp(words[0], "mv") == 0) {
                words[1] = strtok(cp, ""); // words[1] is rest of string
                break;
            }
            if (i==0 && strcasecmp(words[0], "rd") == 0) {
                break;
            }
            if (i==0 && strcasecmp(words[0], "wr") == 0) {
                words[1] = strtok(cp, ""); // words[1] is rest of string
                break;
            }
        }
        action(words);
    }
}

void prob5() {
	init_I2C1();
	while(1) {
		set_gpio(0);
		micro_wait(1000000);
		clear_gpio(0);
		micro_wait(1000000);
	}
}

int main(void)
{
	serial_init();

	//Open the serial terminal to see messages for each part
    //test_wiring();
    //prob2();
    //prob3();
    prob4();
    //prob5();
	while(1)
	    ;
}
