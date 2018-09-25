#ifndef CopenHAB_h
#define CopenHAB_h 

//#include <SPI.h>
//#include <Ethernet.h>
//#include <UIPEthernet.h> // Used for Ethernet
//#include <PubSubClient.h>


#define Cx 2

//////////////////s14 to ledy w lazience na dole nie zaznaczone na projekcie
                                   ///    wyjsciaPWM              wyj 12V                                                 wyj 220V
//const char wyjPIN_OPIS[] PROGMEM = {"PWM1","PWM2","PWM3","OUT1","OUT2","OUT3","OUT4","OUT_220_1","OUT_220_2","OUT_220_3","OUT_220_4","OUT_220_5","OUT_220_6","OUT_220_7","OUT_220_8"};
enum wyjPIN_TXT                    { s1,   s2,   s3,   s4,   s5,  s6,  s7,   s8,  s9,  s10,   s11,  s12,    s13,  s14,   s15,    s16,  s17,  s18 };
//enum wyjPIN_ENUM                    {PWM1   ,PWM2,   PWM3, OUT1,   OUT2,   OUT3,  OUT4, OUT_220_1,OUT_220_2, OUT_220_3,  OUT_220_4,  OUT_220_5,  OUT_220_6,  OUT_220_7,  OUT_220_8};
                                      /*10 to cs eth, pin=4 to od SD nie uzywac z w5100*/    
const byte wyjPIN_pin[] PROGMEM   ={ 30,   31,   2,    32,   33,  34,  35,   3,   13,     6,   36,    7,     8,      9,    11,    12,     45,  44 };
static const  char*  wyjItem[]    ={"s1", "s2", "s3", "s4", "s5","s6","s7", "s8","s9", "s10", "s11","s12", "s13", "s14", "s15", "s16", "s17","s18"};
const bool wyjPWM[]               ={  0,    0,    1,    0,    0,   0,   0,    1,  1,     1,     0,    1,    1,      1,    1,      1,      1,   1};
const byte ile_wyjsc=18;

//const char wejPIN_OPIS[] PROGMEM = {"INP1","INP2","INP3","INP4","INP5","INP6","INP7","INP8","INP9","INP10","INP13","INP14","INP15","INP16"};//"INP11","INP12" -Serial
enum wejPIN_TXT                    {  w1  ,    w2 ,   w3  ,  w4,     w5  ,  w6,      w7  ,  w8,  w9,   w10,  w11,  w12,  w13,  w14,  p1,  p2,  p3};
//enum wejPIN_ENUM                 {  INP1, INP2  ,INP3 ,   INP4 , INP5 , INP6 ,   INP7 , INP8 ,  INP9 ,   INP10 ,INP11,  INP12, INP13 , INP14 , INP15 , INP16 };//"INP11","INP12"
const byte wejPIN_pin[] PROGMEM =  {  A0,     A1,     A2,    A3,     25,    A5,      A4,    A8,  A9,   A10,  A11,  A12,  A13,  A14,  22,  23,  24};   //  0,      1
static const  char* wejItem[]  =   {  "w1",   "w2",  "w3",  "w4",   "w5",  "w6" ,   "w7",  "w8","w9", "w10","w11","w12","w13","w14","p1","p2","p3"};
                                      /// brakuje 2=1wire,4,5, 10, 12
const byte ile_wejsc=16;
// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

class CopenHAB
{
  // Update these with values suitable for your network.
 

  public:
   CopenHAB();
   void loop();
   void publishMQTT(char *t, byte stan);
};

#endif
