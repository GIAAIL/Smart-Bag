# Smart-Bag Project 
This repo contains the code for Smart-Bag Project in NYCU
# Content Structure: 
- ## [`Config Website`](https://github.com/Nemo1999/Smart-Bag/tree/master/Config_Website)  
  contrains the source for a static website. The website use [MQTT.js](https://github.com/mqttjs/MQTT.js) 
  to assign the "role" ( position the air bag module on our vest ) of each WEMO module.  
  
  To deploy the website, copy all files into `doc` folder, (or run [`Deplay_to_GitPage.sh`](https://github.com/Nemo1999/Smart-Bag/blob/master/Deploy_to_GitPage.sh))
  GitPage will host the website at [https://nemo1999.github.io/Smart-Bag/](https://nemo1999.github.io/Smart-Bag/) 
  ![Website ScreenShot](https://github.com/Nemo1999/Smart-Bag/blob/master/Pictures/Config_Website.png)
  
- ## [`Controller Module`](https://github.com/Nemo1999/Smart-Bag/tree/master/Controller_Package)
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
 
