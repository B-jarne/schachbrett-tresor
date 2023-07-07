//Anzahl der Pins
#define COUNTOFPINS 16

//Sensor Pins A0-15 die ausgelesen werden sollen
int sensorPins[COUNTOFPINS] = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15};
//Digital Pins die jeweils an/aus geschaltet werden um Analoge Pins auszulesen
int digitalPins[COUNTOFPINS] = {24,26,28,30,32,34,36,38,39,41,43,45,47,49,51,53};

//Abspeichern des Pattern
int sensorValues[COUNTOFPINS];

//Vorgegebenes Pattern was übereinstimmen muss
int PATTERN[COUNTOFPINS] = {1,-1,0,-1,0,-1,1,0,-1,1,0,1,0,1,-1,0};

//Digitaler Ausgang um an den D1-Mini zu übermitteln ob das Pattern stimmt
int outputPin = 31;

//Aktueller Sensor Wert(aktualisiert sich bei jedem Durchlauf in der Schleife)
int sensorValue = 0;

void setup () {
 Serial.begin (9600);

 //Initialisierung der digitalPins
 for(int pin : digitalPins) {
  pinMode(pin,OUTPUT);
 }
 //Initialisieren des Ausgangs Pins für den D1-Mini
 pinMode(outputPin, OUTPUT);
}
void loop () {
  //Einlesen des Patterns
  readPattern();
  delay(1000);
  Serial.println(isPatternCorrect());
  //Wenn Pattern Korrekt -> HIGH Signal an den D1-Mini übermitteln
  //sonst -> LOW Signal übermitteln 
  if(isPatternCorrect()) {
    digitalWrite(outputPin, HIGH);
    }
  else {
    digitalWrite(outputPin, LOW);
  }
  delay(50);
}

//Methode zum auslesen vom Pattern
void readPattern() {
  for(int i = 0; i < COUNTOFPINS; i++) {
    //Digitaler Pin wird auf HIGH gesetzt um das analoge Signal zu lesen
    digitalWrite(digitalPins[i],HIGH);
    delay(50);

    //Analoges Signal wird gelesen
    sensorValue = analogRead(sensorPins[i]);
    Serial.println(sensorValue);
    
    if(sensorValue < 480) {
      sensorValues[i] = -1;
    }
    else if(sensorValue > 535) {
      sensorValues[i] = 1;
      }
    else {
      sensorValues[i] = 0;
   }
   //Digitalen Pin wieder auf LOW schalten
   digitalWrite(digitalPins[i],LOW);
  }
  Serial.println("Values: ");
  for(int value : sensorValues) {
    Serial.print(value);
    Serial.print(", ");
    }
   Serial.println();
}

// Überprüft ob das Pattern korrekt ist
bool isPatternCorrect() {
  for(int i = 0; i < COUNTOFPINS; i++) {
    if(PATTERN[i] != sensorValues[i]) {
      return false;
    }
  }
  return true;
}
