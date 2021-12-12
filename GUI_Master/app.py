from flask import Flask , render_template, jsonify, request
import random

app = Flask(__name__)

@app.route('/')
def hello():
    return render_template('index.html')

@app.route('/scan_devices')
def scan_devices():
    device_names = {
        "Wemo-fc:34:97:b8:0c:15":{
            "role": None,
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-cg:e4:g7:aa:0d:53":{
            "role": None,
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-dc:64:a4:1b:75:43":{
            "role": None,
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-44:aa:c9:c7:76:33":{
            "role": None,
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-cc:bd:b8:3d:00:13":{
            "role": None,
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-c7:33:64:79:18:d4":{
            "role": None,
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-aa:dd:a8:42:0a:16":{
            "role": None,
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        }
    }
    return jsonify(status="success",devices=device_names)

@app.route('/remove_role', methods=['POST'])
def remove_role():
    json_data = request.get_json()
    print("Remove Role: device:{}, role:{}".format(json_data["device_name"], json_data["role_name"]))
    return jsonify(status="success")

@app.route('/assign_role', methods=['POST'])
def assign_role():
    json_data = request.get_json()
    print("Assign Role: device:{}, role:{}".format(json_data["device_name"], json_data["role_name"]))
    return jsonify(status="success")

@app.route('/test_device', methods=['POST'])
def test_device():
    json_data = request.get_json()
    print("Test Device: device:{}".format(json_data["device_name"]))
    return jsonify(status="success")

@app.route('/remove_device', methods=['POST'])
def remove_device():
    json_data = request.get_json()
    print("Remove Device: device:{}".format(json_data["device_name"]))
    return jsonify(status="success")