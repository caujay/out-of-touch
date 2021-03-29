#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

byte data[200] ={};
int packetsize = 0;
int arr[5];
String receiveddata="";
WiFiUDP Server;

void setup() {
pinMode(16, OUTPUT);
pinMode(15, OUTPUT);
pinMode(13, OUTPUT);
pinMode(12, OUTPUT);
pinMode(14, OUTPUT);


  
  Serial.begin(115200);
  WiFi.begin("Jest_moc", "olimp123");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Server.begin(3004);
  Serial.println("Server started...");
}

void loop() {
 
    char message = Server.parsePacket();
    packetsize = Server.available();
    if (message)
    {
     Server.read(data,packetsize);
     IPAddress remoteip=Server.remoteIP();
     //Server.write(data);
     Server.endPacket();
    }

    if(packetsize) {
      for (int i=0;packetsize > i ;i++)
      {
        //Serial.println(data[i]);
      }
    }

memcpy(arr, data, 20);
Serial.print(arr[0]);
Serial.print("\t"); 
Serial.print(arr[1]);
Serial.print("\t");
Serial.print(arr[2]);
Serial.print("\t");
Serial.print(arr[3]);
Serial.print("\t");
Serial.print(arr[4]);
Serial.println("");


if(arr[0]==2) {digitalWrite(14, HIGH);}
if(arr[0]==1) {digitalWrite(14, LOW);}
if(arr[0]==0) {digitalWrite(14, LOW);}

if(arr[1]==2) {digitalWrite(12, HIGH);}
if(arr[1]==1) {digitalWrite(12, LOW);}
if(arr[1]==0) {digitalWrite(12, LOW);}

if(arr[2]==2) {digitalWrite(13, HIGH);}
if(arr[2]==1) {digitalWrite(13, LOW);}
if(arr[2]==0) {digitalWrite(13, LOW);}

if(arr[3]==2) {digitalWrite(15, HIGH);}
if(arr[3]==1) {digitalWrite(15, LOW);}
if(arr[3]==0) {digitalWrite(15, LOW);}

if(arr[4]==2) {digitalWrite(16, HIGH);}
if(arr[4]==1) {digitalWrite(16, LOW);}
if(arr[4]==0) {digitalWrite(16, LOW);}

}
