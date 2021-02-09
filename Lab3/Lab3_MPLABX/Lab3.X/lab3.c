/*
 * Archivo: Lab3.c
 * Autor:   Steve Chex
 * Lab 2:   Interrupciones y Librerias
 * Descripción: Comparación de dato en contador binario contra contador
 *              hexadecimal. Incluye "alarma" de comparación entre contadores.
 *              Creado con interrupciones, modulo ADC y librería propia.
 *   
 *Created on 29 de enero de 2021, 08:02 AM
 */
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
#include <stdio.h>
#include "lcd.h" // USO DE LA LIBRERIA MODIFICADA PARA LA PANTALLA LCD
#include "USART.h" // USO DE LA LIBRERIA DE COMUNICACIÓN USART

//******************************************************************************
//                 DEFINICIONES, PROTOTIPOS Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 4000000  //PARA EL USO DE LA FUNCIÓN __delay_ms

void setup(void);
void __interrupt()isr(void);
void adc_start(void);
void mostrar_texto(void);
void mostrar_datos(void);
void voltaje(uint8_t data_in, uint8_t *volt_u, uint8_t *volt_d1, uint8_t *volt_d2);
void numero(uint8_t data_in, uint8_t *cent, uint8_t *dec, uint8_t *uni);
void encabezado_usart(void);

uint8_t pot1 = 0, pot2 = 0, cont = 0; // ALMACENA DATOS DE POTS Y CONTADOR
uint8_t RC_data, TX_data = 5; // VARIABLES PARA TRANSMITIR DATOS A LA CONSOLA
uint8_t vol_u = 0, vol_dec1 = 0, vol_dec2 = 0; // VARIABLES PARA DATOS DE VOLTAJE. (UNIDAD, DECIMAL 1, DECIMAL 2)
uint8_t cont_cent = 0, cont_dec = 0, cont_uni = 0;
uint8_t temp = 0; // VARIABLE TEMPORAL PARA "TRADUCIR" NUMEROS A VOLTAJE
//char allData_t; // VARIABLE QUE RECIBE EL FORMATO PARA ENVIAR A LA PANTALLA
char data_t;
//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    Lcd_Init(); // INICIALIZA LA PANTALLA
    //Lcd_Credits(); // MUESTRA EL CÓDIGO ORIGINAL DE BIENVENIDA (dura alrededor de 1 min)
    while (1) {
        usart_T_erase();
        encabezado_usart(); // MUESTRA LOS DATOS EN LA CONSOLA DE LA PC
        adc_start(); // INICIA LA LECTURA DEL ADC EN EL PRIMER POTENCIOMETRO
        __delay_ms(10); // TIEMPO CONVENIENTE DE ESPERA
        adc_start(); // INICIA LA LECTURA DEL ADC EN EL SEGUNDO POTENCIOMETRO
        __delay_ms(10);
        mostrar_texto(); // ENVIA EL ENCABEZADO A LA LCD
        mostrar_datos(); // ENVIA LOS DATOS A LA LCD Y A LA CONSOLA
        __delay_ms(30);
        // delay combinado: 100 ms
    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

    // CONFIGURACION GENERAL

    //ANSEL = 0; // USADAS PARA DEBUG DE DATOS
    //TRISA = 0;
    ANSELH &= 0B11110100; // RB2 Y RB3 SERÁN DIGITALES, RB1 Y RB0 ANALOGICAS
    TRISD = 0; // TODO PORTD SERÁ UNA SALIDA
    TRISB &= 0B11100011; // RB2 Y RB3 SERAN SALIDAS. RB1 Y RB0 SERAN ENTRADAS
    PORTB = 0;
    PORTD = 0;

    // ADC

    ADCON0 = 0B01110000; // CONVERSION EN B0 (AN12) A Fosc/8 (2ns) DE Tad
    ADCON1 = 0B00000000; // JUSTIFICADO A LA IZQUIERDA. REFERENCIA VDD Y VSS
    ADCON0bits.ADON = 1; //ACTIVANDO MODULO ADC

    // COMUNICACIÓN

    usart_conf();

    // INTERRUPCIONES

    PIE1 |= 0B01100000; // ACTIVANDO INTERRUPCION DEL ADC Y RC USART.
    INTCON |= 0B11000000; // ACTIVANDO INTERRUPCIONES PERIFERICAS.

}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************

void adc_start(void) {
    __delay_ms(20); // TIEMPO DE ESPERA RECOMENDADO PARA EL ADC
    if (0 == ADCON0bits.GO_nDONE) {
        ADCON0bits.GO_nDONE = 1;
    }
}

void mostrar_texto(void) { // MUESTRA EN LA PANTALLA EL ENCABEZADO
    Lcd_Set_Cursor(1, 1);
    // LINE CONTROL ("0----5----0----5")
    Lcd_Write_String("S1:   S2:   S3: ");
}

