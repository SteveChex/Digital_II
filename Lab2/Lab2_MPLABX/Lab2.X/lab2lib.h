/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LAB2LIB
#define	LAB2LIB

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void decodif(volatile uint8_t number, volatile uint8_t* mshex, volatile uint8_t* lshex);
void adc_lect(volatile uint8_t *data);

#endif  /* LAB2LIB*/