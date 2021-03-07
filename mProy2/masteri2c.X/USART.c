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

#define _XTAL_FREQ 4000000

void usart_conf(void) {

    SPBRG = 25; // ESTABLECIENDO BAUDRATE A 9600 MHZ CON Fosc = 4 MHz. PROXIMAMENTE SERÁ SOLICITADO POR LA FUNCIÓN
    SPBRGH = 0;
    BAUDCTL = 0B00000000; // SCKP = 0, BRG16 = 0, WUE = 0, ABDEN  = 0

    /*
     * SCKP     : Polaridad del reloj. Seleccionada "No invertida"
     * BRG16    : Generador de Baud Rate de 16 bits. Apagado
     * WUE      : Wake Up Enable. Seleccionada recepción normal.
     * ABDEN    : Auto Baud Rate. Desactivado.
     */

    TXSTA = 0B00100100; // SYNC = 0, TX9 = 0, BRGH = 1, SENDB = 0, TXEN  = 1

    /*
     * SYNC     : Comunicación Sincrona o Asincrona. Seleccionada Asincrona
     * TX9      : Transmisión de 9 bits. Apagada = transmisión de 8 bits
     * BRGH     : High Baud Rate Select bit. Activado
     * SENDB    : Iniciar envio de "Break Character". Apagado
     * TXEN     : Transmisor habilitado. Activado
     */

    RCSTA = 0B10010000; // SPEN = 1, RX9 = 0, CREN = 1, ADDEN  = 0                           

    /*
     * SPEN     : Habilitar Puerto Serial. Activada
     * RX9      : Recepción de 9 bits. Deshabilitada = recepción de 8 bits
     * CREN     : Habilitar recepción continua de datos. Activada
     * ADDEN    : Habilitar deteccion de dirección. Desactivada.
     */

}

void usart_T(uint8_t t_data) {
    if (1 == TXSTAbits.TRMT) {
        TXREG = t_data;
    }
}
/*
void usart_T_nl(void) {
    if (1 == TXSTAbits.TRMT) {
        TXREG = 13;
    }
}

void usart_T_erase(void) {
    if (1 == TXSTAbits.TRMT) {
        TXREG = 12;
    }
}
*/
void usart_R(uint8_t *r_data) {
    if (1 == PIR1bits.RCIF) {
        *r_data = RCREG;
    }
}
/*
void num_ascii(uint8_t value, char *code) {
    switch (value) {
        case 0:
            *code = 48;
            break;
        case 1:
            *code = 49;
            break;
        case 2:
            *code = 50;
            break;
        case 3:
            *code = 51;
            break;
        case 4:
            *code = 52;
            break;
        case 5:
            *code = 53;
            break;
        case 6:
            *code = 54;
            break;
        case 7:
            *code = 55;
            break;
        case 8:
            *code = 56;
            break;
        case 9:
            *code = 57;
            break;
        default:
            *code = 63;
            break;
    }
}

void usart_T_string(char *data) {
    int i;
    for (i = 0; data[i] != '\0'; i++)
        usart_T_virt(data[i]); //POR CADA LETRA EN LA CADENA, SE LLAMA A LA FUNCION QUE ENVIA LETRAS A LA PANTALLA
    __delay_ms(30);
}
*/
