/*Arduino Oscilloscope reboot
 * 
 * Custom version based on oscilloscope by prof. Noriaki Mitsunaga
 */

// include the display library
#include <SPFD5408_Adafruit_TFTLCD.h>

//--------------------
//SET THE DISPLAY DATA PINS
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

//----------------------

// INITIALISE THE DISPLAY
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

//define the line spacings for printin text on screen
#define txtLINE0   0
#define txtLINE1   16
#define txtLINE2   30
#define txtLINE3   46

// initialise the display parameters
const int LCD_WIDTH = 320;
const int LCD_HEIGHT = 240;
const int SAMPLES =270; //270;       //i.e,number of samples to display at any time rate
const int DOTS_DIV =30;

 //channel 1 input
const int ad_ch0 = 15; 
 //channel 2 input
const int ad_ch1 = 8;

//setting up amplitude scale based on VREF

const unsigned long VREF[] = {150, 300, 750, 1500, 3000};

/*the above works as follows
 * / reference voltage 5.0V ->  150 :   1V/div range (100mV/dot)
                                         It means 5.0 * DOTS_DIV = 150. Use 4.9 if reference voltage is 4.9[V]
                                                                -> 300 : 0.5V/div
                                                                -> 750 : 0.2V/div
                                                                ->1500 : 100mV/div
                                                               -> 3000 :  50mV/div

                                        ________________________________________________________________
 * 
 */

 //for varying scales the array holds the mv/dot values
 //e.g. 33mv/dot -> 990mV/div 
const int MILLIVOL_per_dot[] = {33, 17, 6, 3, 2}; // mV/dot


//interface details initialisations
//---------------START-----------------------

 //trigger mechanism
//setting up ghost variables as representatives
const int MODE_ON = 0;
const int MODE_INV = 1;
const int MODE_OFF = 2;
//declaring a pointer array to the above ghost variables
const char *Modes[] = {"NORM", "INV", "OFF"};
//triggering types variable representatives
const int TRIG_AUTO = 0;
const int TRIG_NORM = 1;
const int TRIG_SCAN = 2;
const int TRIG_ONE  = 3;
//declaring a pointer array to the above ghost variables
const char *TRIG_Modes[] = {"Auto", "Norm", "Scan", "One"};
//trigger type ghost variables
const int TRIG_E_UP = 0;
const int TRIG_E_DN = 1;



 //time scale mechanism
// defining maximum and minimum rates for reference
#define RATE_MIN 0
#define RATE_MAX 13
 //defining a pointer array for the maximum possible time scale constants for an arduino
const char *Rates[] = {"F1-1", "F1-2 ", "F2  ", "5ms", "10ms", "20ms", "50ms", "0.1s", "0.2s", "0.5s", "1s", "2s", "5s", "10s"};

// amplitude scale mechanism
//defining maximum and minimum reference values for use by array below
#define RANGE_MIN 0
#define RANGE_MAX 4
// defining pointer array
const char *Ranges[] = {" 1V ", "0.5V", "0.2V", "0.1V", "50mV"};


//setting a time reference holding variable
unsigned long startMillis;

//----------------END------------------------


//--------------SAMPLING---------------------

 //the below array holds the sampled values - a buffer
byte data[4][SAMPLES];                   // keep twice of the number of channels to make it a double buffer
byte sample=0;                           // index for double buffer


//-----------------END------------------------



//----------------DISPLAY-PROFILE---------------
 //Define colors here
#define  BGCOLOR  0x0000//BLACK
#define GRIDCOLOR 0xFFFF//WHITE
#define CH1COLOR  0XFFE0//YELLOW            
#define CH2COLOR  0X001F//BLUE 




//--------------------END-----------------------



//-------------------DEFAULTS---AND---VARAIBLES-------------
 //Declare variables and set defaults here


byte range0 = RANGE_MIN, ch0_mode = MODE_ON;   //CH0 yellow
short ch0_off = 204;
byte range1 = RANGE_MIN, ch1_mode = MODE_ON; //  CH1 blue
short ch1_off = 204;
// TIME SCALE               || rate
byte rate = 7;
//const char *Rates[] = {"F1-1", "F1-2 ", "F2  ", "5ms", "10ms", "20ms", "50ms", "0.1s", "0.2s", "0.5s", "1s", "2s", "5s", "10s"};
 //TRIGGER
