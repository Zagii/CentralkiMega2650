#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

#include <PubSubClient.h>
#include "CMQTT.h"
#include "Cwejscie.h"
#include "Cwyjscie.h"
#include "CopenHAB.h"

#define WBUDOWANY_LED 13

CMQTT mqtt;

Cwyjscie wyj[ile_wyjsc];
Cwejscie wej[ile_wejsc];



void wyslijStatusDoSerwera(char* itmName,byte wartosc)
{
   StaticJsonBuffer<50> jsonBuffer;
   JsonObject& root = jsonBuffer.createObject();
  // if(wartosc>5 && wartosc <101) //takie lipne założenie że inputy beda mialy stany 0-4
    //  wartosc=wartosc*100/255;   //olewamy pwm 0-4% a reszte przeliczamy na %
   root["item"]=itmName;
   root["wartosc"]=wartosc;
   char buffer[80];
   root.printTo(buffer, sizeof(buffer));
   mqtt.publikuj(mojTopicIDPublish,buffer);
}

void obslugaReakcjiNaWejscie(char* item, byte pin,byte wartosc,bool hardwareInit);
/**
 * wysyla status wyjsc do serwera
 * to wywołuje wyjscie ktore stwierdzilo ze zmienil sie stan pinu i warto o tym poinformowac serwer
 * jesli pin>=0 to zmiana wartosci pinu
 * jesli input>=0 to zmiana wejscia na stan
 */
void wyslijStatusDoSerwera_stary(int pin,byte wartosc,int input, byte stan)
{
   StaticJsonBuffer<50> jsonBuffer;
   JsonObject& root = jsonBuffer.createObject();
  
   if(pin>=0)
   {
   
     for(byte i=0;i<ile_wyjsc;i++)  //pin tu przekazany to id wyjscia a operujemy na pozycji
     {
      if(wyj[i].dajPin()==pin)
      {
        pin=i;
        break;
       }
     }
    root["item"]=pin;
    root["wartosc"]=wartosc;
   }
   if(input>=0)
   {
    root["input"]=input;
    root["stan"]=stan;
    }
    char buffer[80];
    root.printTo(buffer, sizeof(buffer));
     mqtt.publikuj(mojTopicIDPublish,buffer);

}

void wyslijStatusCalyDoSerwera(bool wy)
{
   StaticJsonBuffer<150> jsonBuffer;
   JsonObject& root = jsonBuffer.createObject();
  char bufWe[20];
  char bufWy[20];
//  if(wy)
//  {
    char wartHEX[2]="";
     for(byte i=0;i<ile_wyjsc;i++)
    {
      itoa((int)wyj[i].dajStan()/16,wartHEX,16);
      if(wyj[i].dajStan()==1)strcpy(wartHEX,"F");
      bufWy[i]=(char)wartHEX[0];     
    }
    bufWy[ile_wyjsc]='\0';
    root["wy"]=bufWy;
 // }else
 // {
 
  
  for(byte i=0;i<ile_wejsc;i++)
  {
     itoa(wej[i].dajStan(),wartHEX,10);
       bufWe[i]=(char)wartHEX[0];   
   }
    bufWe[ile_wejsc]='\0';
    root["we"]=bufWe;
 // }
   
   char buffer[80];
   root.printTo(buffer, sizeof(buffer));
  mqtt.publikuj(mojTopicIDPublish,buffer);
    
   
}

 
/**
 * nowy opis:
 * item - oznacza nazwe itemu wartosc to jego state
 * 
 * stary opis:
 * parsowanie rozkazu, moze byc bezposredni rozkaz ustawienia wyjscia na okreslona wartosc
 * czyli { pin:xx, wartosc:yy , czas:zz, naIle:aa, potem:bb } - opcjonalnie czas osiagniecia stanu dla PWM
 * parametry jesli jest pin to 
 * pin - numer pinu zindeksowany wg tablicy wyj[]
 * wartosc -obowiazkowa =na co ustawic dany pin (moze byc wartosc PWM)
 * czas - opcjonalny = płynne przejscie z obecnego stanu do wartosci wartosc w czasie czas
 * naIle oraz potem - opcjonalne ale muszą występować razem, na ile mówi o tym ile ms ma trwac obecny stan, potem to wartosc jaka przyjmie pin po uplywie czasu 'naIle'
 * lub symulacja zdarzenia wejscia
 * jesli jest input
 * czyli { input:aa, stan:bb , robic:0/1} - opcjonalnie informacja czy przetwarzac rozkaz lokalnie czy go wyslac do OpenHABa
 * input - numer pinu wg tablicy wej[] powinien odpowiadac indeksowi wejPIN_TXT => wej[input].pin=wej[wejPIN_TXT[input]].pin 
 *        np dla input=2;  wej[2] - to wejscie na pinie wejPIN_pin[2] czyli 5 co odpowiada p3
 * stan - typ zdarzenia jeden z poniższych       
 *          STAN_KLIK 'a' //pojedynczy klik na wejsciu
 *          STAN_DBKLIK 'b' //podwójny klik na wejsciu
 *          STAN_PRESSS 'c' //dlugie wcisniecie przycisku
 *          STAN_RELEASE 'd' //puszczenie przycisku 
 * robic - opcjonalne jesli brak parametru lub robic=1 to logika operacji zwiazanej z notyfikacja bedzie obsluzona przez nano         
 *          w przeciwnym wypadku zdarzenie bedzie przeslane do serwera i nano bedzie oczekiwac na interpretacje oraz ew rozkaz typu 'pin'
 *          dodatkowo jest tutaj sprawdzanie stanu polaczenia, jesli brak polaczenia z serwerem logika zawsze w trybie awaryjnym bedzie obslugiwana przez nano
 */
