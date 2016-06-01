#include <za_serial.h>

z_port zaber1;
string command;
string reply;

void setup() {
  Serial.begin(9600);
  //za_connect(*zaber1, *COM1);     determine COM port of zaber
  Serial.println("Enter commands");
}

void loop() {
  if(Serial.available() > 0)
    {
      command = Serial.read();
      za_send(zaber1, command);
      za_receive(zaber1, reply);
      Serial.println(reply);
    }
}
