#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include "aipointe.h"
#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif

#ifndef _PGMSPACE_H_
#define _PGMSPACE_H_

#include <inttypes.h>

#define pgm_read_byte(addr) (*(addr))
#define _BV(bit) (1 << (bit))

#define utoa(a,b,c)
#define itoa(a,b,c)
#define ltoa(a,b,c)
#define ultoa(a,b,c)

#endif

/*
   wiringPi pin 0  is BCM_GPIO 17
   wiringPi pin 1  is BCM_GPIO 18
   wiringPi pin 2  is BCM_GPIO 27
   wiringPi pin 3  is BCM_GPIO 22
   wiringPi pin 4  is BCM_GPIO 23
   wiringPi pin 5  is BCM_GPIO 24
   wiringPi pin 6  is BCM_GPIO 25
   wiringPi pin 7  is BCM_GPIO 4
   wiringPi pin 8  is BCM_GPIO 2
   wiringPi pin 9  is BCM_GPIO 3
   wiringPi pin 10 is BCM_GPIO 8
   wiringPi pin 11 is BCM_GPIO 7
   wiringPi pin 12 is BCM_GPIO 10
   wiringPi pin 13 is BCM_GPIO 9
   wiringPi pin 14 is BCM_GPIO 11
   wiringPi pin 15 is BCM_GPIO 14
   wiringPi pin 16 is BCM_GPIO 15
   */
/*
#define HIGH 1
#define LOW 0
*/

/*
   wiringPi pin 6  is BCM_GPIO 25
   wiringPi pin 5  is BCM_GPIO 24
   wiringPi pin 4  is BCM_GPIO 23
   wiringPi pin 3  is BCM_GPIO 22
   */
#define CS0       6
#define CS1       5
#define CS2       4
#define CS3       3
//#define CS_ENABLE 0

// HT1632C PINs

#define DISPLAY_WR   2     /* BCM_GPIO 27 */
#define DISPLAY_DATA 0     /* BCM_GPIO 17 */

// HT1632C Commands

#define HT1632C_READ 0b00000110
#define HT1632C_WRITE 0b00000101
#define HT1632C_CMD 0b00000100

#define HT1632_CMD_SYSON  0x01
#define HT1632_CMD_LEDON  0x03
#define HT1632_CMD_SYSDIS 0x00
#define HT1632_CMD_MASTER 0x18
#define HT1632_CMD_SLAVE  0x10

//byte buffer[32];

#define MATRIX struct _matrix_

MATRIX {
    unsigned char cs;
    unsigned char addr;

};


void ht1632c_clear_display();
void ht1632c_send_command(unsigned char command, unsigned char id);
void ht1632c_send_bits(unsigned char bits, unsigned char firstbit);
void displayChar(int incomingByte);
void chipSelect(char id);
int initPi();

int initPi()
{
    if (wiringPiSetup() == -1)
    {
        printf("IO is not ready\n");
        return 0;
    }

    // All PINs are output
    pinMode(CS0, OUTPUT);
    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);
    pinMode(CS3, OUTPUT);

    pinMode(DISPLAY_WR, OUTPUT);
    pinMode(DISPLAY_DATA, OUTPUT);

    return 1;
}

void chipSelect(char id)
{
    digitalWrite(CS0, HIGH);
    digitalWrite(CS1, HIGH);
    digitalWrite(CS2, HIGH);
    digitalWrite(CS3, HIGH);
    if (id<0)
        return;

    if (id == 0)
        digitalWrite(CS0, LOW);
    if (id == 1)
        digitalWrite(CS1, LOW);
    if (id == 2)
        digitalWrite(CS2, LOW);
    if (id == 3)
        digitalWrite(CS3, LOW);
}


void initMatrix(unsigned char id)
{
    // Enable System oscillator and LED duty cycle generator
    /*if (id==0)ht1632c_send_command(HT1632_CMD_MASTER, id);
    else ht1632c_send_command(HT1632_CMD_SLAVE, id);*/
    ht1632c_send_command(HT1632_CMD_SYSON, id);
    ht1632c_send_command(HT1632_CMD_LEDON, id);
}

void init() {
    unsigned char i;

    // init GPIO
    if (!initPi()) return;
    chipSelect(-1);

    for(i=0; i<4; i++)
        initMatrix(i);

    // clear display
    ht1632c_clear_display();
}

void loop(int* i) {

    // send data
    displayChar(*i);
    usleep(1000000);
    if (*i<94) (*i)++;
    else *i=0;
}

void displayChar(int incomingByte)
{
    int i;
    // get font details
    int infoPosition = incomingByte;
    unsigned char length = aipoint_info[infoPosition].length;
    unsigned int offset = aipoint_info[infoPosition].offset;
    //printf("pos %d - length %d - offset %d\n", infoPosition, length, offset);

    // select display
    chipSelect(0);  

    // send WRITE command
    ht1632c_send_bits(HT1632C_WRITE, 1 << 2);

    // send start address, 00h
    ht1632c_send_bits(0x00, 1 << 6);

    for(i = 0; i < 32; i++) {
        if (i < length) {
            char mychar = pgm_read_byte(aipointe_font + offset + i);
            ht1632c_send_bits(mychar, 1<<7);
        }
        else ht1632c_send_bits(0x00, 1<<7);
    }

    // unselect display
    chipSelect(-1);  
}

// ----------------------------------------
// HT1632C functions
// ----------------------------------------

void ht1632c_clear_display()
{
    int i;
    int j;
    for(i=0; i<4; i++) {
        chipSelect(i);  
        ht1632c_send_bits(HT1632C_WRITE, 1 << 2);
        ht1632c_send_bits(0x00, 1 << 6);
        for(j = 0; j < 32; j++) ht1632c_send_bits(0x00, 1<<7);
        chipSelect(-1);  
    }
}

void ht1632c_send_command(unsigned char command, unsigned char id) 
{
    printf("Sending command %02X\n", command);
    chipSelect(id);  
    //digitalWrite(DISPLAY_CS, LOW);
    ht1632c_send_bits(HT1632C_CMD, 1 << 2);
    ht1632c_send_bits(command, 1 << 7);
    ht1632c_send_bits(0, 1);
    chipSelect(-1);
    //digitalWrite(DISPLAY_CS, HIGH);
}

void ht1632c_send_bits(unsigned char bits, unsigned char firstbit) {

    while(firstbit) {
        digitalWrite(DISPLAY_WR, LOW);
        if (bits & firstbit) digitalWrite(DISPLAY_DATA, HIGH);
        else digitalWrite(DISPLAY_DATA, LOW);
        digitalWrite(DISPLAY_WR, HIGH);
        firstbit >>= 1;
    }
}

int main()
{
    int i=0;
    printf("Setup\n");
    init();
    //chipSelect(0);return 0;
    printf("Loop\n");
    while(1) {
        loop(&i);
    }
    printf("End\n");
    return 0;
}

/* vim:expandtab ts=4 sw=4 nu:
*/
