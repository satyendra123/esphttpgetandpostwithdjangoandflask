#include <WiFi.h>
#include <HTTPClient.h>

WiFiClient client;
HTTPClient http;

String device_id = "Device0001";
String ssid = "Airtel_tejv_3002";
String password = "air73137";

float DHT_Temperature = 22.4;
float DHT_Humidity = 34;

String HTTPS_POST_URL = "http://192.168.1.16:1880/update-sensor/";
String HTTPS_GET_URL = "http://192.168.1.16:1880/get-sensor/";

void setup() {
  Serial.begin(115200);
  setup_wifi();
}

void loop() {

  String pkt = "{";
  pkt += "\"device_id\": \"" + device_id + "\", ";
  pkt += "\"type\": \"Temperature\", ";
  pkt += "\"value\": " + String(DHT_Temperature) + "";
  pkt += "}";
  HTTPS_POST(HTTPS_POST_URL, pkt);

  String pkt2 = "{";
  pkt2 += "\"device_id\": \"" + device_id + "\", ";
  pkt2 += "\"type\": \"Humidity\", ";
  pkt2 += "\"value\": " + String(DHT_Humidity) + "";
  pkt2 += "}";
  HTTPS_POST(HTTPS_POST_URL, pkt2);

  HTTPS_GET(HTTPS_GET_URL);

  delay(1000);
}

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void HTTPS_POST(String HTTPS_POST_URL, String PostPacket) 
{
  Serial.println("\nPosting to: " + HTTPS_POST_URL);
  Serial.println("PostPacket: " + PostPacket);

  Serial.println("Connecting to server..");
  if (http.begin(client, HTTPS_POST_URL)) {
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(PostPacket);
    if (httpCode > 0) {
      Serial.println("httpCode: " + String(httpCode));
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_CREATED) {
        String ServerResponse = http.getString();
        Serial.println("ServerResponse: " + ServerResponse);
      }
    } else {
      Serial.print("Failed to POST. Error: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
  } else {
    Serial.print("Failed to connect to server");
  }
  http.end();
}

void HTTPS_GET(String HTTPS_GET_URL) 
{
  Serial.println("\nGetting from: " + HTTPS_GET_URL);

  Serial.println("Connecting to server..");
  if (http.begin(client, HTTPS_GET_URL)) {
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.println("httpCode: " + String(httpCode));
      if (httpCode == HTTP_CODE_OK) {
        String ServerResponse = http.getString();
        Serial.println("\nServerResponse: " + ServerResponse);
      }
    } else {
      Serial.print("Failed to GET. Error: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
  } else {
    Serial.print("Failed to connect to server");
  }
  http.end();
}



/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
/*
// this is the working code
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Airtel_tejv_3002";
const char* password = "air73137";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.1.16:1880/update-sensor";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";
      String httpRequestData = "sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";             
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      //http.addHeader("Content-Type", "application/json");
      //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
*/

// this is my flask code which receives the data
'''
from flask import Flask, request

app = Flask(__name__)

@app.route('/update-sensor/', methods=['POST'])
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

        return jsonify({
            'message': 'Data received',
            'temperature': temperature,
            'humidity': humidity
        }), 200
    else:
        return jsonify({'error': 'Invalid data format'}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=1880)
