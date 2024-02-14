#include "./klass.hpp"

#define F_CPU = 8000000L



// int main(void)
// {
//     DDRC |= 1 << PINC5; //Set Direction for output on PINC5
//     PORTC ^= 1 << PINC5; //Toggling only Pin 5 on port C
//     DDRC &= ~(1 << PINC4); //Data Direction Register input PINC4
//     PORTC |= 1 << PINC4; //Set PINC4 to a high reading
//     int Pressed = 0; //Initialize/Declare the Pressed variable

//     while (1)
//     {
//         if (bit_is_clear(PINC, 4)) //Check if the button is pressed
//         {
//         //Make sure that the button was released first
//             if (Pressed == 0)
//             {
//                 PORTC ^= 1 << PINC5; //Toggle LED in pin 0
//                 Pressed = 1;
//             }
//         }
//         else
//         {
//         //This code executes when the button is not pressed.
//             Pressed = 0;
//         }
//     }
// }

int main(void){

    klass k;
    k.start();
}