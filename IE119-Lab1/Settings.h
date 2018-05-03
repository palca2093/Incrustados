

#ifndef SETTINGS_H_
#define SETTINGS_H_

short g_16iBlinkCount  = 6; // 2 x # of Blinks

unsigned int g_u32StartPeriodCount = 0x0016E360; // Blink time With MCLK 3MHz = 0.5 s
                                                 //(0x0016E360 = 1 500 000)

unsigned int g_u32NominalPeriodCount = 0x002DC6C0; // Sampling period with MCLK 3MHz
                                                   //(0x00000BB8 = 3 000 = 1 ms)
                                                   //(0x000493E0 = 300 0000 = 0.1 s)
                                                   //(0x0016E360 = 1 500 000 = 0.5 s)
                                                  // (0x002DC6C0 = 3 000 000 = 1 s)

unsigned int g_u32WaitPeriodCount = 0x0000E4E1; // Lamp ON time with MCLK 3MHz/256
                                                //(0x0000E4E1 = 58 593     = 5 s)
                                                //(0x00055D4A = 351 562    = 30 s)
                                                //(0x0141DD76 = 21 083 750 = 30 m)

int g_iLightNumber = 2; //Power mode (LEDs to turn on)
/*
 * 1 : Red      = 5  W
 * 2 : Yellow   = 10 W
 * 3 : Purple   = 15 W
 */

unsigned int g_u32AverageValue = 8100; //Average sound value (obtained experimentally)

unsigned int g_u32ADCResultArray[5] = {g_u32AverageValue, g_u32AverageValue,
                                         g_u32AverageValue,g_u32AverageValue,g_u32AverageValue};
                                        //Array with sounds from ADC, initiated with initial condition
unsigned int g_u32LightNomlevel = 200;



#endif /* SETTINGS_H_ */
