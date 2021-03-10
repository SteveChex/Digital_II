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
#include "colors.h"

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

    // - - - - - - - - - - CODIGO DE INICIALIZACION DEL SENSOR I2C
    // BASICAMENTE ESTE FRAGMENTO ES iniciar(&datos) de colors.c
    //_ _ _ _ leer8(ID, &data);



    /*if ((data != 0x44)&&(data != 0x10)) {
        //PORTBbits.RB0 = 1;
    } else {

        // ASIGNAR TIEMPO DE INTEGRACION
        escribir8(ATIME, 0xEB);
        __delay_ms(5);
        // ASIGNAR GANANCIA
        escribir8(CONTROL, 0x01);
        __delay_ms(5);
        // HABILITAR SENSOR
        escribir8(ENABLE, ENABLE_PON);
        __delay_ms(5);
        escribir8(ENABLE, (ENABLE_PON | ENABLE_AEN));
        //        PORTBbits.RB0 = 0;
        __delay_ms(50);
    }

    // - - - leer8(0x00, &data);
    i2c_iniciar();
    i2c_funcion(ADDRESS, 0);
    //if (!ACKSTAT) {
    i2c_escribir(COMMAND_BIT | 0x00);
    i2c_detener();

    i2c_iniciar();
    i2c_funcion(ADDRESS, 1);
    data = i2c_leer();
    
    i2c_detener();
    PORTBbits.RB2 = 1;

    __delay_ms(5);
    data &= ~ENABLE_AIEN;
    
    escribir8(ENABLE, data);
     */
    __delay_ms(60);

    // - - - - - - - - - - FIN DEL CODIGO DE INICIALIZACION DEL SENSOR I2C

    while (1) {

        escribir8(ATIME, 0xEB);
        __delay_ms(5);
        escribir8(CONTROL, 0x01);
        __delay_ms(5);
        escribir8(ENABLE, ENABLE_PON);
        __delay_ms(5);
        escribir8(ENABLE, (ENABLE_PON | ENABLE_AEN));
        __delay_ms(5);

        i2c_iniciar();
        i2c_funcion(ADDRESS, 0);
        __delay_ms(3);
        //if (!ACKSTAT) {
        i2c_escribir(COMMAND_BIT | ENABLE);
        __delay_ms(3);
        i2c_reiniciar();
        i2c_funcion(ADDRESS, 1);
        __delay_ms(3);
        data = i2c_leer();
        i2c_detener();
        // _ 
        __delay_ms(5);
        data &= ~ENABLE_AIEN;
        escribir8(ENABLE, data);
        __delay_ms(60);

        red = leerColor(RDATAL);

        i2c_iniciar();
        i2c_funcion(ADDRESS, 0);
        __delay_ms(3);
        //if (!ACKSTAT) {
        i2c_escribir(COMMAND_BIT | ENABLE);
        __delay_ms(3);
        i2c_reiniciar();
        i2c_funcion(ADDRESS, 1);
        __delay_ms(3);
        data = i2c_leer();
        i2c_detener();
        __delay_ms(5);
        data |= ENABLE_AIEN;
        escribir8(ENABLE, data);
        __delay_ms(60);
        // I2C
        /*
           i2c_iniciar();
           if (RB1) {
               i2c_funcion(20, 1);
               if (!ACKSTAT) {
                   i2c_recep = i2c_leer();
                   i2c_detener();
                   PORTBbits.RB0 = 0;
               } else {
                   i2c_detener();
                   PORTBbits.RB0 = 1;
               }
           } else {
               i2c_funcion(20, 0);
               if (!ACKSTAT) {
                   i2c_escribir(15);
                   i2c_detener();
                   PORTBbits.RB0 = 0;
               } else {
                   i2c_detener();
                   PORTBbits.RB0 = 1;
               }
           }
         */

        // - - - - Llamada a activarInterrupcio(0|1)



        // - - - - Llamada a leerColores(&red, &green, &blue, &clear);
        /*
         *c = leer16(CDATAL);
         *r = leer16(RDATAL);
         *g = leer16(GDATAL);
         *b = leer16(BDATAL);
         
        __delay_ms(50);
         */
        //activarInterrupcion(0);
        //leerColores(&red, &green, &blue, &clear);
        //activarInterrupcion(1);

        /*
       PORTBbits.RB0 = 1;
       // UART TRANSMISION
       cont = (uint8_t) red;
       usart_T(cont);
       __delay_ms(3);

       cont = (uint8_t) red >> 8;
       usart_T(cont);
       __delay_ms(3);

       cont = (uint8_t) green;
       usart_T(cont);
       __delay_ms(3);

       cont = (uint8_t) green >> 8;
       usart_T(cont);
       __delay_ms(3);

       cont = (uint8_t) blue;
       usart_T(cont);
       __delay_ms(3);

       cont = (uint8_t) blue >> 8;
       usart_T(cont);
       __delay_ms(3);

       cont = (uint8_t) clear;
       usart_T(cont);
       __delay_ms(3);

       cont = (uint8_t) clear >> 8;
       usart_T(cont);
       __delay_ms(3);

         */
        PORTBbits.RB0 = !RB0;
        red >>= 8;
        cont = (uint8_t) red;
        usart_T(cont);
        __delay_ms(3);

        //PORTD = (uint8_t) red;

        __delay_ms(500);

    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

    //CONFIGURACIÓN GENERAL
    TRISD = 0; // PORTD COMO PUERTO DE SALIDA
    ANSELH = 0;
    TRISB = 0B11111010;
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
        RCIF = 0;
    }
    GIE = 1; // REACTIVANDO INTERRUPCIONES
}
