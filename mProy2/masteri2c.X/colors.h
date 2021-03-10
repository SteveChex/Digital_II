/* 
 * Libreria para el uso del sensor de color TCS34725 en PIC16F887
 * 
 * Autor: Denny Steve Otzoy Chex
 * 
 * Adaptación de la librería de arduino "Adafruit_TCS34725.h"
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

// DIRECCIONES Y COMANDOS
#define COMMAND_BIT (0x80)
#define ADDRESS (0X29)

#define ENABLE (0x00)
#define ENABLE_AIEN (0x10)
#define ENABLE_WEN (0x08)
#define ENABLE_AEN (0x02)
#define ENABLE_PON (0x01)

#define ATIME (0x01)
#define WTIME (0x03)

#define CONTROL (0x0F)

#define ID (0x12)

// REGISTROS RGBC DEL MODULO
#define CDATAL (0x14)//CLEAR
#define CDATAH (0x15)
#define RDATAL (0x16)//ROJO
#define RDATAH (0x17)
#define GDATAL (0x18)//VERDE
#define GDATAH (0x19)
#define BDATAL (0x1A)//AZUL
#define BDATAH (0x1B) 

// TIEMPOS DE INTEGRACION

// GANANCIA 

//******************************************************************************
//                      PROTOTIPOS
//******************************************************************************

void escribir8(uint8_t reg, uint8_t val);
void leer8(uint8_t reg, uint8_t *data);
uint16_t leer16(uint8_t reg);
void activarInterrupcion(uint8_t op);
uint16_t leerColor(uint8_t reg);
void iniciar(uint8_t *datos); //Fusion de begin y init

#endif	/* XC_HEADER_TEMPLATE_H */

