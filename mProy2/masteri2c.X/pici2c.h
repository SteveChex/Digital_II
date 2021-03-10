/* 
 * Libreria para el uso del protocolo I2C en el PIC16F887
 * 
 * Autor: Denny Steve Otzoy Chex
 * 
 * Basada en la librería de Electrosome
 * https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * 
 */
#ifndef I2C_PIC_H
#define I2C_PIC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#define _XTAL_FREQ 4000000  // PARA EL USO DE LA FUNCIÓN __delay_ms

void i2c_iniciar(void);
void i2c_funcion(uint8_t addr, uint8_t mode);
void i2c_escribir(uint8_t data);
void i2c_detener(void);
void i2c_reiniciar(void);
uint8_t i2c_leer();

#endif	
