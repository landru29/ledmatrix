#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <wiringPi.h>
#include "bit_array.h"
#include "aipointe.h"

// Vitesse de communication du SPI avec les matrices
#define SPI_SPEED    2000000
// Délai d'attente entre l'écriture sur les matrices et l'activation des chipSelect
#define DELAY        550

// ID des commandes pour les matrices
#define COMMAND      0x4
#define RD           0x6
#define WR           0x5
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


#define SPACING_SIZE      1      // space between letters
#define SPACE_SIZE        2      // "space" character length
#define BLANK_SIZE        10      // blank columns before restarting the string
#define SCROLLING_SPEED   100000    // scrolling speed in ms

// FSM states
#define S_OFF             0
#define S_LETTER          1
#define S_SPACING         2
#define S_SPACE           3
#define S_BLANK           4

#define for_x for (x = 0; x < w; x++)
#define for_y for (y = 0; y < h; y++)
#define for_xy for_x for_y

#define CS0 3 // GPIO 22
#define CS1 4 // GPIO 23
#define CS2 5 // GPIO 24
#define CS3 6 // GPIO 24 A activer en continu avec le 74HC138 (pin E3)

#define DEBUG

int displays = 3; // Nombre des matrices
int height = 8;   // Nombre de ligne sur une matrice
int width = 32;   // Nombre de colonnes sur une matrice

unsigned char *matrix; // Espace mémoire pour l'écriture sur les matrices

char *display_string;  // Message à afficher avec la méthode write_message

unsigned char actual_state;    // Etat courant (voir FSM states)
unsigned char letter_info;     // Position de la lettre dans le tableau des infos de fonte
unsigned char letter_length;   // Nombre de colonne d'une fonte
uint16_t letter_offset;        // Offset dans le tableau des fontes
unsigned char letter_position; // Position de la lettre courante dans le message
unsigned char column_position; // Position de la colonne d'une fonte en cours d'écriture
unsigned char buffer_position; // Emplacement dans l'espace mémoire

/**
 * Modifie le poids faible et le poids fort
 *
 * @param void   *p   La référence de l'élément à modifier
 * @param size_t size la taille de l'élément
 *
 * @return void
 */
void *reverse_endian(void *p, size_t size)
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
 * Affiche un octet pour le debug
 * 
 * @para uint8_t x L'octet à afficher
 *
 * @return void
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
 * @para uint16_t x Le mot à afficher
 *
 * @return void
 */
void print_word(uint16_t x)
{
    print_byte(x>>8);
    print_byte(x);
    printf("\n");
}

/**
 * Select a matrix
 *
 * @param unsigned char id  : id of the matrix
 *
 * @return true
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
            digitalWrite(CS0, 0);
            digitalWrite(CS1, 0);
            digitalWrite(CS2, 1);
            break;
        case 2:
            digitalWrite(CS0, 0);
            digitalWrite(CS1, 1);
            digitalWrite(CS2, 0);
            break;
        case 3:
            digitalWrite(CS0, 0);
            digitalWrite(CS1, 1);
            digitalWrite(CS2, 1);
            break;
        default:
            digitalWrite(CS0, 1);
            digitalWrite(CS1, 1);
            digitalWrite(CS2, 1);
    }
    return 1;
}

void clear_matrix()
{
#ifdef DEBUG
    printf("clear_matrix\n");
#endif
    matrix = memset(matrix, 0, width * height * displays / 8);
}

/**
 * Ecrit les données sur les matrices
 *
 * @param int     chip   Le numéro de matrice
 * @param uint8_t screen Les données
 * @param uint8_t size   La taille à écrire
 *
 * @return void
 */
void write_screen(int chip, uint8_t *screen, uint8_t size)
{
#ifdef DEBUG
    printf("write_screen(%d)\n", chip);
#endif
    uint8_t i;
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
    data = WR;
    data <<= 7;
    data |= 0x3f; //last address on screen
    data <<= 4;
    write = (0x0f & *(screen+31));
    data |= write;
    data <<= 2;
    reverse_endian(&data, sizeof(data));
    // Send data to SPI
    selectChip(chip);
    wiringPiSPIDataRW(0, (unsigned char*)&data, 2);
    usleep(DELAY);
    selectChip(0x0f);
}

/**
 * Ecrire toutes les données sur les matrices
 *
 * @return void
 */
void write_matrix()
{
#ifdef DEBUG
    printf("write_matrix\n");
#endif
    uint8_t i;
    int size = width * height / 8;
    for (i = 0; i < displays; i++) {
        write_screen(i, matrix + (size*i), size);
    }
}

/**
 * Récupère les infos du caractère en cours (longueur & offset)
 *
 * @return void
 */
