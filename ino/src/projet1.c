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

#define HIGH 1
#define LOW 0

// HT1632C PINs

#define DISPLAY_CS 3       /* BCM_GPIO 22 */
#define DISPLAY_WR 4       /* BCM_GPIO 23 */
#define DISPLAY_DATA 5     /* BCM_GPIO 24 */

// HT1632C Commands

#define HT1632C_READ 0b00000110
#define HT1632C_WRITE 0b00000101
#define HT1632C_CMD 0b00000100

#define HT1632_CMD_SYSON 0x01
#define HT1632_CMD_LEDON 0x03

void ht1632c_send_command(unsigned char command);
void ht1632c_send_bits(unsigned char bits, unsigned char firstbit);

void init() {
  // init GPIO
  if (wiringPiSetup() == -1)
  {
    printf("IO is not ready\n");
    return;
  }

  // All PINs are output
  printf("1\n");
  pinMode(DISPLAY_CS, OUTPUT);
  printf("2\n");
  pinMode(DISPLAY_WR, OUTPUT);
  printf("3\n");
  pinMode(DISPLAY_DATA, OUTPUT);
  
  printf("Pin mode OK\n");
  
  // Enable System oscillator and LED duty cycle generator
  ht1632c_send_command(HT1632_CMD_SYSON);
  ht1632c_send_command(HT1632_CMD_LEDON);

}

void loop(int* j) {
  int i;

  // select display
  digitalWrite(DISPLAY_CS, LOW);
 
  // send WRITE command
  ht1632c_send_bits(HT1632C_WRITE, 1 << 2);
  
  // send start address, 00h
  ht1632c_send_bits(0x00, 1 << 6);

  // send data
  for(i = 0; i < 256; i++) {
    digitalWrite(DISPLAY_WR, LOW);
    if (i == *j) digitalWrite(DISPLAY_DATA, HIGH);
    else digitalWrite(DISPLAY_DATA, LOW);
    digitalWrite(DISPLAY_WR, HIGH);
  }
  
  // unselect display
  digitalWrite(DISPLAY_CS, HIGH);

  // cycle LED position to be turned on
  if(*j < 255) (*j)++;
  else *j = 0;
  
  usleep(5000);
}


// ----------------------------------------
// HT1632C functions
// ----------------------------------------

void ht1632c_send_command(unsigned char command) 
{
  printf("Sending command %02X\n", command);  
  digitalWrite(DISPLAY_CS, LOW);
  ht1632c_send_bits(HT1632C_CMD, 1 << 2);
  ht1632c_send_bits(command, 1 << 7);
  ht1632c_send_bits(0, 1);
  digitalWrite(DISPLAY_CS, HIGH);
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
	int j=0;
    printf("Setup\n");
    init();
    printf("Loop\n");
	while(1) {
		loop(&j);
	}
    printf("End\n");
	return 0;
}
