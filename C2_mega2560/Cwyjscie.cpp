#include "Cwyjscie.h"

/**
 * Biblioteka obslugujaca wyjscia
 */




// ----- Initialization and Default Values -----


/**
 * Inicjacja obiektu konieczna w procedurze setup
 */

void Cwyjscie::Init(byte pin,const char* item_nazwa,bool czyWyjsciePWM)
{
  _pin=pin;
  czyPWM=czyWyjsciePWM;
  strcpy(itemNazwa,item_nazwa);
  pinMode(_pin,OUTPUT);
  stan=WYJSCIE_ZGAS;
  ustawWyjscie(stan);
}


/***
 * ustawia wyjscie na sygnal PWM 
 * docelowo-wartosc PWM docelowa
 * czas_ms - czas w ms w ilu ma dojsc do ustalenia wartosci max 5sek
 */
void Cwyjscie::ustawWyjsciePWM(uint8_t docelowo,unsigned long czas_ms)
{
  pwm=true;
  docelowyPWM=docelowo;
  timerPWM=millis();
  int droga=docelowo-stan;
  long liczba_krokow=czas_ms/T_PWM;
  krokPWM=droga/liczba_krokow;
  if(krokPWM==0) krokPWM=droga;
  if(krokPWM==0) pwm=false;
    Serial.print(F("StartPWM timer= "));
        Serial.print(timerPWM);
        Serial.print(F(" stan= "));
        Serial.print(stan);
         Serial.print(F(" docelowyPWM= "));
        Serial.print(docelowyPWM);
        Serial.print(F(" krokPWM= "));
        Serial.print(krokPWM);
        Serial.print(" liczba_krokow= ");
        Serial.print(liczba_krokow);
        Serial.print(F(" droga= "));
        Serial.println(droga);
}

/**
 * Cykliczna weryfikacja obiektu konieczna w procedurze loop
 */
void Cwyjscie::loop()
{
  unsigned long t=millis();
  if(timer) //timer ma priorytet nad pwm, jesli timer mowi zmien stan to zmienia i konczy pwmowac
  {  
    if(t-ustawiono_stan>za_ile_zmien_stan)
    {
      timer=false;
      ustawWyjscie(docelowy_stan);
      docelowyPWM=docelowy_stan;
      pwm=false;
     }
     
  }
  if(pwm)
  {
    if(t-timerPWM>T_PWM)
    {
      timerPWM=t;
      if(((krokPWM>0)&& (stan+krokPWM>=docelowyPWM))||((krokPWM<0)&& (stan+krokPWM<=docelowyPWM)))
      {
      /*  Serial.print(F("KoniecPWM timer= "));
        Serial.print(timerPWM);
        Serial.print(F(" stan= "));
        Serial.print(stan);
        Serial.print(F(" krokPWM= "));
        Serial.print(krokPWM);
        Serial.print(F(" docelowyPWM= "));
        Serial.print(docelowyPWM);
        Serial.print(F(" stan+krokPWM="));
        Serial.println(stan+krokPWM);
        */
        ustawWyjscie(docelowyPWM);
        
        pwm=false;
      }else
      {
      /*  Serial.print("LoopPWM timer= ");
        Serial.print(timerPWM);
        Serial.print(" stan= ");
        Serial.print(stan);
        Serial.print(" krokPWM= ");
        Serial.println(krokPWM);*/
    //  if(stan+krokPWM==1)ustawWyjscie(2);///////////////<<<< do weryfikacji
    //  else
        ustawWyjscie(stan+krokPWM);

      }
    }
  }
  
 }

/***
 * ustawia wyjscie na PWM a po czasie czas_ms zmienia ustawienie na docelowo
 * PWM plynnie dochodzi do celu ale wylacza sie gwaltownie
 */

void Cwyjscie::ustawWyjscieCzasowoPWM(uint8_t celPWM, unsigned long czas_ms , uint8_t docelowo, unsigned long za_ile, bool wybierz_dluzszy_czas)
{
   Serial.print(F("ustawWyjscieCzasowoPWM celPWM="));
  Serial.print(celPWM);Serial.print(F(" docelowo="));
  Serial.print(docelowo);Serial.print(F(" czasMs="));
  Serial.print(czas_ms);Serial.print(F(" za_ile="));
  Serial.print(za_ile);Serial.print(F(" dluzszy_czas="));
  Serial.println(wybierz_dluzszy_czas);
  //zakladamy ze PWM jest mniejszy niz czas do zmiany stanu
  ustawWyjscieCzasowo(stan,docelowo,za_ile,wybierz_dluzszy_czas);
  ustawWyjsciePWM(celPWM,czas_ms);
}


