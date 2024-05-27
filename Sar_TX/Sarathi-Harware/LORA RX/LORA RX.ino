  // Lora Ra-02 Bidirectional Communication With Esp8266

#include <LoRa.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define SS 15
#define RST D4
#define DIO0 D0
#define TRIG_PIN 5 // D1 on NodeMCU
#define ECHO_PIN 4 // D2 on NodeMCU

//const char* ssid = "KAPIL";
//const char* password = "pooja8279";
const char* ssid = "Samsung";
const char* password = "kapil8279";
String data = "";

bool stringComplete = false;      // Whether the string is complete
long long lasttime = 0;
int led = D2; 
int buzzer = D1;
WiFiClient client;
HTTPClient http;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
int timestamp;
String serverName = "http://kapil829.pythonanywhere.com";
void setup()
{
  Serial.begin(9600);
  while (!Serial);  
  
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa Error");
    delay(100);
    while (1);
  }
  else{
    Serial.println("LORA Device Activated");
  }
  data.reserve(200);        // Max Number of character Sended through a Single mssage
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    yield();
  }
  Serial.println("Connected to the WiFi network");
  timeClient.begin();

  pinMode(buzzer,OUTPUT);
  pinMode(led,OUTPUT);
}


 
void loop()
{
  //SendData();
  RecieveData();
  if(millis()-lasttime > 5000){
    digitalWrite(led, 0);
  }
  delay(200);
  
}
void run_ultrasonic(){
   digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pin, returns the sound wave travel time in microseconds
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate the distance
  // Speed of sound wave divided by 2 (there and back)
  float distance = duration * 0.034 / 2;

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Delay before the next reading
  return distance;
}
void RecieveData(){
  stringComplete = false;
  int packetSize = LoRa.parsePacket();
  if (packetSize && run_ultrasonic()) {
    while (LoRa.available()) {
      Serial.print("Message Received : ");
      String coord = LoRa.readString();
      //sendDataToDB(coord);
      sendToApi(coord);
      digitalWrite(led, 1);
      tone(buzzer, 100, 5000);
      lasttime = millis();

    }
  }
}

void sendToApi(String latlong){
  if(WiFi.status() == WL_CONNECTED){
    WiFiClient cl;
    HTTPClient http;
    String serverPath = serverName + "/api/sendcoord?latlong=" + latlong;
    http.begin(cl,serverPath);
    Serial.println(serverPath);
    int code = http.GET();
    if(code > 0){
      String payload = http.getString();
      Serial.println(payload);
      delay(200);
    }
    else{
      Serial.print("Error code");
      Serial.println(code);
    }
  }
}




