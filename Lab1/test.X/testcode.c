/*
 * Archivo: testcode.c
 * Autor:   Steve Chex
 * Lab 1:   Juego de Carreras
 *   
 * Created on 21 de enero de 2021, 02:57 PM
 */
//******************************************************************************
//                       CONFIGURATION BITS
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

#include <xc.h>

//******************************************************************************
//                       DEFINICIONES Y VARIABLES
//******************************************************************************

#define _XTAL_FREQ 8000000  //PARA EL USO DE LA FUNCIÓN __delay_ms

int win = 0; //BANDERA PARA ELEGIR A UN GANADOR
int start = 0; //BANDERA DE CONTROL PARA SABER SI LA CARRERA AUN NO TERMINA

void setup(void); //FUNCIONES PROTOTIPO 
void semaforo(void);

//******************************************************************************
//                            LOOP PRINCIPAL
//******************************************************************************

void main(void) {

    setup();

    while (1) {
        if (PORTBbits.RB0 == 1) start = 1;  // CONTROLA EL INICIO DE LA CARRERA
        if (start == 1) {
            semaforo(); //LLAMA A LA FUNCIÓN QUE INICIA LA CARRERA
            while (win == 0) {
                if (PORTBbits.RB1 == 1) {   //INCREMENTO PARA EL JUGADOR 1
                    __delay_ms(400); //400 ms PARA EVITAR PROBLEMAS EN EL SIMULADOR 
                    PORTD = PORTD << 1; 
                    if (PORTD == 0) win = 1;
                }
                if (PORTBbits.RB2 == 1) {   //INCREMENTO PARA EL JUGADOR 2
                    __delay_ms(400);
                    PORTC = PORTC << 1;
                    if (PORTC == 0) win = 1;
                }
            }

            PORTBbits.RB3 = 0;  //SE APAGA EL SEMAFORO. TERMINA LA CARRERA
            PORTBbits.RB4 = 0;  
            PORTBbits.RB5 = 0;

            if (PORTD > PORTC) {    //SE VERIFICA QUÉ JUGADOR GANÓ
                PORTBbits.RB6 = 1;
                PORTBbits.RB7 = 0;
            } else if (PORTD < PORTC) {
                PORTBbits.RB6 = 0;
                PORTBbits.RB7 = 1;
            } else {
                PORTBbits.RB6 = 1;  //EXISTE LA POSIBILIDAD DE EMPATE
                PORTBbits.RB7 = 1;
            }
            start = 0; //SE REINICIA LA VARIABLE DE CONTROL DE CARRERA
        }

    }
    return;
}
//******************************************************************************
//                            SETUP
//******************************************************************************
void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISC = 0;
    TRISD = 0;
    TRISB = 0B00000111; //declarando pines 0, 1 y 2 como entradas   
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
}

//******************************************************************************
//                            FUNCIONES
//******************************************************************************

void semaforo(void) {
    __delay_ms(300);    //SE DEJA UN DELAY ANTES DE INICIAR EL CONTEO REGRESIVO.
    win = 0;            //SE REINICIAN LAS VARIABLES DE CONTROL Y LOS PUERTOS
    PORTC = 0;
    PORTD = 0;
    PORTBbits.RB6 = 0;  //SE REINICIAN LOS INDICADORES DEL GANADOR
    PORTBbits.RB7 = 0;

    PORTBbits.RB3 = 1;  //SE ENCIENDEN LAS LEDS DEL SEMAFORO EN SECUENCIA
    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 0;
    __delay_ms(400);
    PORTBbits.RB3 = 0;
    PORTBbits.RB4 = 1;
    __delay_ms(700);
    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 1;
    PORTC = 1;          //SE INICIALIZA EL PUERTO DE CADA JUGADOR
    PORTD = 1;
} 