#include <WiFi.h> // Include WIFi Library for ESP32
#include <WebServer.h> // Include WebSwever Library for ESP32
#include <ArduinoJson.h> // Include ArduinoJson Library
#include <WebSocketsServer.h>  // Include Websocket Library
#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
const int potPin1 = 34;
const int potPin2 = 35;
const char* ssid = "Xuong_1";  // Your SSID
const char* password = "68686868"; // Your Password

int interval = 500; // virtual delay
unsigned long previousMillis = 0; // Tracks the time since last event fired

#include "webpage.h"
String jsonString; // Temporary storage for the JSON String
String pin_status = ""; // Holds the status of the pin
uint16_t tempVal; // holds the temperature value
uint16_t humVal;// holds the Humidity value
uint16_t lightVal, potVal;

//DHT dht(DHTPIN, DHTTYPE); // create instance for DHT sensor
WebServer server(80);  // create instance for web server on port "80"
WebSocketsServer webSocket = WebSocketsServer(81);  //create instance for webSocket server on port"81"

void setup() {
  dht.begin();
  // put your setup code here, to run once:
  pinMode(2, OUTPUT); // Set PIN22 As output(LED Pin)
  Serial.begin(115200); // Init Serial for Debugging.
  WiFi.begin(ssid, password); // Connect to Wifi 
  while (WiFi.status() != WL_CONNECTED) { // Check if wifi is connected or not
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  // Print the IP address in the serial monitor windows.
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Initialize a web server on the default IP address. and send the webpage as a response.
  server.on("/", []() {
    server.send(200, "text\html", web);
  });
  server.begin(); // init the server
  webSocket.begin();  // init the Websocketserver
  webSocket.onEvent(webSocketEvent);  // init the webSocketEvent function when a websocket event occurs 
//  dht.begin(); // Init DHT sensor
}

void loop() {
  server.handleClient();  // webserver methode that handles all Client
  webSocket.loop(); // websocket server methode that handles all Client
  unsigned long currentMillis = millis(); // call millis  and Get snapshot of time
  if ((unsigned long)(currentMillis - previousMillis) >= interval) { // How much time has passed, accounting for rollover with subtraction!
    update_temp_hum(); // update temperature data.
    update_light();   //update light data
    update_webpage(); // Update Humidity Data
    update_potHG();
    previousMillis = currentMillis;   // Use the snapshot to set track time until next event
  }
}

// This function gets a call when a WebSocket event occurs
void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED: // enum that read status this is used for debugging.
      Serial.print("WS Type ");
      Serial.print(type);
      Serial.println(": DISCONNECTED");
      break;
    case WStype_CONNECTED:  // Check if a WebSocket client is connected or not
      Serial.print("WS Type ");
      Serial.print(type);
      Serial.println(": CONNECTED");
      if (digitalRead(22) == HIGH) {  //check if pin 22 is high or low
        pin_status = "ON";
        update_webpage(); // update the webpage accordingly
      }
      else {                          
        pin_status = "OFF"; //check if pin 22 is high or low
        update_webpage();// update the webpage accordingly
      }
      break;
    case WStype_TEXT: // check responce from client
      Serial.println(); // the payload variable stores teh status internally
      Serial.println(payload[0]);
      if (payload[0] == '1') { 
        pin_status = "ON";
        digitalWrite(2, HIGH);               
      }
      if (payload[0] == '0') {
        pin_status = "OFF";
        digitalWrite(2, LOW);             
      }
      break;
    default:
      break;
  }
}

void update_temp_hum(){
   // Read temperature as Celsius (the default)
  humVal = dht.readHumidity();
  // Read hum as Celsius (the default)
  tempVal = dht.readTemperature();
}

void update_light(){
  lightVal = analogRead(potPin1);// Read analog
}

void update_potHG(){
  potVal = analogRead(potPin2);// Read analog
}

void update_webpage()
{
  StaticJsonDocument<100> doc;
  // create an object
  JsonObject object = doc.to<JsonObject>();
  object["PIN_Status"] = pin_status ;
  object["Temp"] = tempVal ;
  object["Hum"] = humVal ;
  object["Light"] = lightVal;
  object["PotHG"] = potVal;  
  serializeJson(doc, jsonString); // serialize the object and save teh result to teh string variable.
  Serial.println( jsonString ); // print the string for debugging.
  webSocket.broadcastTXT(jsonString); // send the JSON object through the websocket
  jsonString = ""; // clear the String.
}
