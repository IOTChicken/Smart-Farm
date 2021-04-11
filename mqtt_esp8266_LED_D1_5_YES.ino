#include <ESP8266WiFi.h>
#include <PubSubClient.h>
int a = D1 ;          int b = D2 ;  
int c = D3 ;          int d = D5 ;
int analogPin = A0;   int val = 0;
const char* ssid = "Galaxy S10 Lite";
const char* password = "0611590710";
const char* mqtt_server = "34.87.181.148";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;     char msg[50];     int value = 0;
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");}
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
  Serial.print((char)payload[i]);}
  Serial.println();
if (strcmp(topic,"LED1")==0){
    if ((char)payload[0] == '1') {
      digitalWrite(a, HIGH); 
    } else {
      digitalWrite(a, LOW); 
    }}
    if (strcmp(topic,"LED2")==0){
    if ((char)payload[0] == '1') {
      digitalWrite(c, HIGH); 
    } else {
      digitalWrite(c, LOW); 
    }}
    if (strcmp(topic,"LED3")==0){
    if ((char)payload[0] == '1') {
      digitalWrite(c, HIGH); 
    } else {
      digitalWrite(c, LOW); 
    }}
    if (strcmp(topic,"LED4")==0){
    if ((char)payload[0] == '1') {
      digitalWrite(d, HIGH); 
    } else {
      digitalWrite(d, LOW); 
    }}}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("LED1");
      client.subscribe("LED2");
      client.subscribe("LED3");
      client.subscribe("LED4");} 
      else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }}}
void setup() {
  pinMode(a, OUTPUT);     pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);     pinMode(d, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); }
void loop() {
  val = analogRead(analogPin);
  float ls = val / 10.24; 
  if (ls > 97.65) { 
    digitalWrite(b, HIGH); 
  }
  else {
    digitalWrite(b, LOW); 
  }
  Serial.print("val = ");
  Serial.println(ls); 
  delay(500);
  if (!client.connected()) {
    reconnect(); }
    client.loop();    }
