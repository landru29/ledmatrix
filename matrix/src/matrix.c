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

/* Size of the buffer for data sending */
#define BUFFER_SIZE 1024

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
#ifdef DEBUG
    printf("Chip select:\n");
    printf("\tPin%d => %d\n", CS0, id & 0x01);
    printf("\tPin%d => %d\n", CS1, (id & 0x02)>>1);
    printf("\tPin%d => %d\n", CS2, (id & 0x04)>>2);
#endif
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
int sendcommand(unsigned char cmd, unsigned char id) 
{
    unsigned char data[2];
    data[0] = COMMAND << 5;
    data[0] |= cmd>>3;
    data[1] = cmd << 5;

#ifdef DEBUG
	unsigned char i;
    printf("Command : ");
    for(i=0;i<2;i++)
        printf("%04X ", data);
    printf("\n");
#endif

    chipSelect(id);
    wiringPiSPIDataRW(0, data, 2);
    chipSelect(0x0f);

    return 1;
}

/**
 * Send data to the matrix
 *
 * @param unsigned char address : address where to send data
 * @param unsigned char* data   : array of byte to write
 * @param unsigned char len     : length of the array
 * @param unsigned char id      : id of the matrix
 *
 * @return true
 */
int sendData(unsigned char address, unsigned char* data, unsigned char len, unsigned char id)
{
    unsigned char buffer[BUFFER_SIZE];
    unsigned char i;

    /* Sending address */
    buffer[0] = WR << 5;
    buffer[0] |= address >> 2;
    buffer[1] = address << 6;

    /* Sending data */
    for(i=0;i<len;i++) {
        buffer[i+1] |= data[i] >> 2;
        buffer[i+2] = data[i] << 6;
    }

#ifdef DEBUG
    printf("Writing : ");
    for(i=1;i<len + 2;i++)
        printf("%02X ", buffer[i]);
    printf("\n");
#endif
	
    /* Flush the buffer */
    chipSelect(id);
    wiringPiSPIDataRW(0, buffer, 2 + len);
    chipSelect(0x0f);

    return 1;
}

/**
 * Init the IO port of the Raspberry
 *
 * @return true
 */
int initIO(void)
{
#ifdef DEBUG
    printf("Init SPI\n");
#endif
    if (wiringPiSPISetup(0, 2560000) <0) {
        printf("SPI Setup Failed: %s\n", strerror(errno));
        return 0;
    }

    if (wiringPiSetup() == -1)
    {
        printf("IO is not ready\n");
        return 0;
    }

#ifdef DEBUG
    printf("Setting %d, %d, %d as output\n", CS0, CS1, CS2);
#endif

    pinMode(CS0, OUTPUT);
    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);
    chipSelect(0x0f);

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
#ifdef DEBUG
        printf("Init matrix %d\n", i);
#endif
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
    sendData(0, &data, 1, 0);

    return 0;
}
