#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <wiringPi.h>
#include "bit_array.h"
#include "font.h"
#include "aipointe.h"

#define SPI_SPEED    2000000
#define DELAY        550
#define COMMAND      0x4
#define RD           0x6
#define WR           0x5

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
#define BLANK_SIZE        7      // blank columns before restarting the string
#define SCROLLING_SPEED   150000    // scrolling speed in ms

#define TEXT_BUFFER_SIZE  100    // max characters in string

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

#define CLK  14 // GPIO 11
#define DATA 12 // GPIO 10

#define DEBUG

int displays=3;
int height=8;
int width=32;
int font_width=8;
unsigned char *matrix;
uint8_t *matrix_buf;

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
    if (id == 0) {
        digitalWrite(CS0, 0);
        digitalWrite(CS1, 0);
        digitalWrite(CS2, 0);
    } else if (id == 1) {
        digitalWrite(CS0, 0);
        digitalWrite(CS1, 0);
        digitalWrite(CS2, 1);
    } else if (id == 2) {
        digitalWrite(CS0, 0);
        digitalWrite(CS1, 1);
        digitalWrite(CS2, 0);
    } else {
        digitalWrite(CS0, 1);
        digitalWrite(CS1, 1);
        digitalWrite(CS2, 1);
    }
#ifdef DEBUG
    //printf("Chip select\n");
    //printf("\tPin%d => %d\n", CS0, id & 0x01);
    //printf("\tPin%d => %d\n", CS1, (id & 0x02)>>1);
    //printf("\tPin%d => %d\n", CS2, (id & 0x04)>>2);
#endif
    return 1;
}

void random_screen(int chip, uint8_t fill)
{
    unsigned char matrix[34];
    int i;
    matrix[0] = 0b10100000;
    matrix[1] = 0b00111111;

    for (i=2;i<34;i++){
        matrix[i]=rand()%255;
    }
    wiringPiSPIDataRW(chip,matrix,34);
    usleep(DELAY);
    selectChip(0x0f);
}

void clear_matrix()
{
#ifdef DEBUG
    printf("clear_matrix\n");
#endif
    matrix = memset(matrix,0,width*height*displays/8);
}

void print_display(int num)
{
#ifdef DEBUG
    printf("print_display\n");
#endif
    uint8_t x, y;
    uint8_t n;

    for (x=0; x < width; x++) {
        n = *(matrix+x+(num*width));
        for (y=0; y < height; y++) {
            if ((n &0x80) != 0)
                printf("=");
            else
                printf(" ");
            n = n <<1;
        }
        printf("\n");
    }
}

void write_screen(int chip, uint8_t *screen, uint8_t size)
{
#ifdef DEBUG
    printf("write_screen(%d)\n", chip);
    print_word(screen);
#endif
    uint8_t i;
    uint8_t *output = malloc(size+2);
    uint16_t data;
    uint8_t write;

    *output = 0b10100000;
    *(output+1) = 0;

    bitarray_copy(screen, 0, width*height, (output+1), 2);
    selectChip(chip);
    wiringPiSPIDataRW(0, (unsigned char*)output, size+1);
    usleep(DELAY);
    selectChip(0x0f);
    free(output);
    output = NULL;

    data = WR;
    data <<= 7;
    data |= 0x3f; //last address on screen
    data <<= 4;
    write = (0x0f & *(screen+31));
    data |= write;
    data <<= 2;
    //print_word(data);
    //printf(" = %d\n",i*2+1); 
    reverse_endian(&data, sizeof(data));
    selectChip(chip);
    wiringPiSPIDataRW(0, (unsigned char*)&data, 2);
    usleep(DELAY);
    selectChip(0x0f);
}

void write_matrix()
{
#ifdef DEBUG
    printf("write_matrix\n");
#endif
    uint8_t i;
    int size = width * height / 8;
    for(i=0;i<displays; i++) {
        write_screen(i, matrix + (size*i), size);
        //sendData(0x00, matrix + (size * i), size, i);

        #ifdef DEBUG
            printf("write matrice: %d\n", matrix + (size * i));
        #endif
    }
}

