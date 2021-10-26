//#define PUL D0 //pump腳位LOW
#define PUH D6 //pump腳位HIGH
#define FAL D8 //閥腳位LOW
#define FAH D7 //閥腳位HIGH
int state = 0;
int air = 0;
int astate = 0;
int val; //pwm
String inString = "";

#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <PubSubClient.h>

const char *ssid =  "SMART-BAG";   // AP name
const char *password =  "12345678";   // AP password
const char* DEVICE_NAME = "02";
const char* TOPIC_FORWARD = "AITEST092802";

//const char* mqtt_server = "broker.mqtt-dashboard.com"; //官方測試server
const char* mqtt_server = "192.168.0.100";
//const uint16_t MQTT_PORT = 1883;
const uint16_t MQTT_PORT = 8080;
WiFiClient mqttWifiClient;
PubSubClient mqttClient (mqttWifiClient, mqtt_server, MQTT_PORT);

void myMqttCallback(const MQTT::Publish& pub) {
  String topicStr = pub.topic();
  String payloadStr = pub.payload_string();
  if (topicStr == TOPIC_FORWARD) {
    if (payloadStr == "on") {
      air = 1;
      Serial.println("on");
    } else if
    (payloadStr == "off") {
      air = 0;
      Serial.println("off");
    } else {
      val = payloadStr.toInt();//將MQTT讀取之數值轉為INT
      Serial.print("val:");
      Serial.println(val);
      state = 1;
    }
  }
}
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.setOutputPower(20);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //pinMode(PUL, OUTPUT);
  pinMode(PUH, OUTPUT);
  pinMode(FAL, OUTPUT);
  pinMode(FAH, OUTPUT);
}
void loop() {
  mqttLoop();
  delay(10);
  if (state == 1) {
    if (val == 0) {
      digitalWrite(FAH, LOW);
      digitalWrite(FAL, LOW);
      Serial.print("air:");
      Serial.println(air);
      analogWrite(PUH, val);
      state = 0;
    }
    if (val > 0 && val <= 1024) {

      digitalWrite(FAH, HIGH);
      digitalWrite(FAL, LOW);
      Serial.print("air:");
      Serial.println(air);
      analogWrite(PUH, val);
      state = 0;
    }
    
  }


  //  if (air == 1 && astate == 0) {
  //    digitalWrite(FAH, HIGH);
  //    digitalWrite(FAL, LOW);
  //    Serial.print("air:");
  //    Serial.println(air);
  //    astate = 1;
  //  } else if (air == 0 && astate == 1) {
  //    digitalWrite(FAH, LOW);
  //    digitalWrite(FAL, LOW);
  //    Serial.print("air:");
  //    Serial.println(air);
  //    astate = 0;
  //  }
}
void subscribeTopics() {
  mqttClient.subscribe(MQTT::Subscribe()
                       .add_topic(TOPIC_FORWARD, 0));
}
void mqttLoop() {
  ESP.wdtFeed();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Try to connect to MQTT but no WiFi connection");
    return;
  }

  if (mqttClient.connected()) {
    mqttClient.loop();
  } else {
    0xff;
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    char deviceId[50];
    sprintf(deviceId, "%s_%02X%02X%02X%02X%02X%02X", DEVICE_NAME
            , mac[WL_MAC_ADDR_LENGTH - 6], mac[WL_MAC_ADDR_LENGTH - 5]
            , mac[WL_MAC_ADDR_LENGTH - 4], mac[WL_MAC_ADDR_LENGTH - 3]
            , mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1]);
    Serial.printf("Connect MQTT broker with ID: %s\n", deviceId);
    mqttWifiClient.setTimeout(5000);
    mqttClient.disconnect();
    ESP.wdtFeed();
    delay(10);
    if (mqttClient.connect(deviceId) == true) {
      mqttClient.set_callback(myMqttCallback);
      subscribeTopics();
      Serial.println("MQTT connect success");
    } else {
      Serial.println("MQTT connect fail");
    }
  }
}
