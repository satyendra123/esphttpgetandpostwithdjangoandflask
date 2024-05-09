'''
from flask import Flask, request

app = Flask(__name__)

@app.route('/update-sensor', methods=['POST'])
def update_sensor():
    data = request.form
    print("Received data:")
    for key, value in data.items():
        print(f"{key}: {value}")
    return "Data received", 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=1880)

'''

from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/update-sensor/', methods=['POST'])
def update_sensor():
    data = request.json
    device_id = data.get('device_id')
    sensor_type = data.get('type')
    value = data.get('value')

    temperature = None
    humidity = None

    if device_id and sensor_type and value:
        if sensor_type == 'Temperature':
            temperature = value
        elif sensor_type == 'Humidity':
            humidity = value

        print(f"Received {sensor_type} data from device {device_id}: {value}")

        return jsonify({ 'relay':'OPENEN','message': 'success'}), 200
    else:
        return jsonify({'error': 'Invalid data format'}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=1880)