void get_letter_variables()
{
#ifdef DEBUG
    printf("get_letter_variables (%d)\n", display_string[letter_position]);
#endif
    // Get letter information from font descriptor
    letter_info = display_string[letter_position] - 33;
    letter_length = aipoint_info[letter_info].length;
    letter_offset = aipoint_info[letter_info].offset;
#ifdef DEBUG
    printf("\tletter_position(%d), letter_info(%d), letter_length(%d), letter_offset(%d)\n", letter_position, letter_info, letter_length, letter_offset);
#endif
}

/**
 * Met à jour l'affichage en déplacant le position du buffer
 *
 * @param unsigned char new_byte L'octet à ajouter dans la mémoire
 *
 * @return void
 */
void update_display(unsigned char new_byte)
{
#ifdef DEBUG
    printf("update_display\n");
#endif
    *(matrix + buffer_position) = new_byte;
    buffer_position = (buffer_position - 1) % (displays * width);
    write_matrix();
}

/**
 * Permet d'écrire un message qui va scroller de droite vers la gauche
 *
 * @param char message Le message à afficher
 *
 * @return void
 */
void write_message2(char *message)
{
    uint8_t i, l, k, ShiftAmount, blank, temp, shift_step=1, StringLength, maxlen;
    // Longueur du message
    StringLength = strlen(message);
    // Taille maximale d'ecriture sur les matrices
    maxlen = (displays * width)-1;
    // Espaces blancs
    blank = maxlen - StringLength;
    // Position dans le buffer
    buffer_position = maxlen;
    // On itère sur chaque caractère du message
    for (letter_position=0; letter_position<StringLength; letter_position++) {
        // On récupère les infos de la fonte
        letter_info = message[letter_position] - 33;
        letter_length = aipoint_info[letter_info].length;
        letter_offset = aipoint_info[letter_info].offset;
        // On itère sur la fonte du caractère
        for (ShiftAmount=0; ShiftAmount<letter_length; ShiftAmount++) {
            temp = aipointe_font[letter_offset + ShiftAmount];
            // On déplace l'existant vers la gauche
            for(i=buffer_position; i<maxlen; i++) {
                *(matrix + i) = *(matrix + i + 1);
            }
            // On définit la nouvelle valeur dans la mémoire à la fin
            *(matrix + maxlen) = temp;
            // On demande l'écriture sur les matrices
            write_matrix();
            // On fait une pause le temps pour le scroll
            usleep(SCROLLING_SPEED);
            buffer_position--;
        }
        // On déplace l'existant vers la gauche pour ajouter un espace
        for(i=buffer_position; i<maxlen; i++) {
            *(matrix + i) = *(matrix + i + 1);
        }
        *(matrix + maxlen) = 0x00;
        buffer_position--;
    }
    /*
     * Permet de réaliser la bulle du logo de Digitaleo
     */
    for(k=0; k<6; k++) {
        for(i=buffer_position; i<maxlen; i++) {
            *(matrix + i) = *(matrix + i + 1);
        }
        if (k == 0) {
            *(matrix + maxlen) = 0x08;
        } else {
            *(matrix + maxlen) = 0xf0;
        }
        write_matrix();
        usleep(SCROLLING_SPEED);
        buffer_position--;
    }
    // Création d'un flag pour l'arrêt de la boucle infinie ci-dessous
    char flag=0;
    // Temps que la position du buffer n'est pas à zéro, on remplit d'espace
    while (buffer_position >= 0) {
        // On déplace l'existant vers la gauche
        for(i=buffer_position; i<maxlen; i++) {
            *(matrix + i) = *(matrix + i + 1);
        }
        *(matrix + maxlen) = 0x00;
        write_matrix();
        usleep(SCROLLING_SPEED);
        buffer_position--;
        // buffer_position ne pouvant pas être négatif (sécurité memory),
        // On utilise un flag pour faire la sortie
        if (flag == 1)
            break;
        // Si buffer_position vaut 0 et flag aussi, on lève le flag pour la sortie à la prochaine boucle
        // Sinon, on le texte reste bloqué à la deuxième colonne sur les matrices
        if (buffer_position == 0 && flag == 0)
            flag = 1;
    }
}

/**
 * Première méthode d'écriture d'un message qui tourne en rond comme un moulin
 *
 * @return void
 */
void write_message()
{
#ifdef DEBUG
    printf("write_message\n");
#endif
    unsigned char new_byte;

    char c = display_string[letter_position];
    switch(actual_state) {
        case S_OFF:
            return;
        case S_LETTER:
            if (c == 0) {
                new_byte = 0x00;
                column_position = 1;
                actual_state = S_BLANK;
            } else if (c == 32) {
                new_byte = 0x00;
                column_position = 1;
                actual_state = S_SPACE;
            } else {
                new_byte = aipointe_font[letter_offset + column_position];
                column_position++;
                if (column_position == letter_length) {
                    column_position = 0;
                    actual_state = S_SPACING;
                }
            }
            break;
        case S_SPACING:
            new_byte = 0x00;
            column_position++;
            if (column_position == SPACING_SIZE) {
                column_position = 0;
                letter_position++;
                get_letter_variables();
                actual_state = S_LETTER;
            }
            break;
        case S_SPACE:
            new_byte = 0x00;
            column_position++;
            if (column_position == SPACE_SIZE) {
                column_position = 0;
                letter_position++;
                get_letter_variables();
                actual_state = S_LETTER;
            }
            break;
        case S_BLANK:
            new_byte = 0x00;
            column_position++;
            if (column_position == BLANK_SIZE) {
                column_position = 0;
                letter_position = 0;
                get_letter_variables();
                actual_state = S_LETTER;
            }
            break;
    }
    update_display(new_byte);
}

