/* 
 * Libreria para el uso del sensor de color TCS34725 en PIC16F887
 * 
 * Autor: Denny Steve Otzoy Chex
 * 
 * Adaptación reducida de la librería de arduino "Adafruit_TCS34725.h"
 * https://github.com/adafruit/Adafruit_TCS34725
 * 
 */
#ifndef COLORSENSOR_H
#define	COLORSENSOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#define _XTAL_FREQ 4000000  // PARA EL USO DE LA FUNCIÓN __delay_ms

//******************************************************************************
//                      DEFINICIONES 
//******************************************************************************
//
#define COMMAND_BIT (0x80)
#define ADDR (0X29)


// REGISTROS RGBC DEL MODULO
#define CDATAL (0x14)//CLEAR
#define CDATAH (0x15)
#define RDATAL (0x16)//ROJO
#define RDATAH (0x17)
#define GDATAL (0x18)//VERDE
#define GDATAH (0x19)
#define BDATAL (0x1A)//AZUL
#define BDATAH (0x1B) 

//******************************************************************************
//                      PROTOTIPOS
//******************************************************************************

void escribir8(uint8_t reg, uint8_t val);
void leer8(uint8_t reg, uint8_t *data);
void leer16(uint8_t reg, uint8_t *dataL, uint8_t *dataH);


#endif	/* XC_HEADER_TEMPLATE_H */

