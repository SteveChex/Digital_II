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
#ifndef USART_H
#define	USART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void usart_conf(void);
void usart_T_virt(uint8_t *t_data);
void usart_T_nl(void);
void usart_T_erase(void);
void usart_R(uint8_t *r_data);

#endif	/* XC_HEADER_TEMPLATE_H */

