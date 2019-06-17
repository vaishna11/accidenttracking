
#include <ESP8266WiFi.h>--------
#include <PubSubClient.h>//maintaining mqtt protocol
 
//-------- Customise these values -----------//nodemcu dont have network,to get internet access
const char* ssid = "Cse Dept";
const char* password = "Csedept@789";
#include "DHT.h"//dht sensor
#define DHTPIN D2    // what pin we're connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);
 
#define ORG "xawvvo"
#define DEVICE_TYPE "honor8"
#define DEVICE_ID "123456"
#define TOKEN "12345678"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";


char topic[] = "iot-2/evt/Data/fmt/json";//default topic of ibm
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;//publish data,invoke nodemcu
PubSubClient client(server, 1883,wifiClient);//port of mqttprotocol  is 1883,mqtt protocol client

void setup() {
 Serial.begin(115200);
 Serial.println();
 dht.begin();//read sensor values
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}
 
void loop() {
float h = dht.readHumidity();
float t = dht.readTemperature();//dht. will redirect to library
if (isnan(h) || isnan(t))
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}
PublishData(t,h);
delay(100);
}

void PublishData(float temp, float humid){
 if (!!!client.connected()) {//mqttclient with server port is not connected
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += temp;
  payload+="," "\"humidity\":";//key
  payload += humid;//value
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {//it willbe connecting to server
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
