#include "Cwejscie.h" 

StaticJsonBuffer<30> jsonBuffer;

void jsonFunc(byte id,byte st,char* itemStr)
{
   JsonObject& root = jsonBuffer.createObject();
  root["input"]=id; root["stan"]=st;
  char txt[30];
  root.printTo(txt, sizeof(txt));
  //Logika(txt);
   obslugaReakcjiNaWejscie(itemStr,id,st,true);
 }

void _clickFunc(int id,char* itemStr)
{
  Serial.print(F("Callback Click "));
  Serial.println(itemStr);
 jsonFunc(id,STAN_KLIK, itemStr);
}
void _clickFuncR(int id,char* itemStr)
{
  Serial.print(F("Callback Click_R "));
  Serial.println(itemStr);
 jsonFunc(id,STAN_KLIK_R, itemStr);
}

void _pressFunc(int id,char* itemStr)
{
    Serial.print(F("Callback press  "));
    Serial.println(itemStr);
    jsonFunc(id,STAN_PRESS, itemStr);
}
void _releaseFunc(int id,char* itemStr)
{
    Serial.print(F("Callback release "));
    Serial.println(itemStr);
    jsonFunc(id,STAN_RELEASE,itemStr);
 }
// this function will be called when the button was pressed 2 times in a short timeframe.
void _doubleClickFunc(int id,char* itemStr) {
    Serial.print(F("Callback doubleClick "));
    Serial.println(itemStr);
    jsonFunc(id,STAN_DBKLIK,itemStr);
  
} // doubleclick
void _doubleClickFuncR(int id,char* itemStr) {
    Serial.print(F("Callback doubleClick_R "));
    Serial.println(itemStr);
    jsonFunc(id,STAN_DBKLIK_R,itemStr);
  
} // doubleclick

// ----- Initialization and Default Values -----


void Cwejscie::Init(byte pin, const char* item_nazwa)
{
  Serial.print(F("Wejscie init "));
  _pin=pin;
  strcpy(itemNazwa,item_nazwa);
    pinMode(_pin,INPUT_PULLUP);

  //_pinMode(_pin,_typWejscia);

 // _clickTicks = 800;        // number of millisec that have to pass by before a click is detected.
//  _pressTicks = 1000;       // number of millisec that have to pass by before a lonn button press is detected.
 
  _state = 0; // starting with state 0: waiting for button to be pressed

 // if (activeLow) {
    // button connects ground to the pin when pressed.
 //   _buttonReleased = HIGH; // notPressed
   // _buttonPressed = LOW;
    //digitalWrite(_pin, HIGH);   // turn on pulldown resistor

  //} else {
    // button connects VCC to the pin when pressed.
  //  _buttonReleased = LOW;
  //  _buttonPressed = HIGH;
  //} // if
  buttonState=HIGH;//_buttonReleased;
  buttonState=debounceRead();
 //debug(" ok.");
 Serial.print(_pin);Serial.print(F(" stan= "));
 Serial.println(buttonState);
 }

// explicitely set the number of millisec that have to pass by before a click is detected.
//void Cwejscie::setClickTicks(int ticks) { 
 // _clickTicks = ticks;
//} // setClickTicks


// explicitely set the number of millisec that have to pass by before a lonn button press is detected.
//void Cwejscie::setPressTicks(int ticks) {
 // _pressTicks = ticks;
//} // setPressTicks

