/*
 * Archivo: Lab3.c
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
#include <stdio.h>
#include "lcd.h" // USO DE LA LIBRERIA MODIFICADA PARA LA PANTALLA LCD

//******************************************************************************
//                 DEFINICIONES, PROTOTIPOS Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 4000000  //PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
void __interrupt()isr(void);
void adc_start(void);
void mostrar_datos(void);

uint8_t pot1 = 0, pot2 = 0, cont = 0;
char allData_t;

//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    Lcd_Init(); // INICIALIZA LA PANTALLA
    //Lcd_Credits(); // MUESTRA EL CÓDIGO ORIGINAL DE BIENVENIDA (dura alrededor de 1 min)
    while (1) {
        adc_start(); // INICIA LA LECTURA DEL ADC
        mostrar_datos();
    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

    // CONFIGURACION GENERAL

    ANSELH &= 0B11111100; // RB2 Y RB3 SERÁN DIGITALES, RB1 Y RB0 ANALOGICAS
    TRISD = 0; // TODO PORTD SERÁ UNA SALIDA
    TRISB &= 0B11110011; // RB2 Y RB3 SERAN SALIDAS. RB1 Y RB0 SERAN ENTRADAS
    PORTB = 0;
    PORTD = 0;

    // ADC

    ADCON0 = 0B01110000; // CONVERSION EN B0 (AN12) A Fosc/8 (2ns) DE Tad
    ADCON1 = 0B00000000; // JUSTIFICADO A LA IZQUIERDA. REFERENCIA VDD Y VSS
    ADCON0bits.ADON = 1; //ACTIVANDO MODULO ADC

    // COMUNICACIÓN

    //usart_config();

    // INTERRUPCIONES

    PIE1 |= 0B01000000; //temporal: ACTIVANDO INTERRUPCIONES DEL ADC UNICAMENTE
    INTCON |= 0B11000000; // ACTIVANDO INTERRUPCIONES PERIFERICAS.

}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************

void adc_start(void) {
    __delay_ms(20); // TIEMPO DE ESPERA RECOMENDADO PARA EL ADC
    if (0 == ADCON0bits.GO_nDONE) {
        ADCON0bits.GO_nDONE = 1;
    }
}

void mostrar_datos(void) { // MUESTRA EN LA PANTALLA LOS DATOS ENTRANTES
    // Evitar la declaración de variables dentro de las funciones. 
    // Desconozco todos los casos para los que esto es perjudicial.
    Lcd_Set_Cursor(1, 1);
    // LINE CONTROL ("0----5----0----5")
    Lcd_Write_String(" POT1 POT2 CONT ");
    Lcd_Set_Cursor(2, 1);
    // LINE CONTROL   ("0----5----0----5")
    Lcd_Write_String(&allData_t);
    sprintf(&allData_t, " %3u  %3u  %3u  ", pot1, pot2, cont);
}

//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {
    if (ADIF && ADIE) {
        if (ADCON0bits.CHS2) { // LEYENDO EL VALOR DE B0
            pot1 = ADRESH;
            ADCON0 = 0B01101001; // CAMBIANDO PIN A B1 (AN10)
        } else if (ADCON0bits.CHS1) { // LEYENDO EL VALOR DE B1
            pot2 = ADRESH;
            ADCON0 = 0B01110001; // CAMBIANDO PIN A B0 (AN12)          
        }
        ADIF = 0;
    }
}