void scroll()
{
#ifdef DEBUG
    printf("scroll\n");
#endif
    uint8_t i,n,len,tmp,tmp2,last;
    len = displays*width;

    while(1) {
        tmp = NULL;
        tmp2 = NULL;
        for (i=len-1; i>0; i--) {
            if (tmp == NULL) {
                tmp = *(matrix+i-1);
                *(matrix+i-1) = *(matrix+i);
            } else {
                tmp2 = *(matrix+i-1);
                *(matrix+i-1) = tmp;
                tmp = tmp2;
            }
        }
        *(matrix+(len-1)) = tmp;
        write_matrix();
        usleep(SCROLLING_SPEED);
    }
}
void scroll_matrix()
{
#ifdef DEBUG
    printf("scroll_matrix\n");
#endif
    int i,n,len;
    len = displays * width * height / 8;

    for (i=0; i < len; i++) {
        for (n=0; n<len; n++) {
            if (n >= (len-i-2))
                *(matrix+n) = 0;
            else
                *(matrix+n) = *(matrix + n + 1);
        }
        //    print_display(0);
        write_matrix();
        usleep(SCROLLING_SPEED);
    }
}

void scroll_matrix_once(int offset)
{
#ifdef DEBUG
    printf("scroll_matrix_once\n");
#endif
    int n;
    int len = width * displays;

    for (n=0; n < (len-1); n++) {
        *(matrix + n) = *(matrix + n + 1);
    }
    *(matrix+n) = *(matrix + offset);

    write_matrix();
    usleep(SCROLLING_SPEED);
}

void write_message(char *message)
{
#ifdef DEBUG
    printf("write_message\n");
#endif
    unsigned char new_byte, actual_state;
    int msg_len = strlen(message);
    int max_len = width*displays;
    int i, pix;
    uint8_t letter_info, letter_length, matrice_position = 0, column_position = 0;
    uint16_t letter_offset;
#ifdef DEBUG
    printf("msg_len: %d - max_len: %d\n", msg_len, max_len);
#endif

    actual_state = S_LETTER;
    for (i=0; i < msg_len; i++ ) {
        char c = message[i];
        // Récupérer les infos de la fonte
        // Get letter information from font descriptor
        letter_info = c - 33;

        letter_length = aipoint_info[letter_info].length;
        letter_offset = aipoint_info[letter_info].offset;

        printf("c(%d) - offset(%d) - length(%d) - info(%d)\n", c, letter_offset, letter_length, letter_info);
        if (actual_state == S_LETTER) {
            if (c == 0) {
                new_byte = 0x00;
                *(matrix + matrice_position) = new_byte;
                printf("column(%d) - matrice(%d) - new_byte: %02X\n", column_position, matrice_position, new_byte);
                column_position = 1;
                actual_state = S_BLANK;
                matrice_position++;
            } else if (c == 32) {
                new_byte = 0x00;
                *(matrix + matrice_position) = new_byte;
                printf("column(%d) - matrice(%d) - new_byte: %02X\n", column_position, matrice_position, new_byte);
                column_position = 1;
                actual_state = S_SPACE;
                matrice_position++;
            } else {
                while(column_position < letter_length && matrice_position < max_len) {
                    new_byte = aipointe_font[letter_offset + column_position];
                    printf("column(%d) - matrice(%d) - new_byte: %02X\n", column_position, matrice_position, new_byte);
                    *(matrix + matrice_position) = new_byte;
                    column_position++;
                    matrice_position++;
                }
                column_position = 0;
                actual_state = S_SPACING;
            }
        }
        if (actual_state == S_SPACING) {
            new_byte = 0x00;
            while(column_position < SPACING_SIZE && matrice_position < max_len) {
                *(matrix + matrice_position) = new_byte;
                printf("column(%d) - matrice(%d) - new_byte: %02X\n", column_position, matrice_position, new_byte);
                column_position++;
                matrice_position++;
            }
            column_position = 0;
            actual_state = S_LETTER;
        } else if (actual_state == S_SPACE) {
            new_byte = 0x00;
            while(column_position < SPACE_SIZE && matrice_position < max_len) {
                *(matrix + matrice_position) = new_byte;
                printf("column(%d) - matrice(%d) - new_byte: %02X\n", column_position, matrice_position, new_byte);
                column_position++;
                matrice_position++;
            }
            column_position = 0;
            actual_state = S_LETTER;
        } else if (actual_state == S_BLANK) {
            new_byte = 0x00;
            while(column_position < BLANK_SIZE && matrice_position < max_len) {
                *(matrix + matrice_position) = new_byte;
                printf("column(%d) - matrice(%d) - new_byte: %02X\n", column_position, matrice_position, new_byte);
                column_position++;
                matrice_position++;
            }
            column_position = 0;
            actual_state = S_LETTER;
        }
    }
    write_matrix();
    //for (pix=0; pix<(displays*width);pix++)
        //scroll_matrix_once(pix);
    //scroll();
}

