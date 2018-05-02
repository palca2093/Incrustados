#include <Lab1utilities.h>



Lab1_utilities::Lab1_utilities()
{
}


unsigned char Lab1_utilities::NUM_LIGHT_SEL(int i_iLigntNumber){

    unsigned char l_u16LightNumber;

    switch(i_iLigntNumber){
    case 1:
        l_u16LightNumber = BIT0;
        break;
    case 2:
        l_u16LightNumber = BIT0 | BIT1;
        break;
    case 3:
        l_u16LightNumber = BIT0 | BIT1 | BIT2;
        break;
    }
    return l_u16LightNumber;
}




bool Lab1_utilities::START_UP(unsigned int i_u32Time, short *p_16iBlinkCount)
{
    // Set blinking LEDs Port-Pin
    P2 -> DIR =  BIT0 | BIT1 | BIT2;
    P2 -> OUT = 0;

    // Set input port for main interruption

    P4 -> DIR  &= ~BIT1; //Port 4.1 as input
    P4 -> REN  |=  BIT1; //Input with PU/PD resistor enabled

    P4 -> SEL0 &= ~BIT1; //
    P4 -> SEL1 &= ~BIT1;

    P4 -> IE   |=  BIT1; // Interrupt enable for port 4.1

    NVIC_SetPriority(PORT4_IRQn, 1);
    NVIC_EnableIRQ(PORT4_IRQn);

    TIMER32_2_CONF(i_u32Time);

    while(*(p_16iBlinkCount) > 0)
    {
        // Remain in state till blink ends
    }

    TIMER32_2->CONTROL = 0; // Timer32 disabled
    NVIC_DisableIRQ(T32_INT2_IRQn);

    return false;
}





void Lab1_utilities::ADC_CONF_SINGLE()
{
    // Setting the GPIO port-pin 4.3 as an ADC

    P4 -> SEL0 |=  BIT3; //Port-Pin 4.3 in tertiary function
    P4 -> SEL1 |=  BIT3;
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

void Lab1_utilities::TIMER32_1_CONF(unsigned int i_u32Time)
{
    TIMER32_1 -> LOAD = i_u32Time; //Value the counter will decrement

    // Control Register
    TIMER32_1 -> CONTROL =
            TIMER32_CONTROL_SIZE | //T32 size: 32 bits
            TIMER32_CONTROL_PRESCALE_2 | //Prescale: /256
            TIMER32_CONTROL_IE | //Interrupt: Enable
            TIMER32_CONTROL_MODE | //Mode: Periodic
            TIMER32_CONTROL_ENABLE; //Timer: Enabled

    // Prioriy and interrupt enable
    NVIC_SetPriority(T32_INT1_IRQn, 1);
    NVIC_EnableIRQ(T32_INT1_IRQn);


}


void Lab1_utilities::LIGHT_SENSOR_CONF()
{

    // Port 6 Pins 4-5 as input for I2C
    P6 -> DIR &= ~BIT4;
    P6 -> DIR &= ~BIT5;

    // P6 -> SEL set in primary function (1 in SEL0 4th & 5th pos | 0 in SEL1 4th & 5th pos)

    P6 -> SEL0 |=  BIT4;
    P6 -> SEL0 |=  BIT5;

    P6 -> SEL1 &= ~BIT4;
    P6 -> SEL1 &= ~BIT5;

    I2C_init();

    // Initialize OPT3001 digital ambient light sensor
    // Set predefined slave address
    OPT3001_init();

}


void Lab1_utilities::SWITCH_DEBOUNCE()
{
    int l_iDebounceCount = 0;

    while(l_iDebounceCount <3000) //Wait 1 ms
    {
        l_iDebounceCount++;
    }
}


void Lab1_utilities::INITIAL_STATE(unsigned int i_u32LightNomlevel,
                                 unsigned int i_u32WaitTime,
                                 unsigned int i_u32UpdateTime,
                                 unsigned char i_u16PowerMode )
{

    if(OPT3001_getLux() < i_u32LightNomlevel)
    {
        P2 -> OUT |= i_u16PowerMode; //Turn ON light

        TIMER32_1_CONF(i_u32WaitTime); //Wait period for light ON

    }

    else
    {

        P2 -> OUT &= ~i_u16PowerMode; //Turn OFF light
        TIMER32_2_CONF(i_u32UpdateTime); //Check conditions periodically

    }


}



void Lab1_utilities::SET_LAMP_ON(unsigned char i_u16PowerMode)
{
    //Disable update timer
    TIMER32_2->INTCLR = 0U; //Clearing the Timer32_2 interrupt flag
    TIMER32_2->CONTROL = 0U; //Disable T32_2
    NVIC_DisableIRQ(T32_INT2_IRQn);

    //Set LED

    P2 -> OUT |= i_u16PowerMode; //LED ON

}

void Lab1_utilities::SET_LAMP_OFF(unsigned char i_u16PowerMode)
{
    //Disable current timer
    TIMER32_1->INTCLR = 0U; //Clearing the Timer32_1 interrupt flag
    TIMER32_1->CONTROL = 0U; //Disable T32_1
    NVIC_DisableIRQ(T32_INT1_IRQn);

    //Set LED

    P2 -> OUT &= ~i_u16PowerMode; //LED OFF

}








