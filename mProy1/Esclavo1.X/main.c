/*
 * Archivo: main.c
 * Autor:   Steve Chex
 * Descripción: Mini proyecto 1
 *              Gestión de multiples microcontroladores mediante SPI
 * 
 * 
 */

//******************************************************************************
//                        BITS DE CONFIGURACIÓN
//******************************************************************************


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
void __interrupt()isr(void);  //VECTOR DE INTERRUPCIÓN

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

    // CONFIGURACION GENERAL
    
    ANSELHbits.ANS12 = 1; // PUERTO ANALOGICO: B0
    TRISB = 0B00000111; // ENTRADAS: PUERTOS 0 AL 2
    TRISC = 0; // ASIGNANDO COMO SALIDAS LOS PUERTOS D Y C
    TRISD = 0; // PARA LOS LEDS Y 7 SEGMENTOS
    PORTB = 0; // ESTABLECIENDO VALORES INICIALES EN TODOS LOS PUERTOS
    PORTC = 0; 
    PORTD = 127; // VALOR CONVENIENTE PARA EL CONTADOR

    // ADC

    ADCON0 = 0B01100000; // CONVERSION EN B2 (AN8) A Fosc/8 (2ns) DE Tad
    ADCON1 = 0B00000000; // JUSTIFICADO A LA IZQUIERDA. REFERENCIA VDD Y VSS
    ADCON0bits.ADON = 1; //ACTIVANDO MODULO ADC

    //TIMER2

    PR2 = 0B11111111; // ASIGNANDO EL VALOR MÁXIMO DEL TIMER 2
    TMR2 = 0; // ASIGNANDO VALOR INICIAL PARA EL TIMER 2
    T2CON = 0B00000110; // INICIANDO EL TIMER 2 CON PRESCALER DE 16    

    // INTERRUPCIONES

    IOCB = 0B00000111; // ACTIVAR PINES DE INTERRUPCIÓN
    PIE1 = 0B01000010; // ACTIVAR INT. DEL ADC Y DEL TIMER2
    PIR1bits.ADIF = 0; // LIMPIANDO BANDERA DEL ADC
    INTCON = 0B11001000; // ACTIVAR INT. GLOBALES, DEL PUERTO B, RBIF = 0 E
    // INTERRUPCIONES PERIFERICASf
}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************


//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {
   
}