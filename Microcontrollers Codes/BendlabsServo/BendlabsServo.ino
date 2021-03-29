#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVO_FREQ 50
uint8_t serv_x = 0;
uint8_t serv_y = 2;
int sensor_x=0;
int sensor_y=0;
int pozycja_x=0;
int pozycja_y=0;
int i=0;
int j=0;

#define SERVOMIN  200 
#define SERVOMAX  300 

const char* ssid = "test2";
const char* pw = "22345678";
byte data[200] ={};
int packetsize = 0;
int arr[5];
String receiveddata="";
WiFiUDP Server;

void setup() {
  Serial.begin(115200);
//inicjalizacja serw
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(100);
  
//inicjalizacja wifi  
  WiFi.begin("Jest_moc", "olimp123");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Server.begin(3005);
  Serial.println("Server started...");
  


}

void loop() {
  

 char message = Server.parsePacket();
 packetsize = Server.available();
 if (message)
 {
  Server.read(data,packetsize);
  IPAddress remoteip=Server.remoteIP();
  Server.endPacket();
 }

 if(packetsize) 
 {
  for (int i=0;packetsize > i ;i++)
  {
    //Serial.println(data[i]);
  }
 }

//wyświetlenie odczytu czujników
  memcpy(arr, data, 8);
 // Serial.print("czujni 0 - ");    
 // Serial.print(arr[0]);
 // Serial.print("             czujnik 1 - ");
  //Serial.print(arr[1]);
  //Serial.println("");

if(i==60){

//głaskanie

  sensor_x=300-(arr[0]);
  if (sensor_x>450) {sensor_x=450;}
  if (sensor_x<150) {sensor_x=150;}
  
  if (sensor_x > pozycja_x) 
  { 
    pozycja_x = pozycja_x + 1;
    if (pozycja_x>450) {pozycja_x=450;}
    pwm.setPWM(serv_x, 0, pozycja_x);
    //Serial.println("ruch serva");
  }
  
  if (sensor_x < pozycja_x) 
  { 
    pozycja_x = pozycja_x - 1;
    if (pozycja_x<150) {pozycja_x=150;}
    pwm.setPWM(serv_x, 0, pozycja_x);
    //Serial.println("ruch serva");
  }


 
  
 //pochylenie dłoni
  sensor_y=300-arr[1];
  if (sensor_y>500) {sensor_y=500;}
  if (sensor_y<100) {sensor_y=100;}
  
  if (sensor_y > pozycja_y) 
  { 
    pozycja_y = pozycja_y + 1;
    if (pozycja_y>500) {pozycja_y=500;}
    pwm.setPWM(serv_y, 0, pozycja_y);
    //Serial.println("ruch serva");
  }
  
  if (sensor_y < pozycja_y) 
  { 
    pozycja_y = pozycja_y - 1;
    if (pozycja_y<100) {pozycja_y=100;}
    pwm.setPWM(serv_y, 0, pozycja_y);
    //Serial.println("ruch serva");
  }


    Serial.print(sensor_x);
    Serial.print("\t"); 
    Serial.print(pozycja_x);
    Serial.print("\t"); 
    Serial.print("\t");
    Serial.print(sensor_y);
    Serial.print("\t"); 
    Serial.print(pozycja_y);
    Serial.println("");
}
i++;
if(i==61){i=0;}

}