void mostrar_datos(void) { // MUESTRA EN LA PANTALLA LOS DATOS ENTRANTES
    // Evitar la declaración de variables dentro de las funciones. 
    // Desconozco todos los casos para los que esto es perjudicial.

    Lcd_Set_Cursor(2, 1);
    // LINE CONTROL        ("0----5----0----5")
    // SE ENVIARA EL TEXTO: "X.XXV X.XXV  XXX"
    vol_u = 0;
    vol_dec1 = 0;
    vol_dec2 = 0; // REINICIANDO VARIABLES AUXILIARES

    // POT 1

    voltaje(pot1, &vol_u, &vol_dec1, &vol_dec2); // TRADUCIENDO A VOLTAJE EL VALOR DEL ADC

    num_ascii(vol_u, &data_t); // TRADUCIENDO A ASCII EL ENTERO DEL VOLTAJE
    Lcd_Write_Char(data_t); // ENVIANDO DATO A LA PANTALLA
    usart_T_virt(data_t); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);

    Lcd_Write_Char(46); // PUNTO DECIMAL DEL FORMATO
    usart_T_virt(46); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);

    num_ascii(vol_dec1, &data_t); // TRADUCIENDO A ASCII EL PRIMER DECIMAL
    Lcd_Write_Char(data_t);
    usart_T_virt(data_t); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);

    num_ascii(vol_dec2, &data_t); // TRADUCIENDO A ASCII EL SEGUNDO DECIMAL
    Lcd_Write_Char(data_t);
    usart_T_virt(data_t); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);

    Lcd_Write_String("V "); // ESCRIBIENDO LA DIMENSIONAL EN LA PANTALLA Y SEPARANDO    
    usart_T_virt(86); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);
    usart_T_virt(32); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);
    
    // POT 2

    voltaje(pot2, &vol_u, &vol_dec1, &vol_dec2); // TRADUCIENDO A VOLTAJE EL VALOR DEL ADC 

    num_ascii(vol_u, &data_t); // TRADUCIENDO A ASCII EL ENTERO DEL VOLTAJE
    Lcd_Write_Char(data_t); // ENVIANDO DATO A LA PANTALLA
    usart_T_virt(data_t); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);

    Lcd_Write_Char(46); // PUNTO DECIMAL DEL FORMATO
    usart_T_virt(46); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);

    num_ascii(vol_dec1, &data_t); // TRADUCIENDO A ASCII EL PRIMER DECIMAL
    Lcd_Write_Char(data_t);
    usart_T_virt(data_t); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);
    num_ascii(vol_dec2, &data_t); // TRADUCIENDO A ASCII EL SEGUNDO DECIMAL
    Lcd_Write_Char(data_t);
    usart_T_virt(data_t); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);

    Lcd_Write_String("V  "); // ESCRIBIENDO LA DIMENSIONAL EN LA PANTALLA Y SEPARANDO
    usart_T_virt(86); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);
    usart_T_virt(32); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);
    usart_T_virt(32); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);
    
    // CONTADOR

    numero(cont, &cont_cent, &cont_dec, &cont_uni);

    num_ascii(cont_cent, &data_t);
    Lcd_Write_Char(data_t);
    usart_T_virt(data_t); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);
    num_ascii(cont_dec, &data_t);
    Lcd_Write_Char(data_t);
    usart_T_virt(data_t); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);
    num_ascii(cont_uni, &data_t);
    Lcd_Write_Char(data_t);
    usart_T_virt(data_t); // ENVIANDO DATO A LA TERMINAL
    __delay_ms(2);
    usart_T_nl();
    __delay_ms(2);

}

void voltaje(uint8_t data_in, uint8_t *volt_u, uint8_t *volt_d1, uint8_t *volt_d2) {
    // ALGORITMO DE DIVISIÓN. NO ES UNA DIVISIÓN EXACTA, SINO APROXIMADA
    // UTIL PARA LOS FINES DEL PROYECTO. 
    // DIVIDE EL DATO DEL ADC ENTRE 50 PARA OBTENER UN VALOR ENTRE 0 Y 5 DE VOLTAJE (el adecuado es 51))
    // EL RESIDUO LO DIVIDE ENTRE 10 PARA OBTENER EL PRIMER DECIMAL DE VOLTAJE
    // Y ESTE ULTIMO RESIDUO LO DIVIDE ENTRE 5 PARA EL SEGUNDO DECIMAL.
    while (data_in > 49) { // DIVIDE ENTRE 50
        data_in = data_in - 50;
        temp++; // PARA OBTENER VALORES ENTRE 0 Y 5    
    }
    *volt_u = temp;
    temp = 0;

    while (data_in > 9) { // DIVIDE ENTRE 10
        data_in = data_in - 10;
        temp++; // PARA OBTENER VALORES ENTRE 1 Y 9
    }
    *volt_d1 = temp;
    temp = 0;
    while (data_in > 0) { // RESTA 1 PARA OBTENER EL ULTIMO DECIMAL (NO ES PRECISO)
        data_in--;
        temp++;
    }
    *volt_d2 = temp;
    temp = 0;
}

void numero(uint8_t data_in, uint8_t *cent, uint8_t *dec, uint8_t *uni) {
    while (data_in > 99) {
        data_in = data_in - 100;
        temp++;
    }
    *cent = temp;
    temp = 0;
    while (data_in > 9) {
        data_in = data_in - 10;
        temp++;
    }
    *dec = temp;
    temp = 0;
    while (data_in > 0) {
        data_in--;
        temp++;
    }
    *uni = temp;
    temp = 0;
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
//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {
    if (ADIF && ADIE) {
        if (ADCON0bits.CHS2) { // LEYENDO EL VALOR DE B0
            pot1 = ADRESH;
            ADCON0 = 0B01101001; // CAMBIANDO PIN A B1 (AN10)
        } else if (ADCON0bits.CHS1) { // LEYENDO EL VALOR DE B1
            pot2 = ADRESH;
            ADCON0 = 0B01110001; // CAMBIANDO PIN A B0 (AN12)          
        }
        ADIF = 0;
    }
    if (RCIE && RCIF) {
        usart_R(&RC_data);
        if (43 == RC_data) {
            cont++;
        } else if (45 == RC_data) {
            cont--;
        }
    }
}
