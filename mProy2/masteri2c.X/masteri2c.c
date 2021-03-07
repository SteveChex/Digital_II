/*
 * Archivo: masteri2c.c
 * Autor:   Steve Chex
 * Descripción: Maestro
 *              Recibe los datos del sensor I2C y los envia al ESP32
 */

//******************************************************************************
//                        BITS DE CONFIGURACIÓN
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

#include "USART.h"
#include "pici2c.h"
//#include "colors.h"

//******************************************************************************
//                 DEFINICIONES, PROTOTIPOS Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 4000000  // PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
void __interrupt()isr(void);

// VARIABLES PARA RECEPCION DE DATOS

uint8_t i2c_recep = 0, usart_recep = 0;

// VARIABLES PARA PROCESO DE DATOS

//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    __delay_ms(10);
    while (1) {
        // I2C
        i2c_iniciar();
        if (RB1) {
            i2c_funcion(0x29, 1);
            if (!ACKSTAT) {
                i2c_leer(0, &i2c_recep);
                i2c_detener();
                PORTBbits.RB0 = 0;
            } else {
                i2c_detener();
                PORTBbits.RB0 = 1;
            }
        } else {
            i2c_funcion(0x29, 0);
            if (!ACKSTAT) {
                i2c_escribir(15);
                i2c_detener();
                PORTBbits.RB0 = 0;
            } else {
                i2c_detener();
                PORTBbits.RB0 = 1;
            }
        }
        
        // UART TRANSMISION

        usart_T(i2c_recep);
        __delay_ms(2);
        
        PORTD = usart_recep;
        
        __delay_ms(200);

    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

    //CONFIGURACIÓN GENERAL
    TRISD = 0; // PORTD COMO PUERTO DE SALIDA
    ANSELH = 0;
    TRISB = 0B00000010;
    PORTB = 0;
    PORTD = 0; // LIMPIANDO PUERTOS
    PORTC = 0;

    // COMUNICACIÓN I2C
    TRISC = 0B00011000; //ASIGNANDO ENTRADAS PARA EL MODULO I2C

    SSPSTAT = 0B10000000; // DESHABILITANDO CONTROL DE SLEW RATE
    SSPCON2 = 0; // BORRANDO BANDERAS
    SSPADD = 9; //FRECUENCIA DE 100KHZ DE BAUDRATE
    SSPCON = 0B00101000; // ACTIVAR PUERTO SERIAL EN MODO MAESTRO I2C

    // COMUNICACIÓN USART
    usart_conf();

    // INTERRUPCIONES
    PIR1 = 0;
    PIE1 = 0B00100000;
    INTCON = 0B11000000;
}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************

//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {
    GIE = 0; // DESACTIVANDO INTERRUPCIONES GLOBALES MOMENTANEAMENTE
    if (RCIF) {
        usart_R(&usart_recep);
        RCIF = 0;
    }
    GIE = 1; // REACTIVANDO INTERRUPCIONES
}
