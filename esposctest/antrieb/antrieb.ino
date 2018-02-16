#include <OSCBundle.h>
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
OSCMessage msg_status("");

double ctime = 0;
double ltime = 0;

void send_msg( OSCMessage& msg ) {
  Udp.beginPacket("192.168.188.38",9877);
  msg.send(Udp);
  Udp.endPacket();
}

void send_status( char* text, float v=0 ) {
  msg_status.set(0, text );
  msg_status.set(1, v );
  send_msg( msg_status );
}

void setup() {
  pinMode(15, INPUT);
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
  String path = "/" + WiFi.hostname();
  msg.setAddress((path+"/antrieb").c_str());
  msg_status.setAddress((path+"/status").c_str());
  msg_status.add("ok");
  msg_status.add(0.0f);
  send_status("online");
}

void loop() {
  if( digitalRead(15) ){
    ctime = millis();
    msg.set(0,12750.0/(ctime-ltime));
    ltime = ctime;
    send_msg(msg);
    while( digitalRead(15) )
      delay(10);
  }
}

/*
 * Local Variables:
 * mode: c++
 * c-basic-offset: 2
 * End:
 */
