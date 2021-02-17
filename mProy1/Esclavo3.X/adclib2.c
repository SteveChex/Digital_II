/*
 * File:   adclib.c
 * Author: Steve Chex
 * 
 * Created on 12 de febrero de 2021, 10:05 AM
 */


#include <xc.h>
#include <stdint.h>
#include "adclib2.h"

#define _XTAL_FREQ 4000000  //PARA EL USO DE LA FUNCIÓN __delay_ms

void adc_lect(volatile uint8_t *data) { // LECTURA DEL ADC
    *data = ADRESH;
}

void adc_start(void) {
    if (0 == ADCON0bits.GO_nDONE) {
        __delay_ms(5); // TIEMPO DE ESPERA RECOMENDADO PARA EL ADC
        ADCON0bits.GO_nDONE = 1;
    }
}