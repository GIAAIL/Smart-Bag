from mqtt import client as mqtt_client

class controller:
    def __init__(self):
        self.client = None
    
    def __del__(self):
        if self.client:
            self.client.disconnect()
    
    def connect(self, broker, port, client_id="mart-bag Controller"):
        if self.client:
            self.client.disconnect()
        self.client = mqtt_client(broker, port, client_id)
        self.client.subscribe("/test/send_device_state")
        self.update_device_state()

    def update_device_state(self):
        if self.client == None: 
            print("not connected to broker, cannot update device state")
            return 
        self.client.publish("/test/request_device_state","please send state at /test/send_device_state")
    
    def connected(self):
        if self.client == None:
            return False
        return self.client.connected

    def state_getter(self):
        if self.client == None: 
            print("not connected to broker, cannot get device state")
            exit(1)
        self.client.publish("/test/request_device_state","please send state at /test/send_device_state")
        return self.client["/test/send_device_state"]

    def state_setter(self, state):
        print("Warning: state_setter is not implemented")
        return
    
    def state_delete(self):
        print("Warning: state_delete is not implemented")
        return
    
    property("device_state", state_getter, state_setter, state_delete)
