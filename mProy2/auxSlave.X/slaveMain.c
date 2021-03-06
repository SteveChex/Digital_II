/*
 * Archivo: masteri2c.c
 * Autor:   Steve Chex
 * Descripción: Esclavo auxiliar
 *              Usado para depurar el funcionamiento del protocolo I2C
 *              En proteus.
 *              (El i2c debugger no funciona adecuadamente como esclavo)
 */

//******************************************************************************
//                        BITS DE CONFIGURACIÓN
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

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

#define _XTAL_FREQ 4000000  // PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
void __interrupt()isr(void);

// VARIABLES PARA RECEPCION DE DATOS

uint8_t recep = 0;
uint8_t clear = 0;
uint8_t conste = 0B10101010;
uint8_t z = 0;

// VARIABLES PARA PROCESO DE DATOS

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

    //CONFIGURACIÓN GENERAL
    TRISD = 0; // PORTD COMO PUERTO DE SALIDA
    PORTD = 0; // LIMPIANDO PUERTOS

    // COMUNICACIÓN I2C

    TRISC = 0B00011000; //ASIGNANDO ENTRADAS PARA EL MODULO I2C

    SSPSTAT = 0B10000000; // DESHABILITANDO CONTROL DE SLEW RATE
    SSPADD = 0x30; // DIRECCIÓN DEL ESCLAVO (7F)
    SSPCON2 = 1; // AJUSTANDO USO DEL RELOJ
    SSPCON = 0B00110110; // ACTIVANDO ESCLAVO (7BIT ADDRESS) Y LIBERANDO RELOJ

    // INTERRUPCIONES
    
    SSPIF = 0;
    PIE1 = 0B00001000; // ACTIVAR INTERRUPCIONES DEL MODULO I2C
    INTCON = 0B11000000; // NOOOOOOO ACTIVAR INT. GLOBALES Y PERIFERICAS
}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************

//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {
    if (SSPIF == 1) {
        SSPCONbits.CKP = 0;

        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)) {
            z = SSPBUF; // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0; // Clear the overflow flag
            SSPCONbits.WCOL = 0; // Clear the collision bit
            SSPCONbits.CKP = 1;
        }

        if (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            z = SSPBUF;
            while (!BF);
            PORTD = SSPBUF;
            SSPCONbits.CKP = 1;
        } else if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW) {
            z = SSPBUF;
            BF = 0;
            SSPBUF = 0B10101010;
            SSPCONbits.CKP = 1;
            while (SSPSTATbits.BF);
        }

        SSPIF = 0;
    }
}