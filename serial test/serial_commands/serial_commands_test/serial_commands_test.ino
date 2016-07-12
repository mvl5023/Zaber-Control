//#include <SoftwareSerial.h>

struct reply {
  String fullResponse;
  bool isReply;
  int deviceAddress;
  int axisNumber;
  bool isRejected;
  bool isBusy;
  bool hasWarning;
  String warningFlag;
  String responseDataString;
  int responseData;
};
 
// Define device number 
String deviceNum = "1";
 
// Define resolution (0.000047625 for X-LRM200A-E03)
float mmResolution = 0.000047625; 

// Define common command numbers
String Home = "home";
String MoveAbs = "move abs";
String MoveRel = "move rel";
String Stop = "stop";
String SetMaxspeed = "set maxspeed";
String GetPos = "get pos";

//On Mega, RX must be one of the following: pin 10-15, 50-53, A8-A15
int RXpin = 11;
int TXpin = 4;

//SoftwareSerial rs232(RXpin, TXpin);   //RX, TX
 
void setup()
{
  Serial.begin(115200);
}

void loop()
{
  delay(1000);
  Serial.println("/renumber");
  pollUntilIdle(deviceNum);
  //requestCommand(deviceNum, Home, " ");
  Serial.println("/home");
  pollUntilIdle(deviceNum);
  
  requestCommand(deviceNum, SetMaxspeed, "50000"); 
  requestCommand("2", SetMaxspeed, "50000"); 
 
  while (true)
  {
    requestCommand(deviceNum, MoveAbs, mm(20));
    pollUntilIdle(deviceNum);
    delay(1000);  

    requestCommand(deviceNum, MoveAbs, mm(10));
    pollUntilIdle(deviceNum); 
    delay(1000);
      
  }
}
 
// Converts mm values to data values
String mm(float mmValue)
{
  long dataValue;
  dataValue = mmValue / mmResolution;
  String dataR = String(dataValue);
  return dataR;
} 
 
struct reply requestCommand(String device, String command, String data)
{
  Serial.println("/" + device + " " + command + " " + data);
  
  String fullResponse = Serial.readStringUntil('\n');
  
  String messageType = fullResponse.substring(0,1);
  String nn = fullResponse.substring(1,3);
  String a = fullResponse.substring(4,5);
  String fl = fullResponse.substring(6,8);
  String bbbb = fullResponse.substring(9,13);
  String ww = fullResponse.substring(14,16);
  String xxx = fullResponse.substring(17,fullResponse.length()-1);
    
  bool isReply = messageType.equals("@");
  int deviceAddress = nn.toInt();
  int axisNumber = a.toInt();
  bool isRejected = fl.equals("RJ");
  bool isBusy = bbbb.equals("BUSY");
  bool hasWarning = !(ww.equals("--"));
  int responseData = xxx.toInt();
  
  reply response = {fullResponse, isReply, deviceAddress, axisNumber, isRejected, isBusy, hasWarning, ww, xxx, responseData  };
  return response;
}

void pollUntilIdle(String device)
{
  while(requestCommand(device, "", "").isBusy);
  {
    delay(10);
  }
}
