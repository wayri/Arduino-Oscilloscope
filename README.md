# Osciduino

Arduino based Oscilloscope with advanced features and two channels. It is a DIY **Oscilloscope** based on popular Arduino platform.


Osciduino is an arduino based oscilloscope (tried and tested on arduino uno r3 and mega 2560). Though I would recommend using mega if you can.

### Why and what can it do
it is an educational project to get the insight into the internal workings of a buffered digital oscilloscope (DSO). 
this version includes advanced functionality to the setup, by including features like dual channel, offset, trigger, time/div, volts/div, and like wise.

### Inputs and few things to consider
The inputs are taken from 2 analog input pins on arduino. the pins however support only upto +5 volts. so ths oscilloscope works well only if the signal being viewed is +5 volts peak to peak maximum and doesn't have any negative halfs. to view negative halfs, the oscilloscope has to be operated in a +2.5 volts peak mode. this is accomplished externally by scaling the voltage and then offsetting it by +2.5 volts.   

### What should I know
An additional display library is included in the master for SPFD5408, if required.
Also the schematics for the setup are included in the "arduino files" folder

> Currently the code works best with the provided library and thus a specific display driver SPDF5408.
This however does not mean that it will not work with other drivers. It's just that behaviour might be different
as I have not tested it with any other display. You are welcome to add support for your display driver.


## Installation - Quick start

This is really simple to do just follow the following steps and you are up-and-running. 

First and foremost plug in the *Display to Arduino*. For this see [How to assemble the hardware](https://wayri.github.io/Osciduino/wiki)

- Plug in your arduino (uno or mega) to the computer using the usb-2.0
- Open the **Osciduino-3.ino** in arduino IDE
- Select your board from "Tools" menu
- Click upload
- You are up and running




### Current problems

The setupr requires an external resistive control interface. Which requires callibration on every boot. As resistance changes easily with temperature and long durations of running.

### Further Links and references

- [Osciduino-Beta](https://github.com/wayri/osciduino-beta)

















