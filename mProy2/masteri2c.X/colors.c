/* 
 * Libreria para el uso del sensor de color TCS34725
 * 
 * Autor: Denny Steve Otzoy Chex
 * 
 * Adaptación reducida de la librería de arduino "Adafruit_TCS34725.h"
 * https://github.com/adafruit/Adafruit_TCS34725
 * 
 */
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include "colors.h"
#include "pici2c.h"

#define _XTAL_FREQ 4000000  // PARA EL USO DE LA FUNCIÓN __delay_ms

void escribir8(uint8_t reg, uint8_t val) {
    i2c_iniciar();
    i2c_funcion(ADDR, 0);
    i2c_escribir(COMMAND_BIT | reg);
    i2c_escribir(val);
    i2c_detener();
}

void leer8(uint8_t reg, uint8_t *data) {
    i2c_iniciar();
    i2c_funcion(ADDR, 1);
    i2c_escribir(COMMAND_BIT | reg);
    i2c_detener();

    i2c_iniciar();
    i2c_funcion(ADDR, 0);
    i2c_leer(0, &(*data));
}

void leer16(uint8_t reg, uint8_t *dataL, uint8_t *dataH) {
    i2c_iniciar();
    i2c_funcion(ADDR, 1);
    i2c_escribir(COMMAND_BIT | reg);
    i2c_detener();

    i2c_iniciar();
    i2c_funcion(ADDR, 0);
    i2c_leer(0, &(*dataH));
}