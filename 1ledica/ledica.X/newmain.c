#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <stdio.h>
#include <stdlib.h>
#include <pic12f675.h>
#include <xc.h>


#define _XTAL_FREQ 4000000 // 4MHZ internal crystal

#define LED_DIR TRISIO2
#define LED     GP2

int main(void) {
   // TRISIO     = 0x20; // GP5 configured as input, all others are output
    //TRISIO     = 0b00100000; // GP5 configured as input, all others are output
   
    LED_DIR = 0;
    LED = 1 ;
    __delay_ms(1000);
    LED = 0 ;
    __delay_ms(1000);
    
    return 0;
}
