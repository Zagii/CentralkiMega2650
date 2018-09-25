#include "CMQTT.h"
#include "CopenHAB.h"

extern CMQTT mqtt;
  //PubSubClient client(ethClient);
  //IPAddress ip(192, 168, 1, 100);
  IPAddress server(192, 168, 1, 123);
    byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };


CMQTT::CMQTT()
{
  client.setClient(ethClient);
}

void CMQTT::begin(String idTopic,String u,String pwd)
{
  mojTopicIDSubscribe=idTopic+"/Sub/";
  mojTopicIDPublish=idTopic+"/Pub/";
  mojTopicIDPublishDebug="DebugTopic/"+idTopic;
  mojTopicIDPing=idTopic+"/Ping/";
  usr=u;
  passwd=pwd;
 
  client.setServer(mqtt_server, port);
  //  client.setServer(server,1883);
    client.setCallback(MQTTcallback);
    Ethernet.begin(mac);
    delay(1500);
    Serial.print(F("ETH ip:"));
    Serial.println(Ethernet.localIP());
    Serial.println(F("MQTT"));
    reconnect();
    lastReconnectAttempt = 0;
}
boolean CMQTT::reconnect() 
{
   if (!client.connected()) 
   {
      client.disconnect();
      Serial.print(F("Proba polaczenia z MQTT .."));
      if (client.connect(mojID,usr,passwd)) 
      {
        // Powitanie po połączeniu
        client.publish(mojTopicIDPing,"Polaczenie C2");
        // ... and resubscribe
        client.subscribe("C2");
    //    client.subscribe("C1Ping");
    //    client.subscribe("C1P");
 /*      for(byte i=0;i<ile_wyjsc;i++)
    {
      char top[10];
      sprintf(top,"%s_%s",mojTopicID,wyjNazwa[i]);
      client.subscribe(top);
    }*/
        probaReconnect=0;
      }else
      {
        Serial.print("nieudane: ");
        Serial.println(client.state());
      }
    }
  return client.connected();
}

void CMQTT::loop()
{
    popStatus=terazStatus;
    terazStatus=client.state();
    if(terazStatus!=popStatus)
    {
      Serial.print(F("Status MQTT: "));
      Serial.println(terazStatus);
    }
  
  if (!client.connected()) {
   unsigned long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
       ++probaReconnect;
      // Proba polaczenia
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected

    client.loop();
     if(millis()-lastPingMillis>60000)
    {
      lastPingMillis=millis();
      lp++;
      
      char tmp[50];
      unsigned long sec = millis() / 1000;
      unsigned long min = sec / 60;
      unsigned long hr = min / 60;
      unsigned long dd = hr /24;

      sec=sec%60;
      min=min%60;
      hr=hr%24;
      

          int fr=freeRam();
         Serial.print("Ram: "); Serial.print(fr);
         Serial.print(" mqttCzeka: ");Serial.print(isMsgAvailable());
            Serial.print(" mqttState: ");Serial.println(client.state());
      sprintf ( tmp,"%s: %lu= %02lud%02lu:%02lu:%02lu, freeRAM=%d",mojTopicID,lp,dd,hr, min,sec,fr);
      
      client.publish("C1Ping",tmp);
    }  
  }

}

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print(F("Topic:"));
  Serial.print(topic);

  Serial.print(F(" dlugosc= "));
  Serial.print(length);
  if(mqtt.isMsgAvailable())
  {
    Serial.println(F("Ignorowanie, czeka poprzedni msg"));  
    return;
  }
  Serial.print(F(" MQTT_tresc= "));
  char* msgMqtt=mqtt.getMsg();
  for(int i=0;i<length;i++)
  {
    msgMqtt[i]=(char)payload[i];
  }
  msgMqtt[length]='\0';
  Serial.println(msgMqtt);

  strcpy(mqtt.getTopic(),topic);
  
  if(strcmp(mojTopicIDPing,topic)==0) //powrocil komunikat ping
  {
    //ostatniPing=millis();
    if(led_status==0)led_status=1;else led_status=0;
    digitalWrite(WBUDOWANY_LED, led_status);  
    return;
  }

  mqtt.setCzekaMsg(true);
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

