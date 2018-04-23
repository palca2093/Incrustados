

#ifndef SETTINGS_H_
#define SETTINGS_H_

short g_16iBlinkCount  = 6; // 2 x # of Blinks

unsigned int g_u32StartPeriodCount = 0x0016E360; // Blink count time (0x0016E360 = 1 500 000)
                                                        //With MCLK 3MHz = 0.5 s

unsigned int g_u32NominalPeriodCount = 0x00000BB8; // Sampling count time (0x00000BB8 = 3 000)
                                                        //With MCLK 3MHz = 1 ms

int g_iLightNumber = 3; //Power mode (LEDs to turn on)



float g_fLightNomlevel = 1000;
unsigned int g_uLight;
unsigned int g_uSound;


#endif /* SETTINGS_H_ */
