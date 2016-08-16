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

int axisX = 1;    // Device ID of X-axis stage
int axisY = 2;    // Device ID of Y-axis stage

unsigned long posX = 0;   // Variable which tracks the absolute position of the X-axis stage (in microsteps)
unsigned long posY = 0;   // Variable which tracks the absolute position of the Y-axis stage (in microsteps)

int homer = 1;      // home the stage
int renumber = 2;   // renumber all devices in the chain
int moveAbs = 20;   // move absolute
int moveRel = 21;   // move relative
int stopMove = 23;  // Stop
int speedSet = 42;    // Speed to target = 0.00002906799 * (data) mm/s
int getPos = 60;      // Query the device for its position
int storePos = 16;    // Position can be stored in registers 0 to 15
int returnPos = 17;   // returns the value (in microsteps) of the position stored in the indicated register
int move2Pos = 18;    // move to the position stored in the indicated register
int reset = 0;        // akin to toggling device power

float outData1;
float outData2;
float replyFloat;
long replyData;
String comm1;
String comm2;

//  On Mega, RX must be one of the following: pin 10-15, 50-53, A8-A15
int RXPin = 2;
int TXPin = 3;

SoftwareSerial rs232(RXPin, TXPin);   //RX, TX

void setup() 
{
  Serial.begin(9600);

  // Sets the stages to use binary protocol
  rs232.begin(115200);
  delay(1000);  
  rs232.println("/tools setcomm 9600 1");
  delay(500);
  Serial.println(rs232.readStringUntil('\n'));
  delay(100);
  rs232.end();
  delay(200);
  

  /*
  // Sets the stages to use ASCII protocol
  rs232.begin(9600);
  delay(1000);
  replyData = sendCommand(1, 124, 115200);
  delay(100);
  rs232.end();
  */
  
  rs232.begin(9600);
  delay(500);
  replyData = sendCommand(0, 42, 172000);       // Set speed to 5 mm/s

  Serial.println("Zaber linear stage movement test sketch using binary protocol");
  Serial.println("Enter absolute displacement in mm for X and Y, separated by a space:");
}

void loop()
{
  if(Serial.available() > 0)
  {
    comm1 = Serial.readStringUntil(' ');
    comm2 = Serial.readStringUntil('\n');
    if((comm1 == "getpos") || (comm2 == "getpos"))
    {
      /*
      replyData = sendCommand(axisX, getPos, 0);
      replyFloat = (pow(256, 3) * float(reply[5])) + (pow(256, 2) * float(reply[4])) + (256 * float(reply[3])) + float(reply[2]);
      posX = long(replyFloat);
      Serial.println(posX);
      replyData = sendCommand(axisY, getPos, 0);
      replyFloat = (pow(256, 3) * float(reply[5])) + (pow(256, 2) * float(reply[4])) + (256 * float(reply[3])) + float(reply[2]);
      posY = long(replyFloat);
      Serial.println(posY);
      */

      posX = sendCommand(axisX, getPos, 0);
      posY = sendCommand(axisY, getPos, 0);
    }
    else
    {
      outData1 = comm1.toInt();
      outData2 = comm2.toInt();
      replyData = sendCommand(axisX, moveAbs, outData1);
      replyData = sendCommand(axisY, moveAbs, outData2);
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
   float replyFloat;
   byte dumper[1];
   
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
   
   // Clearing serial buffer
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

   replyFloat = (cubed * float(reply[5])) + (squared * float(reply[4])) + (256 * float(reply[3])) + float(reply[2]); 
   repData = long(replyFloat);
      
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
