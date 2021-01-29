/*
 * File:   lab2.c
 * Author: Steve Chex
 *
 * Created on 29 de enero de 2021, 08:02 AM
 */
// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//******************************************************************************
//                            LIBRERIAS
//******************************************************************************

#include <xc.h>
#include "lab2lib.h"


//******************************************************************************
//                    DEFINICIONES Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 8000000  //PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
//void __interrupt()isr(void);

//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    while (1) {

    }

    return;
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    ANSELHbits.ANS8 = 1;
    TRISB = 0B00000111; // SALIDAS: PUERTOS 0 AL 2
    TRISC = 0;
    TRISD = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    INTCON = 0B10001000;
    IOCB = 0B00000011;

}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************




//******************************************************************************
//                          INTERRUPCIONES
//******************************************************************************

void __interrupt()isr(void) {
    if (RBIE && RBIF) {
        if (1 == PORTBbits.RB0) {
            PORTD++;
        }
        if (1 == PORTBbits.RB1) {
            PORTD--;
        }
        RBIF = 0;
    }
}