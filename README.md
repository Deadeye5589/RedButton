# RedButton
Musical Emergency button, cause everything is awesome

Want a big red button that plays the "Everything is awesome" tune from the Lego Movie
or your favourite movie quotes like "Bullshit Private Pauler"?

## Well here you go.  
**What you need to buy:**

* https://www.amazon.de/gp/product/B074JBGWYW/ - The Big Red Button which is this all about
* https://www.amazon.de/gp/product/B07CRY9QTK - DFPlayer Module which plays sound from a Mini SD Card
* https://www.amazon.de/gp/product/B076KS2QDS - Digispark a small size Atiny85 Board
* Some WS2812 LEDs to your liking - The project uses 4 LEDs
* A 5V Power Supply with more than 500mA
* A Mini SD Card

**Other useful stuff:**

* Soldering Iron
* Wires
* Arduino Uno
* 12V Power Supply

## First things first  
Upon arival of the Atiny85 Digispark the IC will use an Arduino compatible bootloader. Which reduces the useable memory size. So we need to get rid of it using High Voltage Programming, following this tutorial:  
https://www.instructables.com/id/How-to-unlock-Digispark-ATtiny85-and-convert-it-to/

## The Build
Next is to write the actual program into the now unchained Atiny85. For this you can use the SPI interface of the Tiny. A precompiled hex file can be found inside this project.  

Then wire everything up according to the provided schematic drawing. Of course the speaker and the tact switch inside the button housing will be used. So all you have to do is to rewire those 2 components correctly.  

As for the LEDs. I have removed the original LEDs from the PCB inside the button housing and than glued some WS2812 stripes onto the PCB. This is the quick and dirty solution. Feel free to experiment with your own ideas here.

## Preparing the SD Card
The program was written to support a maximum of 10 folders, with up to 255 songs in each folder. This also means that you will need to provide 10 folders on the SD card or the program might crash if you cycle through the folders.  
The folders must be named:  
* 01  
* 02  
* 03  
* 04  
* 05  
* 06  
* 07  
* 08  
* 09  
* 10  

Next you can throw any MP3 or Wave file in this folders you want to play. Since it is not possible to leave folders empty, I suggest that you simply copy your songs in each folder that you don't need too. This was the program will always find a file to play.  
The files must be named 001 ... 255.mp3 or 001...255.wav

Inside the root folder you can also store a single filed named 001.mp3 or 001.wav  

## How to use
The Red Button has 3 functions:  
* Short Button Press
* Long Button Press
* Double Button Press

**Short Button Press**  
Will play a song from the current folder. Looping through all available songs in this folder.

**Long Button Press**  
Will always play the single file stored in the root directory of the SD card. 

**Double Button Press**  
Will cycle through the folders 1 ... 10 ... 1. Each time the last song you played for each folder will be stored. Upon return to this folder the next song in order will be played. The button will blink red if you change a folder. When you arrive back at folder 1 the button blinks green to indicate that you reached the first folder. 

