//Positions of items on LCD
#define LCD_SEM_0       0x02
#define LCD_SEM_1       0x05
#define LCD_H           0x00
#define LCD_M           0x03
#define LCD_S           0x06
#define LCD_TEMP_CONST  0x40
#define LCD_RH_CONST    0x48
#define LCD_TEMP        0x43
#define LCD_HUM         0x4C

void LCDInit();
void LCDPutConstantSymbols();
void LCDOn();
void LCDOff();