byte trig_mode = TRIG_AUTO, trig_lv = 25, trig_edge = TRIG_E_UP, trig_ch = 1; // trigger settings
/*
 * TRIGGER LEVEL IS CONTROLLED BY trig_lv
 */

// RUN + STOP MECHANISM
byte Start = 1; //  Start sampling
//MENU TOGGLE
byte menu = 0; //  Default menu



//---------------------------END-----------------------------



//------------------------SETUP -- BOOT----------------------------------

void setup() {
 
  
 //setting prescalar
 //---------------------------begin--------------------------------------------
  
  //setting prescalars, the reference speed for ADC
  
    ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); // clear prescaler bits
  
  // uncomment as required
  
//ADCSRA |= bit (ADPS0);                                //  2  => 16MHz/2/13~=615KHz sampling freq
 // ADCSRA |= bit (ADPS1);                              //    4  => 16MHz/4/13~=307.6KHz sampling freq

// prefer these for stability
//ADCSRA |= bit (ADPS0) | bit (ADPS1);                   // 8  => 16MHz/8/13~=153KHz sampling frequency for 4 samples 38KHz

//more stable choices
 ADCSRA |= bit (ADPS2);                                 //16 ~=76KHz    --> for 4 samples 19KHz         !<----
 //   ADCSRA |= bit (ADPS0) | bit (ADPS2);                 //  32 => 16MHz/32/13~=38.4KHz sampling frequency for 4 samples it means 9.5 KHz clean || the code is not adapted to this high level of sampling rate, so expect a lot of errors and goofiness
 // ADCSRA |= bit (ADPS1) | bit (ADPS2);                  // 64             ~=19KHz sampling frequency
 // ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);    //128

   //  end of setup
  
  
  
  
  
  
 //----------------------------end------------------------------------------------------
  
  
  
  // setting reference ground------

pinMode(28,OUTPUT);
digitalWrite(28,LOW);

  //--------------------------------
  
      
 // resetting tft to erase any content already present on screen
  tft.reset();
  //initialising display driver
  tft.begin(0x9341); 
  //setting display orientation
  tft.setRotation(3);


//-----------------STARTING PICTURES TEXT AND ANIMATIONS--------------

 // DRAWING BACKGROUND
  tft.fillScreen(BGCOLOR);
  
 // PRINTING TEXT
  tft.setTextColor(GRIDCOLOR);
  tft.setTextSize(1);
  tft.setCursor(70, 90);
  tft.print("Arduino OscilloScope ");
  tft.setCursor(70, 110);
  tft.print("TFT LCD (320x240)");   ;
  tft.setCursor(70, 130);
  tft.print("(c) 2016 Port yawar");
  tft.setCursor(210, 220);
  tft.print("initialising . , ,");
  delay(3500);
  tft.fillScreen(BGCOLOR);

//---------------------DEFINE CONTROL PINS----------------------------

pinMode(21, INPUT);
attachInterrupt(digitalPinToInterrupt(21), switch_r, RISING);

//-------------------------------END----------------------------------

 //test call
//Serial.begin(9600);

//----FUNCTION CALLS
 // Draw GRID ON SCREEN
 //   DrawGrid();
// draw the info regarding current settings on screen
 // DrawText();

}

//////////////////////////////////////////////////////////////////////////////////////

//===================================================================================

//=================---------FUNCTION DEFINITIONS----------===========================


//----------------------SEND DATA TO SERIAL FOR DEBUGGING AND ANALYSIS PURPOSES
/*
 * This function requires serial to be initialised in setup first otherwise it will
 * behave in an unknown way
 */
 
void switch_r()
{
// Tis function cycles through time/div rate by increments. 
//press the hardware button only once and wait for the rate to change. because the system is slow

// switch connections
/*
 *        -------------  +5V
 *            |
 *            +------+
 *            |      |
 *            \     _|__
 *         SW  \    ____  C                         ^Arduino Board
 *              \    |                             | 
 *            o  \   |                             | 
 *            +------+----------o  V_out   --> o---|  [pin 21] 
 *            |                                    | arduino mega
 *            |                                    | (select any interrupt pin and r
 *            \>                                   | emember to change code at line 232   
 *           </   R                                | to select the interrupt pin)
 *            \>
 *            |
 *           _|_
 *           \_/ GND
 *           
 *           here R is the pull down resistor 
 *                C is the debouncing capacitor
 *                SW is the tactile switch
 */
  if(rate<14 && rate >=0)
  {
  rate++;
  }else{
    rate = 0;
  }
}
 
