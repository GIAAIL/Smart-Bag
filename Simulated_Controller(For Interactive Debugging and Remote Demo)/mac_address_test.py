#!/usr/bin/python3
# created by Nemo on 2021/8/13 for drone project

import time
from time import sleep
import math
import random
import mqtt
import numpy as np

# Initialize mqtt client ----------------------------------------------------------
"""
broker = '192.168.0.100'
port = 8080

"""
broker = '127.0.0.1'
port = 9000

client_id = f'Smart-Bags Controller *(python script)'
# username = 'nemo'
# password = 'public'

cl = mqtt.client_with_memory(broker, port, client_id)
test_topic = '/test/ping'


while(not cl.connected):
    sleep(1)
    print(f'connecting to mqtt broker at {broker}:{port}')

cl.subscribe(test_topic)

sleep(0.5)

# ping current devices, adding them into `members` array
cl[test_topic].clear()
cl.publish(test_topic, "who_are_you")
sleep(0.3)
members = cl[test_topic].copy()

# Read log files-----------------------------------------------------------------------------------

while True:
    command = input()
    if command == 'end':
        break
    elif command == 'ping':
        cl[test_topic].clear()
        cl.publish(test_topic, "who_are_you")
        sleep(0.3)
        members = cl[test_topic].copy() #updating members array
        members.remove("who_are_you")
        print("subscribing devices: ")
        for m in members:
            cl.subscribe(m)
            print(f'\t Device: {m}')
    elif command.isnumeric():
        for m in members:
            cl.publish(m, command)
    else:
        print('invalid command')

print("turning off all devices")
for m in members:
    cl.publish(m, '0')

time.sleep(0.1)
