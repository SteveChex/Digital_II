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
void usart_T(uint8_t t_data);
/*void usart_T_nl(void);
void usart_T_erase(void);*/
void usart_R(uint8_t *r_data);
/*void num_ascii(uint8_t value, char *code);
void usart_T_string(char *data);*/

#endif

