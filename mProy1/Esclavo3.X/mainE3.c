/*
 * Archivo: mainE3.c
 * Autor:   Steve Chex
 * Descripción: ESCLAVO 3
 *              Sensor de temperatura y transmisión de la información medida.
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
#include "adclib2.h"

//******************************************************************************
//                 DEFINICIONES, PROTOTIPOS Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 4000000  //PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
void __interrupt()isr(void); //VECTOR DE INTERRUPCIÓN

uint8_t temp_data = 0;
//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    while (1) {
        adc_start();
        PORTD = temp_data;
    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

    //CONFIGURACIÓN GENERAL

    TRISB &= 0B11111111; //RB0 ENTRADA. EL RESTO NO SE USARÁ
    TRISD = 0; // PORTD COMO PUERTO DE SALIDA
    ANSELH &= 0B00010000; // RB0 COMO ENTRADA ANALOGICA
    PORTD = 0; // LIMPIANDO PUERTOS
    PORTB = 0;

    // ADC

    ADCON0 = 0B01110000; // CONVERSION EN B0 (AN8) A Fosc/8 (2ns) DE Tad
    ADCON1 = 0B00000000; // JUSTIFICADO A LA IZQUIERDA. REFERENCIA VDD Y VSS
    ADCON0bits.ADON = 1; //ACTIVANDO MODULO ADC

    // INTERRUPCIONES

    PIE1 = 0B01000000; // ACTIVAR INT. DEL ADC
    PIR1bits.ADIF = 0; // LIMPIANDO BANDERA DEL ADC
    INTCON = 0B11001000; // ACTIVAR INT. GLOBALES Y PERIFERICAS
}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************


//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {
    if (1 == ADIF) {
        adc_lect(&temp_data);
        ADIF = 0;
    }

}