void SendData() {
  Serial.print(Rates[rate]);
  Serial.println("/div (30 samples)");
  for (int i=0; i<SAMPLES; i ++) {
      Serial.print(data[sample + 0][i]*MILLIVOL_per_dot[range0]);
      Serial.print(" ");
      Serial.println(data[sample + 1][i]*MILLIVOL_per_dot[range1]);
   } 
}


 //--------------------------DRAW GRID ON SCREEN FOR RELATIVE REFERENCE FOR END USER------------------
/*
 * Since drawing grid takes time and the code is seqentially executed,
 * the code takes precious time which could have been used by other more necessary functions
 * like efficient sampling.
 * 
 * It is advisable to keep calls to this function minimal and if possible
 * for debugging purposes only.
 */
void DrawGrid() {
    for (int x=0; x<=SAMPLES; x += 2) {  //Horizontal Line
      for (int y=0; y<=LCD_HEIGHT; y += DOTS_DIV) {
        tft.drawPixel(x, y, GRIDCOLOR);
        
      }
      if (LCD_HEIGHT == 240)
        tft.drawPixel(x, LCD_HEIGHT-1, GRIDCOLOR);
    }
    for (int x=0; x<=SAMPLES; x += DOTS_DIV ) { // Vertical Line
      for (int y=0; y<=LCD_HEIGHT; y += 2) {
        tft.drawPixel(x, y, GRIDCOLOR);
        
      }
    }
}


//-------------------DRAW TEXT------------------------

/*
 * This function prints the current state of settings on the screen
 * like trigger state. channel state
 */

void DrawText() {
tft.fillRect(SAMPLES,10,50,82,BGCOLOR);
  tft.setTextColor(GRIDCOLOR);     // 24/7 |0856    -->   tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.setCursor(SAMPLES, 20);
  tft.print(Ranges[range1]);
  tft.println("/DIV");
  tft.setCursor(SAMPLES+3, 30);
  tft.print(Rates[rate]);
  tft.println("/DV");                       
  tft.setCursor(SAMPLES+3, 40);
  tft.println(TRIG_Modes[trig_mode]);
  tft.setCursor(SAMPLES+3, 50);
  tft.println(trig_edge == TRIG_E_UP ? "UP" : "DN"); 
  tft.setCursor(SAMPLES+3, 60);
  tft.println(Modes[ch1_mode]);
  tft.setCursor(SAMPLES, 70);
  tft.println(trig_ch == 0 ? "T:1" : "T:2"); 
 
  
}

//------------------DRAW GRID OVERWRITE DIRECTOR--------------------
/*
 * SEE DRAW GRID ABOVE
 */
 
void DrawGrid(int x) {
    if ((x % 2) == 0)
      for (int y=0; y<=LCD_HEIGHT; y += DOTS_DIV)
        tft.drawPixel(x, y, GRIDCOLOR);
    if ((x % DOTS_DIV) == 0)
      for (int y=0; y<=LCD_HEIGHT; y += 2)
        tft.drawPixel(x, y, GRIDCOLOR);
}


//++++++++++++++++--------DRAW GRAPH------------+++++++++++++++++++++