void evolve(void *u, int w, int h)
{
#ifdef DEBUG
    printf("evolve\n");
#endif
    int x,y,y1,x1;
    unsigned (*univ)[w] = u;
    unsigned new[h][w];

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            int n = 0;
            //printf("x: %d y: %d \n", x, y);
            for( y1 = y - 1; y1 <= y + 1; y1++)
                for ( x1 = x - 1; x1 <= x + 1; x1++){
                    //printf(" x1: %d y1: %d [%d][%d] \n", x1, y1,(y1 + h) % h,(x1 + w) % w);
                    if (univ[(y1 + h) % h][(x1 + w) % w])
                        n++;
                }
            if (univ[y][x]) n--;
            new[y][x] = (n == 3 || (n == 2 && univ[y][x]));
        }
    }

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) { 
            univ[y][x] = new[y][x]; 
        }
    }
}

void show_game(void *u, int w, int h)
{
#ifdef DEBUG
    printf("show_game\n");
#endif
    int x,y;

    int (*univ)[w] = u;
    for (x = 0; x < w; x++) {
        for (y = 0; y < h; y++) {
            //draw_pixel(x, y, univ[y][x]);
        }
    }
    write_matrix();
}

void game(int w, int h)
{
#ifdef DEBUG
    printf("game\n");
#endif
    int x,y,i;
    unsigned univ[h][w];
    while(1) {
        for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++) { 
                univ[y][x] = rand() < RAND_MAX / 10 ? 1 : 0; 
            }
        }
        show_game(univ, w, h);
        sleep(1);
        for (i=0; i<60; i++) {
            evolve(univ, w, h);
            show_game(univ, w, h);
            usleep(300000);
        }
    }
}

void send_command(int chip, uint8_t cmd)
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

void blink(int chip, int blinky)
{
#ifdef DEBUG
    printf("blink\n");
#endif
    if (blinky)
        send_command(chip, BLINK_ON);
    else
        send_command(chip, BLINK_OFF);
}

void set_brightness(int chip, uint8_t pwm)
{
#ifdef DEBUG
    printf("set_brightness\n");
#endif
    if (pwm > 15) pwm = 15;
    send_command(chip, PWM_CONTROL | pwm);
}

int main(void)
{
    uint8_t i, j;

    matrix = calloc(displays * width * height / 8, sizeof(char));
    if (matrix == NULL) {
        printf("ERREUR : probleme de memoire !\n");
        exit(EXIT_FAILURE);
    }
    //matrix_buf = (uint8_t *)malloc(font_width);

    if (wiringPiSPISetup(0, SPI_SPEED) <0) {
        printf ("SPI Setup Failed: %s\n", strerror(errno));
        return 1;
    }

    if (wiringPiSetup() == -1)
        return 1;

    pinMode(CS0, OUTPUT);
    pinMode(CS1, OUTPUT);
    pinMode(CS2, OUTPUT);

    for (i=0; i < displays; i++) {
        send_command(i, SYS_DIS);
        send_command(i, COMMON_8NMOS);
        /*if (i==0)*/ send_command(i, MASTER_MODE);
        //else send_command(i, SLAVE_MODE);
        send_command(i, SYS_EN);
        send_command(i, LED_ON);
        set_brightness(i, 15);
        blink(i, 0);
    }

    /*for(i=0;i<(displays*width);i++) {
        *(matrix+i) = 0x18;
    }
    write_matrix(0, matrix, 32);*/
    //game(width * displays, height);
    write_message("Et now le scroll");
    //write_message("Merry Christmas and a Happy New Year! I really hope this works and looks cool. Let's go eat some ice cream cones.");
    // write_message("Good Morning Carolyn & Elena! Are you ready for a fun day? Perhaps a trip to the library?");
    free(matrix);
    //free(matrix_buf);
    matrix = NULL;

    return 0;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/
