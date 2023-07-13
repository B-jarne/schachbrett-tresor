//Bibliotheken 

#include <ESP8266WiFi.h>
#include <WiFiClient.h>


//Pin welcher das HIGH / LOW Signal vom Arduino Mega annimmt
#define pin D8

//Daten zum verbinden mit dem WiFi
const char* ssid = "";
const char* password = "";

//WiFiClient um zwischen den D1-Mini's Daten zu schicken
WiFiClient client;

void setup() {
  pinMode(pin,INPUT);
  Serial.begin(9600);

  //Starten der WiFi Verbindung
  WiFi.begin(ssid, password);


  //Warten bis die Wifi Verbindung hergestellt wurde
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zum WLAN herstellen...");
  }
  Serial.println("WLAN verbunden!");

  //Verbinden mit dem D1-Mini vom Tresor
  while(!client.connected()) {
    client.connect("IP_VOM_TRESOR",420);
    delay(1000);
    Serial.println("Verbinde mit Tresor...");
  }
  Serial.println("Mit Tresor verbunden.");
}

void loop() {
  if (client.connected()) {
    //Auslesen vom pin wenn HIGH -> senden einer 1 an den D1-Mini vom Tresor
    if(digitalRead(pin) == HIGH) {
      Serial.println("OPEN");
      client.println("1");
    }
    //Auslesen vom pin wenn LOW -> senden einer 0 an den D1-Mini vom Tresor
    else {
      Serial.println("CLOSE");
      client.println("0");
    }
    delay(50);
    }
}
