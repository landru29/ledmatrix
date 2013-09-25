#ifndef _DECLARE_
#define _DECLARE_

//Macro definition of ports used

//8 control ports
#define CS1 RB0
#define CS2 RB1
#define CS3 RB2
#define CS4 RB3
#define CS5 RB4
#define CS6 RB5
#define CS7 RB6
#define CS8 RB7

//Clock line simulating SPI communication (this
//port is also the clock line of SPI communication
//integrated by MCU)
#define CS_OFF CS1=1;CS2=1;CS3=1;CS4=1;CS5=1;CS6=1;CS7=1;CS8=1;
#define CS_ON CS1=0;CS2=0;CS3=0;CS4=0;CS5=0;CS6=0;CS7=0;CS8=0;
#define CLK RC3

//Data line simulating SPI communication (this
//port is also the data line of SPI communication
//integrated by MCU)
#define DAT RC5

//Two switches
#define SW1 RC0
#define SW2 RC1

//Following is the functions defined in a way of macro definition.
#define CLK_DELAY; NOP()

//Following definition facilitates compilation of HT1632C control commands.
#define RC_MASTER_MODE 0b100000110000 //Set master mode and clock source from on-chip RC oscillator
#define SYS_DIS 0b100000000000 //Turn off both system oscillator and LED duty cycle generator
#define SYS_EN 0b100000000010 //Turn on system oscillator
#define LED_OFF 0b100000000100 //Turn off LED duty cycle generator
#define LED_ON 0b100000000110 //Turn on LED duty cycle generator
#define N_MOS_COM8 0b100001000000 //N-MOS open drain output and 8 common option
#define PWM_16 0b100101011110 //PWM 16/16 duty
#endif
