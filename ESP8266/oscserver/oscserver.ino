#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

#include "wifiaccessdata.h"

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

OSCErrorCode error;

int ledState = 0;

int tvorher = 0;

const unsigned int localPort = 9000;        // local port to listen for UDP packets (here's where we send the packets)


void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  WiFi.disconnect();
  delay(500);
  WiFi.begin( ssid, pass );
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  Serial.println();
  // you're connected now, so print out the data:
  Serial.println("Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Udp.begin(localPort);

}

void led(OSCMessage &msg) {
  ledState = msg.getInt(0);
  digitalWrite(BUILTIN_LED, ledState);
  Serial.print("/led: ");
  Serial.println(ledState);
}

void loop() {
  //kopiert aus OSC examples, liest OSC-Paket
  OSCMessage msg;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      msg.dispatch("/led", led);
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
  //auslesen des Entfernungsmessers
  int tjetzt = millis();
  int dauer = tjetzt-tvorher;
  if (dauer >= 100){
    int entf=analogRead(A0);
    tvorher = tjetzt;
    Serial.print(dauer);
    Serial.print(" ");
    Serial.println (entf);
    OSCMessage entfernung("/entfernung");
    entfernung.add(entf);
    entfernung.add(dauer);
    Udp.beginPacket("192.168.188.119", 9000);
    entfernung.send(Udp);
    Udp.endPacket();
    entfernung.empty();
  }
}
