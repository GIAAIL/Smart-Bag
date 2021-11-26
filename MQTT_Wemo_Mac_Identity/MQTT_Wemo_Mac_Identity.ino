/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "test/sensor" every 100 mili seconds
  - subscribes to the topic "test/command", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "test/command" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

/*
String M1_topic_in = String("test/command/Motor1");
String M2_topic_in = String("test/command/Motor2");
String LED_topic_in = String("test/command/LED");
String Time_topic_out = String("test/sensor/Time");
*/
//special channel for testing 
String Ping_topic = String("/test/ping");


// Pin for Motors
const int  Motor1  = D6;
const int  Motor2  = D7;
const int Valve1 = D5;
const int Valve2 = D0;
//const int LED = D5;

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

char deviceId[50];
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

// WIFI for SMART_BAG AP
const char* ssid = "SMART-BAG";
const char* password = "12345678";
const char* mqtt_server = "192.168.0.102";
const int mqtt_port = 9000;

/* WIFI at classroom
const char* ssid = "danki";
const char* password = "ki1314ki";
const char* mqtt_server = "192.168.1.105";
const int mqtt_port = 8080;
*/



void setup_wifi() {

  delay(10);
  //We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String tp = String(topic);
  payload[length]='\0';
  String pl = String((char*) payload);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (tp == String(deviceId)){
      // 幫浦PWM控制
      analogWrite(Motor1, pl.toInt());
      analogWrite(Motor2, 0);
      //analogWrite(Valve1, pl.toInt());
      //Serial.printf("PWM %d",pl.toInt());
      //digitalWrite(Valve2, LOW);
      if(pl.toInt() == 0){
        // 讓氣嘴接往洩氣口
        analogWrite(Valve1, 255);
        digitalWrite(Valve2, LOW);
        } 
      else{
        // 讓氣嘴接往馬達幫浦
        analogWrite(Valve1, LOW);
        digitalWrite(Valve2, LOW);
        }
      
    } 
      //Serial.println(pl.toInt());
  else if (tp == Ping_topic){
      //Serial.println('ping');
      if (pl == String("who_are_you")){
          client.publish(Ping_topic.c_str(), deviceId);
      }  
  }  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    Serial.printf("Connect MQTT broker with ID: %s\n", deviceId);
  
    // Attempt to connect
    if (client.connect(deviceId)) {
      // ... and resubscribe
      client.subscribe(deviceId); 
      client.subscribe(Ping_topic.c_str());
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
  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);
  pinMode(Valve1, OUTPUT);
  pinMode(Valve2, OUTPUT);
  Serial.begin(9600);
  analogWriteFreq(100);
  
  // Create client ID based on device MacAdress
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  //char deviceId[50]; this variable is moved to global
  sprintf(deviceId, "%s_%02X%02X%02X%02X%02X%02X", "WEMO"
            , mac[WL_MAC_ADDR_LENGTH - 6], mac[WL_MAC_ADDR_LENGTH - 5]
            , mac[WL_MAC_ADDR_LENGTH - 4], mac[WL_MAC_ADDR_LENGTH - 3]
            , mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1]);
  Serial.printf("Device ID: %s\n", deviceId);
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  unsigned long now = millis();
  
  if (now - lastMsg > 3000) {
    lastMsg = now;
  }
  
}
