#ifndef CMQTT_h
#define CMQTT_h 
#include <Ethernet.h>
#include <PubSubClient.h>

#define WBUDOWANY_LED 13

  void MQTTcallback(char* topic, byte* payload, unsigned int length);
  int freeRam();
 


static  byte led_status=0;

class CMQTT
{
  String mqtt_server = "m20.cloudmqtt.com";
  uint16_t port=11379;
  String usr="";
  String passwd="";
  //static const char* mojID="C1mega"; 
  String mojTopicIDSubscribe="C1/Sub/"; //topic z komunikatami z serwera
  String mojTopicIDPublish="C1/Pub/"; // topic z komunikatami do serwera
  String mojTopicIDPublishDebug="DebugTopic/C1/"; // topic z komunikatami do serwera
  String mojTopicIDPing="C1/Ping/"; //topic z komukatami keepalive

  long lastReconnectAttempt = 0;
  int probaReconnect=0;


  // Update these with values suitable for your hardware/network.
  byte popStatus=-9;
  byte terazStatus=-8;
  unsigned long lastPingMillis=0;
  unsigned long lp=0;

  char msg[128];
  char msgTopic[20];
  bool czekaMsg=false;

  EthernetClient ethClient;
  PubSubClient client;
  
  public:
  CMQTT();
  void begin(String idTopic,String u,String pwd);
  bool reconnect();
  void loop();
  bool publikuj(char* topic,char* msg){return client.publish(topic,msg);}
  bool isMsgAvailable(){return czekaMsg;}
  char* getMsg(){return msg;}
  char* getTopic(){return msgTopic;}
  void dawajMsg(char *t,char* m){strcpy(t,msgTopic);strcpy(m,msg);czekaMsg=false;return;}
  void setCzekaMsg(bool czeka){czekaMsg=czeka;}
  byte dajStatusPolaczenia(){return client.state();}
};

#endif
