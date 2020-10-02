
// proteus radi, elektronika ne salje korektne baytove


#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)



#include <xc.h>
#include <pic12f675.h>
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 4000000
#define Baudrate              1200                      //bps
#define OneBitDelay           (1000000/Baudrate)
#define DataBitCount          8                         // no parity, no flow control
#define UART_RX               GP5						// UART RX pin
#define UART_TX               GP4						// UART TX pin
#define UART_RX_DIR			  TRISIO5					// UART RX pin direction register
#define UART_TX_DIR			  TRISIO4					// UART TX pin direction register
#define LED_DIR               TRISIO2
#define LED                   GP2

//Function Declarations
void InitSoftUART(void);
unsigned char UART_Receive(void);
void UART_Transmit(const char);
//void __interrupt() myISR(void);



// Main function
void main()
{	
  //   UART_RX_DIR = 0;	
  //   UART_TX_DIR = 0;	

    unsigned char buff = 0;

	ANSEL  = 0x00;       // Set ports as digital I/O, not analog input
	ADCON0 = 0x00;		 // Shut off the A/D Converter
	CMCON  = 0x07;		 // Shut off the Comparator
	VRCON  = 0x00;	     // Shut off the Voltage Reference
	OSCCAL = 64;

    
	InitSoftUART();		 // Intialize Soft UART

    LED = 1;
    
    unsigned char buff_rec = 0;
	while(1)
	{
        buff_rec = UART_Receive();
        
        UART_Transmit(buff_rec);
	}
}

//void __interrupt() myISR(void)
//{
//	if(INTF)  //If External Edge INT Interrupt
//	{
//		UART_Transmit(UART_Receive());
//
//		INTF = 0;    // Clear the interrupt
//	}
//}


void InitSoftUART(void)		// Initialize UART pins to proper values
{
    UART_RX_DIR = 1;		// Input
	UART_TX_DIR = 0;		// Output
    
	UART_TX = 1;			// TX pin is high in idle state
    
  //  INTCONbits.GIE=0;
   // INTCONbits.PEIE=0;
	
	//INTCON     |= 0x90;     // Enable Global + INT interrupt
	//OPTION_REG &= 0xbf;     // Make INT as negedge triggered
}



unsigned char UART_Receive(void)
{
   INTCONbits.GIE=0;  //off
   INTCONbits.PEIE=0;
    
	unsigned char DataValue = 0;

	//wait for start bit
	while(UART_RX==1);

	__delay_us(OneBitDelay);
	__delay_us(OneBitDelay/2);   // Take sample value in the mid of bit duration

	for ( unsigned char i = 0; i < DataBitCount; i++ )
	{
		if ( UART_RX == 1 )   //if received bit is high
		{
			DataValue += (1<<i);
		}

		__delay_us(OneBitDelay);
	}

	// Check for stop bit
	if ( UART_RX == 1 )       //Stop bit should be high
	{
		__delay_us(OneBitDelay/2);
           INTCONbits.GIE=1;
           INTCONbits.PEIE=1;
		return DataValue;
	}
	else                      //some error occurred !
	{
		__delay_us(OneBitDelay/2);
           INTCONbits.GIE=1;
           INTCONbits.PEIE=1;
		return 0x000;
	}

}




void UART_Transmit(const char DataValue)
{
	/* Basic Logic
	   
	   TX pin is usually high. A high to low bit is the starting bit and 
	   a low to high bit is the ending bit. No parity bit. No flow control.
	   BitCount is the number of bits to transmit. Data is transmitted LSB first.

	*/
    INTCONbits.GIE=0;
    INTCONbits.PEIE=0;
	// Send Start Bit
	UART_TX = 0;
	__delay_us(OneBitDelay);

	for ( unsigned char i = 0; i < DataBitCount; i++ )
	{
		//Set Data pin according to the DataValue
		if( ((DataValue>>i)&0x1) == 0x1 )   //if Bit is high
		{
			UART_TX = 1;
		}
		else      //if Bit is low
		{
			UART_TX = 0;
		}

	    __delay_us(OneBitDelay);
	}

	//Send Stop Bit
	UART_TX = 1;
	__delay_us(OneBitDelay);
    
    
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
}
