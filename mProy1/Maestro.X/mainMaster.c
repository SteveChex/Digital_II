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
void spi_pot(void);
void spi_count(void);
void spi_temp(void);
void map_var(uint8_t v_in, uint8_t *v_out, uint8_t inMin, uint8_t inMax,
        uint8_t outMin, uint8_t outMax);
void voltaje(uint8_t data_in, uint8_t *volt_u, uint8_t *volt_d1,
        uint8_t *volt_d2);
void conts(uint8_t data_in, uint8_t *uni, uint8_t *dec, uint8_t *cent);

// VARIABLES PARA RECEPCION DE DATOS
uint8_t pot = 39, cont = 39, temp = 0;
// VARIABLES PARA PROCESO DE DATOS
uint8_t pot_u = 0, pot_d1 = 0, pot_d2 = 0, temp_m = 0, cont_u = 0, cont_d = 0,
        cont_c = 0, aux = 0;
char sdata;
//******************************************************************************
//                         LOOP PRINCIPAL
//******************************************************************************

void main(void) {
    setup();
    Lcd_Init(); // INICIALIZA LA PANTALLA
    encabezado_usart();
    while (1) {
        spi_pot();
        __delay_ms(5);
        spi_count();
        __delay_ms(5);
        spi_temp();
        __delay_ms(5);
        map_var(temp, &temp_m, 0, 255, 13, 150);
        voltaje(pot, &pot_u, &pot_d1, &pot_d2);
        mostrar_titulo_lcd();
        mostrar_dato();

        usart_T_nl();
    }
}

//******************************************************************************
//                              SETUP
//******************************************************************************

