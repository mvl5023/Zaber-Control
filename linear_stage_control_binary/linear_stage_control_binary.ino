/*   Test sketch for linear stage - binary protocol
 *      
 *   Michael Lipski
 *   AOPL
 *   Summer 2016
 *   
 *   Allows user to input a displacement in mm and sends an absolute move command to the Zaber X-LRM200A linear stages.  Retrieves reply data
 *   and prints it to the serial monitor.
 */

#include <zaberx.h>

#include <SoftwareSerial.h>

//    Zaber rotational stage variables
byte command[6];
byte reply[6];
byte buff[6];
byte dumper;

int axisX = 1;    // Device ID of azimuth stage
int axisY = 1;     // Device ID of elevation stage

long posX = 0;   // Variable which tracks the absolute position of the azimuth stage (in microsteps)
long posY = 0;   // Variable which tracks the absolute position of the elevation stage (in microsteps)

int homer = 1;      // home the stage
int renumber = 2;   // renumber all devices in the chain
int moveAbs = 20;   // move absolute
int moveRel = 21;   // move relative
int stopMove = 23;  // Stop
int speedSet = 42;    // Speed to target = 0.00219727(V) degrees/sec (assuming 64 microstep resolution)
int getPos = 60;      // Query the device for its position
int storePos = 16;    // Position can be stored in registers 0 to 15
int returnPos = 17;   // returns the value (in microsteps) of the position stored in the indicated register
int move2Pos = 18;    // move to the position stored in the indicated register
int reset = 0;        // akin to toggling device power

float outData;
long replyData;
String comm;

//On Mega, RX must be one of the following: pin 10-15, 50-53, A8-A15
int RXPin = 2;
int TXPin = 3;

SoftwareSerial rs232(RXPin, TXPin);   //RX, TX

void setup() 
{
  Serial.begin(9600);
  rs232.begin(115200);
  delay(1000);
  rs232.println("/tools setcomm 9600 1");
  delay(500);
  Serial.println(rs232.readStringUntil('\n'));
  delay(100);
  rs232.end();
  delay(200);
  rs232.begin(9600);
  delay(500);

  Serial.println("Zaber linear stage movement test sketch using binary protocol");
  Serial.println("Enter absolute displacement in mm");
}

void loop()
{
  if(Serial.available() > 0)
  {
    comm = Serial.readStringUntil('\n');
    if(comm == "getpos")
    {
      posX = sendCommand(axisX, getPos, 0);
    }
    else if(comm == "read")
    {
      rs232.readBytes(buff, 6);
      Serial.print(buff[0]);
      Serial.print(' ');
      Serial.print(buff[1]);
      Serial.print(' ');
      Serial.print(buff[2]);
      Serial.print(' ');
      Serial.print(buff[3]);
      Serial.print(' ');
      Serial.print(buff[4]);
      Serial.print(' ');
      Serial.println(buff[5]);
    }
    else
    {
      outData = comm.toFloat();
      replyData = sendCommand(axisX, moveAbs, mm(outData));
    }
    delay(1000);
  }
}

long sendCommand(int device, int com, long data)
{
   unsigned long data2;
   unsigned long temp;
   unsigned long repData;
   long replyNeg;
   
   // Building the six command bytes
   command[0] = byte(device);
   command[1] = byte(com);
   if(data < 0)
   {
     data2 = data + quad;
   }
   else
   {
     data2 = data;
   }
   temp = data2 / cubed;
   command[5] = byte(temp);
   data2 -= (cubed * temp);
   temp = data2 / squared;
   command[4] = byte(temp);
   data2 -= (squared * temp);
   temp = data2 / 256;
   command[3] = byte(temp);
   data2 -= (256 * data2);
   command[2] = byte(data2);

   while(rs232.available() > 0)
   {
     rs232.readBytes(dumper, 1);
   }
   // Sending command to stage(s)
   rs232.write(command, 6);

   delay(20);
   
   // Reading device reply
   if(rs232.available() > 0)
   {
     rs232.readBytes(reply, 6);
   }

   repData = (cubed * reply[5]) + (squared * reply[4]) + (256 * reply[3]) + reply[2];
   if(reply[5] > 127)
   {
     replyNeg = repData - quad;
   }
   
   // Printing full reply bytes as well as reply data in decimal 
   Serial.print(reply[0]);
   Serial.print(' ');
   Serial.print(reply[1]);
   Serial.print(' ');
   Serial.print(reply[2]);
   Serial.print(' ');
   Serial.print(reply[3]);
   Serial.print(' ');
   Serial.print(reply[4]);
   Serial.print(' ');
   Serial.println(reply[5]);
   Serial.print("\tData:");
   if(reply[5] > 127)
   {
     Serial.println(replyNeg);
     return replyNeg;
   }
   else
   {
     Serial.println(repData);  
     return repData;
   }    
}


