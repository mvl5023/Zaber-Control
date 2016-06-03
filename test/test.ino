#include <za_serial.h>

z_port zaber1;
//za_reply reply;

void setup() {
  Serial.begin(9600);  
  za_connect(zaber1, COM3);     //determine COM port of zaber
  Serial.println("Enter commands");
}

void loop() {
  if(Serial.available() > 0)
    {
      za_send(zaber1, Serial.read());
      delay(100);
      /*za_receive(zaber1, reply);
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
      Serial.println(reply.response_data);*/
    }
}
