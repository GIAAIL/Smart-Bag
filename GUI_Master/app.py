from flask import Flask , render_template, jsonify

app = Flask(__name__)

@app.route('/')
def hello():
    return render_template('index.html')

@app.route('/scan_devices')
def scan_devices():
    device_names = {
        "Wemo-11aa":{
            "role": "B11",
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-22aa":{
            "role": "B11",
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-33aa":{
            "role": "B11",
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-44aa":{
            "role": "B11",
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        },
        "Wemo-55aa":{
            "role": "B11",
            "Connect Time": "2019-01-01 12:00:00",
            "Control Value": "0"
        }
    }
    print(jsonify(devices=device_names))
    return jsonify(devices=device_names)