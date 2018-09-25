
/* tu odbywa sie awaryjna logika dzialania centralki w przypadku:
 *  a) braku połączenia
 *  b) debugowania z openHab (symulacji wcisniecia przyciskow)
 *  c) awarii
 *  
 *  parametry:
 *  item- nazwa itemu 
 *  pin - index / pozycja itemu w tabeli
 *  wartosc - stan na jaki zostało zmienione wejscie
 *  hardwareInit - kto zlecil wywołanie false=przyszlo z mqtt; true-wywolane przez fizyczny przycisk -obiekt Cwejscie
 */

void obslugaReakcjiNaWejscie(char* item, byte pin,byte wartosc,bool hardwareInit)
{
  //weryfikacja openHab kaze zdecydowac lub czy jest awaria polaczenia
  if(!hardwareInit || mqtt.dajStatusPolaczenia()!=0)
  {
    // tu podejmowana jest decyzja co ma sie zadziac w reakcji na wejscie
    // najpierw sproboj odswiezyc GUI openHAB
   //???????????????? wyslijStatusDoSerwera(item, wartosc);

    byte stan=wartosc;
    Serial.print("obslugaReakcjiNaWejscie(item="); Serial.print(item);
    Serial.print(", pin=");Serial.print(pin);
    Serial.print(", wartosc=");Serial.print(wartosc);
    Serial.print(", hardwareInit=");Serial.print(hardwareInit);
    Serial.println(")");Serial.println(pgm_read_byte_near(wejPIN_pin+pin));

    byte indWejscie=0;
   for(byte i=0;i<ile_wejsc;i++)
   {

     if(pgm_read_byte_near(wejPIN_pin+i)==pin)
     {
      indWejscie=i;
     }
   }

    
   switch(indWejscie)
    {
      case p1: //INP1 : //PIR kuchni
        if(stan==STAN_RELEASE)
        {
          wyj[s12].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MALY);
          wyj[s11].ustawWyjscieCzasowo(wyj[s11].dajStan(), WYJSCIE_ZGAS, CZAS_MALY);
          
        }
        //wyslijStatusDoSerwera(item,STAN_RELEASE);
      break;
      case p2: //INP2 : //PIR w salonie
        if(stan==STAN_KLIK)
        {
          wyj[s4].ustawWyjscieCzasowo(wyj[s4].dajStan(), WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s5].ustawWyjscieCzasowo(wyj[s5].dajStan(), WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s6].ustawWyjscieCzasowo(wyj[s6].dajStan(), WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s7].ustawWyjscieCzasowo(wyj[s7].dajStan(), WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s8].ustawWyjscieCzasowoPWM(wyj[s8].dajStan(),CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s9].ustawWyjscieCzasowoPWM(wyj[s9].dajStan(),CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s10].ustawWyjscieCzasowoPWM(wyj[s10].dajStan(),CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s16].ustawWyjscieCzasowoPWM(wyj[s16].dajStan(),CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s3].ustawWyjscieCzasowoPWM(wyj[s3].dajStan(),CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s14].ustawWyjscieCzasowoPWM(wyj[s14].dajStan(),CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s15].ustawWyjscieCzasowoPWM(wyj[s15].dajStan(),CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
        }
      break;
      case p3: //INP3 : // PIR w jadalni
         if(stan==STAN_KLIK)
         {
          wyj[s1].ustawWyjscieCzasowo(wyj[s1].dajStan(), WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s2].ustawWyjscieCzasowo(wyj[s2].dajStan(), WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s13].ustawWyjscieCzasowoPWM(wyj[s13].dajStan(),CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
         }
      break;
      case w1: //INP9 : //w1 salon led
      case w8:
       if(stan==STAN_PRESS||stan == STAN_RELEASE)
       {
          if(wyj[s3].dajStan()>0||wyj[s14].dajStan()>0||wyj[s15].dajStan()>0||wyj[s16].dajStan()>0)
          {
           wyj[s3].ustawWyjsciePWM(WYJSCIE_ZGAS,CZAS_ZMIANY_PWM);
           wyj[s14].ustawWyjsciePWM(WYJSCIE_ZGAS,CZAS_ZMIANY_PWM);
           wyj[s15].ustawWyjsciePWM(WYJSCIE_ZGAS,CZAS_ZMIANY_PWM);
           wyj[s16].ustawWyjsciePWM(WYJSCIE_ZGAS,CZAS_ZMIANY_PWM);
           }else
           {
           wyj[s3].ustawWyjscieCzasowoPWM(JASNOSC_MALA,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
           wyj[s14].ustawWyjscieCzasowoPWM(JASNOSC_MALA,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
           wyj[s15].ustawWyjscieCzasowoPWM(JASNOSC_MALA,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
           wyj[s16].ustawWyjscieCzasowoPWM(JASNOSC_MALA,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
            
           }
            
       }
       if(stan==STAN_KLIK)
       {
           wyj[s3].ustawWyjscieCzasowoPWM(JASNOSC_MAX,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
           wyj[s14].ustawWyjscieCzasowoPWM(JASNOSC_MAX,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
           wyj[s15].ustawWyjscieCzasowoPWM(JASNOSC_MAX,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
           wyj[s16].ustawWyjscieCzasowoPWM(JASNOSC_MAX,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
       }
        if(stan==STAN_DBKLIK)
        {
           if(wyj[s8].dajStan()>0||wyj[s9].dajStan()>0||wyj[s10].dajStan()>0)
           { 
            wyj[s8].ustawWyjsciePWM(WYJSCIE_ZGAS,CZAS_ZMIANY_PWM);
            wyj[s9].ustawWyjsciePWM(WYJSCIE_ZGAS,CZAS_ZMIANY_PWM);
            wyj[s10].ustawWyjsciePWM(WYJSCIE_ZGAS,CZAS_ZMIANY_PWM);
           }
           else
           {
            wyj[s8].ustawWyjscieCzasowoPWM(JASNOSC_MAX,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
            wyj[s9].ustawWyjscieCzasowoPWM(JASNOSC_MAX,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
            wyj[s10].ustawWyjscieCzasowoPWM(JASNOSC_MAX,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
           }
        }
      break;
      case w2: //INP10 : //w2 salon gora
      Serial.println("w2");
         if(stan==STAN_PRESS||stan == STAN_RELEASE)
        {
          if(wyj[s4].dajStan()>0||wyj[s5].dajStan()>0||wyj[s6].dajStan()>0||wyj[s7].dajStan()>0)
          {
             wyj[s4].ustawWyjscie(WYJSCIE_ZGAS,true);
             wyj[s5].ustawWyjscie(WYJSCIE_ZGAS,true);
             wyj[s6].ustawWyjscie(WYJSCIE_ZGAS,true);
             wyj[s7].ustawWyjscie(WYJSCIE_ZGAS,true);
          } 
          else
          {
             wyj[s4].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
             wyj[s5].ustawWyjscie(WYJSCIE_ZGAS, true);
             wyj[s6].ustawWyjscie(WYJSCIE_ZGAS, true);
             wyj[s7].ustawWyjscie(WYJSCIE_ZGAS, true);
           }
        }
         if(stan==STAN_KLIK||stan==STAN_KLIK_R)
         {
             wyj[s4].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
             wyj[s5].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
             wyj[s6].ustawWyjscie(WYJSCIE_ZGAS, true);
             wyj[s7].ustawWyjscie(WYJSCIE_ZGAS, true);
         }
         if(stan==STAN_DBKLIK||stan==STAN_DBKLIK_R)
         {
             wyj[s4].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
             wyj[s5].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
             wyj[s6].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
             wyj[s7].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
         }
      break;
      case w3: //INP13 : //w3 wlacznik jadalnia LED
      case w9:
        if(stan==STAN_PRESS||stan == STAN_RELEASE)
        {
            if(wyj[s13].dajStan()>0)
            {
              wyj[s13].ustawWyjsciePWM(WYJSCIE_ZGAS,CZAS_ZMIANY_PWM);
            }
            else
            {
              wyj[s13].ustawWyjscieCzasowoPWM(JASNOSC_MALA,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
            }  
        }
         if(stan==STAN_KLIK)
         {
           wyj[s13].ustawWyjscieCzasowoPWM(JASNOSC_MAX,CZAS_ZMIANY_PWM, WYJSCIE_ZGAS, CZAS_MALY);
         }
         if(stan==STAN_DBKLIK)
         {
          
         }
      break;
      case w4: //INP14 : //w4 wlacznik jadalnia gora
      //case w10:
       if(stan==STAN_PRESS||stan == STAN_RELEASE)
        {
             if(wyj[s1].dajStan()>0||wyj[s2].dajStan()>0)
             {
               wyj[s1].ustawWyjscie(WYJSCIE_ZGAS,true);
               wyj[s2].ustawWyjscie(WYJSCIE_ZGAS,true);
             }else
             {
               wyj[s1].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
               wyj[s2].ustawWyjscie(WYJSCIE_ZGAS,true);
             }
             
        }
       if(stan==STAN_KLIK||stan==STAN_KLIK_R)
        {
            wyj[s2].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
        }
       if(stan==STAN_DBKLIK||stan==STAN_DBKLIK_R)
       {
          wyj[s1].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
          wyj[s2].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, WYJSCIE_ZGAS, CZAS_MALY);
       }
      break;
      case w5: //INP15 : //w5 wlacznik kuchnia 1
      case w13:

       if(stan==STAN_PRESS||stan == STAN_RELEASE)
        {
             if(wyj[s12].dajStan()>0)
             {
                  wyj[s12].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
             }else
             {
                  wyj[s12].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
             }
        }
       
      break;
      case w6: //INP16 : //w6 wlacznik kuchnia 2
      case w10:
       if(stan==STAN_PRESS||stan == STAN_RELEASE)
        {
             if(wyj[s11].dajStan()>0)
             {
                  wyj[s11].ustawWyjscie(JASNOSC_ZGAS, true);
                  wyj[s12].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
             }else
             {
                  wyj[s11].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
                  wyj[s12].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
             }
        }
      break;
      case w7:
       if(stan==STAN_PRESS||stan == STAN_RELEASE)
       {
          if(wyj[s8].dajStan()>0||wyj[s9].dajStan()>0||wyj[s10].dajStan()>0)
          {
            wyj[s8].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
            wyj[s9].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
            wyj[s10].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
          }else
          {
            wyj[s8].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s9].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s10].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
          }
        }
        if(stan==STAN_KLIK)
        {
            wyj[s8].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s9].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s10].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
        }
        
     break;
     case w11:
       if(stan==STAN_PRESS||stan == STAN_RELEASE)
       {
            wyj[s8].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
            wyj[s9].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
            wyj[s10].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
            wyj[s3].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
            wyj[s11].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
            wyj[s13].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
            wyj[s14].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
            wyj[s15].ustawWyjsciePWM(JASNOSC_ZGAS, CZAS_ZMIANY_PWM);
       }
       if(stan==STAN_KLIK)
       {
            wyj[s8].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s9].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s10].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s3].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s11].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s14].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s15].ustawWyjscieCzasowoPWM(JASNOSC_MALA, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
       }
       if(stan==STAN_DBKLIK)
       {
            wyj[s8].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s9].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s10].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s3].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s11].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s14].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
            wyj[s15].ustawWyjscieCzasowoPWM(JASNOSC_MAX, CZAS_ZMIANY_PWM, JASNOSC_ZGAS, CZAS_MAX);
       }
     break;
     case w12:
      if(stan==STAN_PRESS||stan == STAN_RELEASE)
      {
           wyj[s1].ustawWyjscie(JASNOSC_ZGAS, true);
           wyj[s2].ustawWyjscie(JASNOSC_ZGAS, true);
           wyj[s4].ustawWyjscie(JASNOSC_ZGAS, true);
           wyj[s5].ustawWyjscie(JASNOSC_ZGAS, true);
           wyj[s6].ustawWyjscie(JASNOSC_ZGAS, true);
           wyj[s7].ustawWyjscie(JASNOSC_ZGAS, true);
      }
      if(stan==STAN_KLIK)
      {
         wyj[s1].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
         wyj[s2].ustawWyjscie(JASNOSC_ZGAS, true);
         wyj[s4].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
         wyj[s5].ustawWyjscie(JASNOSC_ZGAS, true);
         wyj[s6].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
         wyj[s7].ustawWyjscie(JASNOSC_ZGAS, true);
                                                           
      }
      if(stan==STAN_DBKLIK)
      {
        wyj[s1].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
        wyj[s2].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
        wyj[s4].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
        wyj[s5].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
        wyj[s6].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
        wyj[s7].ustawWyjscieCzasowo(WYJSCIE_ZAPAL, JASNOSC_ZGAS, CZAS_MAX);
         
      }
       
     break;
     }

     //po całej obsłudze dla przyciskow i pirkow ???? <<<< czy pirki tez
    // zmien stan na gui openHab
   // wyslijStatusDoSerwera(item,STAN_RELEASE);
  }
}
