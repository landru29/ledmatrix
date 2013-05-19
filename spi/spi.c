#include <stdio.h>
#include <errno.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>

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

#define CS1 8
#define CS2 9

#define COMMAND 0x4
#define RD 0x6
#define WR 0x5
#define SYS_DIS 0x00
#define COMMON_8NMOS 0x20
#define COMMON_8PMOS 0x28
#define MASTER_MODE 0x14
#define INT_RC 0x18
#define SYS_EN 0x01
#define LED_ON 0x03
#define LED_OFF 0x02
#define PWM_CONTROL 0xA0
#define BLINK_ON 0x09
#define BLINK_OFF 0x08


void *reverse_endian(void *p, size_t size) {
    char *head = (char *)p;
    char *tail = head + size -1;

    for(; tail > head; --tail, ++head) {
        char temp = *head;
        *head = *tail;
        *tail = temp;
    }
    return p;
}


void sendcommand(unsigned char cmd) {
    unsigned short int data= COMMAND;

    data <<= 8;
    data |= cmd;
    data <<= 5;

    reverse_endian(&data, sizeof(data));

    digitalWrite(CS1, 0);
    wiringPiSPIDataRW(0, (unsigned char *)&data, 2);
    digitalWrite(CS1, 1);
}



int main(void)
{
    printf("Sending data to SPI\n");

    if (wiringPiSPISetup(0, 2560000) <0)
        printf ( "SPI Setup Failed: %s\n", strerror(errno));

    if (wiringPiSetup() == -1)
        return 1;

    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);
    digitalWrite(CS1, 1);
    digitalWrite(CS2, 1);

    sendcommand(SYS_EN);
    sendcommand(LED_ON);
    sendcommand(MASTER_MODE);
    sendcommand(INT_RC);
    sendcommand(COMMON_8NMOS);
    sendcommand(PWM_CONTROL | 15);

    return 0;
}
