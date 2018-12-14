#include <Adafruit_NeoPixel.h>
#include "OneButton.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//WS2812 definitions
#define LED_PIN 3
#define LED_COUNT 4
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

//Button Input defintion 
OneButton button(2, HIGH);
const int DFPlayerBusy = 4;

//Setup for the DFPlayer
SoftwareSerial mySoftwareSerial(0, 1); // RX, TX 
DFRobotDFPlayerMini myDFPlayer;

//Some things to remember all times
volatile int isbusy = 0;
volatile int showType = 0;
volatile int folder = 1;
volatile int lastsong[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
volatile int songcount[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {

  // Setup the WS2812 LEDs for the colorfull effects on the front of the button
  // End of trinket special code
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //Setup everything we need for the button press detection, click, double click, long button press 
  // link the myClickFunction function to be called on a click event.   
  button.attachClick(myClickFunction);

  // link the doubleclick function to be called on a doubleclick event.   
  button.attachDoubleClick(myDoubleClickFunction);

  //l line to long button press.
  button.attachDuringLongPress(myLongPressFunction);

  // set 80 msec. debouncing time. Default is 50 msec.
  button.setDebounceTicks(80);

  pinMode(DFPlayerBusy, INPUT);

  delay(2000);

  mySoftwareSerial.begin(9600);


    while (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    delay(100);
    }

    for(int i = 1 ; i<11 ; i++){
      songcount[i-1]=myDFPlayer.readFileCountsInFolder(i);
      if (songcount[i-1] == -1) {  //Error while Reading.
        //printDetail(myDFPlayer.readType(), myDFPlayer.read());
      }
      if (myDFPlayer.available()) {
      //printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
      }
      delay(200);
    }
    
  myDFPlayer.volume(20);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD); 

  delay(50);
}

void loop() {

    // keep watching the push button:
  button.tick();
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
  lastsong[folder-1]++;
  if(lastsong[folder-1]>songcount[folder-1])
  lastsong[folder-1] = 1;
  
  myDFPlayer.playFolder(folder, lastsong[folder-1]);
  showType++;
  if (showType > 13)
  showType=0;
  startShow(showType);
} 

// this function will be called when the button was pressed 2 times in a short timeframe.
void myDoubleClickFunction() {
  
  //Go to next folder, roll over at 10th folder. Remeber first folder is numbered 1
  folder++;
  if (folder > 10)
  folder=1;

  //blink if different color if we reach the first folder
  if (folder == 1)
  {
  strip.setPixelColor(0,strip.Color(0, 255, 0));
  strip.show();
  }
  // blink color to indicate change of folder
  else 
  {
  strip.setPixelColor(0,strip.Color(255, 0, 0));
  strip.show();
  }
  delay(50);
  strip.setPixelColor(0,0);
  strip.show();
} 

// this function will be called when the button is pressed for a long time.
void myLongPressFunction() {
  myDFPlayer.playFolder(99, 1);
  showType++;
  if (showType > 13)
  showType=0;
  startShow(showType);
} 

