#include "msp.h"
#include "limits.h"
#include <Lab1utilities.h>
extern "C"
{ //Include Driverlib API without compiler name change
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
}

unsigned short g_16iBlinkCount  = 6;
unsigned short *p_16iBlinkCount = &g_16iBlinkCount; //Pointer to blinkcount
bool g_bInitialState = true; //Initial state flag



/**
 * main.c
 */

float g_fNivelLuz = 100;
float g_fDato1 = 0;
float g_fDato2 = 0;
float g_fDato3 = 0;
float g_fDato4 = 0;
float g_fDato5 = 0;
int g_iContPRomedio = 0;

//ESTAS NO EXISTEN, SON LAs QUE SALE CONSTANTEMENTE DEL ADC
float ADCSONIDO = 100;
float ADCLUZ = 100;
//....................................

float calcularPromedio(float l_fDatoADC){

    float l_fPromedioDatos;
    g_fDato2 = g_fDato1;
    g_fDato3 = g_fDato2;
    g_fDato4 = g_fDato3;
    g_fDato5 = g_fDato4;
    g_fDato1 = l_fDatoADC;
    g_iContPRomedio ++;

    if(g_iContPRomedio < 5){
        switch(g_iContPRomedio){
        case 1:
             l_fPromedioDatos = g_fDato1;
        case 2:
             l_fPromedioDatos = (g_fDato1 + g_fDato2)/2;
        case 3:
             l_fPromedioDatos = (g_fDato1 + g_fDato2 + g_fDato2)/3;
        case 4:
             l_fPromedioDatos = (g_fDato1 + g_fDato2 + g_fDato3 + g_fDato4)/4;
        }

    }else{
         l_fPromedioDatos = (g_fDato1 + g_fDato2 + g_fDato3 + g_fDato4 + g_fDato5)/5;
    }
    return l_fPromedioDatos;
}

void CondicionEncendido(){

    float l_fCondicionPromedio = calcularPromedio(ADCSONIDO) + calcularPromedio(ADCSONIDO)*0.1;

    if((ADCSONIDO > l_fCondicionPromedio) && (ADCLUZ < g_fNivelLuz)){

        P2->DIR = BIT0 | BIT1 | BIT2;
        P2->OUT = 8;

    }
}

void main(void)
{

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    Lab1_utilities m_Tools; // Instance "tools" of the utilities class

    g_bInitialState = m_Tools.START_UP(0x0016E360, p_16iBlinkCount); //# of CLK periods (0x0016E360 = 1 500 000) / CLK : MCLK 3MHz

    //Tools.ADC_CONF();

}



void T32_INT1_IRQHandler(void)
{
    __disable_irq();
    TIMER32_2->INTCLR = 0U; //Clearing of interrupt

    if (g_bInitialState) // Start_up?
    {
        P2 -> OUT ^= BIT0 | BIT1 | BIT2; //Invertion of Port-Pin output

        *p_16iBlinkCount--;
    }
    __enable_irq();
    return;
}

