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

//Estas dos no estaban en el encabezado de ejemplo
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"


bool red = false, green = false, blue = false, toggle = false; // Banderas Para el control de las leds
bool pinInt = false; // Para el control de otra señal en la interrupción
int32_t dataIn = 0;

int main(void)
{
    // Configurando el reloj a 40 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_6|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Configurando Leds en el Puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

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
    //IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Asignando interrupciones del timer

    // Configurando UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_1 | GPIO_PIN_0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)); // Esperar hasta que el UART esté listo
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));
    UARTFIFOEnable(UART0_BASE);
    UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX7_8, UART_FIFO_RX1_8);

    //IntEnable(INT_UART0);

    UARTIntEnable(UART0_BASE, UART_INT_RX|UART_INT_TX); // Asignando interrupciones de Recepción

    // Habilitando Periféricos e interrupciones, junto con su prioridad
    UARTEnable(UART0_BASE);
    TimerEnable(TIMER0_BASE, TIMER_A);

    while(1){

        /* La función SysCtlDelay() ejecuta una instrucción de 3 ciclos la cantidad de veces que indiquemos en
         el parámetro de entrada. Por tanto usaré
         N = (FreqWanted)/(SysFreq ÷ 2 ÷ 3) */
        if (red){
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02);
        } else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
        } // end if red
        if (green)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x08);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
        } // end if green
        if (blue)
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x04);
        }
        else
        {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
        } // end if blue

        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10);
        SysCtlDelay(13333/2);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
        SysCtlDelay(13333/2);

        if (toggle){
            toggle = false;
            if (dataIn == 114 || dataIn == 82){
                if (red){
                    red = false;
                } else{
                    red = true;
                }
            } // end if Red r|R
            if (dataIn == 103 || dataIn == 71){
                if (green)
                {
                    green = false;
                }
                else
                {
                    green = true;
                }
            } // end if Green g|G
            if (dataIn == 98 || dataIn == 66)
            {
                if (blue)
                {
                    blue = false;
                }
                else
                {
                    blue = true;
                }
            } // end if blue b|B
        } // end if Toggle

    }// while (1)
}// main()

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_A); // Limpiar la bandera de interrupción
    if (pinInt){
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
        pinInt = false;
    } else {
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x08);
        pinInt = true;
    }

}

void UARTIntHandler(void){
    /*Fragmento de código obtenido de:
     *Getting Started with the Tiva™ TM4C123G LaunchPad Workshop
     *https://www.cse.iitb.ac.in/~erts/html_pages/Resources/Tiva/TM4C123G_LaunchPad_Workshop_Workbook.pdf
    */
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