/**
 * Ustawia wyjscie na wartosc teraz, a po uplywie czasu za_ile przelaczy na wartosc docelowo
 * parametr wybierz_dluzszy_czas decyduje czy jesli pin czeka juz na zmiane okreslona za jakis czas (z poprzedniego wywolania)
 * to czy ma bezwzglednie go nadpisac, czy wybrac dluzszy czas  oczekiwania
 */
 
void Cwyjscie::ustawWyjscieCzasowo(uint8_t teraz, uint8_t docelowo, unsigned long za_ile, bool wybierz_dluzszy_czas)
{
  //bufor na potrzeby UART
//  char buf[100];
 // sprintf(buf,"s=%d; ID=%d ustWyjCzas(teraz=%d, docelowo=%d, za_ile= %ul, wybierz_dluzszy_czas=%d) ",stan,ID, teraz,docelowo,za_ile,wybierz_dluzszy_czas);
  //Serial.print(buf);
  Serial.print(F("ustawWyjscieCzasowo teraz="));
  Serial.print(teraz);Serial.print(F(" docelowo="));
  Serial.print(docelowo);Serial.print(F(" za_ile="));
  Serial.print(za_ile);Serial.print(F(" dluzszy_czas="));
  Serial.println(wybierz_dluzszy_czas);
  if(teraz==docelowo)
  {
    Serial.println(F(" Blad=> teraz==docel"));
   ustawWyjscie(teraz);
   //timer=false;
   return;
  }
  if(!timer) //nikt wczesniej nie wlaczyl timera wiec poprostu go wlaczamy i tyle
  {
      docelowy_stan=docelowo;
      za_ile_zmien_stan=za_ile;
      timer=true;
      Serial.println(F("!timer"));
  } else
  {
    //jesli wczesniej ktos wlaczyl timer to sa nastepujace opcje:
    // 1) nowe polecenie moze wydluzac czas, czyli teraz==stan oraz docelowo==docelowy_stan => tylko wydluzamy czas
    if(teraz==stan && docelowo==docelowy_stan)
    {
      if(wybierz_dluzszy_czas)
      { 
        unsigned long ile_trwa=millis()-ustawiono_stan;
        unsigned long ile_zostalo=za_ile_zmien_stan-ile_trwa;
        if(za_ile>ile_zostalo) //jesli pozostalo wiecej niz wskazuje na to nowe wywolanie nie rob nic jesli mniej to ustaw nowe
        {
          za_ile_zmien_stan=za_ile;
        }
        Serial.println(F("brak zmian, dluzszy czas"));
      } 
      else //zmien czas timera bez wzglednie
      {
        za_ile_zmien_stan=za_ile;
        Serial.println(F("brak zmian, czas bezwzgl."));
      }
    }
    // 2) nowe polecenie moze ustawic ten sam stan na teraz, ale docelowo inny teraz==stan, docelowo!= docelowy_stan => generalnie bledne wywolanie bo to oznacza ze stan==docelowy_stan czyli wystarczy wyłączyć timer
    // 3) nowe polecenie moze ustawic teraz inny stan ale docelowo ten sam stan teraz!=stan, docelowo==docelowy_stan => tu wylaczamy 
    // 4) nowe polecenie moze teraz ustawic inny stan i docelowo inny teraz!=stan, docelowo!=docelowy_stan  => anuluje dotychczasowe ustawienia i traktuje jak nowe polecenie
    if(teraz!=stan)
    {
      Serial.println(F("..restart timera"));
      timer=false;
      ustawWyjscieCzasowo(teraz,docelowo,za_ile,wybierz_dluzszy_czas);
      return;
    }
    
  }
  ustawWyjscie(teraz);
  timer=true;
 }

/**
 * Ustawia wyjście na wartosc u, jesli u > 1 to dla wyjsc obslugujacych PWM zadana bedzie wartosc PWM
 * jesli parametr absolutnie=true to resetowany jest timer od pwm oraz od czasu zmiany stanu
 */
void Cwyjscie::ustawWyjscie(uint8_t u,bool absolutnie)
{
  if(absolutnie) //ustaw na chama
  { 
    timer=false;
    pwm=false;
    
   }
   ustawiono_stan=millis();
   if(stan==u)return;
   
    stan=u; 
    wyslijStatusDoSerwera(itemNazwa,stan);//wyslijStatusDoSerwera(_pin,u,-1, 0);
   
   

   if(czyPWM)
   {
      //  Serial.print("pin=");
      //  Serial.println(_pin);
      //  Serial.print(" PWM=");
       analogWrite(_pin,stan);
       
      // Serial.println(stan);
   }
   else
   {
      if(stan>0)
       {
      
        digitalWrite(_pin,HIGH);
       }
      else
       digitalWrite(_pin,LOW);
   }
 
 }
 
