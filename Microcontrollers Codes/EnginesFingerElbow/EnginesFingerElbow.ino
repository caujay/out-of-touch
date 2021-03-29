#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVO_FREQ 50
uint8_t serv_p1 = 0;
uint8_t serv_p2 = 1;
uint8_t serv_p3 = 2;
uint8_t serv_nad = 3;
uint8_t serv_lok = 4;

int sensor_p1=0;
int sensor_p2=0;
int sensor_p3=0;
int sensor_nad=0;
int sensor_lok=0;

int pozycja_p1=0;
int pozycja_p2=0;
int pozycja_p3=0;
int pozycja_nad=0;
int pozycja_lok=0;

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

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);  // The int.osc. is closer to 27MHz  
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(100);
  
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
  Server.begin(3006);
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


///////////////////////////////////////////////////////
//palec 1 - KCIUK
sensor_p1=1003-1.30*arr[4];
if (sensor_p1>400) {sensor_p1=400;}
if (sensor_p1<100) {sensor_p1=100;}

if (sensor_p1 > pozycja_p1) 
  { 
    pozycja_p1 = pozycja_p1 + 1;
    if (pozycja_p1>400) {pozycja_p1=400;}
    pwm.setPWM(serv_p1, 0, pozycja_p1);
  }
  
  if (sensor_p1 < pozycja_p1) 
  { 
    pozycja_p1 = pozycja_p1 - 1;
    if (pozycja_p1<100) {pozycja_p1=100;}
    pwm.setPWM(serv_p1, 0, pozycja_p1);
  }
  
///////////////////////////////////////////////////////
//palec 2 - WSKAZUJACY
sensor_p2=12*arr[1]-10580;
if (sensor_p2>400) {sensor_p2=400;}
if (sensor_p2<100) {sensor_p2=100;}

if (sensor_p2 > pozycja_p2) 
  { 
    pozycja_p2 = pozycja_p2 + 1;
    if (pozycja_p2>400) {pozycja_p2=400;}
    pwm.setPWM(serv_p2, 0, pozycja_p2);
  }
  
  if (sensor_p2 < pozycja_p2) 
  { 
    pozycja_p2 = pozycja_p2 - 1;
    if (pozycja_p2<100) {pozycja_p2=100;}
    pwm.setPWM(serv_p2, 0, pozycja_p2);
  }
  
///////////////////////////////////////////////////////  
//palec 3 - ŚRODKOWY
sensor_p3=6839-7.31*arr[0];
if (sensor_p3>400) {sensor_p3=400;}
if (sensor_p3<100) {sensor_p3=100;}

if (sensor_p3 > pozycja_p3) 
  { 
    pozycja_p3 = pozycja_p3 + 1;
    if (pozycja_p3>400) {pozycja_p3=400;}
    pwm.setPWM(serv_p3, 0, pozycja_p3);
  }
  
  if (sensor_p3 < pozycja_p3) 
  { 
    pozycja_p3 = pozycja_p3 - 1;
    if (pozycja_p3<100) {pozycja_p3=100;}
    pwm.setPWM(serv_p3, 0, pozycja_p3);
  }
  
/////////////////////////////////////////////////////// 
//NADGARSTEK
sensor_nad=500-2.22*arr[2];
if (sensor_nad>500) {sensor_nad=500;}
if (sensor_nad<100) {sensor_nad=100;}

if (sensor_nad > pozycja_nad) 
  { 
    pozycja_nad = pozycja_nad + 1;
    if (pozycja_nad>500) {pozycja_nad=500;}
    pwm.setPWM(serv_nad, 0, pozycja_nad);
  }
  
  if (sensor_nad < pozycja_nad) 
  { 
    pozycja_nad = pozycja_nad - 1;
    if (pozycja_nad<100) {pozycja_nad=100;}
    pwm.setPWM(serv_nad, 0, pozycja_nad);
  }

/////////////////////////////////////////////////////// 
//ŁOKIEĆ

sensor_lok=800-arr[3];
if (sensor_lok>345) {sensor_lok=345;}
if (sensor_lok<220) {sensor_lok=220;}

if (sensor_lok > pozycja_lok) 
  { 
    pozycja_lok = pozycja_lok + 1;
    if (pozycja_lok>345) {pozycja_lok=345;}
    pwm.setPWM(serv_lok, 0, pozycja_lok);
  }
  
  if (sensor_nad < pozycja_nad) 
  { 
    pozycja_lok = pozycja_lok - 1;
    if (pozycja_lok<220) {pozycja_lok=220;}
    pwm.setPWM(serv_lok, 0, pozycja_lok);
  }




Serial.print(arr[4]);
Serial.print("\t");
Serial.print(sensor_p1);
Serial.print("\t");
Serial.print("\t");
Serial.print(arr[1]);
Serial.print("\t");
Serial.print(sensor_p2);
Serial.print("\t");
Serial.print("\t");
Serial.print(arr[0]);
Serial.print("\t");
Serial.print(sensor_p3);
Serial.print("\t");
Serial.print("\t");
Serial.print(arr[2]);
Serial.print("\t");
Serial.print(sensor_nad);
Serial.print("\t");
Serial.print("\t");
Serial.print(arr[3]);
Serial.print("\t");
Serial.print(sensor_lok);
Serial.println("");




}
