#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include "OneButton.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "WearLeveling.h"

//WS2812 definitions
#define LED_PIN 11
#define LED_COUNT 4
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

//Button Input defintion 
OneButton button(10, HIGH); 
const int DFPlayerBusy = 12;

//Setup for the DFPlayer
SoftwareSerial mySoftwareSerial(8, 9); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

//Some things to remember all times
volatile int isbusy = 0;
volatile int showType = 0;
volatile int folder = 1;
volatile int lastsong = 1;
volatile int songcount[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {

  uint8_t c = 0xFF;

  //First of all check if the EEPROM is brand new
  //in this case we will format the EEPROM for wear leveling first
  //to indicate that the EEPROM was formatted we store an flag in the last eeprom cell
  c = (EEPROM.read(1023) & 0xFF);
  if(c == 0xFF){
      for(int i=0; i<11; i++){
       loadEeprom(i);
      }

      for(int i=0; i<11; i++){
       //writeValue(i, i+x);
       writeValue(i, 1);
      }

       EEPROM.write(1023, 0x01); 
  }

  // Setup the WS2812 LEDs for the colourful effects on the front of the button
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //Setup everything we need for the button press detection, click, double click, long button press 
  // link the myClickFunction function to be called on a click event.   
  button.attachClick(myClickFunction);

  // link the double click function to be called on a double click event.   
  button.attachDoubleClick(myDoubleClickFunction);

  //l line to long button press.
  button.attachDuringLongPress(myLongPressFunction);

  // set 50 msec. de-bouncing time.
  button.setDebounceTicks(50);

  //Load the EEPROM wear leveling information
  for(int i=0; i<11; i++){
    loadEeprom(i);

  }
  //Grap the information which folder the file was played from last time before power up and also which song was played from this folder
  folder = getValue(10);    //index 10 contains eeprom value with last played folder
  lastsong = getValue(folder-1);

  //Set busy pin from DFPlayer as input so we can register when a song has finished playing
  pinMode(DFPlayerBusy, INPUT);

  //Start the sw serial for the DFPlayer
  mySoftwareSerial.begin(9600);

  //Wait for the DFPlayer to be ready by polling its boot message
  while (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
   delay(100);
  }

  //Now we count the number of songs in the folders
   for(int i = 1 ; i<11 ; i++){
      songcount[i-1]=myDFPlayer.readFileCountsInFolder(i);
      if (songcount[i-1] == -1) {  //Error while Reading.
        //printDetail(myDFPlayer.readType(), myDFPlayer.read());
      }
      if (myDFPlayer.available()) {
      //printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
      }
      delay(100);
	}

  //Set some of the DFPlayer parameters to our liking
  myDFPlayer.volume(20);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD); 
}

void loop() {

    // keep watching the push button:
  button.tick();
}

//Call to illuminate WS2812 LEDs until the DFPlayer indicates that it has finished playing the selected song
void doShow(void){
  showType++;
  if (showType > 13)
  showType=0;
  startShow(showType);
}

void startShow(int i) {
  switch(i){
    case 0: theaterChase(strip.Color(255, 255, 255), 100); // White
            break;
    case 1: theaterChase(strip.Color(255,   0,   0), 100); // Red
            break;
    case 2: theaterChase(strip.Color(  0,   0, 255), 100); // Blue
            break;
    case 3: theaterChase(strip.Color(  0,   255, 0), 100); // Green
            break;
    case 4: theaterChase(strip.Color(255,   0, 255), 100); // 
            break;
    case 5: theaterChase(strip.Color(0, 255, 255), 100); // 
            break;
    case 6: theaterChase(strip.Color(255, 255, 0), 100); // 
            break;
    case 7: theaterChase(strip.Color(255, 255, 255), 200); // White
            break;
    case 8: theaterChase(strip.Color(255,   0,   0), 200); // Red
            break;
    case 9: theaterChase(strip.Color(  0,   0, 255), 200); // Blue
            break;
    case 10: theaterChase(strip.Color(  0,   255, 0), 200); // Green
            break;
    case 11: theaterChase(strip.Color(255,   0, 255), 200); // 
            break;
    case 12: theaterChase(strip.Color(0, 255, 255), 200); // 
            break;
    case 13: theaterChase(strip.Color(255, 255, 0), 200); // 
            break;
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  while(isbusy == 0)
  {
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
    isbusy = digitalRead(DFPlayerBusy);
  }
  isbusy = 0;
  strip.show();
}

// this function will be called when the button was pressed 1 times.
void myClickFunction() {
 
  myDFPlayer.playFolder(folder, lastsong);
  doShow();

  lastsong++;
  if(lastsong>songcount[folder-1])
    lastsong = 1;
  //Store a new last song number to the matching folder information inside the EEPROM
  writeValue(folder-1, lastsong);
} 

// this function will be called when the button was pressed 2 times in a short timeframe.
void myDoubleClickFunction() {
  
  //Go to next folder, roll over at 10th folder. Remeber first folder is numbered 1
  folder++;
  if (folder > 10)
    folder=1;

  //Store information that we changed to folder to the EEPROM and read file count of our new folder
  writeValue(10, folder);

  //And load which song was last played in this folder
  lastsong = getValue(folder-1);
 
  //Give a short green blink to indicate that we switched folders
  if (folder != 1)
  {
  strip.setPixelColor(0,strip.Color(255, 0, 0));
  strip.show();
  }
  //blink red if we reach the first folder
  else 
  {
  strip.setPixelColor(0,strip.Color(0, 255, 0));
  strip.show();
  }
  delay(50);
  strip.setPixelColor(0,0);
  strip.show();
} 

// this function will be called when the button is pressed for a long time.
void myLongPressFunction() {
  myDFPlayer.playFolder(99, 1);
  doShow();
} 
