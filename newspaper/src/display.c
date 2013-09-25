/**
 * Bibliothèque de fonctions de communication avec le hardware
 * via la bibliothèque WiringPi
 *
 * @file    display.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "display.h"
#ifdef __arm__
#include <wiringPi.h>
#include <wiringPiSPI.h>
#else
    void digitalWrite(int a, int b)
    {
		a = a;
		b = b;
	}
    void wiringPiSPIDataRW(int a, unsigned char* b, unsigned int c)
    {
		a = a;
		b = b;
		c = c;
	}
#endif
#include "bit_array.h"

// Vitesse de communication du SPI avec les matrices
#define SPI_SPEED    2000000

// ID des commandes pour les matrices
#define CMD_ID_COMMAND      0x4
#define CMD_ID_WR           0x5
// Différents paramètres d'initialisation des matrices
#define SYS_DIS      0x00
#define SYS_EN       0x01
#define LED_OFF      0x02
#define LED_ON       0x03
#define PWM_CONTROL  0xA0
#define BLINK_OFF    0x08
#define BLINK_ON     0x09
#define SLAVE_MODE   0x10
#define MASTER_MODE  0x18
#define COMMON_8NMOS 0x20
#define COMMON_8PMOS 0x28

// Délai d'attente entre l'écriture sur les matrices et l'activation des chipSelect
#define DELAY        550

unsigned char CS0 = 3; //GPIO22
unsigned char CS1 = 4; //GPIO23
unsigned char CS2 = 5; //GPIO24
unsigned char CS3 = 6; //GPIO25 A activer en continu avec le 74HC138 (pin E3)
//#define CS0 3 // GPIO 22
//#define CS1 4 // GPIO 23
//#define CS2 5 // GPIO 24
//#define CS3 6 // GPIO 25 A activer en continu avec le 74HC138 (pin E3)

//#define DEBUG

void print_byte(uint8_t x);
void print_word(uint16_t x);

/**
 * Spécifie les pins de chip select
 *
 * @param id identifiant du chipSelect (0|1|2|3)
 * @param value numéro de pin
 */
void setCs(unsigned char id, unsigned char value)
{
    switch (id) {
        case 0:
            CS0 = value;
            break;
        case 1:
            CS1 = value;
            break;
        case 2:
            CS2 = value;
            break;
        case 3:
            CS3 = value;
            break;
        default:
            break;
    }
}

/**
 * Lit les pins de chip select
 *
 * @param id identifiant du chipSelect (0|1|2|3)
 * @return numéro de pin
 */
int getCs(unsigned char id)
{
    switch (id) {
        case 0:
            return CS0;
            break;
        case 1:
            return CS1;
            break;
        case 2:
            return CS2;
            break;
        case 3:
            return CS3;
            break;
        default:
            return -1;
            break;
    }
}


/**
 * Modifie le poids faible et le poids fort
 *
 * @param p    La référence de l'élément à modifier
 * @param size La taille de l'élément
 */
