//Bibliotheken

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>

const char* ssid = "";
const char* password = "";

#define SERVO_PIN 2
#define LOCKED 180
#define UNLOCKED 90

//Aktueller Zustand von der Tresortür
bool door_status = 0;

//WifiServer auf dem Port 420
WiFiServer server(420);
Servo servo;

void setup() {
  Serial.begin(9600);

  //Starten der WiFi Verbindung
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zum WLAN herstellen...");
  }
  Serial.println(WiFi.localIP());
  Serial.println("WLAN verbunden!");

  //Starten vom Server zum verbinden vom Schachbrett
  server.begin();
  //Tür wird auf den geschlossenen Standardzustand gesetzt
  lockDoor();
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("Tresor verbunden!");
    
    while (client.connected()) {
      if (client.available()) {
        //Auslesen der Empfangenen Daten
        String data = client.readStringUntil('\r');
        //Leerzeichen entfernen
        data.trim();
        //Wenn man Daten empfangen hat -> bewegen der Tür
        if(data != "") {
          moveDoor(data);
        }
      }
    }
  }
}

void moveDoor(String data) {
  Serial.println(data);
  if(data == "1") {
    //Wenn die Tür schon im vorgegebenen Status ist -> nicht tun
    if(door_status == 1) {
      return;
    }
    //sonst -> bewegen
    else {
      Serial.println("OPEN");
      openDoor();
      //Aktuellen Zustand der Tresortür aktualisieren
      door_status = 1;
    }
  }

  else if(data == "0") {
    //Wenn die Tür schon im vorgegebenen Status ist -> nicht tun
    if(door_status == 0) {
      return;
    }
    else {
      Serial.println("CLOSE");
      lockDoor();
      //Aktuellen Zustand der Tresortür aktualisieren
      door_status = 0;
    }
  }
}

void lockDoor() {
  //Servo verbinden
  servo.attach(SERVO_PIN);
  //Servo bewegen
  servo.write(LOCKED);
  delay(1000);
  //Servo entfernen
  servo.detach();
}

void openDoor() {
  //Servo verbinden
  servo.attach(SERVO_PIN);
  //Servo bewegen
  servo.write(UNLOCKED);
  delay(1000);
  //Servo entfernen
  servo.detach();
}
