#include <ESP8266WiFi.h>
#include <espnow.h>
#include<Servo.h>
Servo servo;
#define lpgPin A0
#include "DHT.h"
#define DHTPIN 2 
#define servoPin 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


typedef struct struct_message {
  int lpg,t,h;
} struct_message;

struct_message myData;



void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}
 
void setup() {
  servo.attach(servoPin);
  Serial.begin(115200);
 pinMode(lpgPin,INPUT);
  dht.begin();
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }


  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  

  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int lpg = analogRead(lpgPin);
  Serial.print("Humidity:");
  Serial.println(h);
   Serial.print("Temperature:");
  Serial.println(t);
  Serial.print("LPG:");
  Serial.println(lpg);
  myData.t = t;
  myData.h = h;
  myData.lpg = lpg;

    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
delay(100);
}
