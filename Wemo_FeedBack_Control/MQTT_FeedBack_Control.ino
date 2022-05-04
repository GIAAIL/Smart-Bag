/*
  The script use PID controller to control the air pumping motor power
  with the feedback pressure sensor voltage from A0 pin.
  
  It shold be used with interactive_demo.py in `Simulated_Controller` folder
  You also need to start a Mosquitto MQTT Broker(see `Mosquitto MQTT Broker` folder) locally for this script on wemo and the python controller to communicate with each other. 
*/ 
    
/* 
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

const int  Motor1  = D6;
const int  Motor2  = D7;
const int LED = D8;

String M1_topic_in = String("/test/command/Motor1");
String M2_topic_in = String("/test/command/Motor2");
String LED_topic_in = String("/test/command/LED");
String Time_topic_out = String("/test/sensor/Time");
String Ping_topic = String("/test/ping");

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

/*
const char* ssid = "5F_2.4G1";
const char* password = "12345678";
const char* mqtt_server = "192.168.1.106";
*/
/*
const char* ssid = "danki";
const char* password = "ki1314ki";
const char* mqtt_server = "192.168.1.105";
*/

const char* ssid = "SMART-BAG";
const char* password = "12345678";
const char* mqtt_server = "192.168.0.102";
const int mqtt_port = 9000;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
int target_value = 0;
int prev_error = 0;
float integration = 0;
int motor_control = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }

  randomSeed(micros());

  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String tp = String(topic);
  payload[length]='\0';
  String pl = String((char*) payload);
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
  }
  //Serial.println();
  if (tp == LED_topic_in){
     // Switch on the LED if an 1 was received as first character
    if (pl == "on") {
      digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
      // but actually the LED is on; this is because
      // it is active low on the ESP-01)
    }else if (pl == "off"){
      digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    }else{
      analogWrite(LED, pl.toInt());  
    }
  }
  else if (tp == M1_topic_in){
      target_value = pl.toInt();
      //analogWrite(Motor1, pl.toInt());
      //Serial.println(pl.toInt());
    }
  else if (tp == M2_topic_in){
      target_value = pl.toInt();
      //analogWrite(Motor2, pl.toInt());
      //Serial.println(pl.toInt());
    }
  else if (tp == Ping_topic){
      //Serial.println('ping');
      if (!pl.endsWith(String("reply"))){
          client.publish(Ping_topic.c_str(), (pl+"reply").c_str());
        }
      
    }  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      //Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("test/sensor", "hello world");
      // ... and resubscribe
      client.subscribe(LED_topic_in.c_str());
      client.subscribe(M1_topic_in.c_str());
      client.subscribe(M2_topic_in.c_str());
      client.subscribe(Ping_topic.c_str());
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT); 
  pinMode(LED, OUTPUT);
  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  delay(100);
  Serial.print('\n');
  Serial.print("Target_value");
  Serial.print('\t');
  Serial.print("P");
  Serial.print('\t');
  Serial.print("I");
  Serial.print('\t');
  Serial.print("D");
  Serial.print('\t');
  Serial.print("Motor_control");
  Serial.print('\t');
  Serial.println("Current_Value");  
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  unsigned long now = millis();
  
  if (now - lastMsg > 30) {
  
    lastMsg = now;
    int current_value = analogRead(A0);
    // P
    int current_error = target_value - current_value;
    // I
    integration *= 0.99;
    integration += current_error;
    // D
    int derivative = current_error - prev_error;

    //float a = -0.2;
    //float b = 0.25 - a * 4;
    // calculate control signal (weighted sum of P, I, D)
    int motor_control =  target_value / 3.5 + 1.0 * current_error  + 0.1 * integration + 0.01* derivative;
    
    // control motors
    analogWrite(Motor1, motor_control);
    analogWrite(Motor2, motor_control);
    
    //store current error
    prev_error = current_error;
    
    // plot PID and current_value
    
    Serial.print(target_value);
    Serial.print('\t');
    Serial.print(current_error);
    Serial.print('\t');
    Serial.print(integration/10);
    Serial.print('\t');
    Serial.print(derivative);
    Serial.print('\t');
    Serial.print(motor_control);
    Serial.print('\t');
    Serial.println(current_value);

    
  /*
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(Time_topic_out.c_str(), msg);
  */
  }
  
  
}