void Logika(char* txt)
{
  StaticJsonBuffer<100> jsonBuffer;
 
  JsonObject& root=jsonBuffer.parseObject(txt);
  if (!root.success()) {
    Serial.print(txt);
    Serial.println(F(" - parseObject() failed"));
   
    return;
  }
  char item[10];
  #define ITEM_BLAD 0
  #define ITEM_WEJSCIE 1
  #define ITEM_WYJSCIE 2
  byte typItemu=ITEM_BLAD; //0-brak; 1-wejscie; 2-wyjscie
  byte pin;
  byte wartosc=0;
  byte input=0;
  byte stan=0;
  byte potem=0;
  unsigned long naIle=0;
  unsigned long czas=0;
  #define JASNOSC_MALA 150
  #define JASNOSC_MAX 254
  #define JASNOSC_ZGAS 0
  #define CZAS_MALY 300000 //5min
  #define CZAS_MAX  900000 //15min
  #define CZAS_ZMIANY_PWM 2000 //2 sek
  #define CZAS_BRAMA 500 // 0,5sek


  //////////////////////////////////// wymuszenie wartosci pinu przez serwer ///////////////////////////////
  if (root.containsKey("item")&&root.containsKey("wartosc"))
  {
    strcpy(item,root["item"]);
    pin=255;
    strcpy(item,item+3);//omijamy id centralki w nazwie itemu
    for(byte i=0;i<ile_wyjsc;i++)
    {
      if(strcmp(item,wyjItem[i])==0)
      {
       pin=i;  
       typItemu=ITEM_WYJSCIE;
      }
    }
    if(typItemu==ITEM_BLAD)
    {
      for(byte i=0;i<ile_wejsc;i++)
      {
        if(strcmp(item,wejItem[i])==0)
        {
         pin=i;  
         typItemu=ITEM_WEJSCIE;
        }
      }
    }
    if(typItemu==ITEM_BLAD)
    {
      Serial.print(F("Item nie istnieje: "));
      Serial.println(item);
      return;
    }
   wartosc=root["wartosc"];

    

    if(typItemu==ITEM_WYJSCIE)
    {
      if(wartosc>0)
      {
      /*  if(wyjPWM[pin]) 
          wartosc=wartosc;//*255/100; //openHab dla analogowych (PWM) ma zakres [0,100]%  
        else
          wartosc=255; //bo trzeba durnego openHaba przetłumaczyc na ludzkie dla cyfrowych jest 0/255
        */
      }
      if(root.containsKey("czas"))
      {
        czas=root["czas"];
        if(root.containsKey("naIle")&&root.containsKey("potem"))
        {
          naIle=root["naIle"];
          potem=root["potem"];
          wyj[pin].ustawWyjscieCzasowoPWM(wartosc, czas , potem, naIle, true);
        }else
        {
            wyj[pin].ustawWyjsciePWM(wartosc,czas);
        }
       }else
       {      
         if(root.containsKey("naIle")&&root.containsKey("potem"))
          {
            naIle=root["naIle"];
            potem=root["potem"];
            wyj[pin].ustawWyjscieCzasowo( wartosc, potem, naIle, true);
          }
          else
          {
            wyj[pin].ustawWyjscie(wartosc);
          }
         
       }
     Serial.print(F("Zmienilem pin: "));Serial.print(pin); Serial.print(" | ");Serial.print(wyjItem[pin]); Serial.print(F(" na wartosc: "));Serial.println(wartosc);
     return;
    }

    if(typItemu==ITEM_WEJSCIE)
    {
      obslugaReakcjiNaWejscie(item,pin,wartosc,false);
    }

    return; //dalszej czesci nie robimy
  }
  //////////////////////////// koniec części wymuszającej wartość pinu przez serwer /////////////////////////////////

  
  //////////////////////////// przetworzenie komunikatu o zdarzeniu na jednym z wejsc ///////////////////////////////
  if (root.containsKey("input")&&root.containsKey("stan"))
  {
    input=root["input"];  stan=root["stan"];
    byte robic=1;
    if(root.containsKey("robic")) // jesli robic ma wartosc inna niz domyslna to komunikat zostanie przekazany do serwera
    {
      robic=root["robic"];
     }
 
    if(robic==0)
    {
      // to tylko wyslij komunikat ze ktos cos takiego zlecil
      // licz na to ze serwer obsluzy logike i zleci stan na jakims pinie

   //  wyslijStatusDoSerwera(-1,0,input,stan);
      wyslijStatusDoSerwera(item,stan);
      return;
    }

   
   
   Serial.print(F("Zasymulowalem input: "));Serial.print(input);Serial.print(F(" zdarzenie: "));Serial.println(stan);
   return;
  }
   Serial.println(F("Rozkaz nieznany.. "));
  
}


