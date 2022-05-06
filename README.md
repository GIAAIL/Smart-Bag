# Smart-Bag Project 
- This repo contains the code for Smart-Bag Project in NYCU part of [GIAAIL](https://github.com/GIAAIL)
![](https://github.com/Nemo1999/Smart-Bag/blob/master/Pictures/Smart_Bag_Final_Demo.gif)

- For the communication of all the components in this repo, see [this excalidraw diagram](https://excalidraw.com/#room=55850e406bf89b3ca01c,bQ7NPqvCC9gKfAy7OdJU-A)
![](https://github.com/Nemo1999/Smart-Bag/blob/master/Pictures/SmartBag_Communication.svg)

- For debbuging the MQTT Message Mechanism, it is recommanded to use tools such as [MQTTBOX](https://chrome.google.com/webstore/detail/mqttbox/kaajoficamnjijhkeomgfljpicifbkaf/related)

- This repo contains code for 
  1. ([Wemo Client](#wemo-identity)) -- Arduino IDE code for connect WiFi / MQTT client / Controlling Motor and Air Vault
  2. ([Config Website](#Config-Website) ) -- Device-Role Assigning Website , which provides a GUI interface for assigning positions of air-bag modules.
  3. ([Controller Module](#Controller)) -- Controller Module that provide a python interface to directly assign power to each Vest Location (instead of device ID) 
  4.  ( [Mosquitto MQTT Broker Script](#mosquitto)) -- Script to help you start a MQTT server Locally
  5. MQTT Control Logic ( seperated in all the modules, including controller / website / device )
  6. ([Mosquitto Feedback Controll](#Feedback)) -- An experimental side project that use PID controller to adjust the motor power with pressure feedback, see [This Youtube Video](https://www.youtube.com/watch?v=mnSC4qjzziw) for a quick demo.



- Configuration Website is hosted by GitPage, available [Here](https://nemo1999.github.io/Smart-Bag/Config_Website/index.html)

- Currently all components use MQTT Broker at [emqx.com](https://www.emqx.com/en/mqtt/public-mqtt5-broker)
  - For more stable result, consider hosting a local broker (see [Mosquitto MQTT Broker Script](#mosquitto))

## [`Config Website`](https://github.com/Nemo1999/Smart-Bag/tree/master/Config_Website)  <a name="Config-Website"></a>
  contrains the source for a static website. The website use [MQTT.js](https://github.com/mqttjs/MQTT.js) 
  to communicate with each WEMO module.  
  
  The website is hosted by GitPage and is available at [https://nemo1999.github.io/Smart-Bag/Config_Website/index.html](https://nemo1999.github.io/Smart-Bag/Config_Website/index.html)
  ![Website ScreenShot](https://github.com/Nemo1999/Smart-Bag/blob/master/Pictures/Config_Website.png)
  
## [`Controller Module`](https://github.com/Nemo1999/Smart-Bag/tree/master/Controller_Package)<a name="Controller"></a>
  contrains python module that implements the controller for the  WEMO modules, 
  the code use [paho.mqtt](https://github.com/eclipse/paho.mqtt.python) to communicate to `Config Website`, 
  and fetch the current role configuration (correspondence between __WEMO's MAC-address__ and __role__).
  The user of the controller only needs to specify the "role" and "pwm-power" to control the WEMO modules.
  
  For example: 
  ```python
    from Controller_Package.controller import Bags_Controller
    
    controller = Bags_Controller()
    
    # Connect to MQTT broker
    controller.connect("broker.emqx.io", 1883)
    
    """
     fetch role configuration from Config_Website 
     (This step will be automatic ones connected to broker,
       so user don't need to call this function)
    """
    # controller.update_device_state()
    
    # set the air pumping power of each assigned module to 100 
    # power value control mechanism: 
    # min=1, max=255, 
    # special value 0 means "open air gate" (let pressure drops quickly) 
    for r in controller.available_roles()
        controller.set_val(r, 100)
  ```
## [`Mosquitto MQTT Broker`](https://github.com/Nemo1999/Smart-Bag/tree/master/Mosquitto_MQTT_Broker)<a name="mosquitto"></a>
  This contains minimal script and setting to setup [mosquitto](https://mosquitto.org/) broker.
  
  [`start_server.sh`](https://github.com/Nemo1999/Smart-Bag/blob/master/Mosquitto_MQTT_Broker/start_server.sh) print all the message and clients info in `stdout` for convinient debugging.
  
## [`Wemo_Mac_Identity`](https://github.com/Nemo1999/Smart-Bag/tree/master/Wemo_Macadress_Identity)<a name="wemo-identity"></a>
  
- You need to setup the Arduino IDE / Enter MQTT and WiFi-AP info to make Wemo Available
- See the [README.md in the folder](https://github.com/Nemo1999/Smart-Bag/tree/master/Wemo_Macadress_Identity) for more detail.
## [`Wemo Feed Back Controll`](https://github.com/Nemo1999/Smart-Bag/tree/master/Wemo_FeedBack_Control)<a name="Feedback"></a>
- I implement a simple PID controller that use the feedback from pressure sensing resister to adjust the power of air pumping motor.
- Pressure Sensor is here ![](https://github.com/Nemo1999/Smart-Bag/blob/master/Wemo_FeedBack_Control/Pressure_Sensor.jpg)
- The result can be seen in the graph below, we can we that the gray curve (Sensed Pressure Value (in scaled resister voltage)), match the blue curve (targeted control value given from the interactive controller)
- ![](https://github.com/Nemo1999/Smart-Bag/blob/master/Wemo_FeedBack_Control/Screenshot%20from%202021-11-26%2018-08-06.png)
- Note that the pink curve (motor power) is constantly adjusting itself to fit the gray curve to the blue curve.
- See a more interactive demo in [this video](https://www.youtube.com/watch?v=mnSC4qjzziw), note that when I blocked the air hole on the airbag with my finger, the motor quickly power dropped, but the sensor pressure stay in a stable level!!
