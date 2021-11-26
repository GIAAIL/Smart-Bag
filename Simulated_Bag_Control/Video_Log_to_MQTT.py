#!/usr/bin/python3
# created by Nemo on 2021/8/13 for drone project

import vlc
import time
import math
import random
import mqtt

# Initialize mqtt client ----------------------------------------------------------

broker = '127.0.0.1'
port = 9000

client_id = f'python-mqtt-{random.randint(0, 1000)}'
# username = 'nemo'
# password = 'public'

cl = mqtt.client(broker, port, client_id)

while(not cl.connected):
    time.sleep(1)
    print(f'connecting to mqtt broker at {broker}:{port}')

cl.subscribe("test/sensor/Time")

# Read log files-----------------------------------------------------------------------------------

duration = 30  #distance update duration (in mili-seconds)

# Read Logs from file
log_file  = open('datalog.txt', 'r')
logs = log_file.readlines()
dists = []
for l in logs:
    d = map(int, l.split('[')[1][:-2].split(','))
    dists.append(list(d))


# Start VLC player ---------------------------------------------------------------
player = vlc.MediaPlayer('CityPark.mp4')
player.play()

# Wait for the player to start
time.sleep(0.3)

# get frame rate
fps = player.get_fps()
print(f"fps = {fps}")
if fps == 0:
    # if fps is unavailable, default to 30/sec
    fps = 30




last_player_time = 0
estimated_player_time = 0


while player.is_playing() == 1:


    # use below block to pause and check that frame match with log
    """
    player.pause()
    input()
    player.pause()
    """


    # Because player.get_time update very slow (about 4fps)
    # I need to manually track current play time and
    # Reset estimated time when player update its get_time
    player_time = player.get_time()
    if player_time == last_player_time:
        estimated_player_time += duration
    else :
        last_player_time = player_time
        estimated_player_time = player_time

    current_frame_index = (estimated_player_time * fps) // 1000
    print(f"frame = {current_frame_index:5} ", end='')

    print(dists[math.floor(current_frame_index)])
    dists_sample  = dists[math.floor(current_frame_index)][0::2]

    pwms = ["0" if x > 3000 else str(int((1 - x/3000) * 1024)) for x in dists_sample]

    print(f"distances = {dists_sample[0]:5.1f} / {dists_sample[1]:5.1f} / {dists_sample[2]:5.1f}", end='')
    print(f"pwn     = {pwms[0]:7} / {pwms[1]:7} / {pwms[2]:7}")

    cl.publish('test/command/LED', pwms[1])

    cl.publish("test/command/Motor1", pwms[0])

    cl.publish("test/command/Motor2", pwms[2])

time.sleep(0.1)
print("turn off motor and LED")
cl.publish('test/command/LED', "0")

cl.publish("test/command/Motor1", "0")

cl.publish("test/command/Motor2", "0")
