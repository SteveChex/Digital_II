/* 
 * Libreria de comunicacion USART
 * 
 * Implementa la comunicación USART y su configuración
 * 
 * Autor: Denny Steve Otzoy Chex
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EUSART_H
#define	EUSART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void usart_config(void);

#endif	/* XC_HEADER_TEMPLATE_H */

