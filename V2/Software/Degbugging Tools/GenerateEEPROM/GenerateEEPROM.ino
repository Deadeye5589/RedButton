#include "OneButton.h"
#include "Arduino.h"
#include "WearLeveling.h"

//Button Input defintion 
OneButton button(2, HIGH); //2

void setup(){
  button.attachClick(myClickFunction);
  button.setDebounceTicks(50);
  
	//Initialize the EEPROM wear levelling and load last song for each directory
	for(int i=0; i<11; i++){
	  loadEeprom(i);
	}
}

void loop() {

  // keep watching the push button:
  button.tick();
}

// this function will be called when the button was pressed 1 times.
void myClickFunction() {
 
  for(int i=0; i<11; i++){
  writeValue(i, 1);
  }

} 
