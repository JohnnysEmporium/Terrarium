AVR-DHT library used - https://github.com/efthymios-ks/AVR-DHT | Thanks to courtesy of [@efthymios-ks](https://github.com/efthymios-ks)

HD44780 library used - yet to be found and author located. I've downloaded this lib a long time ago and didn't write down it's source. I feel ashamed... Such a good lib... only one amidst dozens of others that actually did work for me.


bit manipulation - http://www.rjhcoding.com/avrc-bit-manip.php

external interrupts - https://www.youtube.com/watch?v=eZH4B4PIIPU

timers - http://fabacademy.org/archives/2015/eu/students/gunnarsson.thorarinn_b.b/docs/d7/Timers.pdf


TODO:

- Fixup the way +5v and GND is connected on the board

- add connectors for buttons and solder their circuits

- use second timer for milis(?)


3 buttons: 
    - time +
    - time -
    - switch to "last fed: $(days)" view

2 relays:
    - light
    - water filter

2(3?) MOSFET transistors
    - water pump
    - humidifier
    - LCD light (?)

rest is soldering, connecting power to pump and humidifier, hooking up the relays.




Buttons:

1                   2                  3
switch display      -                  long-press to enable time edition
increase time       decrease time      short press to change position (after long-press)
-                   -                  short press to wake up
-                   -                  short press to turn on display