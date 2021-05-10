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

int main(void)
{
    // Configurando el reloj a 40 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_6|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // Configurando Puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // Configurando Puerto A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);

    // Configurando Timer 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 80000); // Insertando valor para una frecuencia de 0.5 kHz

    IntMasterEnable(); // Configurando Interrupciones del timer
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntEnable(INT_TIMER0A);

    TimerEnable(TIMER0_BASE, TIMER_A); // Habilitando Timer0

    // Configurando UART
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 38400,(UART_CONFIG_WLEN_8|UART_CONFIG_STOP_ONE|UART_CONFIG_PAR_NONE));




    while(1){
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10);
        /* La función SysCtlDelay() ejecuta una instrucción de 3 ciclos la cantidad de veces que indiquemos en
        el parámetro de entrada. Por tanto usaré
        N = (FreqWanted)/(SysFreq ÷ 2 ÷ 3) */
        SysCtlDelay(13333/2);
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00);
        SysCtlDelay(13333/2);

    }// while (1)
}// main()

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_A); // Limpiar la bandera de interrupción
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0F);
}