void ClearAndDrawGraph() {
  int clear = 0;
  
  if (sample == 0)
    clear = 2;
#if 0
   for (int x=0; x<SAMPLES; x++) {
     GLCD.SetDot(x, LCD_HEIGHT-data[clear+0][x], WHITE);
     GLCD.SetDot(x, LCD_HEIGHT-data[clear+1][x], WHITE);
     GLCD.SetDot(x, LCD_HEIGHT-data[sample+0][x], BLACK);
     GLCD.SetDot(x, LCD_HEIGHT-data[sample+1][x], BLACK);
  }
#else
   for (int x=0; x<(SAMPLES-1); x++) {
     tft.drawLine(x, LCD_HEIGHT-data[clear+0][x], x+1, LCD_HEIGHT-data[clear+0][x+1], BGCOLOR);
     tft.drawLine(x, LCD_HEIGHT-data[clear+1][x], x+1, LCD_HEIGHT-data[clear+1][x+1], BGCOLOR);
     if (ch0_mode != MODE_OFF)
       tft.drawLine(x, LCD_HEIGHT-data[sample+0][x], x+1, LCD_HEIGHT-data[sample+0][x+1], CH1COLOR);
     if (ch1_mode != MODE_OFF)
       tft.drawLine(x, LCD_HEIGHT-data[sample+1][x], x+1, LCD_HEIGHT-data[sample+1][x+1], CH2COLOR);
   }
#endif
}




//-----------------------plot the data as individual dots on display--------------------
void ClearAndDrawDot(int i) {
  int clear = 0;

  if (i <= 1)
    return;
  if (sample == 0)
    clear = 2;
#if 0
   for (int x=0; x<SAMPLES; x++) {
     GLCD.SetDot(x, LCD_HEIGHT-data[clear+0][x], WHITE);
     GLCD.SetDot(x, LCD_HEIGHT-data[clear+1][x], WHITE);
     GLCD.SetDot(x, LCD_HEIGHT-data[sample+0][x], BLACK);
     GLCD.SetDot(x, LCD_HEIGHT-data[sample+1][x], BLACK);
  }
#else
  tft.drawLine(i-1, LCD_HEIGHT-data[clear+0][i-1], i, LCD_HEIGHT-data[clear+0][i], BGCOLOR);
  tft.drawLine(i-1, LCD_HEIGHT-data[clear+1][i-1], i, LCD_HEIGHT-data[clear+1][i], BGCOLOR);
  if (ch0_mode != MODE_OFF)
    tft.drawLine(i-1, LCD_HEIGHT-data[sample+0][i-1], i, LCD_HEIGHT-data[sample+0][i], CH1COLOR);
  if (ch1_mode != MODE_OFF)
    tft.drawLine(i-1, LCD_HEIGHT-data[sample+1][i-1], i, LCD_HEIGHT-data[sample+1][i], CH2COLOR);
#endif
  //DrawGrid(i);
 
}


//---------------------draw data for each channel-----------------------------
void DrawGraph() {
   for (int x=0; x<SAMPLES; x++) {
     tft.drawPixel(x, LCD_HEIGHT-data[sample+0][x], CH1COLOR);
     tft.drawPixel(x, LCD_HEIGHT-data[sample+1][x], CH2COLOR);
  }
}


//------------CLEAN THE PLOTS DRAWN EARLIER------------------------
void ClearGraph() {
  int clear = 0;
  
  if (sample == 0)
    clear = 2;
  for (int x=0; x<SAMPLES; x++) {
     tft.drawPixel(x, LCD_HEIGHT-data[clear+0][x], BGCOLOR);
     tft.drawPixel(x, LCD_HEIGHT-data[clear+1][x], BGCOLOR);
  }
}

//---------------GRAPHING AID|| PROVIDES VERTICAL LOCATION FOR PLOTTING DATA POINTS--------------------
inline unsigned long adRead(byte ch, byte mode, int off)
{
  unsigned long a = analogRead(ch);
  a = ((a+off)*VREF[ch == ad_ch0 ? range0 : range1]+512) >> 10;
  a = a>=(LCD_HEIGHT+1) ? LCD_HEIGHT : a;
  
  if (mode == MODE_INV)
    return LCD_HEIGHT - a;
  return a;
}



//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||



//-----------------------------THE MAIN LOOP---------------------------------------


