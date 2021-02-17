/*
 * File:   adclib.c
 * Author: Steve Chex
 * 
 * Created on 12 de febrero de 2021, 10:05 AM
 */


#include <xc.h>
#include <stdint.h>
#include "adclib.h"

void adc_lect(volatile uint8_t *data) { // LECTURA DEL ADC
    *data = ADRESH;
}

void adc_start(void) {
    __delay_ms(5); // TIEMPO DE ESPERA RECOMENDADO PARA EL ADC
    if (0 == ADCON0bits.GO_nDONE) {
        ADCON0bits.GO_nDONE = 1;
    }
}