const clientId = 'GUI_Master_' + Math.random().toString(16).substr(2, 8)
console.log("This is clientId: " + clientId)
const host = 'ws://broker.emqx.io:8083/mqtt'
const options = {
  keepalive: 60,
  clientId: clientId,
  protocolId: 'MQTT',
  protocolVersion: 4,
  clean: true,
  reconnectPeriod: 1000,
  connectTimeout: 30 * 1000,
  will: {
    topic: 'WillMsg',
    payload: 'Connection Closed abnormally..!',
    qos: 0,
    retain: false
  },
}

console.log('Connecting mqtt client')
const client = mqtt.connect(host, options)

client.on('error', (err) => {
  console.log('Connection error: ', err)
  client.end()
})

client.on('reconnect', () => {
  console.log('Reconnecting...')
})

client.on('connect', () => {
  console.log('Client connected: ' + clientId)
  // Subscribe
  client.subscribe('/test/test_smart_bag_devices', { qos: 2 })
  client.subscribe('/test', { qos: 2 })
})

client.on('message',(topic, message) => {
  if(topic=='/test/test_smart_bag_devices'){
    if(message.toString() != "who_are_you"){
      let temp = message.toString().split(' ')
      let client_name = temp[0]
      let client_value = temp[1]
      
      if(connected_devices.hasOwnProperty(client_name)){
        connected_devices[client_name]["Control Value"] = client_value
      }
      else{
        connected_devices[client_name] = {
          role: null,
          "Control Value": client_value,
          "Connect Time": new Date().toLocaleString(),
          "last_control_time": new Date().getTime()
        }
      }
    }
  }
})


// Usage Examples: 


// Publish
//client.publish('/test/test_smart_bag_devices', 'ws connection demo...!\nwho_are_you', { qos: 2, retain: false })



// Received
/*
client.on('message', (topic, message, packet) => {
  console.log('Received Message: ' + message.toString() + '\nOn topic: ' + topic)
})
*/


// Unsubscribe
/*
client.unubscribe('testtopic', () => {
  console.log('Unsubscribed')
})
*/