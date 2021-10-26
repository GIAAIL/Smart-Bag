#!/usr/bin/python3
# created by Nemo on 2021/8/13 for drone project

import time
import math
import random
import mqtt
import numpy as np

# Initialize mqtt client ----------------------------------------------------------

broker = '192.168.0.100'
port = 8080

client_id = f'python-mqtt-{random.randint(0, 1000)}'
# username = 'nemo'
# password = 'public'

cl = mqtt.client(broker, port, client_id)

while(not cl.connected):
    time.sleep(1)
    print(f'connecting to mqtt broker at {broker}:{port}')

cl.subscribe("/test/sensor/Time")
cl.subscribe("/test/ping")
# Read log files-----------------------------------------------------------------------------------

while True:
    command = input()
    if command == 'end':
        break
    elif command == 'ping':
        for i in range(5):
            publish_nano = time.time_ns()
            time_code = str(publish_nano)
            cl.publish('/test/ping', time_code)
            while True:
                msg = cl['/test/ping']
                #print(msg,time_code)
                if msg is not None and  msg[0] == time_code + 'reply':
                    receive_nano = time.time_ns()
                    break
            duration = receive_nano - publish_nano
            duration = duration / (10 ** 6)
            print(f'ping response time: {duration} ms')
            time.sleep(0.5)
    elif command.isnumeric():

        cl.publish("/test/command/Motor1", command)
        cl.publish("/test/command/Motor2", command)
    else:
        print('invalid command')

print("turn off motor and LED")
print("reset motors")

cl.publish('/test/command/LED', "0")

cl.publish("/test/command/Motor1", "0")

cl.publish("/test/command/Motor2", "0")

time.sleep(0.1)
