#ifndef Cwejscie_h
#define Cwejscie_h 

#define  Cwejscie_DEBOUNCE_DELAY  100    // the debounce time; increase if the output flickers

#include "Arduino.h"

#include <ArduinoJson.h>

#define _clickTicks 300
#define _pressTicks 500

 #define STAN_RELEASE '0' //puszczony przycisk
 #define STAN_KLIK '1' //był pojedynczy klik na wejsciu
 #define STAN_DBKLIK '2' //byłpodwójny klik na wejsciu
 #define STAN_PRESS '3' //dlugie wcisniecie przycisku
 #define STAN_KLIK_R '4' //był pojedynczy klik na wejsciu
 #define STAN_DBKLIK_R '5' //byłpodwójny klik na wejsciu
 
 

//#define DEBUG

// ----- Callback function types -----
/*
extern "C" {
  typedef void (*callbackFunction)(int id);
}*/

/*
extern "C" {
  typedef int (*p_digitalRead)(uint8_t pin);
}
extern "C" {
  typedef void (*p_pinMode)(uint8_t pin,uint8_t mode);
}*/

extern void Logika(char *txt);
extern void obslugaReakcjiNaWejscie(char* item, byte pin,byte wartosc,bool hardwareInit);

class Cwejscie
{

 
public:
  // ----- Constructor -----
 Cwejscie(){};
   void  Init(byte pin,const char* item_nazwa);
  
  // ----- Set runtime parameters -----

  // set # millisec after single click is assumed.
 // void setClickTicks(int ticks);

  // set # millisec after press is assumed.
 // void setPressTicks(int ticks);

  // attach functions that will be called when button was pressed in th especified way.
 // void attachClick(callbackFunction newFunction);
 // void attachDoubleClick(callbackFunction newFunction);
 // void attachPress(callbackFunction newFunction);
 // void attachRelease(callbackFunction newFunction);

 // void attach_digitalRead(p_digitalRead dr);
 // void attach_pinMode(p_pinMode pm);

  // ----- State machine functions -----

  // call this function every some milliseconds for handling button events.
  void loop(void);
  byte dajStan(){return _state;};
private:
 // int ID; //identyfikator przycisku
  byte _pin;        // hardware pin number. 
  char itemNazwa[7];
 // int _typWejscia; //INPUT_PULLUP/INPUT etc..
 // int _clickTicks; // number of ticks that have to pass by before a click is detected
 // int _pressTicks; // number of ticks that have to pass by before a lonn button press is detected

  //int _buttonReleased;//==HIGH
  //int _buttonPressed;//==LOW
  
///////////////////////// debounce ///////////
  byte buttonState;             // the current reading from the input pin
  byte lastButtonState = HIGH;   // the previous reading from the input pin
  
  // the following variables are long's because the time, measured in miliseconds,
  // will quickly become a bigger number than can be stored in an int.
  long lastDebounceTime = 0;  // the last time the output pin was toggled

///////////////////
   byte debounceRead();
  //void debug(String txt);
 // void debug(String txt,int wartosc);
  // These variables will hold functions acting as event source.
  
  // These variables that hold information across the upcomming tick calls.
  // They are initialized once on program start and are updated every time the tick function is called.
  byte _state;
  unsigned long _startTime; // will be set in state 1

};

#endif





