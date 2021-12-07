from flask import Flask , render_template, jsonify, request
import random

app = Flask(__name__)

@app.route('/')
def hello():
    return render_template('index.html')

@app.route('/scan_devices')
def scan_devices():
    device_names = {
        "Wemo-11aa":{
            "role": "B23",
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-22aa":{
            "role": "B21",
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-33aa":{
            "role": "F32",
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-44aa":{
            "role": "F42",
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-55aa":{
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