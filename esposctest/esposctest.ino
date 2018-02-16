#include <OSCBundle.h>
#include <SLIPEncodedUSBSerial.h>
#include <SLIPEncodedSerial.h>
#include <OSCBoards.h>
#include <OSCData.h>
#include <OSCTiming.h>
#include <OSCMatch.h>
#include <OSCBoards.h>
#include <OSCMessage.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "wifiaccessdata.h"
int status = WL_IDLE_STATUS;     // the Wifi radio's status
WiFiUDP Udp;
OSCMessage msg("");
  
// hardware clock:
double rt = 0;
double lsend = 0;

void send_msg( OSCMessage& msg ) {
  IPAddress target(WiFi.localIP());
  for(uint8_t tip=50;tip<55;++tip){
    target[3] = tip;
    Udp.beginPacket(target,9800);
    //Udp.beginPacketMulticast(IPAddress(224,0,0,183),9877,WiFi.localIP(),1);
    msg.send(Udp);
    Udp.endPacket();
  }
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  WiFi.disconnect();
  delay(500);
  WiFi.begin(ssid, pass );
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  Serial.println();
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  Udp.begin(9999);
  msg.setAddress("/noemergency");  
}

void loop() {
  rt = 0.001*millis();
  if( (rt >= lsend+0.1) || (rt < lsend ) ){
    lsend = rt;
    msg.set(0,rt);
    send_msg(msg);
  }
}

/*
 * Local Variables:
 * mode: c++
 * c-basic-offset: 2
 * End:
 */
