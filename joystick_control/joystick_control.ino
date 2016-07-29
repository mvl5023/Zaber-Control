#include <SoftwareSerial.h>

int vrX;
int vrY;

int veloX;
int veloY;

int pinX = 0;
int pinY = 1;

int RXpin = 4;
int TXpin = 5;
int tactPin = 2;

int axisX = 1;
int axisY = 2;

String moveVelX = "/1 move vel ";
String moveVelY = "/2 move vel ";
String comm;

unsigned long speedFactor = 50000;

// Create a software serial port to communicate with the Zaber stages
SoftwareSerial rs232(rsRX, rsTX);   

void setup()
{
  rs232.begin();
  delay(1000);
  pinMode(tactPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(tactPin), zStop, falling);
  rs232.println("/renumber");
  delay(500);
  rs232.println("/home");
  delay(6000);
}

void loop() 
{
  vrX = analogRead(pinX);
  vrY = analogRead(pinY);
  veloX = map(vrX, 0, 1023, -10, 10) * speedFactor;
  veloY = map(vrY, 0, 1023, -10, 10) * speedFactor;
  comm = moveVelX + veloX;
  rs232.println(comm);
  comm = moveVelY + veloY;
  rs232.println(comm);
  delay(200);  
}

void zStop()
{
  rs232.println("/stop");
}