void setup(void) {

    //CONFIGURACIÓN GENERAL

    ANSEL = 0;
    TRISA = 0;
    PORTA = 0;

    ANSELH = 0; // QUITANDO ENTRADAS ANALÓGICAS DE LA 8 A LA 13
    TRISD = 0; // PORTD COMO PUERTO DE SALIDA

    PORTD = 0; // LIMPIANDO PUERTOS
    PORTC = 0;

    // COMUNICACIÓN UART

    usart_conf(); // SE APROVECHARÁ LA CONFIGURACIÓN USADA PARA EL LAB 3

    // COMUNICACIÓN SPI

    TRISB = 0B00011111; //PINES 7-5 DEL PUERTO B SERAN SALIDAS
    PORTB = 0B11100000; // DESACTIVANDO LA RECEPCIÓN DE DATOS
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
    Lcd_Write_String("S1:    S2:  S3: ");
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

void mostrar_dato(void) { // MUESTRA LOS DATOS EN LA PANTALLA LCD Y TERMINAL
    Lcd_Set_Cursor(2, 1);
    // LINE CONTROL ("0----5----0----5")
    //       HEADER ("S1:    S2: S3:  ")
    //       FORMAT ("X.XXV  XXX XXX°C")

    // VOLTAJE (S1) = "X.XXV"
    num_ascii(pot_u, &sdata);
    Lcd_Write_Char(sdata);
    usart_T_virt(sdata);
    __delay_ms(2);
    Lcd_Write_Char(46); // PUNTO PARA EL FORMATO
    usart_T_virt(46);
    __delay_ms(2);
    num_ascii(pot_d1, &sdata);
    Lcd_Write_Char(sdata);
    usart_T_virt(sdata);
    __delay_ms(2);
    num_ascii(pot_d2, &sdata);
    Lcd_Write_Char(sdata);
    usart_T_virt(sdata);
    __delay_ms(2);
    Lcd_Write_Char(86);
    usart_T_virt(86);
    __delay_ms(2);

    Lcd_Write_String("  "); // ESPACIOS DE FORMATO
    usart_T_virt(32);
    __delay_ms(2);
    usart_T_virt(32);
    __delay_ms(2);

    // CONTADOR (S2) = "XXX"
    conts(cont, &cont_u, &cont_d, &cont_c);
    num_ascii(cont_c, &sdata);
    Lcd_Write_Char(sdata);
    usart_T_virt(sdata);
    __delay_ms(2);
    num_ascii(cont_d, &sdata);
    Lcd_Write_Char(sdata);
    usart_T_virt(sdata);
    __delay_ms(2);
    num_ascii(cont_u, &sdata);
    Lcd_Write_Char(sdata);
    usart_T_virt(sdata);
    __delay_ms(2);

    Lcd_Write_String("  "); // ESPACIOS DE FORMATO
    usart_T_virt(32);
    __delay_ms(2);
    usart_T_virt(32);
    __delay_ms(2);

    // TEMPERATURA (S3) = "XXX°C"
    conts(temp_m, &cont_u, &cont_d, &cont_c);
    num_ascii(cont_c, &sdata);
    Lcd_Write_Char(sdata);
    usart_T_virt(sdata);
    __delay_ms(2);
    num_ascii(cont_d, &sdata);
    Lcd_Write_Char(sdata);
    usart_T_virt(sdata);
    __delay_ms(2);
    num_ascii(cont_u, &sdata);
    Lcd_Write_Char(sdata);
    usart_T_virt(sdata);
    __delay_ms(2);
    Lcd_Write_Char(67);
    usart_T_virt(67);
    __delay_ms(2);
}

void spi_pot(void) { // INICIA LA RECEPCIÓN DEL DATO DEL POTENCIÓMETRO
    PORTBbits.RB7 = 0;
    __delay_ms(1);
    SSPBUF = 0;
}

void spi_count(void) { // INICIA LA RECEPCIÓN DEL DATO DEL CONTADOR
    PORTBbits.RB6 = 0;
    __delay_ms(1);
    SSPBUF = 0;
}

void spi_temp(void) { // INICIA LA RECEPCIÓN DEL DATO DEL SENSOR DE TEMPERATURA
    PORTBbits.RB5 = 0;
    __delay_ms(1);
    SSPBUF = 0;
}
// LA SIGUIENTE ES UNA FUNCIÓN DE MAPEO. CONSISTE EN UNA RELACIÓN LINEAL ENTRE
// LA VARIABLE v_in Y LA VARIABLE v_out, AÑADIENDO DESFASES Y RANGOS. 
// SIMILAR A LA FUNCIÓN map() DE ARDUINO.
// EXTRAIDA Y ADAPTADA DE LA PÁGINA
// https://deepbluembedded.com/map-function-embedded-c/

void map_var(uint8_t v_in, uint8_t *v_out, uint8_t inMin, uint8_t inMax,
        uint8_t outMin, uint8_t outMax) {
    *v_out = ((((v_in - inMin)*(outMax - outMin)) / (inMax - inMin)) + outMin);
}

void voltaje(uint8_t data_in, uint8_t *volt_u, uint8_t *volt_d1, uint8_t *volt_d2) {
    // ALGORITMO DE DIVISIÓN. NO ES UNA DIVISIÓN EXACTA, SINO APROXIMADA
    // UTIL PARA LOS FINES DEL PROYECTO. 
    // DIVIDE EL DATO DEL POTENCIOMETRO ENTRE 50 PARA OBTENER UN VALOR ENTRE 0 Y 5 DE VOLTAJE
    // EL RESIDUO LO DIVIDE ENTRE 10 PARA OBTENER EL PRIMER DECIMAL DE VOLTAJE
    // Y ESTE ULTIMO RESIDUO LO DIVIDE ENTRE 5 PARA EL SEGUNDO DECIMAL.
    while (data_in > 49) { // DIVIDE ENTRE 50
        data_in = data_in - 50;
        aux++; // PARA OBTENER VALORES ENTRE 0 Y 5    
    }
    *volt_u = aux;
    aux = 0;

    while (data_in > 9) { // DIVIDE ENTRE 10
        data_in = data_in - 10;
        aux++; // PARA OBTENER VALORES ENTRE 1 Y 9
    }
    *volt_d1 = aux;
    *volt_d2 = data_in; // ULTIMO DECIMAL
    aux = 0;
}

void conts(uint8_t data_in, uint8_t *uni, uint8_t *dec, uint8_t *cent) {
    // ALGORITMO DE DIVISIÓN. NO ES UNA DIVISIÓN EXACTA, SINO APROXIMADA
    // UTIL PARA LOS FINES DEL PROYECTO. SIMILAR A LA FUNCIÓN voltaje()
    // DIVIDE EL DATO DE ENTRADA PARA OBTENER SUS UNIDADES, DECENAS Y CENTENAS
    // Y LUEGO LAS DEVUELVE A VARIABLES SEPARADAS
    while (data_in > 99) { // DIVIDE ENTRE 100
        data_in = data_in - 100;
        aux++; // PARA OBTENER CENTENAS    
    }
    *cent = aux;
    aux = 0;

    while (data_in > 9) { // DIVIDE ENTRE 10
        data_in = data_in - 10;
        aux++; // PARA OBTENER DECENAS
    }
    *dec = aux;
    *uni = data_in; // LAS UNIDADES SON LO QUE SOBRO
    aux = 0;
}
//******************************************************************************
//                          INTERRUPCION
//******************************************************************************

void __interrupt()isr(void) {
    GIE = 0; // DESACTIVANDO INTERRUPCIONES GLOBALES MOMENTANEAMENTE
    if (1 == SSPIF) {
        if (0 == PORTBbits.RB7) { // "MULTIPLEXANDO" RECEPCION DE DATOS
            PORTBbits.RB7 = 1;
            pot = SSPBUF;
        } else if (0 == PORTBbits.RB6) {
            PORTBbits.RB6 = 1;
            cont = SSPBUF;
        } else if (0 == PORTBbits.RB5) {
            PORTBbits.RB5 = 1;
            temp = SSPBUF;
        }
        SSPIF = 0;
    }
    GIE = 1; // REACTIVANDO INTERRUPCIONES
}
