#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#include "Arduino.h"
#include "ads_two_axis.h"

#define ADS_RESET_PIN       (13)         // Pin number attached to ads reset line.
#define ADS_INTERRUPT_PIN   (12)          // Pin number attached to the ads data ready line. 

WiFiUDP Client;

int arr[2];

void ads_data_callback(float * sample);
void deadzone_filter(float * sample);
void signal_filter(float * sample);

float ang[2];

volatile bool newData = false;

void signal_filter(float * sample)
{
    static float filter_samples[2][6];

    for(uint8_t i=0; i<2; i++)
    {
      filter_samples[i][5] = filter_samples[i][4];
      filter_samples[i][4] = filter_samples[i][3];
      filter_samples[i][3] = (float)sample[i];
      filter_samples[i][2] = filter_samples[i][1];
      filter_samples[i][1] = filter_samples[i][0];
  
      filter_samples[i][0] = filter_samples[i][1]*(0.36952737735124147f) - 0.19581571265583314f*filter_samples[i][2] + \
      0.20657208382614792f*(filter_samples[i][3] + 2*filter_samples[i][4] + filter_samples[i][5]);   

      sample[i] = filter_samples[i][0];
    }
}

void deadzone_filter(float * sample)
{
  static float prev_sample[2];
  float dead_zone = 0.5f;

  for(uint8_t i=0; i<2; i++)
  {
    if(fabs(sample[i]-prev_sample[i]) > dead_zone)
      prev_sample[i] = sample[i];
    else
      sample[i] = prev_sample[i];
  }
}

void ads_data_callback(float * sample)
{
  signal_filter(sample);      // Low pass IIR filter

  deadzone_filter(sample);   // Deadzone filter
  
  ang[0] = sample[0];
  ang[1] = sample[1];
  
  newData = true;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(2000);
  
  ads_init_t init;

  init.sps = ADS_100_HZ;
  init.ads_sample_callback = &ads_data_callback;
  init.reset_pin = ADS_RESET_PIN;                 // Pin connected to ADS reset line
  init.datardy_pin = ADS_INTERRUPT_PIN;           // Pin connected to ADS data ready interrupt
  
  int ret_val = ads_two_axis_init(&init);         // Initialize ADS hardware abstraction layer, and set the sample rate
  delay(100);
  ads_two_axis_run(true);                         // Start reading data!

//inicjalizacja wifi
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
  if(newData)
  {
    newData = false;
    arr[0]=ang[0]*2;
    arr[1]=ang[1]*2;
    const int port=3002;
    char serverack = Client.parsePacket();
    const char ip[]="192.168.1.11";

    Client.beginPacket(ip,port);
    Client.write((byte*) arr, sizeof(arr));
    Client.endPacket();
    
    Serial.print(ang[0]);
    Serial.print("\t"); 
    Serial.print(ang[1]);
    Serial.println("");
 
  }
}
