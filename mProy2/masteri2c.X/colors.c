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

void escribir8(uint8_t reg, uint8_t val) { // PROTOCOLO PARA ESCRIBIR AL SENSOR
    i2c_iniciar();
    i2c_funcion(ADDRESS, 0);
    i2c_escribir(COMMAND_BIT | reg);
    i2c_escribir(val);
    i2c_detener();
    return;
}

uint8_t leer8(uint8_t reg) { // PROTOCOLO PARA LEER DEL SENSOR
    uint8_t dato = 0;
    i2c_iniciar();
    i2c_funcion(ADDRESS, 0);
    i2c_escribir(COMMAND_BIT | reg);
    i2c_reiniciar();
    i2c_funcion(ADDRESS, 1);
    if (!ACKSTAT) {
        dato = i2c_leer();
        i2c_detener();
    } else {
        dato = 0;
        i2c_detener();
    }
    PORTBbits.RB2 = 0;
    return dato;
}

void pedirLectura(uint8_t reg) {
    i2c_iniciar();
    i2c_funcion(ADDRESS, 0);
    i2c_escribir(COMMAND_BIT | reg);
}

uint8_t obtenerLectura() {
    uint8_t data = 0;
    i2c_reiniciar();
    i2c_funcion(ADDRESS, 1);
    data = i2c_leer();
    i2c_detener();
}

uint16_t leerColor(uint8_t reg) { // PROTOCOLO FALLIDO PARA LEER 16 BITS DEL SENSOR
    uint16_t data = 0, temp = 0;
    uint8_t low = 0, high = 0;
    i2c_iniciar();
    i2c_funcion(ADDRESS, 0);
    __delay_ms(3);
    //if (!ACKSTAT) {
    i2c_escribir(COMMAND_BIT | reg);
    __delay_ms(3);
    i2c_reiniciar();
    i2c_funcion(ADDRESS, 1);
    __delay_ms(3);
    low = i2c_leer();
    __delay_ms(3);
    i2c_reiniciar();
    i2c_funcion(ADDRESS, 0);
    __delay_ms(3);
    i2c_escribir(COMMAND_BIT | reg + 1);
    __delay_ms(3);
    i2c_reiniciar();
    i2c_funcion(ADDRESS, 1);
    __delay_ms(3);


    high = i2c_leer();
    __delay_ms(3);
    i2c_detener();
    PORTD = high;
    data = high;
    temp = low;
    data <<= 8;
    data |= temp;
    //    PORTBbits.RB0 = 0;
    /*} else {
        i2c_detener();
        PORTBbits.RB0 = 1;
    }*/

    return data;
}

void activarInterrupcion(uint8_t op) { // ACTIVA LA RECEPCION DE DATOS
    uint8_t data = 0;
    data = leer8(0x00);

    //PORTBbits.RB2 = 1;
    __delay_ms(5);

    if (op == 1) {
        data |= ENABLE_AIEN;
    } else if (op == 0) {
        data &= ~ENABLE_AIEN;
    }
    escribir8(ENABLE, data);
    __delay_ms(60);
    return;
}

void leerColores(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) { 
    // PROTOCOLO FALLIDO PARA LEER 4 VARIABLES DE 16 BITS DEL SENSOR
    *r = leer16(RDATAL);
    //RD1 = 0;
    //__delay_ms(60);
    //*c = leer16(CDATAL);
    //RD1 = 0;
    //__delay_ms(60);
    //*g = leer16(GDATAL);
    //RD1 = 0;
    // __delay_ms(60);
    //*b = leer16(BDATAL);

    __delay_ms(60);


    return;
}

void iniciar(uint8_t *datos) { //Fusion de begin y init de la librería adafruit
    // PROTOCOLO DE INICIO DEL SENSOR
    uint8_t data;
    data = leer8(ID);
    __delay_ms(5);
    if ((data != 0x44)&&(data != 0x10)) {
        // PORTBbits.RB0 = 1;
        return;
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
        //PORTBbits.RB0 = 0;
        *datos = data;
        __delay_ms(50);
        return;
    }
}