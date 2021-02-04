/*
 * File:   lab2lib.c
 * Author: Steve Chex
 *
 * Created on 29 de enero de 2021, 08:13 AM
 */


#include <xc.h>
#include <stdint.h>
#include "lab2lib.h"

void decodif(uint8_t number, uint8_t* mshex, uint8_t* lshex) {
    uint8_t msbits, lsbits; // DEFINIENDO VARIABLES PARA CADA NIBBLE
    msbits = number >> 4; // ELIMINANDO LOS BITS MENOOS SIGNIFICATIVOS
    lsbits = number & 0B00001111; // ELIMINANDO LOS BITS MÁS SIGNIFICATIVOS

    switch (msbits) { // TRANSFORMANDO LOS MSB EN UNA CADENA HEX
        case 0:
            *mshex = 0B00111111;
            break;
        case 1:
            *mshex = 0B00000110;
            break;
        case 2:
            *mshex = 0B01011011;
            break;
        case 3:
            *mshex = 0B01001111;
            break;
        case 4:
            *mshex = 0B01100110;
            break;
        case 5:
            *mshex = 0B01101101;
            break;
        case 6:
            *mshex = 0B01111101;
            break;
        case 7:
            *mshex = 0B00000111;
            break;
        case 8:
            *mshex = 0B01111111;
            break;
        case 9:
            *mshex = 0B01101111;
            break;
        case 10:
            *mshex = 0B01110111;
            break;
        case 11:
            *mshex = 0B01111100;
            break;
        case 12:
            *mshex = 0B00111001;
            break;
        case 13:
            *mshex = 0B01011110;
            break;
        case 14:
            *mshex = 0B01111001;
            break;
        case 15:
            *mshex = 0B01110001;
            break;
        default:
            *mshex = 0B10000000; //EL CASO DEFAULT ACTIVA EL PUNTO
    }
    switch (lsbits) {   // TRANSFORMANDO LOS LSB EN UNA CADENA HEX
        case 0:
            *lshex = 0B00111111;
            break;
        case 1:
            *lshex = 0B00000110;
            break;
        case 2:
            *lshex = 0B01011011;
            break;
        case 3:
            *lshex = 0B01001111;
            break;
        case 4:
            *lshex = 0B01100110;
            break;
        case 5:
            *lshex = 0B01101101;
            break;
        case 6:
            *lshex = 0B01111101;
            break;
        case 7:
            *lshex = 0B00000111;
            break;
        case 8:
            *lshex = 0B01111111;
            break;
        case 9:
            *lshex = 0B01101111;
            break;
        case 10:
            *lshex = 0B01110111;
            break;
        case 11:
            *lshex = 0B01111100;
            break;
        case 12:
            *lshex = 0B00111001;
            break;
        case 13:
            *lshex = 0B01011110;
            break;
        case 14:
            *lshex = 0B01111001;
            break;
        case 15:
            *lshex = 0B01110001;
            break;
        default:
            *lshex = 0B10000000; //EL CASO DEFAULT ACTIVA EL PUNTO
    }
}


