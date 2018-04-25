

#ifndef SETTINGS_H_
#define SETTINGS_H_

short g_16iBlinkCount  = 6; // 2 x # of Blinks

unsigned int g_u32StartPeriodCount = 0x0016E360; // Blink time With MCLK 3MHz = 0.5 s
                                                 //(0x0016E360 = 1 500 000)

unsigned int g_u32NominalPeriodCount = 0x002DC6C0; // Sampling period with MCLK 3MHz
                                                   //(0x00000BB8 = 3 000 = 1 ms)
                                                  // (0x002DC6C0 = 3 000 000 = 1 s)

unsigned int g_u32WaitPeriodCount = 0x000ABA95; // Lamp ON time with MCLK 3MHz/256
                                                //(0x00055D4B = 351 563    = 30 s)
                                                //(0x000ABA95 = 703 125    = 60 s)
                                                //(0x0141DD76 = 21 093 750 = 30 m)

int g_iLightNumber = 3; //Power mode (LEDs to turn on)



float g_iLightNomlevel = 500;



#endif /* SETTINGS_H_ */
