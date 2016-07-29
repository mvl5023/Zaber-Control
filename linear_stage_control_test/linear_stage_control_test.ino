/*   Test sketch for linear stage
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

// Define common command numbers
String axisX = "/1 ";
String axisY = "/2 ";
String Home = "home ";
String moveAbs = "move abs ";
String moveRel = "move rel ";
String Stop = "stop ";
String SetMaxspeed = "set maxspeed ";
String GetPos = "get pos ";
String renum = "renumber ";
String comm;
String reply;


long posX = 0;
long posY = 0;

long outData;
long replyData;

int RXPin = 2;
int TXPin = 3;

SoftwareSerial rs232(RXPin, TXPin);

void setup() 
{
  Serial.begin(9600);
  rs232.begin(115200);
  delay(500);
  
  Serial.println("Zaber X-LRM200A movement test sketch");
  rs232.println("/renumber");
  delay(100);
  getReply();
  delay(100);
  sendCommand(axisX, SetMaxspeed, 200000);
  getReply();
  delay(100);
    
  Serial.println("Enter displacement from home in mm:");
}

void loop() 
{
  if(Serial.available() > 0)
  {
    comm = Serial.readStringUntil('\n');
    outData = mm(comm.toInt());
    sendCommand(axisX, moveAbs, outData);    
    getReply();
    delay(500);
  }
  rs232.println("/get pos");
  delay(500);
  getReply();
  delay(3000);
}

void sendCommand(String axis, String command, long data)
{
  String outString;  
  
  outString = axis + command;
  outString += data;
  rs232.println(outString);
  delay(100);
}

void getReply()
{    
  String sub;
  
  reply = rs232.readStringUntil('\n');
  sub = reply.substring(17, reply.length() - 1);
  replyData = sub.toInt();
  Serial.println(reply);
  Serial.println(replyData);
}

