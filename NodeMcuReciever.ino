#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ArduinoJson.h>
StaticJsonDocument<256> doc;
#define buzzer 14
#define led1 12
#define led2 13
#define led3 15
#include<Servo.h>
Servo servo;
typedef struct struct_message {
    int lpg,t,h;
} struct_message;


struct_message myData;


void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("LPG: ");
  // Serial.println(myData.lpg);
  // Serial.print("Temperature:");
  // Serial.println(myData.t);
  // Serial.print("Humidity:");
  // Serial.println(myData.h);
}
 
void setup() {

Serial.begin(115200);
WiFi.mode(WIFI_STA);
servo.attach(5);
pinMode(buzzer,OUTPUT);
pinMode(led1,OUTPUT);
pinMode(led2,OUTPUT);
pinMode(led3,OUTPUT);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  if(myData.lpg>=500){
    digitalWrite(led1,HIGH);
  }
  else{
    digitalWrite(led1,LOW);
  }

  if(myData.t > 39){
  digitalWrite(led2,HIGH);
  }
  else{
    digitalWrite(led2,LOW);
  }
  if(myData.h > 60){
  digitalWrite(led3,HIGH);
  }
  else{
    digitalWrite(led3,LOW);
  }
  doc["h"] = myData.h;
  doc["t"] = myData.t;
  doc["lpg"] = myData.lpg;
  serializeJson(doc,Serial);
  Serial.println();
}
