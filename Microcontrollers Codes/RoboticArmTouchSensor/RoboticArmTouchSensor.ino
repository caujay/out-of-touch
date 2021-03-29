#include <WiFi.h>
#include <WiFiUDP.h>

int p[5]; //palce
int s[5]; //silniki

WiFiUDP Client;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin("Jest_moc","olimp123");
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
Serial.println("WiFi connected");
//Client.begin(81);

}

void loop()
{

  p[0]=touchRead(T3);
  p[1]=touchRead(T5);
  p[2]=touchRead(T0);
  p[3]=touchRead(T6);
  p[4]=touchRead(T7);

//kciuk
  if(p[0]>=16 || p[0]<=6){s[0]=0;}
  if(p[0]>=12 && p[0]<=15){s[0]=1;}
  if(p[0]>=7 && p[0]<=11){s[0]=2;}

//palec wskazujący
  if(p[1]>=19 || p[1]<=6){s[1]=0;}
  if(p[1]>=14 && p[1]<=18){s[1]=1;}
  if(p[1]>=7 && p[1]<=13){s[1]=2;}

//palec środkowy
  if(p[2]>=16 || p[2]<=7){s[2]=0;}
  if(p[2]>=12 && p[2]<=15){s[2]=1;}
  if(p[2]>=8 && p[2]<=11){s[2]=2;}

//palec serdeczny
  if(p[3]>=16 || p[3]<=7){s[3]=0;}
  if(p[3]>=12 && p[3]<=15){s[3]=1;}
  if(p[3]>=8 && p[3]<=11){s[3]=2;}

//palec mały
  if(p[4]>=18 || p[4]<=7){s[4]=0;}
  if(p[4]>=16 && p[4]<=17){s[4]=1;}
  if(p[4]>=8 && p[4]<=15){s[4]=2;}

const char ip[]="192.168.1.11";
const int port=3001;
char serverack = Client.parsePacket();
  Client.beginPacket(ip,port);
  Client.write((byte*) s, sizeof(s));
  Client.endPacket();

Serial.print(p[0]);
Serial.print("\t"); 
Serial.print(p[1]);
Serial.print("\t");
Serial.print(p[2]);
Serial.print("\t");
Serial.print(p[3]);
Serial.print("\t");
Serial.print(p[4]);
Serial.println("");
digitalWrite(LED_BUILTIN, LOW); 
}
