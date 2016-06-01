#include <za_serial.h>

z_port zaber1;
string command;
za_reply reply;

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
      Serial.print(reply.message_type);
      Serial.print(reply.device_address);
      Serial.print(" ");
      Serial.print(reply.axis_number);
      Serial.print(" ");
      Serial.print(reply.reply_flags);
      Serial.print(" ");
      Serial.print(reply.device_status);
      Serial.print(" ");
      Serial.print(reply.warning_flags);
      Serial.print(" ");
      Serial.println(reply.response_data);
    }
}