void *reverseEndian(void *p, size_t size)
{
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
 * @param id matrix ID
 */
int selectChip(unsigned char id)
{
#ifdef DEBUG
    //printf("Chip select\n");
#endif
    switch(id) {
        case 0:
            digitalWrite(CS0, 0);
            digitalWrite(CS1, 0);
            digitalWrite(CS2, 0);
            break;
        case 1:
            digitalWrite(CS0, 1);
            digitalWrite(CS1, 0);
            digitalWrite(CS2, 0);
            break;
        case 2:
            digitalWrite(CS0, 0);
            digitalWrite(CS1, 1);
            digitalWrite(CS2, 0);
            break;
        case 3:
            digitalWrite(CS0, 1);
            digitalWrite(CS1, 1);
            digitalWrite(CS2, 0);
            break;
        default:
            digitalWrite(CS0, 1);
            digitalWrite(CS1, 1);
            digitalWrite(CS2, 1);
    }
    return 1;
}

/**
 * Ecrit les données sur une matrice
 *
 * @param chip   Le numéro de matrice
 * @param screen Pointeur sur les données
 * @param size   La taille à écrire
 * @param width  Largeur d'une matrice
 * @param height Hauteur d'une matrice
 */
void writeScreen(int chip, unsigned char *screen, uint8_t size, uint8_t width, uint8_t height)
{
#ifdef DEBUG
    printf("writeScreen(%d, %d, %d)\n", chip, width, height);
#endif
    uint8_t *output = malloc(size+2);
    uint16_t data;
    uint8_t write;

    // Command ID WRITE
    *output = 0b10100000;
    *(output+1) = 0;

    // Copy the data
    bitarray_copy(screen, 0, width*height, (output+1), 2);
    // Send data to SPI
    selectChip(chip);
    wiringPiSPIDataRW(0, (unsigned char*)output, size+1);
    usleep(DELAY);
    selectChip(0x0f);
    // Free memory
    free(output);
    output = NULL;

    // Command ID WRITE
    data = CMD_ID_WR;
    data <<= 7;
    data |= 0x3f; //last address on screen
    data <<= 4;
    write = (0x0f & *(screen+31));
    data |= write;
    data <<= 2;
    reverseEndian(&data, sizeof(data));
    // Send data to SPI
    selectChip(chip);
    wiringPiSPIDataRW(0, (unsigned char*)&data, 2);
    usleep(DELAY);
    selectChip(0x0f);
}

#ifdef __arm__
/**
 * Ecrire toutes les données sur les matrices
 *
 * @param viewport Le buffer des données à écrire
 * @param nbMatrix Nombre de matrices sur lesquelles ecrire
 * @param width    La largeur d'une matrice
 * @param height   La hauteur d'une matrice
 */
void writeMatrix(unsigned char* viewport, uint8_t nbMatrix, uint8_t width, uint8_t height)
{
#ifdef DEBUG
    printf("writeMatrix\n");
#endif
    uint8_t i;
    uint8_t size = width * height / 8;
    for (i = 0; i < nbMatrix; i++) {
        writeScreen(i, viewport + (width * i), size, width, height);
    }
}
#endif

/**
 * Envoie les commandes d'initialisation à une matrice
 *
 * @param chip Le numéro de matrice
 * @param cmd  La commande
 */
void sendCommand(uint8_t chip, uint8_t cmd)
{
#ifdef DEBUG
    printf("sendCommand: ");
    print_word((uint16_t)cmd);
#endif
    uint16_t data=0;

    data = CMD_ID_COMMAND;

    data <<= 8;
    data |= cmd;
    data <<= 5;

    reverseEndian(&data, sizeof(data));

    selectChip(chip);
    wiringPiSPIDataRW(0, (unsigned char*)&data, 2);
    usleep(DELAY);
    selectChip(0x0f);
}

#ifdef __arm__
/**
 * Permet d'activer ou non le blink sur une matrice
 *
 * @param chip   Le numéro de matrice
 * @param blinky Flag d'activation (0 ou 1)
 */
void displayBlink(uint8_t chip, uint8_t blinky)
{
#ifdef DEBUG
    printf("blink\n");
#endif
    if (blinky)
        sendCommand(chip, BLINK_ON);
    else
        sendCommand(chip, BLINK_OFF);
}
#endif

/**
 * Permet de régler la luminosité d'une matrice
 *
 * @param chip Le numéro de matrice
 * @param pwm  Le niveau de luminosité (0 - 15)
 */
void setBrightness(uint8_t chip, uint8_t pwm)
{
#ifdef DEBUG
    printf("set_brightness\n");
#endif
    if (pwm > 15) pwm = 15;
    sendCommand(chip, PWM_CONTROL | pwm);
}

#ifdef __arm__
/**
 * Initialisation des matrices
 *
 * @param nbMatrix Nombre de matrices à initialiser
 * @param width    Taille d'une matrice
 * @param height   Hauteur d'une matrice
 *
 * @return init's status
 */
int8_t initDisplay(uint8_t nbMatrix, uint8_t width, uint8_t height)
{
#ifdef DEBUG
    printf("init display (%d)\n", nbMatrix);
#endif
    uint8_t i;

    // Initialisation du SPI avec la lib WiringPi
    if (wiringPiSPISetup(0, SPI_SPEED) < 0) {
        printf("SPI Setup Failed: %s\n", strerror(errno));
        return -1;
    }

    // Initialise le GPIO
    if (wiringPiSetup() < 0) {
        printf("Setup Failed: %s\n", strerror(errno));
        return -1;
    }

    // Initialisation du mode output pour les pins du chipSelect
    pinMode(CS0, OUTPUT);
    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);
    pinMode(CS3, OUTPUT);
    // Activation du pin le chipSelect
    digitalWrite(CS3, 1);

    // Initialisation des matrices
    for (i=0; i < nbMatrix; i++) {
        // Set master mode
        sendCommand(i, MASTER_MODE);
        // Turn on system oscillator
        sendCommand(i, SYS_EN);
        // Turn on LED duty cycle generator
        sendCommand(i, LED_ON);
        // Set brightness at full
        setBrightness(i, 15);
        // Turn off blinking function
        displayBlink(i, 0);
    }

    // Clear buffer
    unsigned char *buffer;
    // Allocation de l'espace mémoire pour les matrices
    buffer = calloc(nbMatrix * width * height / 8, sizeof(char));
    for (i=0; i < (nbMatrix * width); i++)
        *(buffer+i) = 0x00;
    writeMatrix(buffer, nbMatrix, width, height);
    free(buffer);

    return 0;
}
#endif

/**
 * Affiche un octet pour le debug
 *
 * @param x L'octet à afficher
 */
void print_byte(uint8_t x)
{
    int n;

    for(n=0; n<8; n++){

        if (n%4==0)
            printf(" ");
        if ((x &0x80) != 0)
            printf("1");
        else
            printf("0");

        x = x <<1;
    }
}

/**
 * Affiche un mot de 16 bits pour le debug
 *
 * @param x Le mot à afficher
 */
void print_word(uint16_t x)
{
    print_byte(x>>8);
    print_byte(x);
    printf("\n");
}

/* vim: set expandtab ai nu ts=4 sw=4:
*/
