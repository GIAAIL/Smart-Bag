import paho.mqtt.client as mqtt
import json
from collections import defaultdict
import time


class Bags_Controller:
    def __init__(self):
        self.client = mqtt.Client()
        self.connected = False
        self.device_status = None
        self.role_to_device = defaultdict(lambda:None)
    
    def __del__(self):
        if self.client:
            self.client.disconnect()
    
    def connect(self, broker, port, client_id="mart-bag Controller"):
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                self.connected = True
                print(f"connected to broker {broker}:{port}")
            else:
                print("Failed to connect, return code %d\n", rc)

        
            
        if self.client:
            self.client.disconnect()
            time.sleep(0.5)
            self.connected = False

        self.client.on_connect = on_connect
        self.client.on_message = self.__message_call_back 
        self.client.connect(broker, port)
        self.client.loop_start()
        while self.connected == False:
            print(f"connecting to broker {broker}:{port}...") 
            time.sleep(0.5)
        self.client.subscribe("/test/send_device_state")
        #self.client.subscribe("/test/test_smart_bag_devices")
        self.update_device_state()
    def __message_call_back(self, client, userdata, msg):
        #print("hello")
        if msg.topic == "/test/send_device_state":
            #print(msg.payload.decode())
            self.device_status = json.loads(msg.payload.decode())
            #print(f'updated device_status : {self.device_status}')
            self.role_to_device = defaultdict(lambda:None)
            for device in self.device_status:
                role = self.device_status[device]["role"]
                if role != None:
                    self.role_to_device[role] = device
            print(f"Updated Devices:")
            for role in self.role_to_device:
                print(f"{role} : {self.role_to_device[role]}")
            print()
        else:
            print(f"message from {msg.topic} : {msg.payload.decode()}")

    def update_device_state(self):
        if self.client == None: 
            print("not connected to broker, cannot update device state")
            return 
        self.client.publish("/test/request_device_state","please send state at /test/send_device_state", qos=1)
    
    def role_is_available(self, role):
        return self.role_to_device[role] != None
    def available_roles(self):
        return set(self.role_to_device.keys())
    def set_val(self, role, value):
        if self.role_to_device[role] != None:
            device = self.role_to_device[role]
            self.client.publish(device, f'{value}')


if __name__ == "__main__":
    controller = Bags_Controller()
    controller.connect("broker.emqx.io", 1883)
    while True:
        print(controller.available_roles())
        time.sleep(5)