/*
 * Archivo: Lab2.c
 * Autor:   Steve Chex
 * Lab 2:   Interrupciones y Librerias
 * Descripción: Comparación de dato en contador binario contra contador
 *              hexadecimal. Incluye "alarma" de comparación entre contadores.
 *              Creado con interrupciones, modulo ADC y librería propia.
 *   
 *Created on 29 de enero de 2021, 08:02 AM
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
#include <stdint.h>



//******************************************************************************
//                 DEFINICIONES, PROTOTIPOS Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 4000000  //PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
void __interrupt()isr(void);


//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    while (1) {

    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************




//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {

}
