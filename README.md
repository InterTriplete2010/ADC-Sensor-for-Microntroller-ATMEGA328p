# ADC-Sensor-for-Microntroller-ATMEGA328p

This code is used to read the values of a sensor (photoresistor in my case) as output of the ADC by using ATMEGA328P. When the value of the ADC is below 100 (arbitrary value), then the LED connected to port PD5 will be on. When it is > 100, then the LED connected to PB1 will be on. If a user has 2 arduinos available, they can read the output transmitted with the function "USART_Transmit(ADC)".

If you have any questions and/or want to report bugs, please e-mail me (Ale) at: pressalex@hotmail.com
