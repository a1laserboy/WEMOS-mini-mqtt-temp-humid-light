A project to use a Wemos mini D1 with GY-49 light sensor and GY-21 temperature and humidity sensor to broadcast MQTT over wifi.

I used it to send it's data to Home Assistant and it's working beautifully :)

I have adapted the code originally from https://github.com/daromer2/TempHumLight-ESP01

The author used an ESP01 in his project but the ability to power the D1 mini directly from micro USB and simple programming was more useful for me.

I have removed the OTA stuff as in my situation it's not needed and commented out the digital pins as they were stopping it working in the D1

I found a problem with the SparkFun_Si7021_Breakout_Library.h because the containing folder as downloaded from Spakfun was named differently, I had to rename it to make it work.

Massive thanks to Morris_B on the ESPHome discord server for his help and of course the author for his video, web page and code.

Author's YouTube video: https://youtu.be/oviTd5PK_Js

I'm very inexperienced with arduino, ESP's, Home assistant. If I can do it, you can too :)
