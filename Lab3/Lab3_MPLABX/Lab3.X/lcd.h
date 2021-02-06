/* 
 * Librería LCD con protocolo de 8 bits
 * Adaptación basada en la librería "lcd.h" desarrollada por electroSome.
 * enlace: https://electrosome.com/lcd-pic-mplab-xc8/ 
 * 
 * Permite una fácil comunicación con una pantalla LCD.
 */


// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>


void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Lcd_Credits(void);

#endif	/* XC_HEADER_TEMPLATE_H */

