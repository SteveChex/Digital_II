/* 
 * Libreria para el uso del protocolo I2C en el PIC16F887
 * 
 * Autor: Denny Steve Otzoy Chex
 * 
 * Basada en la librería de Electrosome
 * https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * 
 */
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <pic16f887.h>
#include "pici2c.h"
#define _XTAL_FREQ 4000000  // PARA EL USO DE LA FUNCIÓN __delay_ms

void i2c_iniciar(void) {
    // EL WHILE SE ENCARGA DE QUE NO ESTÉ EN MARCHA NINGUN PROTOCOLO DE COMUNICACIÓN
    // STOP BIT, START BIT, ACKNOWLEDGE O REPATED START, ADEMAS DE VERIFICAR QUE 
    // NO SE ESTEN TRANSMITIENDO DATOS
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    SEN = 1; //ACTIVA LA TRANSMISION DE DATOS
    return;
}

void i2c_funcion(uint8_t addr, uint8_t mode) {
    uint8_t data = addr << 1; // MUEVE LA DIRECCIÓN A SU POSICION 
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));

    if (0 == mode) {
        SSPBUF = data & 0B11111110; // MODO ESCRIBIR
    } else if (1 == mode) {
        SSPBUF = data | 0B00000001; // MODO LEER 
    }
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    return;
}

void i2c_escribir(uint8_t data) {
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    SSPBUF = data; // ESCRIBE EN EL BUFER PARA INICIAR LA TRANSMISION
    return;
}

void i2c_detener(void) {
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111)) {
        PORTBbits.RB1 = SSPCON2bits.ACKEN;
    }
    PEN = 1; //DETIENE LA TRANSMISION DE DATOS
    return;
}

void i2c_reiniciar(void) {
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    RSEN = 1; // ACTIVA EL REPEATED START
}

uint8_t i2c_leer() { // LEE EL DATO DEL ESCLAVO Y LO ENVIA AL MAESTRO
    uint8_t lect = 0;
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    RCEN = 1;
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    lect = SSPBUF; 
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    ACKDT = 0;
    ACKEN = 1;// ENVIA UN ACKNOWLEDGE
    return lect;
}