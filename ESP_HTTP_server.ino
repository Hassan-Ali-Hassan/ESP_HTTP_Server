#include <ESP8266WiFi.h>
#include<ESP8266WebServer.h>

ESP8266WebServer server;
char* ssid = "AeroStaff-4";
char* password = "stewart2";
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

  /*adding routes to our webserver*/
  server.on("/test",echo);
  server.on("/test2",echo2);
  server.begin();
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
  server.send(200,"text/plain","hello man!!"+String(counter)); //this is the response the clients should receive when they send this request
//  Serial.println("I have received a message from somewhere");
}
