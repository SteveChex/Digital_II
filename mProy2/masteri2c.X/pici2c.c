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
#include "pici2c.h"
#define _XTAL_FREQ 4000000  // PARA EL USO DE LA FUNCIÓN __delay_ms

void i2c_iniciar(void) {
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    SEN = 1;
}

void i2c_funcion(uint8_t addr, uint8_t mode) {
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    if (0 == mode) {
        SSPBUF = addr & 0B11111110; // MODO ESCRIBIR
    } else if (1 == mode) {
        SSPBUF = addr | 0B00000001; // MODO LEER 
    }
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
}

void i2c_escribir(uint8_t data) {
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    SSPBUF = data;
}

void i2c_detener(void) {
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    PEN = 1;
}

void i2c_leer(uint8_t ak, uint8_t *lect) {
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    RCEN = 1;
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    *lect = SSPBUF;
    while ((SSPSTAT & 0B00000100) || (SSPCON2 & 0B00011111));
    ACKDT = (ak) ? 1 : 0;
    ACKEN = 1;
}