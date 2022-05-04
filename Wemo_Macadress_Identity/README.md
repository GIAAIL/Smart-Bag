## Setting UP
### 1. Install ESP32 Board INFO on Arduino IDE
  - Choose `Tools/Board:.../Board Manager` on Arduino IDE
  - Search for esp8266 and install the package
  - ![](https://github.com/Nemo1999/Smart-Bag/blob/master/Pictures/Arduino_IDE_Setup.png)
### 2. Install PubSubClient Library 
  - Choose `Tools/Manage Libraries` on Arduino IDE
  - ![](https://github.com/Nemo1999/Smart-Bag/blob/master/Pictures/Arduino_IDE_Install_Dependencies.png)
### 3. Enter *MQTT Broker* and *Wifi AP* info in the code
  - Enter MQTT broker address and port 
  - Enter multiple WIFI Access Point information, including multiple *hostname* and *passwd*
  - The code will try to connect all the WiFi APs in the list one by one, allowing you to spread a large number of board accross several AP's without modifying the code.
  - ![](https://github.com/Nemo1999/Smart-Bag/blob/master/Pictures/Arduino_Code_Entering_WiFi_MQTT_info.png)
  
