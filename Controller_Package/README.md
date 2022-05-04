## Setup

### 1. Install Paho MQTT Client Library
  - Follow the instruction from the [official site](https://www.eclipse.org/paho/index.php?page=clients/python/index.php)
### 2. Place the module for import 
  - Copy the module (the whole `Controller_Package` directory) in your working directory of your controll script
### 3. Access the module in your controll Logic.
  ```python3
  # import module
  from Controller_Package.controller import Bags_Controller
  
  # create a controller instance
  controller = Bags_Controller()
  
  # Connect to MQTT broker
  controller.connect("broker.emqx.io", 1883)
  

  # Force the controller to fetch device "role map" from config website (i.e. the info about which Vest-Position is assigned to which Wemo-Name)
  # This update will be automatically trigger periodically after connected to the broker, so typically users don't need to run this function. 
  controller.update_device_state()
  
  # get current available "roles" (Vest positions that are registered with a connect wemo device)
  # return a string representing the Vest Position IDs (see Config_Website for the IDs)
  controller.available_roles()
  
  # set the control value with `controller.set_val(role_id, value)`
  # The interpretation of the contorll value is up to the Wemo Code (see `Wemo_MacAddress_Identity` folder)
  # Currently the controll mechanism is as follows : 
  #   min: 1
  #   max: 255, 
  #   0 is a reserved value that means "open air gate" (let pressure drops quickly)
  # we don't want to force the control value to 0 for a long time, 
  # because "open air gate" is power consuming a lot of current (the air vault is powered by eletric magnet)
  # 
  
  for r in controller.available_roles()
      controller.set_val(r, 100)
  ```
