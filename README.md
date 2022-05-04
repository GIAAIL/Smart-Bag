# Smart-Bag Project 
This repo contains the code for Smart-Bag Project in NYCU

- Web Configuration Demo available [here](https://nemo1999.github.io/Smart-Bag/Config_Website/index.html)

- Currently all components use MQTT Broker at [emqx.com](https://www.emqx.com/en/mqtt/public-mqtt5-broker)

- For more stable result, consider hosting a local broker (see [Mosquitto MQTT Broker Script](#mosquitto))

# Content Structure: 
- [Config Website](#Config-Website) ( Website to assign positions of air-bag modules )
- [Controller Module](#Controller)  ( Python library for Controller )
- [Mosquitto MQTT Broker Script](#mosquitto)

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
  
## [`Wemo_Mac_Identity`]()
  