void setup()
{
    pinMode(WBUDOWANY_LED, OUTPUT); 
    digitalWrite(WBUDOWANY_LED,HIGH);
    Serial.begin(19200);
    Serial.print(F("Setup "));
  //  char buf[10];
   for(byte i=0;i<ile_wyjsc;i++)
   {
  //  strcpy_P(buf, (char*)pgm_read_word(&(wyjItem[i])));
   // Serial.println(buf);
    wyj[i].Init(pgm_read_byte_near(wyjPIN_pin+i),wyjItem[i],wyjPWM[i]);
   }
   
   for(byte i=0;i<ile_wejsc;i++)
   {

    wej[i].Init(pgm_read_byte_near(wejPIN_pin+i),wejItem[i]);
   }

    mqtt.Init();
    Serial.println(F("Start"));
    digitalWrite(WBUDOWANY_LED,LOW);

   /*  analogWrite(10,10);
      delay(500);
     analogWrite(10,60);
     delay(500);
     analogWrite(10,100);
     delay(500);
     analogWrite(10,150);
     delay(500);
     analogWrite(10,220);*/
}

unsigned long m=0;

void loop()
{

  mqtt.loop();

  if(mqtt.isMsgAvailable())
  {
    char t[20];
    char ms[128];
    mqtt.dawajMsg(t,ms);
    Serial.print(F("Przetwarzam t: "));
    Serial.print(t);
    Serial.print("; m: ");
    Serial.println(ms);
    char c=ms[0];
    for(int i=0;i<strlen(ms);i++)
    {
      if(ms[i]=='=')ms[i]=':';///zmiana bo openhab nei umie
     }
      
    Logika(ms);
    
  }
  for(byte i=0;i<ile_wyjsc;i++)
  {
      wyj[i].loop();
  }
     
  for(byte j=0;j<ile_wejsc;j++)
  {
      wej[j].loop();
   }
  if(millis()-m>60000)//////////////zmienic na czesciej
  {
    m=millis();
    wyslijStatusCalyDoSerwera(true); 
  }

}
