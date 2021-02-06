/* 
 * Librería LCD con protocolo de 8 bits
 * Adaptación basada en la librería "lcd.h" desarrollada por electroSome.
 * enlace: https://electrosome.com/lcd-pic-mplab-xc8/ 
 * 
 * Permite una fácil comunicación con una pantalla LCD.
 */


#include <xc.h>
#include <stdint.h>
#include "lcd.h"

#define _XTAL_FREQ 8000000

#define RS RB2
#define EN RB3
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

void Lcd_Port(char a) { // MODIFICA EL VALOR DE LOS PINES, DE ACUERDO AL VALOR DE a
    if (a & 1) // SI EL "BIT 1" DEL DATO LEIDO ESTA ACTIVO
        D0 = 1;// ENCIENDE EL PIN 1 DEL PUERTO DE COMUNICACIÓN CON LA PANTALLA
    else
        D0 = 0;// O LO APAGA
    if (a & 2)
        D1 = 1;// Y REALIZA EL MISMO PROCEDIMIENTO CON EL RESTO DE LOS PINES
    else
        D1 = 0;
    if (a & 4)
        D2 = 1;
    else
        D2 = 0;
    if (a & 8)
        D3 = 1;
    else
        D3 = 0;
    if (a & 16)
        D4 = 1;
    else
        D4 = 0;
    if (a & 32)
        D5 = 1;
    else
        D5 = 0;
    if (a & 64)
        D6 = 1;
    else
        D6 = 0;
    if (a & 128)
        D7 = 1;
    else
        D7 = 0;
}

void Lcd_Cmd(char a) { // CAMBIA EL ESTADO DE LOS PINES Y ENVIA COMANDOS A LA PANTALLA LED
    RS = 0; // => RS = 0
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) { // FUNCIÓN PARA BORRAR LA PANTALLA. ENVIA EL COMANDO 0B0000 0001
    Lcd_Cmd(0X01);
}

void Lcd_Set_Cursor(char a, char b) { // COLOCA EL CURSOR DE ESCRITURA EN EL PUNTO a, b. a ES UNA DE LAS FILAS DE LA PANTALLA
    char temp, z, y;
    if (a == 1) { // PRIMERA FILA
        temp = 0x80 + b - 1; // esta linea está curiosa, no entiendo porqué hace eso.
        /*
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
         */
        Lcd_Cmd(temp); // ENVIO EL CONJUNTO COMPLETO, EN VEZ DE MANDAR 4 BITS
    } else if (a == 2) { // SEGUNDA FILA
        temp = 0xC0 + b - 1; // tampoco entiendo esta. no entiendo porqué los valores 0x80 y 0xC0.
        /*
        z = temp >> 4;
        y = temp & 0x0F;
        Lcd_Cmd(z);         // pero de alguna manera, esto se traduce a la función "Cursor/Display Shift"
        Lcd_Cmd(y);         // Y escribe la posicuón deseada.
         */
        Lcd_Cmd(temp); // ENVIO EL CONJUNTO COMPLETO, EN VEZ DE MANDAR 4 BITS
    }
}

void Lcd_Init(void) { // INICIANDO LA PANTALLA LCD CON EL PROTOCOLO DE 8 BITS 
    Lcd_Port(0x00); // SEÑALES INICIALES
    __delay_ms(20);
    Lcd_Cmd(0x30);
    __delay_ms(5);
    Lcd_Cmd(0x30);
    __delay_ms(11);
    Lcd_Cmd(0x30);
    //////////////////// BITS DE CONFIGURACIÓN
    Lcd_Cmd(0x38); // MODO DE FUNCIONAMIENTO
    Lcd_Cmd(0x0C); // CONFIG. DE VISUALIZADOR 
    Lcd_Cmd(0x06); // MODO DE INTRODUCCION DE CARACTERES
}

void Lcd_Write_Char(char a) { // ENVIA LETRAS A LA PANTALLA
    /*char temp, y;
    temp = a & 0x0F;
    y = a & 0xF0;
    RS = 1; // => RS = 1
    Lcd_Port(y >> 4); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;
    Lcd_Port(temp);
    EN = 1;
    __delay_us(40);
    EN = 0;
     */
    RS = 1;
    Lcd_Port(a); // ESCRIBE EL VALOR DE LA CADENA DE TEXTO EN LOS PUERTOS DE LA PANTALLA
    EN = 1;
    __delay_us(40); 
    EN = 0;     // MEDIANTE EL FLANCO DE 1 A 0, SE ENVIAN LOS DATOS
}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]); //POR CADA LETRA EN LA CADENA, SE LLAMA A LA FUNCION QUE ENVIA LETRAS A LA PANTALLA
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x1C); // ENVIA EL COMANDO DE DESPLAZAMIENTO, PROGRAMADO A LA DERECHA
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x18); // ENVIA EL COMANDO DE DESPLAZAMIENTO, PROGRAMADO A LA IZQUIERDA
}

void Lcd_Credits(void) { // PARTE DEL CÓDIGO ORIGINAL DE ELECTOSOME, CONSERVADO COMO REFERENCIA AL AUTOR ORIGINAL.
    // ESENCIALMENTE, MUESTRA TEXTO QUE DESCRIBE AL AUTOR DE LA LIBRERIA, USANDO
    // ALGUNAS DE LAS FUNCIONES ANTERIORMENTE CREADAS.
    unsigned int a;
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("LCD Library for");
    Lcd_Set_Cursor(2, 1);
    Lcd_Write_String("MPLAB XC8");
    __delay_ms(2000);
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("Developed By");
    Lcd_Set_Cursor(2, 1);
    Lcd_Write_String("electroSome");
    __delay_ms(2000);
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("www.electroSome.com");

    for (a = 0; a < 15; a++) {
        __delay_ms(300);
        Lcd_Shift_Left();
    }

    for (a = 0; a < 15; a++) {
        __delay_ms(300);
        Lcd_Shift_Right();
    }

    Lcd_Clear();
    Lcd_Set_Cursor(2, 1);
    Lcd_Write_Char('e');
    Lcd_Write_Char('S');
    __delay_ms(1200);
    Lcd_Clear();
}

