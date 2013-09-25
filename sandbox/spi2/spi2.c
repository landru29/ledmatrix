#include <stdio.h>
#include <errno.h>
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <unistd.h>

/* wiringPi pin 7  is BCM_GPIO 4 */
#define CS 7


int sendData(unsigned char data)
{
    digitalWrite(CS, 1);
    wiringPiSPIDataRW(0, (unsigned char *)&data, 1);
    digitalWrite(CS, 0);
    return 0;
}

int initIO()
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

    pinMode(CS, OUTPUT);
    digitalWrite(CS, 0);

    return 1;
}

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

    printf("Sending %02X to SPI\n", data);
    sendData(data);

    return 0;
}
