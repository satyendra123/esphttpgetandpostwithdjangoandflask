# esphttpgetandpostwithdjangoandflask
actually i want to send the sensor data by http post method and flask as a server and i am getting the data. my esp32 and the mylaptop are connected with the same wifi so both are in same network. and my esp32 ip is 192.168.1.35 and my pc ip is for wifi is 192.168.1.16. so in my arduino code i will send the post data to this ip only.

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


/* this is the flask server
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

*/
