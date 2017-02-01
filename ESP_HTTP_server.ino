#include <ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include <ESP8266mDNS.h> //this is used to assign to the module a distinct web address, rather than use its ip address for making requests

ESP8266WebServer server(80);
MDNSResponder mdns;
char* ssid = "AeroStaff-4";
char* password = "stewart2";
int position[6] = {2,84,756,52,4,78}; //the array containing the x and y positions of all rover
int INDEX = 0; //which is the index of the robot itself, not that of the entry in the array
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
  server.on("/test",echo);
  server.on("/test2",echo2);
  server.on("/position",managePositions);
  server.begin();

  MDNS.addService("http","tcp",80);
}

void loop() 
{
  server.handleClient();
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
  String UP = server.arg("up"); //stands for update position

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
  const int bufferSize = 10;
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
  value[index] = container.toInt();
  position[2*(INDEX-1)] = value[0];
  position[2*(INDEX-1)+1] = value[1];
}

