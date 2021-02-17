/* 
 * File: adclib.h   
 * Author: Steve Chex 
 * Comments: Student-created Library for PIC16F887
 * Revision history: 
 */

#ifndef ADCLIB_H
#define	ADCLIB_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void adc_lect(volatile uint8_t *data);
void adc_start(void);

#endif	/* XC_HEADER_TEMPLATE_H */

