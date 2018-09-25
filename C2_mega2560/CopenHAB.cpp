/*
#include "CopenHAB.h"

 byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
  IPAddress ip(192, 168, 1, 111);
  IPAddress server(54, 170, 168, 125);
 // EthernetClient ethClient;
 // PubSubClient client(server, 11379, callback, ethClient);


// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  // In order to republish this payload, a copy must be made
  // as the orignal payload buffer will be overwritten whilst
  // constructing the PUBLISH packet.

  // Allocate the correct amount of memory for the payload copy
  byte* p = (byte*)malloc(length);
  // Copy the payload to the new buffer
  memcpy(p,payload,length);
//  client.publish("outTopic", p, length);
  // Free the memory
  free(p);
}

CopenHAB::CopenHAB()
{
 //  Ethernet.begin(mac, ip);
 // if (client.connect("ardCli","juxnbhef","aYqhqc3-p5m4")) {
 //   client.publish("outTopic","a");
  //  client.subscribe("inTopic");
    //char t[6];
    //int l=Cx*1000+A0;
    //itoa(l,t,10);
    //Serial.println(t);
  //  client.publish("1054",0);
  //}
 }
 void CopenHAB::loop()
 {
//  client.loop();
 }
 void CopenHAB::publishMQTT(char * t, byte stan)
 {
  
 }*/
