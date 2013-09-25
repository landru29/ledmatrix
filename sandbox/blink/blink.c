#include <stdio.h>
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


#define	LED	16

int main (void)
{
  printf ("Raspberry Pi blink\n") ;

  if (wiringPiSetup () == -1)
    return 1 ;

  pinMode (LED, OUTPUT) ;

  for (;;)
  {
    digitalWrite (LED, 1) ;	// On
    delay (500) ;		// mS
    digitalWrite (LED, 0) ;	// Off
    delay (500) ;
  }
  return 0 ;
}

