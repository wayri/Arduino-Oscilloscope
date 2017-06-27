# Osciduino
Arduino based Oscilloscope with advanced features and two channels


Osciduino is an arduino based oscilloscope (tried and tested on arduino uno r3 and mega 2560). 
it is an educational project to get the insight into the internal workings of a buffered digital oscilloscope (DSO). 
this version includes advanced functionality to the setup, by including features like dual channel, offset, trigger, time/div, volts/div, and like wise.
the inputs are taken from 2 analog input pins on arduino. the pins however support only upto +5 volts. so ths oscilloscope works well only if the signal being viewed is +5 volts peak to peak maximum and doesn't have any negative halfs. to view negative halfs, the oscilloscope has to be operated in a +2.5 volts peak mode. this is accomplished externally by scaling the voltage and then offsetting it by +2.5 volts.   


An additional display library is included in the master for SPFD5408, if required.
Also the schematics for the setup are included in the "arduino files" folder



### Further Links and references

- [Osciduino-Beta](https://github.com/wayri/osciduino-beta)

