/**
 * Boucle sur la méthode write_message pour réaliser le scroll
 *
 * @return void
 */
void scroll()
{
#ifdef DEBUG
    printf("scroll\n");
#endif
    while(1) {
        write_message();
        usleep(SCROLLING_SPEED);
    }
}

/**
 * Envoie les commandes d'initialisation à une matrice
 *
 * @param uint8_t chip Le numéro de matrice
 * @param uint8_t cmd  La commande
 *
 * @return void
 */
void send_command(uint8_t chip, uint8_t cmd)
{
#ifdef DEBUG
    printf("send_command: ");
    print_word(cmd);
#endif
    uint16_t data=0;

    data = COMMAND;

    data <<= 8;
    data |= cmd;
    data <<= 5;

    reverse_endian(&data, sizeof(data));

    selectChip(chip);
    wiringPiSPIDataRW(0, (unsigned char*)&data, 2);
    usleep(DELAY);
    selectChip(0x0f);
}

/**
 * Permet d'activer ou non le blink sur une matrice
 *
 * @param uint8_t chip   Le numéro de matrice
 * @param uint8_t blinky Flag d'activation (0 ou 1)
 *
 * @return void
 */
void blink(uint8_t chip, uint8_t blinky)
{
#ifdef DEBUG
    printf("blink\n");
#endif
    if (blinky)
        send_command(chip, BLINK_ON);
    else
        send_command(chip, BLINK_OFF);
}

/**
 * Permet de régler la luminosité d'une matrice
 *
 * @param uint8_t chip Le numéro de matrice
 * @param uint8_t pwm  Le niveau de luminosité (0 - 15)
 *
 * @return void
 */
void set_brightness(uint8_t chip, uint8_t pwm)
{
#ifdef DEBUG
    printf("set_brightness\n");
#endif
    if (pwm > 15) pwm = 15;
    send_command(chip, PWM_CONTROL | pwm);
}

/**
 * Méthode de lancement du programme
 *
 * @return int
 */
int main(void)
{
    uint8_t i, j;
    
    // Allocation de l'espace mémoire pour les matrices
    matrix = calloc(displays * width * height / 8, sizeof(char));
    printf("Matrice: begin(%d) - end(%d)\n", matrix, matrix+(displays*width));

    // Check l'allocation mémoire
    if (matrix == NULL) {
        printf("ERREUR : probleme de memoire !\n");
        exit(EXIT_FAILURE);
    }

    // Initialisation du SPI avec la lib WiringPi
    if (wiringPiSPISetup(0, SPI_SPEED) < 0) {
        printf ("SPI Setup Failed: %s\n", strerror(errno));
        return 1;
    }

    // Initialise le GPIO
    if (wiringPiSetup() == -1)
        return 1;

    // Initialisation du mode output pour les pins du chipSelect
    pinMode(CS0, OUTPUT);
    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);
    pinMode(CS3, OUTPUT);
    // Activation du pin le chipSelect
    digitalWrite(CS3, 1);

    // Initialisation des matrices
    for (i=0; i < displays; i++) {
        // Set master mode
        send_command(i, MASTER_MODE);
        // Turn on system oscillator
        send_command(i, SYS_EN);
        // Turn on LED duty cycle generator
        send_command(i, LED_ON);
        // Set brightness at full
        set_brightness(i, 15);
        // Turn off blinking function
        blink(i, 0);
    }

    // Définit la chaine à afficher
    display_string = "Et maintenant le scroll";

    // Initialise les variables pour l'écriture des messages
    letter_position = 0; // Position de la lettre dans le message
    column_position = 0; // Position de la colonne de la fonte pour la lettre courante
    buffer_position = displays * width; // Position dans le buffer (mémoire)
    //get_letter_variables(); // Récupère les infos de la première lettre du message
    actual_state = S_LETTER; // Etat pour l'écriture du message (@see write_message)

    // Clear buffer
    for (i=0; i < (displays * width); i++)
        *(matrix+i) = 0x00;
    write_matrix();
    // Temps de pause avant démarrage de l'affichage
    usleep(5000000);

    // Ecriture du message avec la première méthode (fonctionne avec la variable display_string)
    //scroll();

    // Ecriture du message avec la deuxième méthode
    write_message2("Digitaleo");

    // Libération de la mémoire
    free(matrix);
    matrix = NULL;

    return 0;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/
