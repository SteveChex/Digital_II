/*
 * File:   testcode.c
 * Author: Steve Chex
 *
 * Created on 21 de enero de 2021, 02:57 PM
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

#include <xc.h>

#define _XTAL_FREQ 8000000

int win = 0;
int start = 0;

void setup(void);
void semaforo(void);

/*declarar 
 * ENTRADAS:
 * portb bits = 0
 * portb bits = 1
 * portb bits = 2
 * 
 * SALIDAS:
 * portc bits = 0-7
 * portd bits = 0-7
 * portb bits = 3-5
 */

void main(void) {
    setup();
    
    while (1) {
        
        if (PORTBbits.RB0 == 1) start = 1;
        
        if (start == 1){
            semaforo();
            while (win  == 0){
                if (PORTBbits.RB1 == 1){
                    __delay_ms(400);
                    PORTD = PORTD << 1;
                    if (PORTD == 0) win  = 1;
                }
                if (PORTBbits.RB2 == 1){
                    __delay_ms(400);
                    PORTC = PORTC << 1;
                    if (PORTC == 0) win  = 1;
                }
            }
            
            PORTBbits.RB3 = 0;
            PORTBbits.RB4 = 0;
            PORTBbits.RB5 = 0;
            
            if (PORTD > PORTC){
                PORTBbits.RB6 = 1;
                PORTBbits.RB7 = 0;
            }
            else if (PORTD < PORTC){
                PORTBbits.RB6 = 0;
                PORTBbits.RB7 = 1;
            }
            else {
                PORTBbits.RB6 = 1;
                PORTBbits.RB7 = 1;
            }
            start = 0;
        }
        
    }
    return;
}

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
void semaforo(void){
    __delay_ms(300);
    win = 0;
    PORTC = 0;
    PORTD = 0;            
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 0;
    
    PORTBbits.RB3 = 1;
    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 0;
    __delay_ms(400);
    PORTBbits.RB3 = 0;
    PORTBbits.RB4 = 1;
    __delay_ms(700);
    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 1;
    PORTC = 1;
    PORTD = 1;
}   