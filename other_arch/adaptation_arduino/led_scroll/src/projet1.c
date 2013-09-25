#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <unistd.h>
#include <time.h>
#include "aipointe.h"
#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif

#ifndef _PGMSPACE_H_
#define _PGMSPACE_H_

#include <inttypes.h>

typedef const unsigned char prog_uchar;
#define pgm_read_byte_near(x) (*(prog_uchar*)x)
#define pgm_read_byte(addr) (*(addr))

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

#define SPACING_SIZE      1      // space between letters
#define SPACE_SIZE        3      // "space" character length
#define BLANK_SIZE        7      // blank columns before restarting the string
#define SCROLLING_SPEED   250    // scrolling speed in ms

#define TEXT_BUFFER_SIZE  100    // max characters in string

// FSM states

#define S_OFF             0
#define S_LETTER          1
#define S_SPACING         2
#define S_SPACE           3
#define S_BLANK           4


void ht1632c_clear_display();
void ht1632c_send_command(unsigned char command, unsigned char id);
void ht1632c_send_bits(unsigned char bits, unsigned char firstbit);
void get_letter_variables();
void scroll();
void updateDisplay(unsigned char new_byte);
void ht1632c_display_buffer(unsigned char* buffer);
void chipSelect(char id);
int initPi();

unsigned char display_buffer[32];
char text_buffer[TEXT_BUFFER_SIZE];
char display_string[TEXT_BUFFER_SIZE];

unsigned char actual_state;
unsigned char letter_info;
unsigned char letter_length;
unsigned int letter_offset;
unsigned char letter_position;
unsigned char column_position;
unsigned char text_position;
unsigned char buffer_position;
unsigned char space_count;
long previous_millis;

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

    // clear buffer
    for (i = 0; i < 32; i++) display_buffer[i] = 0x00;  
  
    // reset FSM state and timer
    actual_state = S_OFF;
    previous_millis = 0;
}

void loop() {

    long current_millis = millis();
    // Time to scroll?
    if (current_millis - previous_millis > SCROLLING_SPEED) {
        previous_millis = current_millis;
        printf("millis: %d\n", previous_millis);
        scroll();
    }

}

void get_letter_variables() {
  
  // Get letter information from font descriptor
  printf("letter_pos: %d\n", letter_position);
  letter_info = display_string[letter_position] - 33;
  letter_length = aipoint_info[letter_info].length;
  letter_offset = aipoint_info[letter_info].offset;
  printf("letter: %d\n", letter_info);
}

void scroll() {

    printf("scrool %d\n", actual_state);
    unsigned char new_byte;

    switch(actual_state) {

        case S_OFF:
            return;

        case S_LETTER:

            // End of string reached?
            if(display_string[letter_position] == '\0') {
                printf("EOS\n");
                new_byte = 0x00;
                column_position = 1;
                actual_state = S_BLANK;
                break;
            }

            // Character to be displayed is space?
            if(display_string[letter_position] == ' ') {
                printf("Space\n");
                new_byte = 0x00;
                column_position = 1;
                actual_state = S_SPACE;
                break;
            }

            new_byte = pgm_read_byte_near(aipointe_font + letter_offset/* + column_position*/);
            column_position++;
            if(column_position == letter_length) {
                column_position = 0;
                actual_state = S_SPACING;
            }
            break;

            // End of character reached? Send space
        case S_SPACING:
            new_byte = 0x00;
            column_position++;
            if(column_position == SPACING_SIZE)  {
                column_position = 0;
                letter_position++;
                get_letter_variables();
                actual_state = S_LETTER;
            }
            break;

            // Send "space" character
        case S_SPACE:
            new_byte = 0x00;
            column_position++;
            if(column_position == SPACE_SIZE)  {
                column_position = 0;
                letter_position++;
                get_letter_variables();
                actual_state = S_LETTER;
            }
            break;

            // Send "blank" before the next string  
        case S_BLANK:

            new_byte = 0x00;
            column_position++;
            if(column_position == BLANK_SIZE) {
                letter_position = 0;
                column_position = 0;        
                get_letter_variables();
                actual_state = S_LETTER;
            }    
            break;    
    }

    updateDisplay(new_byte);
    printf("End scroll\n");
}

void updateDisplay(unsigned char new_byte) {

    printf("updateDisplay\n");
    int i;
    display_buffer[buffer_position] = new_byte;
    buffer_position = (buffer_position + 1) % 32;


    chipSelect(0);  
    ht1632c_send_bits(HT1632C_WRITE, 1 << 2);
    ht1632c_send_bits(0x00, 1 << 6);

    for(i = 0; i < 32; i++) {
        ht1632c_send_bits(display_buffer[(i + buffer_position) % 32], 1<<7);  
    }
    chipSelect(-1);
    printf("End updateDisplay\n");
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

void ht1632c_display_buffer(unsigned char* buffer) {
    int i;
    chipSelect(0);  
    ht1632c_send_bits(HT1632C_WRITE, 1 << 2);
    ht1632c_send_bits(0x00, 1 << 6);
    for(i = 0; i < 32; i++) ht1632c_send_bits(buffer[i], 1<<7);
    chipSelect(-1);  
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
    int i;
    printf("Scrolling Demo\n");
    init();
    //chipSelect(0);return 0;
    printf("Tapez votre message: ");
    scanf("%s", &text_buffer);
    // Add string terminator to rx buffer
    //text_buffer[text_position] = '\0';

    // Copy received string
    strcpy(display_string, text_buffer);

    // initialize variables
    text_position = 0;      
    letter_position = 0;
    column_position = 0;
    buffer_position = 0;
    get_letter_variables();
    actual_state = S_LETTER;

    // clear buffer
    for(i = 0; i < 32; i++) display_buffer[i] = 0x00;       

    printf("Now displaying: %s\n", display_string);
    //text_position = strlen(text_buffer);
    while(1) {
        loop();
    }
    printf("End\n");
    return 0;
}

/* vim:expandtab ai ts=4 sw=4 nu:
*/
