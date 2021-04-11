#include <ESP8266WiFi.h>            #include <Wire.h>
#include <PubSubClient.h>           #include <OneWire.h>
#include <DallasTemperature.h>      #define ONE_WIRE_BUS D3 
OneWire oneWire(ONE_WIRE_BUS);      DallasTemperature sensors(&oneWire);
int a = D1 ;    int b = D2 ;
#define wifi_ssid "Galaxy S10 Lite"
#define wifi_password "0611590710"
#define mqtt_server "34.87.181.148"
#define tempc "sensor/tempc"
WiFiClient espClient;   PubSubClient client(espClient);
long lastMsg = 0;     char msg[50];     int value = 0;
void setup() {
    pinMode(a, OUTPUT);   pinMode(b, OUTPUT);
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    sensors.begin();}
    String macToStr(const uint8_t* mac){
    String result;
    for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
    result += ':';}
    return result;}
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(wifi_ssid);
    WiFi.begin(wifi_ssid, wifi_password);
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
      digitalWrite(a, LOW);}}
      if (strcmp(topic,"LED2")==0){
    if ((char)payload[0] == '1') {
      digitalWrite(b, HIGH); 
    } else {
      digitalWrite(b, LOW); }}} 
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
      client.subscribe("LED1");
      client.subscribe("LED2");   
      Serial.print(mqtt_server);
      Serial.print(" as ");
      Serial.println(clientName);
  if (client.connect((char*) clientName.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);}}}   
void loop() {
      if (!client.connected()) {
      reconnect();}
      client.loop();
      delay(100);
  sensors.requestTemperatures(); //อ่านข้อมูลจาก library
  float t = sensors.getTempCByIndex(0);
      if (isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;  }
if (t >30) { 
    digitalWrite(a, HIGH); digitalWrite(b, HIGH);}
  else {
    digitalWrite(a, LOW); digitalWrite(b, LOW); }   
      Serial.print("Temperature:");
      Serial.println(String(t).c_str());
      client.publish(tempc, String(t).c_str(), true);}
