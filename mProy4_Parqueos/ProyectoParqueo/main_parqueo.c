/*
    IE3027 - ELECTRÓNICA DIGITAL II
    DENNY STEVE OZTOY CHEX, 18476
    PROYECTO 4 - PARQUEOS + WIFI
*/
//*****************************************************************************************************
//                                          LIBRERIAS
//*****************************************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

//*****************************************************************************************************
//                              PROTOTIPOS, VARIABLES Y DEFINICIONES
//*****************************************************************************************************

bool red = false, green = false, blue = false, toggle = false; // Banderas Para el control de las leds
bool pinInt = false; // Para el control de otra señal en la interrupción
int32_t espacios = 0;

//*****************************************************************************************************
//                                        CONFIGURACION
//*****************************************************************************************************

int main(void)
{
    // Configurando el reloj a 80 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_6|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Configurando puertos de entrada y salida
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xff); // Configurando el puertoB como salida
    GPIOPadConfigSet(GPIO_PORTB_BASE, 0xFF, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, 0X0f); // Configurando el puertoD <0:3> como entrada pull-up
    GPIOPadConfigSet(GPIO_PORTD_BASE, 0x0f, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
/*
    // Configurando Puerto A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3|GPIO_PIN_4);

    // Configurando Interrupciones Generales
    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    IntEnable(INT_UART0);

    // Configurando Timer 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 40000); // Insertando valor para una frecuencia de 0.5 kHz
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Asignando interrupciones del timer

    // Configurando UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1 | GPIO_PIN_0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)); // Esperar hasta que el UART esté listo
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));
    UARTFIFOEnable(UART0_BASE);
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX7_8, UART_FIFO_RX1_8);


    UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_TX); // Asignando interrupciones de Recepción

    // Habilitando Periféricos e interrupciones, junto con su prioridad
    UARTEnable(UART0_BASE);
    TimerEnable(TIMER0_BASE, TIMER_A);

 */

//*****************************************************************************************************
//                                        LOOP PRINCIPAL
//*****************************************************************************************************
    while(1){
        /* La función SysCtlDelay() ejecuta una instrucción de 3 ciclos la cantidad de
         * veces que indiquemos en el parámetro de entrada. Por tanto usaré
         * N = (FreqWanted)/(SysFreq ÷ 2 ÷ 3) */
        espacios = GPIOPinRead(GPIO_PORTD_BASE, 0x0f);

        if ((espacios & 0x00000001) == 0) // Comprobando parqueo 1
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x01);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x00);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0x02);
        }

        if ((espacios & 0x00000002) == 0) // Comprobando parqueo 2
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x04);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x08);
        }

        if ((espacios & 0x00000004) == 0) // Comprobando parqueo 3
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x10);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x00);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0x20);
        }

        if ((espacios & 0x00000008) == 0) // Comprobando parqueo 4
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x40);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x00);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0x00);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_7, 0x80);
        }


        //SysCtlDelay(13333/2);


    }// while (1)
}// main()

//*****************************************************************************************************
//                                  FUNCIONES E INTERRUPCIONES
//*****************************************************************************************************
/*
void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_A); // Limpiar la bandera de interrupción
}
*/
/*
void UARTIntHandler(void){
    uint32_t ui32Status;
    int16_t tempData;
    ui32Status = UARTIntStatus(UART0_BASE, true); //get interrupt status
    UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts

    while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
     {
     tempData = UARTCharGetNonBlocking(UART0_BASE);
     if (tempData != 13){
         dataIn = tempData;
     }
     UARTCharPutNonBlocking(UART0_BASE, tempData); //echo character
     }
    toggle = true;
}
*/
