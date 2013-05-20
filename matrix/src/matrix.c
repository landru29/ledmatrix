#include <stdio.h>
#include <errno.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <unistd.h>

/*
 wiringPi pin 8  is BCM_GPIO 2
 wiringPi pin 9  is BCM_GPIO 3
 wiringPi pin 7  is BCM_GPIO 4
 */
#define CS0 8
#define CS1 9
#define CS2 7

/* Matrix commands */
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


/**
 * Reverse bytes
 *
 */
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

/**
 * Select a matrix
 *
 * @param unsigned char id  : id of the matrix
 *
 * @return true
 */
int chipSelect(unsigned char id)
{
	digitalWrite(CS0, id & 0x01);
	digitalWrite(CS1, (id & 0x02)>>1);
	digitalWrite(CS2, (id & 0x04)>>2);
	return 1;
}

/**
 * Send a command to a matrix
 *
 * @param unsigned char cmd : command to send to the matrix
 * @param unsigned char id  : id of the matrix
 *
 * @return true
 */
int sendcommand(unsigned char cmd, unsigned char id) {
    unsigned short int data= COMMAND;

    data <<= 8;
    data |= cmd;
    data <<= 5;

    reverse_endian(&data, sizeof(data));

    chipSelect(id);
    wiringPiSPIDataRW(0, (unsigned char *)&data, 2);
    chipselect(0x0f);
    
    return 1;
}

/**
 * Init the IO port of the Raspberry
 *
 * @return true
 */
int initIO(void)
{
    if (wiringPiSPISetup(0, 2560000) <0) {
        printf("SPI Setup Failed: %s\n", strerror(errno));
        return 0;
    }

    if (wiringPiSetup() == -1)
    {
        printf("IO is not ready\n");
        return 0;
    }

    pinMode(CS0, OUTPUT);
    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);
    chipselect(0x0f);

    return 1;
}

/**
 * Init all the matrix
 *
 * @param unsigned char nbMatrix : number of matrix to initialize
 *
 * @return true
 */
int initMatrix(unsigned char nbMatrix)
{
	unsigned char i;
	for(i=0;i<nbMatrix; i++) {
		sendcommand(SYS_EN, i);
	    sendcommand(LED_ON, i);
	    sendcommand(MASTER_MODE, i);
	    sendcommand(INT_RC, i);
	    sendcommand(COMMON_8NMOS, i);
	    sendcommand(PWM_CONTROL | 15, i);
	}
	return 1;
}


/**
 * Main function
 *
 */
int main(int argc, char** argv)
{
    unsigned char data;

    if (argc<2) {
        printf("Syntax : %s hexa\nExample : %s aa\n", argv[0], argv[0]);
        return 0;
    }

    if (getuid()!=0) {
        printf("Only root user can launch this command; please connect as root or user sudo command\n");
        return 0;
    }

    sscanf(argv[1], "%02x", &data); 

    printf("Setting up IO\n");
    if (!initIO())
        return 1;
        
    if (!initMatrix(1))
    	return 1;
    	
    printf("Sending %02X to the matrix\n", data);
    /* @todo : code the printing of the code */

    return 0;
}
