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
//#include "colors.h"  // Librería fallida

//******************************************************************************
//                 DEFINICIONES, PROTOTIPOS Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 4000000  // PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
void __interrupt()isr(void);

uint8_t i2c_recep = 15, usart_recep = 0;
uint8_t cont = 0;
// VARIABLES DE SENSOR
uint8_t sen_Encendido = 0, dir = 0x29;
uint16_t red = 0, blue = 0, green = 0, clear = 0;

uint8_t data = 0;

//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    __delay_ms(10);

    // - - - - - - CODIGO DE INICIALIZACION DEL SENSOR I2C - - - - -

    /*escribir8(ATIME, 0xD5);
    __delay_ms(5);
    escribir8(CONTROL, 0x01);
    __delay_ms(5);
    escribir8(ENABLE, ENABLE_PON);
    __delay_ms(5);
    escribir8(ENABLE, (ENABLE_PON | ENABLE_AEN));
    __delay_ms(105);
     */
    // - - - - - - - - - - FIN DEL CODIGO DE INICIALIZACION DEL SENSOR I2C

    while (1) {
        /*******************************************************
         * BLOQUE PARCIALMENTE FUNCIONAL PARA EL SENSOR DE COLOR
         ******************************************************/
        //Las lecturas a los registros Read only generan un error en la 
        //comunicación
        /* data = leer8(ENABLE);

         __delay_ms(5);
         data &= ~ENABLE_AIEN;
         escribir8(ENABLE, data);
         __delay_ms(105);

         //PORTD = leer8(ENABLE);

         PORTD = leer8(RDATAL);
         //__delay_ms(50);
         __delay_ms(105);

         PORTBbits.RB2 = 1;
         data = leer8(ENABLE);
         __delay_ms(5);
         data |= ENABLE_AIEN;
         escribir8(ENABLE, data);
         __delay_ms(105);
         */
        /***************************************************
         *              FIN DEL BLOQUE
         ***************************************************/
        
        // I2C
        i2c_iniciar();
        if (RB1) {
            i2c_funcion(20, 1); // LLAMADA AL ESCLAVO EN MODO LECTURA
            if (!ACKSTAT) {
                i2c_recep = i2c_leer(); // LEER AL ESCLAVO
                i2c_detener();
                PORTBbits.RB0 = 0;
            } else {
                i2c_detener();
                PORTBbits.RB0 = 1;
            }
        } else {
            i2c_funcion(20, 0); // LLAMADA AL ESCLAVO EN MODO ESCRITURA
            if (!ACKSTAT) {
                i2c_escribir(15); // ESCRIBIRLE AL ESCLAVO
                i2c_detener();
                PORTBbits.RB0 = 0;
            } else {
                i2c_detener();
                PORTBbits.RB0 = 1;
            }
        }
        usart_T(i2c_recep); // ENVIO USART
        __delay_ms(3);
        PORTD = usart_recep; //RECEPCIÓN USART
    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

    //CONFIGURACIÓN GENERAL
    TRISD = 0; // PORTD COMO PUERTO DE SALIDA
    ANSELH = 0;
    TRISB = 0B11111000;
    PORTB = 0;
    PORTD = 0; // LIMPIANDO PUERTOS

    // COMUNICACIÓN I2C
    TRISC = 0B00011000; //ASIGNANDO ENTRADAS PARA EL MODULO I2C

    SSPSTAT = 0B10000000; // DESHABILITANDO CONTROL DE SLEW RATE
    SSPCON2 = 0; // BORRANDO BANDERAS
    SSPADD = 1; //FRECUENCIA DE 400KHZ DE BAUDRATE
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
        cont++;
        RCIF = 0;
    }
    GIE = 1; // REACTIVANDO INTERRUPCIONES
}
