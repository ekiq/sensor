#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define RE 23
#define DE 25

const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};

byte values[11];
SoftwareSerial mod(16, 17); // RX: Pin 16, TX: Pin 17
int RXPin = 4;
int TXPin = 15;
int GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(TXPin, RXPin);

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
}

byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(nitro,sizeof(nitro))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
      values[i] = mod.read();
      Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(phos,sizeof(phos))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
      values[i] = mod.read();
      Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}

byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(mod.write(pota,sizeof(pota))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
      values[i] = mod.read();
      Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayInfo();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println("No GPS detected");
    while (true);
  }
//
  byte val1, val2, val3;
  val1 = nitrogen();
  delay(250);
  val2 = phosphorous();
  delay(250);
  val3 = potassium();
  delay(250);

  Serial.print("Nitrogen: ");
  Serial.print(val1);
  Serial.println(" mg/kg");

  Serial.print("Phosphorous: ");
  Serial.print(val2);
  Serial.println(" mg/kg");

  Serial.print("Potassium: ");
  Serial.print(val3);
  Serial.println(" mg/kg");

  delay(1000);
}

void displayInfo() {
  if (gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  }
  else {
    Serial.println("Location: Not Available");
  }
}