void loop() {

  
  if (trig_mode != TRIG_SCAN) {

  
      unsigned long st = millis();
      byte oad;
      
      if (rate == 0)
        oad = adRead(ad_ch0, ch0_mode, ch0_off);
      else
        oad = adRead(ad_ch1, ch1_mode, ch1_off);

       // TRIGGERING MECHANISM EVALUATED
      for (;;) {
        byte ad;
        if (trig_ch == 0)
          ad = adRead(ad_ch0, ch0_mode, ch0_off);
        else
          ad = adRead(ad_ch1, ch1_mode, ch1_off);

        if (trig_edge == TRIG_E_UP) {
           if (ad >= trig_lv && ad > oad)
            break; 
        } else {
           if (ad <= trig_lv && ad < oad)
            break; 
        }
        oad = ad;

             if (trig_mode == TRIG_SCAN)
          break;
        if (trig_mode == TRIG_AUTO && (millis() - st) > 100)
          break; 
      }
  }
  
   //sample and draw depending on the sampling rate
    if (rate <= 5 && Start) {
    // change the index for the double buffer
    if (sample == 0)
      sample = 2;
    else
      sample = 0;

    if (rate == 0) { // full speed, channel 0 only
      unsigned long st = millis();
      for (int i=0; i<SAMPLES; i ++) {
        data[sample+0][i] = adRead(ad_ch0, ch0_mode, ch0_off);
      }
      for (int i=0; i<SAMPLES; i ++)
        data[sample+1][i] = 0;
      // Serial.println(millis()-st);
    } else if (rate == 1) {//  full speed, channel 1 only
      unsigned long st = millis();
      for (int i=0; i<SAMPLES; i ++) {
        data[sample+1][i] = adRead(ad_ch1, ch1_mode, ch1_off);
      }
      for (int i=0; i<SAMPLES; i ++)
        data[sample+0][i] = 0;
      // Serial.println(millis()-st);
    } else if (rate == 2) { // full speed, dual channel
      unsigned long st = millis();
      for (int i=0; i<SAMPLES; i ++) {
        data[sample+0][i] = adRead(ad_ch0, ch0_mode, ch0_off);
        data[sample+1][i] = adRead(ad_ch1, ch1_mode, ch1_off);
      }
       //Serial.println(millis()-st);
    }else if (rate >= 3 && rate <= 5) { // .5ms, 1ms or 2ms sampling
      const unsigned long r_[] = {5000/DOTS_DIV, 10000/DOTS_DIV, 20000/DOTS_DIV};
      unsigned long st0 = millis();
      unsigned long st = micros();
      unsigned long r = r_[rate - 3];
      for (int i=0; i<SAMPLES; i ++) {
        while((st - micros())<r) ;
        st += r;
        data[sample+0][i] = adRead(ad_ch0, ch0_mode, ch0_off);
        data[sample+1][i] = adRead(ad_ch1, ch1_mode, ch1_off);
      }
      // Serial.println(millis()-st0);
    }

   // Once data for either channel is buffered it is then printed on screen
    ClearAndDrawGraph();
     
    DrawText();




    
  } else if (Start) { // 5ms - 500ms sampling
 //  copy currently showing data to another
    if (sample == 0) {
      for (int i=0; i<SAMPLES; i ++) {
        data[2][i] = data[0][i];
        data[3][i] = data[1][i];
      }
    } else {
      for (int i=0; i<SAMPLES; i ++) {
        data[0][i] = data[2][i];
        data[1][i] = data[3][i];
      }      
    }

    const unsigned long r_[] = {50000/DOTS_DIV, 100000/DOTS_DIV, 200000/DOTS_DIV,
                      500000/DOTS_DIV, 1000000/DOTS_DIV, 2000000/DOTS_DIV, 
                      5000000/DOTS_DIV, 10000000/DOTS_DIV};
    unsigned long st0 = millis();
    unsigned long st = micros();
    for (int i=0; i<SAMPLES; i ++) {
      while((st - micros())<r_[rate-6]) {
       
       
        if (rate<6)
          break;
      }
      if (rate<6) { // sampling rate has been changed
        tft.fillScreen(BGCOLOR);
        break;
      }
      st += r_[rate-6];
      if (st - micros()>r_[rate-6])
          st = micros(); // sampling rate has been changed to shorter interval
      if (!Start) {
         i --;
         continue;
      }
      data[sample+0][i] = adRead(ad_ch0, ch0_mode, ch0_off);
      data[sample+1][i] = adRead(ad_ch1, ch1_mode, ch1_off);
      ClearAndDrawDot(i);     
    }
   //  Serial.println(millis()-st0);
   // DrawGrid();
    DrawText();
    
  } else {
    
     
  }
  if (trig_mode == TRIG_ONE)
    Start = 0;


}

