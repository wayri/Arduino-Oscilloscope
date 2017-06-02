# Osciduino
Arduino based Oscilloscope with advanced features and two channels


Osciduino is an arduino based oscilloscope (tried and tested on arduino uno r3 and mega 2560). 
it is an educational project to get the insight into the internal workings of a buffered digital oscilloscope (DSO). 
this version includes advanced functionality to the setup, by including features like dual channel, offset, trigger, time/div, volts/div, and like wise.
the inputs are taken from 2 analog input pins on arduino. the pins however support only upto +5 volts. so ths oscilloscope works well only if the signal being viewed is +5 volts peak to peak maximum and doesn't have any negative halfs. to view negative halfs, the oscilloscope has to be operated in a +2.5 volts peak mode. this is accomplished externally by scaling the voltage and then offsetting it by +2.5 volts.   


An additional display library is included in the master for SPFD5408, if required.
Also the schematics for the setup are included in the "arduino files" folder






Following is the port log, of the ports that have been done to the code; (also included in the master file)





PORT LOG

------------------------------------------------------------------------------------------------------------------------------
//24/07/2016   -  2100


POrt was successfully compiled

//replaced define tags with hex code in color - and applied designated changes| required|

//included the correct tft library

//corrected the tft.begin(X) tag in setup, where X - is the driver hex code identifier 




------------------------------------------------------------------------------------------------------------------------------
//25/07/2016  - 1506


Port was succesfully compiled and run

//replaced the pin for ch1 | from pin 5 to pin 8 in Analog read enabling easy access to the pin




-------------------------------------------------------------------------------------------------------------------------------
//14/09/2016   - 2219


Port was Succesfully compiled and run,          /I\ - the voltage text was overwriting itself and not refreshed.  GIC(get it correct)
                                                                             STATUS - CORRECTED

//added ability to display voltage approximated on screen.   by calculation using ratios and proportions.

//          ! - for 5 volts as maximum value giving 1023 as sensor reading. and 0v = 0 sensor read.
//--> 5  =  1023
   ---   -----  , X is the RT sensor read. and V is the corresponding voltage.
    V      X   


--------------------------------------------------------------------------------------------------------------------------------
//3/11/2016     2036


Port was Succesfully compiled and run      

// removed grid to increase efficiency in processing time              /!\ - commented all grid function calls  
                                                                           ! - add option to toggle grid

//at line 88

 //    -->  increased rate of sampling from '3' to '6'            --| it seemingly increases buffer hold time and thus allows to see live variation 											in frequency unlike earlier at '3'

---------------------------------------------------------------------------------------------------------------------------------
//26/11/2016     2110

used || 'map' function to make it usable for negative voltages as well as positive 
but the code is commented out as of now  ___line 28 and 32 for map and hold variable respectively.

//at line 72           in array lengths reset the first block to 4 instead of 6 


//at line 88
//       -->  decreased rate of sampling from '6' to '3' 

BELOW ------------------ BELOW ----------------------- BELOW-----------------------------------------------
Port was NOT successful                                  /!\ -  but has some inherent defect
								STATUS - CORRECTED
//Created custom resistor configuration  ! - refer notes

//at line 160
//   fed the values in the array  
       --> verified by Serial printing of values
//      all good till now

PROBLEM - the for loop ||   Serial.prinln(sw);   || doesn't give any value   i.e, blank Serial at given baud rate 9600
              =>~~ seemingly the for loop isn't entered during execution

sOLUTION, THERE wAS a Stray 'return;' in the "CheckSW()" FUNCTION. rEMOVED iT.


-------------------------------------------------------------------------------------------------------------------------------------
//6/04/2017  	 2140
Ported the new switch arrray    || refer notes --> ARR2
corrected the value display


/!\ - there is a problem with switch array. if the setup has narrow analog values, it may interfere with the setup leading to the need to recalibrate it each time manually. This problem should be gone with th touch port.

/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
the next part is to display which menu we are in. 
display frequency
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
Stage One is Complete


_____________________________________________________________________________________________________________________________________
___________________________________________/\______________________________________________/\________________________________________


Stage TWO      >> Port the resistor array to touch screen
               >> Ability To visualise BiPolar signals

























