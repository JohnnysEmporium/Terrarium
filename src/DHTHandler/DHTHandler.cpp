#include "DHTHandler.hpp"
#include <LCDHandler/LCDHandler.hpp>

// global variables for temp and hum average
const int DHTArrSize = 10;
int temp_hum_run;
int TEMP_VALUES[DHTArrSize] = {10};
int HUM_VALUES[DHTArrSize] = {10};
int IS_STARTING_CNT;

bool isInit = false;

double temperature[1];
double humidity[1];
int temp_i, hum_i;
double temp_d, hum_d;
char temp_char[5];
char hum_char[5];
int temp_avg;
int hum_avg;

// returns average temperature from 10 samples
int calculateAverageTemp() {
    int temp_sum = 0;
    uint8_t i;
    for (i = 0; i < DHTArrSize; i++) {
        temp_sum += TEMP_VALUES[i];
    }

    return temp_sum / DHTArrSize;
}

// returns average humidity from 10 samples
int calculateAverageHum() {
    int hum_sum = 0;
    uint8_t i;
    for (i = 0; i < DHTArrSize; i++) {
        hum_sum += HUM_VALUES[i];
    }

    return hum_sum / DHTArrSize;
}

void DHTInit() {
    DHT_Setup();
}

bool DHTisInit() {
    return isInit;
}

char* DHTgetTempChar() {
    return temp_char;
}

char* DHTgetHumChar() {
    return hum_char;
}

void DHTUpdate(double* temp, double* hum) {
    cli();
    DHT_Read(temperature, humidity);
    sei();

    if (!isInit && sizeof(TEMP_VALUES) == 10) {
        isInit = true;
    }

    switch (DHT_GetStatus()) {
        case (DHT_Ok):
            PORTD ^= (1 << PD5);
            temp_i = temperature[0] * 10;
            hum_i = humidity[0] * 10;
            TEMP_VALUES[temp_hum_run] = temp_i;
            HUM_VALUES[temp_hum_run] = hum_i;

            temp_hum_run++;
            if (temp_hum_run >= 10) {
                temp_hum_run = 0;
            }

            temp_avg = calculateAverageTemp();
            hum_avg = calculateAverageHum();

            *temp = temp_avg;
            *hum = hum_avg;

            temp_d = static_cast<double>(temp_avg) / 10.0;
            hum_d = static_cast<double>(hum_avg)/ 10.0;

            dtostrf(temperature[0], -4,1, temp_char);
            dtostrf(humidity[0], -4,1, hum_char);
            break;

        // These are here for debugging, remove for final version. FUCKING FINALLY HEHEHEHEHE - final version! - removing :3
        // case (DHT_Error_Checksum):
        //     LCDGoTo(0x00, 1);
        //     LCDPuts("DHT ERR CHECKSUM");
        //     break;

        // case (DHT_Error_Timeout):
        //     PORTD ^= (1 << PD6);
        //     LCDGoTo(0x00, 1);
        //     LCDPuts("DHT ERR TIMEOUT"); 
        //     break;
    }
};