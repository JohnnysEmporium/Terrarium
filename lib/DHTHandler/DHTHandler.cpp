#include "../AVR-DHT/DHT.h"
#include "../hd44780/hd44780.h"
#include "../LCDHandler/LCDHandler.hpp"
#include <stdlib.h>

// global variables for temp and hum average
const int arrsize = 10;
int temp_hum_run;
int TEMP_VALUES[arrsize] = {10};
int HUM_VALUES[arrsize] = {10};
int IS_STARTING_CNT;

//returns average temperature from 10 samples
int calculateAverageTemp(){
    int temp_sum = 0;
    uint8_t i;
    for(i = 0; i < arrsize; i++){
        temp_sum += TEMP_VALUES[i];
    }

    return temp_sum / arrsize;
}

//returns average humidity from 10 samples
int calculateAverageHum(){
    int hum_sum = 0;
    uint8_t i;
    for(i = 0; i < arrsize; i++){
        hum_sum += HUM_VALUES[i];
    }
    
    return hum_sum / arrsize;  
}

void DHTInit(){
    DHT_Setup();
}

void DHTMain(){

    double temperature[1];
    double humidity[1];
    int temp_i, hum_i;
    double temp_d, hum_d;
    char temp[5];
    char hum[5];
    int temp_avg;
    int hum_avg;

    DHT_Read(temperature, humidity);
    
    switch (DHT_GetStatus()){
        
        case (DHT_Ok):      
            temp_i = temperature[0] * 10;
            hum_i = humidity[0] * 10;
            TEMP_VALUES[temp_hum_run] = temp_i;
            HUM_VALUES[temp_hum_run] = hum_i;
            temp_hum_run++;

            if(temp_hum_run >= 10){
                temp_hum_run = 0;
            }

            temp_avg = calculateAverageTemp();
            hum_avg = calculateAverageHum();
            
            temp_d = (double)temp_avg / 10;
            hum_d = (double)hum_avg / 10;
            
            dtostrf(temp_d,-4,1,temp);
            dtostrf(hum_d,-4,1,hum);
            lcd_goto(LCD_TEMP);
            lcd_puts(temp);
            lcd_goto(LCD_HUM);
            lcd_puts(hum);  
            break;
        
        case (DHT_Error_Checksum):
            lcd_goto(LCD_TEMP_CONST);
            lcd_puts("DHT ERR CHECKSUM");
            break;

        case (DHT_Error_Timeout):
            lcd_goto(LCD_TEMP_CONST);
            lcd_puts("DHT ERR TIMEOUT"); 
            break;
    } 

};