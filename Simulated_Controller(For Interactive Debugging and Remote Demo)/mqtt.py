# created by Nemo at 2021/8/27 for smart drone project
# modified from https://www.emqx.com/en/blog/how-to-use-mqtt-in-python

import paho.mqtt.client as mqtt
from collections import defaultdict



# This mqtt client only cache most recent message
class client:
    def __init__(self, broker, port, client_id):
        self.connected=False
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                self.connected = True
            else:
                print("Failed to connect, return code %d\n", rc)

        self.broker = broker
        self.prot = port
        self.client_id = client_id
        self.client = mqtt.Client()
        self.client.connect(broker, port, 60)
        self.client.on_connect = on_connect
        self.client.on_message = self.__message_call_back__
        self.client.loop_start()
        self.subscribes = set()
        self.cache = defaultdict(lambda: None)
    def subscribe(self, topic):
        if topic not in self.subscribes:
            self.subscribes.add(topic)
            self.client.subscribe(topic)
    # def add_call_back(self, on_message):
    def publish(self, topic, msg):
        self.client.publish(topic, msg)
    def __message_call_back__(self, client, userdata, msg):
        self.cache[msg.topic] = msg.payload.decode(), userdata
        #print(f'from topic {msg.topic}, message={msg.payload.decode()}, user={userdata}')
    def __getitem__(self, index):
        return self.cache[index]
    def disconnect(self):
        self.client.disconnect()
    def __del__(self):
        self.disconnect()


# this mqtt client remember past messages in a list
# use `client[topic].clear()` to clear the history of a particular topic

class client_with_memory:
    def __init__(self, broker, port, client_id):
        self.connected=False
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                self.connected = True
            else:
                print("Failed to connect, return code %d\n", rc)

        self.broker = broker
        self.prot = port
        self.client_id = client_id
        self.client = mqtt.Client()
        self.client.connect(broker, port, 60)
        self.client.on_connect = on_connect
        self.client.on_message = self.__message_call_back__
        self.client.loop_start()
        self.subscribes = set()
        self.cache = defaultdict(lambda:[])
    def subscribe(self, topic):
        if topic not in self.subscribes:
            self.subscribes.add(topic)
            self.client.subscribe(topic)
    # def add_call_back(self, on_message):
    def publish(self, topic, msg):
        self.client.publish(topic, msg)
    def __message_call_back__(self, client, userdata, msg):
        self.cache[msg.topic].append(msg.payload.decode())
        #print(f'from topic {msg.topic}, message={msg.payload.decode()}, user={userdata}')
    def __getitem__(self, index):
        return self.cache[index]
    def disconnect(self):
        self.client.disconnect()
    def __del__(self):
        self.disconnect()



# def on_message(client, userdata, msg):
# print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")
