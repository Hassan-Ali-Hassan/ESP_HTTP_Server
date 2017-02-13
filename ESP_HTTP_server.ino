#include <ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include <ESP8266mDNS.h> //this is used to assign to the module a distinct web address, rather than use its ip address for making requests

ESP8266WebServer server(80);
MDNSResponder mdns;
char* ssid = "AeroStaff-4";
char* password = "stewart2";
String flag = "0"; //an variable to give order to move or stop
//int position[6] = {2,84,756,52,4,78}; //the array containing the x and y positions of all rover
int position[6] = {0,0,120,275,0,0};
int INDEX = 0; //which is the index of the robot itself, not that of the entry in the array
float t = 0;
float oldTime = 0;
void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("IP Address:  ");
  Serial.println(WiFi.localIP());
  if(mdns.begin("Myesp8266"/*instead of using .com as a domain name, the default will be .local*/,WiFi.localIP()))
  {
    Serial.print("mdns responder has started");
  }
  /*adding routes to our webserver*/
//  server.on("/test",echo);
//  server.on("/test2",echo2);
  server.on("/pos1",pos1Fun);
  server.on("/pos2",pos2Fun);
  server.on("/pos3",pos3Fun);
  server.on("/position",managePositions);
  server.begin();

  MDNS.addService("http","tcp",80);
}

void loop() 
{
  server.handleClient();
  t = (float)millis()/1000.0;
  if(t-oldTime > 0.1)
  {
    for(int j = 0; j < 6; j++)
    {
      Serial.print(position[j]);
      Serial.print("\t");
    }
    Serial.println(t);
    oldTime = t;
  }
}

void pos1Fun()
{
  String response = "#"+flag+"$"; 
  response += String(position[2])+","+String(position[3])+"#"; //reporting position of agent 2
  INDEX = 1;
  String UP = server.arg("up");
  if(UP.length() > 0)
  {
    updatePosition(UP);
  }
  server.send(200,"text/plain",response); 
}

void pos2Fun()
{
  String response = "#"+flag+"$";
  INDEX = 2;
  response += String(position[0])+","+String(position[1])+"#"; //reporting position of agent 1
  response += String(position[4])+","+String(position[5])+"#"; //reporting posititonof agent 3
  String UP = server.arg("up");
  if(UP.length() > 0)
  {
    updatePosition(UP);
  }
  server.send(200,"text/plain",response); 
}

void pos3Fun()
{
  String response = "#"+flag+"$";
  INDEX = 3;
  response += String(position[2])+","+String(position[3])+"#"; //reporting position of agent 2
  String UP = server.arg("up");
  if(UP.length() > 0)
  {
    updatePosition(UP);
  }
  server.send(200,"text/plain",response); 
}

void echo()
{
  static int counter = 0;
  counter++;
  server.send(200,"text/plain","hello man!!"+String(counter)); //this is the response the clients should receive when they send this request
//  Serial.println("I have received a message from somewhere");
}

void echo2()
{
  static int counter = 0;
  counter++;
  String n = server.arg("p1");
  String m = server.arg("p2");
  Serial.print(m.length());
  Serial.print("\t");
  Serial.println(n);
  server.send(200,"text/plain","hello man!!"+String(counter)); //this is the response the clients should receive when they send this request
//  Serial.println("I have received a message from somewhere");
}

void managePositions()
{
  static int counter = 0;
  counter++;
  String response = "#";
  String p1 = server.arg("p1");
  String p2 = server.arg("p2");
  String p3 = server.arg("p3");
  String I = server.arg("index");
  String status = server.arg("status");
  String UP = server.arg("up"); //stands for update position

  if(status == "1" || status == "0") //if you receive one of these characters, change the flag value
  {
    flag = status;
    response += flag+"$";
  }
  else //the slave is inquiring the flag status, just send flag value to it
  {
    response += flag+"$";
  }
  if(p1.length() > 0)
  {
    response += String(position[0])+","+String(position[1])+"#";
  }
  if(p2.length() > 0)
  {
    response += String(position[2])+","+String(position[3])+"#";
  }
  if(p3.length() > 0)
  {
    response += String(position[4])+","+String(position[5])+"#";
  }
  if(I.length() > 0)
  {
    INDEX = I.toInt();
  }
  if(UP.length() > 0)
  {
    updatePosition(UP);
  }
  server.send(200,"text/plain",response); 
}

void updatePosition(String msg)
{
  const int bufferSize = 20;
  int i = 0;
  int index = 0;
  int L = msg.length();
  int value[2] = {0,0};
  String container;
  char buffer[bufferSize];
  msg.toCharArray(buffer,bufferSize);
  for(i = 0; i < L; i++)
  {
    if(buffer[i] != ',')
    {
      container += buffer[i];
    }
    else
    {
      value[index] = container.toInt();
      container = "";
      index++;
    }
  }
  value[index] = container.toInt(); //because at the end of the string there is no ','
  position[2*(INDEX-1)] = value[0];
  position[2*(INDEX-1)+1] = value[1];
}

