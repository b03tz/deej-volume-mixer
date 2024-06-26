# Deej Volume Mixer
This repository contains my implementation for the Deej Volume mixer that I created. It's a 5 channel mixer with RGB LED strips.

The mixer supports themes and brightness control which get stored into the EEPROM so your settings are remembers if you power it down!

![Mixer](https://i.ibb.co/C0q6cFZ/1.jpg)

* Here's a making-off video: https://www.youtube.com/watch?v=anTwEfMaGJQ
* How theme selection/brightness works: https://www.youtube.com/watch?v=iqhkKfsA3G0
* You can check out the models here: https://makerworld.com/en/models/419682#profileId-322350

# Software
I'm running this on an Arduino Nano3 with the sliders connected to A0, A1, A2, A3, A4 and the LED data strips connected to pin 6, 5, 4, 3 and 2.
You can change the code anyway you want to!

For the PC side you need deej: https://github.com/omriharel/deej this repo will explain how to configure it and get things running!

# Hardware
* You need 12x5 LEDs on a 144 led p/meter strip: https://nl.aliexpress.com/item/1005005574282373.html
* You need 5 linear sliders: https://nl.aliexpress.com/item/1005006614304157.html

Also you'll need some M3 screws, M4 screws and M3/M4 heated inserts.

* I got these inserts: https://nl.aliexpress.com/item/1005004870993068.html
* And M3/M4 screws from here: https://nl.aliexpress.com/item/1005006034879477.html