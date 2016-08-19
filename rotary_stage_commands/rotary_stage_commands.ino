/*   Testing movement commands of Zaber T-series rotational stages
 *      
 *   Michael Lipski
 *   AOPL
 *   Summer 2016
 *   
 *   Used to test Arduino control of Zaber T-series rotational stages.  Includes function sendCommand, which is used in pyranometer tracker sketches.
 */

#include <SoftwareSerial.h>

#include <zaberx.h>


byte command[6];
byte reply[6];

int homer = 1;
int renumber = 2;
int moveAbs = 20;
int moveRel = 21;
int stopMove = 23;
int speedSet = 42;    // Speed to target = 0.00219727(V) degrees/sec (assuming 64 microstep resolution)
int getPos = 60;
int storePos = 16;    // Position can be stored in registers 0 to 15
int returnPos = 17;
int move2Pos = 18;
int reset = 0;

//On Mega, RX must be one of the following: pin 10-15, 50-53, A8-A15
int RXpin = 3;
int TXpin = 4;

SoftwareSerial rs232(RXpin, TXpin);   //RX, TX

void setup() 
{
  Serial.begin(9600);
  rs232.begin(9600);
  delay(2000);
  sendCommand(0, renumber, 0);
  delay(1000);
  sendCommand(0, speedSet, 4551);   // Set speed to 10 degrees/sec
  delay(1000);
  sendCommand(0, homer, 0);
  delay(5000);
}

void loop() 
{
  sendCommand(0, moveAbs, stepsD(10));
  delay(2500);
  sendCommand(0, moveAbs, stepsD(20));
  delay(2500);
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

   /*
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
   */
   if(reply[5] > 127)
   {
     //Serial.println(replyNeg);
     return replyNeg;
   }
   else
   {
     //Serial.println(repData);  
     return repData;
   }    
}

