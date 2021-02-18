/*
 * Archivo: mainMaster.c
 * Autor:   Steve Chex
 * Descripción: Maestro
 *              Recibe los datos de los 3 esclavos y la envia a la pantalla
 *              LCD y a la terminal virtual usando SPI Y UART
 * 
 * 
 */

//******************************************************************************
//                        BITS DE CONFIGURACIÓN
//******************************************************************************


// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//******************************************************************************
//                            LIBRERIAS
//******************************************************************************

#include <xc.h>
#include <stdint.h>
#include "USART.h"
#include "lcd.h"

//******************************************************************************
//                 DEFINICIONES, PROTOTIPOS Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 4000000  // PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
void __interrupt()isr(void);
void mostrar_titulo_lcd(void);
void encabezado_usart(void);
void mostrar_dato(void);

uint8_t pot = 0, cont = 0, temp = 0;
//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    Lcd_Init(); // INICIALIZA LA PANTALLA
    while (1) {
        encabezado_usart();
        mostrar_titulo_lcd();
        mostrar_dato();
        __delay_ms(2);
        PORTBbits.RB7 = 0;
        SSPBUF = 255;

        usart_T_nl();
    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

    //CONFIGURACIÓN GENERAL

    ANSELH = 0; // QUITANDO ENTRADAS ANALÓGICAS DE LA 8 A LA 13
    TRISD = 0; // PORTD COMO PUERTO DE SALIDA

    PORTD = 0; // LIMPIANDO PUERTOS
    PORTC = 0;

    // COMUNICACIÓN UART

    usart_conf(); // SE APROVECHARÁ LA CONFIGURACIÓN USADA PARA EL LAB 3

    // COMUNICACIÓN SPI

    TRISB = 0B00011111; //PINES 7-5 DEL PUERTO B SERAN SALIDAS
    PORTB = 0B01100000; // COMENZAR LA SECUENCIA CON EL ESCLAVO 1 (RB7))
    TRISC = 0B00010000; //ASIGNANDO SALIDAS EN EL PUERTO C

    SSPSTAT = 0B00000000; // MIDDLE SAMPLED & FALLING EDGE (CKP = 1)
    SSPCON2 = 0; // BORRANDO EL REGISTRO (UTIL SOLO PARA I2C)
    SSPCON = 0B00110000; // CONFIGURANDO MODO MAESTRO E INICIANDO EL MODULO    

    // INTERRUPCIONES

    PIE1 = 0B00001000; // ACTIVAR INT. DEL ADC
    INTCON = 0B11000000; // ACTIVAR INT. GLOBALES Y PERIFERICAS
}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************

void mostrar_titulo_lcd(void) { // MUESTRA EN LA PANTALLA EL ENCABEZADO
    Lcd_Set_Cursor(1, 1);
    // LINE CONTROL ("0----5----0----5")
    Lcd_Write_String("S1:   S2:   S3: ");
}

void encabezado_usart(void) {
    // MUESTRA EL MISMO ENCABEZADO QUE LA PANTALLA LCD
    // ENVIANDO DATOS SECUENCIALMENTE

    // "S1:_-_"

    __delay_ms(2);
    usart_T_virt(83);
    __delay_ms(2);
    usart_T_virt(49);
    __delay_ms(2);
    usart_T_virt(58);
    __delay_ms(2);
    usart_T_virt(32);
    __delay_ms(2);
    usart_T_virt(32);
    __delay_ms(2);
    usart_T_virt(32);
    __delay_ms(2);

    // "S2:_-_"

    usart_T_virt(83);
    __delay_ms(2);
    usart_T_virt(50);
    __delay_ms(2);
    usart_T_virt(58);
    __delay_ms(2);
    usart_T_virt(32);
    __delay_ms(2);
    usart_T_virt(32);
    __delay_ms(2);
    usart_T_virt(32);
    __delay_ms(2);

    // "S3:_"
    usart_T_virt(83);
    __delay_ms(2);
    usart_T_virt(51);
    __delay_ms(2);
    usart_T_virt(58);
    __delay_ms(2);
    usart_T_virt(32);
    __delay_ms(2);
    usart_T_nl();
    __delay_ms(2);

}

void mostrar_dato(void) {
    Lcd_Set_Cursor(2, 1);
    // LINE CONTROL ("0----5----0----5")
    Lcd_Write_Char(pot);
}

//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {
    GIE = 0; // DESACTIVANDO INTERRUPCIONES GLOBALES MOMENTANEAMENTE
    if (SSPIF) {
        pot = SSPBUF; // LEYENDO DATO RECIBIDO
        PORTBbits.RB7 = 1;
        SSPIF = 0;
    }

    GIE = 1; // REACTIVANDO INTERRUPCIONES
}
