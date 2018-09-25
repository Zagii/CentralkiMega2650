#ifndef Cwyjscie_h
#define Cwyjscie_h 


/**
 * Biblioteka obslugujaca wyjscia
 */

#include "Arduino.h"

//#define DEBUG

#define WYJSCIE_ZAPAL 255 //HIGH
#define WYJSCIE_ZGAS LOW

#define WYJSCIE_NORMALNE 0
#define WYJSCIE_MCP 1

#define T_PWM 50
/*
extern "C" {
  typedef void (*p_digitalWrite)(uint8_t pin,uint8_t  co);
}
extern "C" {
  typedef void (*p_analogWrite)(uint8_t pin,int  co);
}*/
/*
extern "C" {
  typedef void (*p_pinMode)(uint8_t pin,uint8_t mode);
}
*/

extern void wyslijStatusDoSerwera(char* itmName,byte wartosc);

class Cwyjscie
{
    private:
 // byte ID; //identyfikator przycisku
  byte _pin;        // hardware pin number. 
 // int _typWyjscia; //INPUT_PULLUP/INPUT etc..

  
  unsigned long ustawiono_stan;
  bool timer=false;
  bool pwm=false;
  unsigned long timerPWM=0;
  int krokPWM=0;
  uint8_t docelowyPWM;
  byte docelowy_stan;
  unsigned long za_ile_zmien_stan;
  byte stan=0; //0-zgaszone; 1-zapalone; >1 - PWM

  char itemNazwa[7];
  bool czyPWM=false;
  
 
  public:
   
    void  Init(byte pin,const char* item_nazwa,bool czyWyjsciePWM);
  // ----- Constructor -----
   Cwyjscie(){};

  void ustawWyjscie(uint8_t u,bool absolutnie=false);
  void ustawWyjscieCzasowo(uint8_t teraz, uint8_t docelowo, unsigned long za_ile, bool wybierz_dluzszy_czas=true);
  void loop();
  void  ustawWyjsciePWM(uint8_t docelowo,unsigned long czas);
  void ustawWyjscieCzasowoPWM(uint8_t celPWM, unsigned long czas_ms , uint8_t docelowo, unsigned long za_ile, bool wybierz_dluzszy_czas=true);
  byte dajStan(){return stan;};
  byte dajPin(){return _pin;};
};
#endif
