#include <Lab1utilities.h>



Lab1_utilities::Lab1_utilities()
{
}

bool Lab1_utilities::START_UP(unsigned int i_u32Time, short *p_16iBlinkCount, unsigned char l_u8PowerMode)
{
    TIMER32_2_CONF(i_u32Time);

    // Set blinking LEDs Port-Pin
    P2 -> DIR =  l_u8PowerMode;
    P2 -> OUT = 0;

    while(*(p_16iBlinkCount) > 0)
    {
        // Remain in state till blink ends
    }

    TIMER32_2->CONTROL = 0; // Timer32 disabled
    NVIC_DisableIRQ(T32_INT2_IRQn);

    return false;
}



float Lab1_utilities::PROM_SOUND(float l_fADCData){

    float l_fDataProm;


    if(g_iContPR < 5){
        switch(g_iContPR){
        case 0:
            l_fDataProm = l_fADCData;
        case 1:
            l_fDataProm = g_fData1;
        case 2:
            l_fDataProm = (g_fData1 + g_fData2)/2;
        case 3:
            l_fDataProm = (g_fData1 + g_fData2 + g_fData2)/3;
        case 4:
            l_fDataProm = (g_fData1 + g_fData2 + g_fData3 + g_fData4)/4;
        }

    }else{
        l_fDataProm = (g_fData1 + g_fData2 + g_fData3 + g_fData4 + g_fData5)/5;
    }

        g_fData2 = g_fData1;
        g_fData3 = g_fData2;
        g_fData4 = g_fData3;
        g_fData5 = g_fData4;
        g_fData1 = l_fADCData;
        g_iContPR ++;

    return l_fDataProm;
}










void Lab1_utilities::ADC_CONF_SINGLE()
{
    // Setting the GPIO port-pin 4.3 as an ADC

    P4 -> SEL0 =  BIT3; //Port-Pin 4.3 in tertiary function
    P4 -> SEL1 =  BIT3;
    P4 -> DIR &= ~BIT3; //Port-Pin 4.3 as input

    // Control 0 register

    ADC14 -> CTL0 =
            ADC14_CTL0_PDIV_0     | //Pre-divide: /1
            ADC14_CTL0_SHS_0      | //Sample&Hold Source: Same bit as ADC14SC***
            ADC14_CTL0_DIV_0      | //Clock divide: /1
            ADC14_CTL0_SSEL__MCLK | //Clock: MCLK
            ADC14_CTL0_SHT0_1     | //Sample&Hold Time 4 PulseMode: 8****
            ADC14_CTL0_ON         | //ADC ready
            ADC14_CTL0_SHP;         //SourceSignal Sample&Hold: Sampling timer****

    //Memory Control Register 0

    ADC14 -> MCTL[0] = ADC14_MCTLN_INCH_10 | // Input Chanel: Ain+ = A10 ; Ain- = A11;
                                                    //A10 is located in GPIO 4.3
                     ADC14_MCTLN_VRSEL_0;  // Sources Combination: V(R+)=AVCC, V(R-)=AVSS ***

    ADC14 -> CTL0 |= ADC14_CTL0_ENC; //Enable conversation

    ADC14 -> IER0 = ADC14_IER0_IE0; //Enable ADC interruptions

    NVIC_SetPriority(ADC14_IRQn,1);
    NVIC_EnableIRQ(ADC14_IRQn);

}

void Lab1_utilities::TIMER32_2_CONF(unsigned int i_u32Time)
{
    TIMER32_2 -> LOAD = i_u32Time; //Value the counter will decrement

    // Control Register
    TIMER32_2 -> CONTROL =
            TIMER32_CONTROL_SIZE | //T32 size: 32 bits
            TIMER32_CONTROL_PRESCALE_0 | //Prescale: /1
            TIMER32_CONTROL_IE | //Interrupt: Enable
            TIMER32_CONTROL_MODE | //Mode: Periodic
            TIMER32_CONTROL_ENABLE; //Timer: Enabled

    // Prioriy and interrupt enable
    NVIC_SetPriority(T32_INT2_IRQn, 1);
    NVIC_EnableIRQ(T32_INT2_IRQn);


}


void Lab1_utilities::LIGHT_SENSOR_CONF()
{

    // Port 6 Pins 4-5 as input for I2C
    P6 -> DIR &= ~BIT4;
    P6 -> DIR &= ~BIT5;

    // P6 -> SEL set in primary function (0 in SEL0 4th & 5th pos | 1 in SEL1 4th & 5th pos)

    P6 -> SEL0 |=  BIT4;
    P6 -> SEL0 |=  BIT5;

    P6 -> SEL1 &= ~BIT4;
    P6 -> SEL1 &= ~BIT5;

    I2C_init();

    // Initialize OPT3001 digital ambient light sensor
    // Set predefined slave address
    OPT3001_init();

}
/*
unsigned int Lab1_utilities::CHECK_LIGHT()
{
    unsigned int l_u32Light_measure = 0U;
    l_u32Light_measure = OPT3001_getLux();

    if(l_u32Light_measure > )



}*/
