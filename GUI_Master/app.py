from flask import Flask , render_template, jsonify

app = Flask(__name__)

@app.route('/')
def hello():
    return render_template('index.html')

@app.route('/scan_devices')
def scan_devices():
    device_names = ['Wemo-11aa','Wemo-22bb','Wemo-33cc','Wemo-44dd']
    print(jsonify(devices=device_names))
    return jsonify(devices=device_names)