# tell bash to terminate script if any error occurrs
set -e

# start mosquitto server
mosquitto -c mosquitto.conf &
sleep 0.5
# add subscriber to print all messages
mosquitto_sub -v -h 127.0.0.1 -p 9000 -t '#'&
sleep 0.5
while true
do
    echo "server listening on 0.0.0.0:9000"
    sleep 3
done

trap ' kill $(jobs -p)'  SIGINT SIGTERM EXIT
