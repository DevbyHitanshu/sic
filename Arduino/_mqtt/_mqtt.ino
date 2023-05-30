#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Paaji";
const char* password = "12345678";
const char* mqtt_server = "192.168.225.76";

const int emgPin = A0;
const int threshold = 400;
const int movementThreshold = 500;
const int consecutiveMovementThreshold = 500;
const int consecutiveMovementDelay = 3000;

unsigned long lastMovementTime = 0;
bool wristMovementDetected = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi, IP address: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqtt_server, 1883);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    
    int emgValue = analogRead(emgPin);
    if (emgValue > threshold) {
      // Check for wrist movement
      if (!wristMovementDetected) {
        if (emgValue > movementThreshold) {
          wristMovementDetected = true;
          lastMovementTime = millis();
        }
      } else {
        unsigned long currentTime = millis();
        unsigned long timeDifference = currentTime - lastMovementTime;

        if (emgValue > consecutiveMovementThreshold && timeDifference < consecutiveMovementDelay) {
          // Two consecutive wrist movements within the specified time
          int hello = 2;
          client.publish("hello", String(hello).c_str());
          wristMovementDetected = false;
          
        } else if (timeDifference >= consecutiveMovementDelay) {
          // Only one wrist movement detected within the time window
          int hello = 1;
          client.publish("hello", String(hello).c_str());
          wristMovementDetected = false;
        }
      }
    }
   
    
    delay(100);
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("NodeMCU")) {
            Serial.println("connected");
            client.subscribe("hello");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}
