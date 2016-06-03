void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);     //pin 18 = tx, pin 19 = rx on Mega
  delay(100);
  if(Serial1.available())
    {
      Serial.println("Serial data available");
    }
   else
    {
      Serial.println("Unable to read serial port");
    }
}

void loop() {
  // put your main code here, to run repeatedly:

}
