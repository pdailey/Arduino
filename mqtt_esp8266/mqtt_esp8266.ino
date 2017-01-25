#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include "WiFiConfig.h"  // wifi ssid and password

//const char room[] = "Kitchen/";
//const char room[] = "Bathroom/";
const char room[] = "Living_Room/";
//const char room[] = "Basement/";
//const char room[] = "Hall/";


const char* mqtt_server = "192.168.0.103";
//const char* mqtt_server = "PetersMac.local";

// pin no and type of DHT22 sensor
DHT dht(4, DHT22);
const unsigned int ms_dht = 10000; // Time between sampling/publishing dht readings

// pin no for Motion Sensor  (HC-S501)
const byte pirPin = 2; // Input for HC-S501
bool mvmt; // Place to store read PIR Value
bool lastMvmt;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char topic[50];
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("!");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(room)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "Connected");
      
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT); 
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  dht.begin();
  pinMode(pirPin, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  // Send Temperature periodically
  if (now - lastMsg > ms_dht) {
    lastMsg = now;

    // Read from sensors
    float t = dht.readTemperature();
    float h = dht.readHumidity();
  
    publishSensorVal("Temperature", t);
    publishSensorVal("Humidity", h);
  } 

  // Send motion if it has changed
  mvmt = digitalRead(pirPin);
  if(mvmt != lastMvmt){
    publishSensorVal("Movement", int(mvmt));   
    
    digitalWrite(BUILTIN_LED, !mvmt);
    lastMvmt = mvmt;
  }
}

void publishSensorVal(const char *sensor, float val) {
  char topic[32];
  strcpy(topic, room);   // eg "Living_Room/"
  strcat(topic, sensor); // eg "Living_Room/Temperature"
  
  Serial.println(topic);
  Serial.println(msg);

  dtostrf(val, 4, 1, msg); /* 4 is mininum width, 1 is precision; float value is copied onto msg*/
  client.publish(topic, msg);
}
