#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>


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
#define CS0       3
#define CS1       4
#define CS2       5
#define CS3       6
//#define CS_ENABLE 0

// HT1632C PINs

#define DISPLAY_WR   14     /* BCM_GPIO 27 */
#define DISPLAY_DATA 12     /* BCM_GPIO 17 */

// HT1632C Commands

#define HT1632C_READ 0b00000110
#define HT1632C_WRITE 0b00000101
#define HT1632C_CMD 0b00000100

#define HT1632_CMD_SYSON  0x01
#define HT1632_CMD_LEDON  0x03
#define HT1632_CMD_SYSDIS 0x00
#define HT1632_CMD_MASTER 0x18
#define HT1632_CMD_SLAVE  0x10



#define MATRIX struct _matrix_

MATRIX {
    unsigned char cs;
    unsigned char addr;

};



void ht1632c_send_command(unsigned char command, unsigned char id);
void ht1632c_send_bits(unsigned char bits, unsigned char firstbit);
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
    digitalWrite(CS3, HIGH);

    pinMode(DISPLAY_WR, OUTPUT);
    pinMode(DISPLAY_DATA, OUTPUT);

    return 1;
}

void chipSelect(char id)
{
    digitalWrite(CS0, id & 0x01);
    digitalWrite(CS1, (id & 0x02)>>1);
    digitalWrite(CS2, (id & 0x04)>>2);
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
    chipSelect(0x0f);

    for(i=0; i<4; i++)
        initMatrix(i); 
}

void loop(int* j) {
    int i;

    // send data
    for(i = 0; i < 1024; i++) {
        if (i==0) {
            chipSelect(0);
            ht1632c_send_bits(HT1632C_WRITE, 1 << 2);
            ht1632c_send_bits(0x00, 1 << 6);
        }
        if (i==256) {
            chipSelect(1);
            ht1632c_send_bits(HT1632C_WRITE, 1 << 2);
            ht1632c_send_bits(0x00, 1 << 6);
        }
        if (i==512) {
            chipSelect(2);
            ht1632c_send_bits(HT1632C_WRITE, 1 << 2);
            ht1632c_send_bits(0x00, 1 << 6);
        }
        if (i==768) {
            chipSelect(3);
            ht1632c_send_bits(HT1632C_WRITE, 1 << 2);
            ht1632c_send_bits(0x00, 1 << 6);
        }
        digitalWrite(DISPLAY_WR, LOW);
        if (i == *j) digitalWrite(DISPLAY_DATA, HIGH);
        else digitalWrite(DISPLAY_DATA, LOW);
        digitalWrite(DISPLAY_WR, HIGH);
    }

    // unselect display
    //digitalWrite(DISPLAY_CS, HIGH);
    chipSelect(0x0f);

    // cycle LED position to be turned on
    if(*j < 1023) (*j)++;
    else *j = 0;

    usleep(10000);
}


// ----------------------------------------
// HT1632C functions
// ----------------------------------------

void ht1632c_send_command(unsigned char command, unsigned char id) 
{
    printf("Sending command %02X\n", command);
    chipSelect(id);  
    //digitalWrite(DISPLAY_CS, LOW);
    ht1632c_send_bits(HT1632C_CMD, 1 << 2);
    ht1632c_send_bits(command, 1 << 7);
    ht1632c_send_bits(0, 1);
    chipSelect(0x0f);
    //digitalWrite(DISPLAY_CS, HIGH);
}

void ht1632c_send_bits(unsigned char bits, unsigned char firstbit) {

    while(firstbit) {
        digitalWrite(DISPLAY_WR, LOW);
        if (bits & firstbit) digitalWrite(DISPLAY_DATA, HIGH);
        else digitalWrite(DISPLAY_DATA, LOW);
        usleep(550);
        digitalWrite(DISPLAY_WR, HIGH);
        firstbit >>= 1;
    }
}

int main()
{
    int j=0;

    printf("Setup\n");
    init();
    //chipSelect(0);return 0;
    printf("Loop\n");
    while(1) {
        loop(&j);
    }
    printf("End\n");
    return 0;
}

/* vim:set ai expandtab ts=4 sw=4 nu:
*/
