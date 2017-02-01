#include <ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include <ESP8266mDNS.h> //this is used to assign to the module a distinct web address, rather than use its ip address for making requests

ESP8266WebServer server(80);
MDNSResponder mdns;
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
  if(mdns.begin("Myesp8266"/*instead of using .com as a domain name, the default will be .local*/,WiFi.localIP()))
  {
    Serial.print("mdns responder has started");
  }
  /*adding routes to our webserver*/
  server.on("/test",echo);
  server.on("/test2",echo2);
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