/*
// save function for click event
void Cwejscie::attachClick(callbackFunction newFunction)
{
  _clickFunc = newFunction;
} // attachClick


// save function for doubleClick event
void Cwejscie::attachDoubleClick(callbackFunction newFunction)
{
  _doubleClickFunc = newFunction;
} // attachDoubleClick


// save function for press event
void Cwejscie::attachPress(callbackFunction newFunction)
{
 // _pressFunc = newFunction;
} // attachPress
void Cwejscie::attachRelease(callbackFunction newFunction)
{
//  _releaseFunc = newFunction;
} // puszczenie przycisku
*/
/*
void Cwejscie::attach_digitalRead(p_digitalRead dr)
{
  _digitalRead = dr;
} // odczyt pinu


void Cwejscie::attach_pinMode(p_pinMode pm)
{
  _pinMode = pm;
} // odczyt pinu
*/
void Cwejscie::loop(void)
{
  
  // Detect the input information 
  byte buttonLevel = debounceRead(); // current button signal.
  unsigned long now = millis(); // current (relative) time in msecs.

  switch(_state)
  {
    case 0: // przycisk puszczony stan początkowy
     if (buttonLevel == LOW)
     {
            //Serial.print(_state);  Serial.print("->");
      _state = 1; // step to state 1
      //Serial.println(_state);
      _startTime = now; // remember starting time
  //    debug("stan",_state);
     } 
    break;
    case 1: // czeka na puszczenie przycisku
      if (buttonLevel == HIGH) {
      //Serial.print(_state);   Serial.print("->");
      _state = 2; // step to state 2
      //Serial.println(_state);
      //debug("stan",_state);
    } else 
    if ((buttonLevel == LOW) && (now - _startTime  >  _pressTicks)) {
      if (_pressFunc){ _pressFunc(_pin,itemNazwa);}
      //Serial.print(_state);     Serial.print("->");
      _state = 6; // step to state 6
      //Serial.println(_state);
  //    debug("stan",_state);
    } else {
      // wait. Stay in this state.
    } // if
    break;
    case 2:
     // waiting for menu pin being pressed the second time or timeout.
    if (now-_startTime > _clickTicks) {
      
      // this was only a single short click
      _clickFunc(_pin,itemNazwa);//if (_clickFunc){ _clickFunc(_pin);}
        //    Serial.print(_state);   Serial.print("->");
      _state = 0; // restart.
      //Serial.println(_state);
      //debug("stan",_state);
    } else if (buttonLevel == LOW) {
      //  Serial.print(_state);      Serial.print("->");
      _state = 3; // step to state 3
      //Serial.println(_state);
  //    debug("stan",_state);
    } // if
break;
    case 3:
   
    if (now-_startTime > _clickTicks) {
      //Serial.print(_state);   Serial.print("->");
      _state = 4; // koniec czasu uznaj ze byl klik.
      //Serial.println(_state);
    } else if (buttonLevel == HIGH) {
      if (_doubleClickFunc){  _doubleClickFunc(_pin,itemNazwa);}
        //    Serial.print(_state);    Serial.print("->");
      _state = 0; // koniec
      //Serial.println(_state);
  //    debug("stan",_state);
    } // if
    break;
    case 4: //czekaj az drugi klik sie skonczy
     if (buttonLevel == HIGH)
     {
      //  Serial.print(_state);   Serial.print("->");
      _state=0;
      //Serial.println(_state);
      }
    break;
    
    case 5:
    // waiting for menu pin being released finally.
    if (buttonLevel == HIGH) {
      // this was a 2 click sequence.
     
      if (_doubleClickFunc){  _doubleClickFunc(_pin,itemNazwa);}
        //    Serial.print(_state);    Serial.print("->");
      _state = 0; // restart.
      //Serial.println(_state);
    } // if
    
    case 6: // waiting for menu pin being release after long press.
     if (buttonLevel == HIGH) {

       _state = 7; // step to state 1
      //Serial.println(_state);
      _startTime = now; // remember starting time
     }
     break;
     case 7:
        if (buttonLevel == LOW) {
      //Serial.print(_state);   Serial.print("->");
      _state = 8; // step to state 2
      //Serial.println(_state);
      //debug("stan",_state);
    } else 
    if ((buttonLevel == HIGH) && (now - _startTime  >  _pressTicks)) {
       if(_releaseFunc){ _releaseFunc(_pin,itemNazwa);}
      //Serial.print(_state);     Serial.print("->");
      _state = 0; // step to state 6
      //Serial.println(_state);
  //    debug("stan",_state);
    } else {
      // wait. Stay in this state.
    } // if
    break;
   case 8:
     // waiting for menu pin being pressed the second time or timeout.
    if (now-_startTime > _clickTicks) {
      
      // this was only a single short click
      _clickFuncR(_pin,itemNazwa);//if (_clickFunc){ _clickFunc(_pin);}
        //    Serial.print(_state);   Serial.print("->");
      _state = 6; // restart.
      //Serial.println(_state);
      //debug("stan",_state);
    } else if (buttonLevel == HIGH) {
      //  Serial.print(_state);      Serial.print("->");
      _state = 9; // step to state 3
      //Serial.println(_state);
  //    debug("stan",_state);
    } // if
    break;
      case 9:
   
    if (now-_startTime > _clickTicks) {
      //Serial.print(_state);   Serial.print("->");
      _state = 10; // koniec czasu uznaj ze byl klik.
      //Serial.println(_state);
    } else if (buttonLevel == LOW) {
      if (_doubleClickFuncR){  _doubleClickFuncR(_pin,itemNazwa);}
        //    Serial.print(_state);    Serial.print("->");
      _state = 6; // koniec
      //Serial.println(_state);
  //    debug("stan",_state);
    } // if
    break;
    case 10: //czekaj az drugi klik sie skonczy
     if (buttonLevel == LOW)
     {
      //  Serial.print(_state);   Serial.print("->");
      _state=6;
      //Serial.println(_state);
      }
    break;
     
  }

}

byte Cwejscie::debounceRead()
{
  // read the state of the switch into a local variable:
  byte reading;// =_digitalRead(_pin);
  reading=digitalRead(_pin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    Serial.print(F("start debounce wejscia "));Serial.println(_pin);
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > Cwejscie_DEBOUNCE_DELAY) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
       Serial.print(F("Zmiana wejscia "));Serial.print(_pin);
       Serial.print(F("***buttonState"));Serial.println(buttonState);
      
      }
    }
 
  lastButtonState = reading;
  return buttonState;
}

// end.



