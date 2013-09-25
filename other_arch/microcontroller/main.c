#include <pic18fregs.h>
#include "config.h"
#include "Declare.h"



//**************************************************************************************************
//Function Name: device file configuration
//Function Feature: configure MCU’s working modes and status
//Input Argument: INTIO: INTOSCIO- internal oscillator, OSC1 and OSC2 used as I/O
//ports
// WDTDIS: Disable watchdog timer
// PWRTDIS: Disable power-delay timer
// MCLREN: Enable MCLR
// PROTECT: Do NOT protect the code
// BORDIS: Brown out reset disable
// BORV25: Brown-out reset voltage set to 2.5V nominal
// PLLEN:
// DEBUGEN: In-circuit debugger enabled
//
// VCAPDIS: Voltage regulator capacitor disable
// Output Argument: void
//**************************************************************************************************

/*__CONFIG(INTIO & WDTDIS & PWRTEN & MCLREN & UNPROTECT & BORDIS &
BORV25 & PLLEN & DEBUGEN);
__CONFIG(VCAPDIS);*/

//Function Prototype Declaration
void SystemInit(void); //System Initialization
void SetHT1632CAs3208(void); //Set HT1632C to work in 32*8 mode
void CommandWrite(unsigned int command); //Write commands to all HT1632Cs
void AddressWrite(unsigned char address);
//Write address
void SPI_ModelConfigure(void);
void SPI_DataSend(const unsigned char data);
void PrintChar(unsigned char c);
//Transmit single char
void Print(void);


void main()
{
    SystemInit();
    SetHT1632CAs3208();
    CS_ON;
    Print();
    CS_OFF;
    while(1)
    //Stay here forever
    {
        ;
    }
}


//**************************************************************************************************
//Function Name: system initialization
//Function Feature: set corresponding data reading and writing of PORTB and PORTC
//Input Argument: void
//Output Argument: void
//**************************************************************************************************
void SystemInit(void)
{
    IRCF1 = 1; //Set the frequency of the internal oscillator as 8MHz
    IRCF0 = 0;
    BRGH=0; //Select low baud rate mode, default status after power-on reset
    OSCTUNE = 0x1f; //Oscillator at the maximum frequency

    ANSELB = 0x00; //PORTB as a digital I/O port
    TRISB = 0x00; //PORTB as an output port
    PORTB = 0x00; //Clear PORTB output
    TRISCbits.TRISC0 = 1; //PORTC0 (SW1 port) as an input port
    TRISCbits.TRISC1 = 1; //PORTC1 (SW2 port) as an input port
    TRISCbits.TRISC3 = 0; //PORTC3 (CLK signal) as an output port
    TRISCbits.TRISC5 = 0; //PORTC5 (DATA signal) as an output port
    T0IE = 0; //Turn off interruption of timer0
}


//**************************************************************************************************
//Function Name: SetHT1632C_As3208
//Function Feature: write basic configuration to HT1632C in command words
//Input Argument: void
//Output Argument: void
//**************************************************************************************************
void SetHT1632CAs3208(void)
{
    CommandWrite(SYS_EN); //Enable system oscillator
    CommandWrite(LED_ON); //Turn on LED

    //Select on-chip RC as the system clock
    //working in master mode
    CommandWrite(RC_MASTER_MODE);
    //N-MOS open-drain output and 32 ROW * 8
    //COM
    CommandWrite(N_MOS_COM8);
    //Set the grade of initial PWM brightness as
    //light_degree (16/16)
    CommandWrite(PWM_16);
}


//**************************************************************************************************
//Function Name: CommandWrite
//Function Feature: write control commands to all HT1632Cs
//Input Argument: command words written to “command”, specifically stated in “declare”
//function
//Output Argument: void
//Argument Description: compile control commands to all external HT1632Cs for the
//requirement of the project
//**************************************************************************************************
void CommandWrite(unsigned int command)
{
    unsigned char i;
    unsigned int j;
    command = command & 0x0fff; //12-bit command word, upper four bits masked
    CS_OFF; //Disable all HT1632Cs
    CLK_DELAY;
    CS_ON //Enable all HT1632Cs
    CLK_DELAY;
    for(i=0; i<12; i++) //Write command words in HI1632C register
    {
        CLK = 0;
        CLK_DELAY;
        j = command & 0x0800; //Return the MSB
        command = command << 1; //Move the control character to the left one
        j = j >> 11; //Position the value at the LSB
        DAT = j; //Send the value to the data port
        CLK_DELAY;
        CLK = 1; //Data transmission (data valid on rising edge)
        CLK_DELAY;
    }
    CS_OFF; //Disable all HT1632Cs
}


//**************************************************************************************************
//Function Name: AddressWrite
//Function Feature: write start address of data to HT1632C
//Input Argument: address: address to be written
//Output Argument: void
//**************************************************************************************************
void AddressWrite(unsigned char address)
{
    unsigned char i,temp;
    SSPCON = 0x11;
    address = address & 0x7f; //7-bit address, mask the MSB
    CLK = 0; //Clock line is 0
    CLK_DELAY;
    DAT = 1; //Send “1” to data port

    CLK_DELAY;
    CLK = 1; //Data transmission
    CLK_DELAY;
    CLK = 0;
    CLK_DELAY;
    DAT = 0; //Send “0” to data port
    CLK_DELAY;
    CLK = 1; //Data transmission
    CLK_DELAY;
    CLK = 0;
    CLK_DELAY;
    DAT = 1; //Send “1” to data port
    CLK_DELAY;
    CLK = 1; //Data transmission
    CLK_DELAY;
    for(i=0; i<7; i++) //Write “address” to HT1632C register
    {
        CLK = 0; //Clock line is 0
        CLK_DELAY;
        temp = address & 0x40; //Return the MSB
        address = address << 1; //Shift left once
        temp = temp >> 6; //Position the value at the LSB
        DAT = temp; //Send the value to the data port
        CLK_DELAY;
        CLK = 1; //Data transmission
        CLK_DELAY;
    }
}


//**************************************************************************************************
//Function Name: SPI_ModelConfigure
//Function Feature: configure the corresponding data transfer port of PIC microcontroller
//for SPI communication
//Input Argument: void
//Output Argument: void
//**************************************************************************************************
void SPI_ModelConfigure(void)
{
    SSPIF = 0; //Initial state: waiting to send data
    SSPCON = 0x31; //Write in this register: SSPEN=1 (enable serial port); CKP=1
    //(CLK high in an idle state); CLK is FOSC/16
    SSPSTAT = 0x80; //Write in this register: SMP=1(Input data sampled at end of
    //data output time); CKE=0(data stable on rising edge of SCK)
}
//**************************************************************************************************

