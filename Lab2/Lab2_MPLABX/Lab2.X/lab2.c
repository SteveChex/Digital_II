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
#include "lab2lib.h"  // NOMBRE DE LA LIBRERÍA CREADA PARA EL LABORATORIO


//******************************************************************************
//                 DEFINICIONES, PROTOTIPOS Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 4000000  //PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
void adc_start(void);   // FUNCIÓN QUE INICIA LA CONVERSIÓN DEL ADC
void __interrupt()isr(void);  //VECTOR DE INTERRUPCIÓN

uint8_t adc_value = 0;
uint8_t hex_h, hex_l;

//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    while (1) {
        adc_start();  // FUNCIÓN DE INICIO DEL ADC
        decodif(adc_value, &hex_h, &hex_l); // FUNCIÓN CONVERSORA A HEXADECIMAL
        if (PORTD < adc_value){ // ALARMA DE COMPARACIÓN ENTRE CONTADORES
            PORTBbits.RB3 = 1;  // CONTADOR HEX MAYOR A CONTADOR BINARIO
        }
        else {
            PORTBbits.RB3 = 0;  // CONTADOR HEX MENOR A CONTADOR BINARIO
        }
        if (PORTBbits.RB0 || PORTBbits.RB1){ // INCREMENTO/DECREMENTO CONTINUO
            __delay_ms(400); // DELAY POR CAPACIDAD DE SIMULACION EN MI PC
            RBIF = 1; // FORZAR LA INTERRUPCION
        }
    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

    // CONFIGURACION GENERAL

    ANSEL = 0; // LIMPIANDO PUERTOS ANALÓGICOS
    ANSELH = 0;
    ANSELHbits.ANS8 = 1; // PUERTO ANALOGICO: B2   
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
    // INTERRUPCIONES PERIFERICAS
}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************

void adc_start(void) {
    __delay_ms(10); // TIEMPO DE ESPERA RECOMENDADO PARA EL ADC
    if (0 == ADCON0bits.GO_nDONE) {
        ADCON0bits.GO_nDONE = 1;
    }
}


//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {
    if (ADIF && ADIE) { // LECTURA DEL VALOR ANALOGICO
        adc_lect(&adc_value); // FUNCIÓN QUE LEE EL ADC Y BORRA EL BIT ADIF
    }
    if (TMR2IE && TMR2IF) { // MULTIPLEXADO DISPLAYS
        if (1 == PORTBbits.RB4) {
            PORTBbits.RB4 = 0; // APAGA EL DISPLAY ACTUAL
            PORTC = hex_l; // ESCRIBE EL NUEVO DATO
            PORTBbits.RB5 = 1; // ENCIENDE EL SIGUIENTE DISPLAY
        }
        else { // MISMA ESTRUCTURA QUE ANTES
            PORTBbits.RB5 = 0;
            PORTC = hex_h;
            PORTBbits.RB4 = 1;
        }
        TMR2IF = 0;
    }
    if (RBIE && RBIF) { // CAMBIO DE ESTADO EN EL CONTADOR BINARIO
        if (1 == PORTBbits.RB0) {
            PORTD++; // INCREMENTO EN EL PUERTO B
        }
        if (1 == PORTBbits.RB1) {
            PORTD--; // DECREMENTO EN EL PUERTO B
        }
        RBIF = 0;
    }
}