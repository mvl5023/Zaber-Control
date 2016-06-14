#include <SoftwareSerial.h>

//On Mega, RX must be one of the following: pin 10-15, 50-53, A8-A15
int RXpin = 11;
int TXpin = 4;

SoftwareSerial rs232(RXpin, TXpin);   //RX, TX

void setup() {
  Serial.begin(9600);
  rs232.begin(115200);     //pin 4 = tx, pin 11 = rx 
  delay(5000);
  rs232.print("/home\n");
  if(rs232.available())
  {
    Serial.println("Serial data available");
  }
  else
  {
    Serial.println("Unable to connect");
  }
}

void loop() {
  delay(1000);
  rs232.write("/get pos\n");
  delay(100);
  Serial.println(rs232.readStringUntil('\n'));
}



