#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

WiFiUDP Client;

#include "Wire.h"
#include "I2Cdev.h"

#include "MPU6050.h"
MPU6050 accelgyro;

#include <SparkFun_ADS1015_Arduino_Library.h>
ADS1015 fingerSensor;

#include <Adafruit_ADS1015.h>
Adafruit_ADS1015 ads1015(0x4B);

int16_t ax, ay, az;  // define accel as ax,ay,az
int16_t gx, gy, gz;  // define gyro as gx,gy,gz

int16_t adc0, adc1, adc2, adc3;

int16_t sensor=0;
int kat=0;

uint16_t data;

int s[5];

bool stan=0;

void setup() {
  Wire.begin();      // join I2C bus
  Serial.begin(115200);    //  initialize serial communication
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  if (fingerSensor.begin() == false) {
     Serial.println("Device not found. Check wiring.");
     while (1);
  } 
  
  fingerSensor.setGain(ADS1015_CONFIG_PGA_TWOTHIRDS); // Gain of 2/3 to works well with flex glove board voltage swings (default is gain of 2)

  ads1015.begin();

pinMode(LED_BUILTIN, OUTPUT);

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

void loop() {
  
  accelgyro.getMotion6(&gy, &gx, &gz, &ay, &ax, &az);  // read measurements from device
  sensor=gy-570;
  kat=90-sensor/182;
  kat=kat/3;
  kat=kat*3;
  s[2]=kat;

  for (int finger = 0; finger < 2; finger++) {
    data = fingerSensor.getAnalogData(finger);
    s[finger]=data;
  }
  
  adc0 = ads1015.readADC_SingleEnded(0);
  adc1 = ads1015.readADC_SingleEnded(1);
  adc2 = ads1015.readADC_SingleEnded(2);
  adc3 = ads1015.readADC_SingleEnded(3);
  s[3]=adc2;
  s[4]=adc3;

  Serial.print(s[4]);
  Serial.print("\t"); 
  Serial.print(s[1]);
  Serial.print("\t");
  Serial.print(s[0]);
  Serial.print("\t");
  Serial.print(s[2]);
  Serial.print("\t");
  Serial.print(s[3]);
  Serial.println("");

  const int port=3003;
  char serverack = Client.parsePacket();
  const char ip[]="192.168.1.11";
  Client.beginPacket(ip,port);
  Client.write((byte*) s, sizeof(s));
  Client.endPacket();
  
}
