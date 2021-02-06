/* 
 * Libreria de comunicacion USART
 * 
 * Implementa la comunicación USART y su configuración
 * 
 * Autor: Denny Steve Otzoy Chex
 * 
 */

#include <xc.h>
#include <stdint.h>
#include "USART.h"

void usart_config(void){

    SPBRG = 25; // ESTABLECIENDO BAUDRATE A 9600 MHZ CON Fosc = 4 MHz. PROXIMAMENTE SERÁ SOLICITADO POR LA FUNCIÓN
    SPBRGH = 0;
    BAUDCTL = 0B00000000; // SCKP = 0, BRG16 = 0, WUE = 0, ABDEN  = 0

    TXSTA = 0B00100100; // SYNC = 0, 8 BITS, BRGH = 1, SENDB = 0, TXEN  = 1
    RCSTA = 0B10010000; // SPEN = 1, RX9 = 0, CREN = 1, ADDEN  = 0                           

}
