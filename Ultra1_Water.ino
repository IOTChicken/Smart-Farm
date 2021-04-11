 #include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NewPing.h>
float trigPin = 14;
float echoPin = 12;
float MAX_DISTANCE = 30;
float duration;
float distance;
#define wifi_ssid "Galaxy S10 Lite"  
#define wifi_password "0611590710"
#define mqtt_server "34.87.181.148"
#define Ultra2 "sensor/Ultra2"
WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
   pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}
String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
     }
      return result;
}
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      String clientName;  
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      clientName += "-";
      clientName += String(micros() & 0xff, 16);
      Serial.print("Connecting to ");
      Serial.print(mqtt_server);
      Serial.print(" as ");
      Serial.println(clientName);
  if (client.connect((char*) clientName.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
      if (!client.connected()) {
        reconnect();
        }
      client.loop();
      delay(5000);
digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = MAX_DISTANCE - (duration * 0.034 / 2);
  float t = distance;
      if (isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
      }
  float tr = t * 3.33333;
      Serial.print("Ping:");
      Serial.println(String(tr).c_str());
      client.publish(Ultra2, String(tr).c_str(), true);
}
