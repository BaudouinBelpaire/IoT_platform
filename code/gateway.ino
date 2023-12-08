#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <time.h>

// --------------------------------------------------------------------------------------------
//        UPDATE CONFIGURATION TO MATCH YOUR ENVIRONMENT
// --------------------------------------------------------------------------------------------

// MQTT connection details
#define MQTT_HOST "test.mosquitto.org"
#define MQTT_PORT 1883
#define MQTT_DEVICEID "d:hwu:esp8266:bb2043"
#define MQTT_USER "" // no need for authentication, for now
#define MQTT_TOKEN "" // no need for authentication, for now
#define MQTT_TOPIC "ESP8266/Group5/State"
#define MQTT_TOPIC_NETWORK "ESP8266/Group5/Network"
#define MQTT_TOPIC_INTERVAL "ESP8266/Group5/Interval"


// --------------------------------------------------------------------------------------------
//        UPDATE CONFIGURATION TO MATCH YOUR ENVIRONMENT
// --------------------------------------------------------------------------------------------

//Timezone info
#define TZ_OFFSET -1  //Hours timezone offset to GMT (without daylight saving time)
#define TZ_DST    60  //Minutes timezone offset for Daylight saving


// Add WiFi connection information
char ssid[] = "RALT_RIoT";  // your network SSID (name) RALT_RIoT  Baudouin's WiFi ELS-Students
char pass[] = "raltriot";  // your network password raltriot jesuisunrobot ELS Students

bool state;
int node;

//const char START_CHAR = '<';
//const char END_CHAR = '>';

// --------------------------------------------------------------------------------------------
//        SHOULD NOT NEED TO CHANGE ANYTHING BELOW THIS LINE
// --------------------------------------------------------------------------------------------

// variables to hold data
StaticJsonDocument<100> jsonDoc;
JsonObject payload = jsonDoc.to<JsonObject>();
JsonObject status = payload.createNestedObject("d");
StaticJsonDocument<100> jsonReceiveDoc;
JsonObject cmdData;
static char msg[50];

int32_t ReportingInterval = 5;  // Reporting Interval seconds
bool NetworkState = 1;  //Initial network state
bool NetworkState_tmp = 1;  //Temporary previous state of the network

String receivedString = ""; // a string to hold incoming data
bool stringComplete = false; // whether the string is complete

unsigned time1;
unsigned globaltime;

char mymsg;
String sendingMsg;

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    receivedString += inChar;
    if (inChar == '>') {
      stringComplete = true;
    }
  }
}

// MQTT objects
void callback(char* topic, byte* payload, unsigned int length);
WiFiClient wifiClient;
PubSubClient mqtt(MQTT_HOST, MQTT_PORT, callback, wifiClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] : ");
  
  payload[length] = 0; // ensure valid content is zero terminated so can treat as c-string
  Serial.println((char *)payload);
  DeserializationError err = deserializeJson(jsonReceiveDoc, (char *)payload);
  if (err) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  } else {
    cmdData = jsonReceiveDoc.as<JsonObject>();
    if (0 == strcmp(topic, MQTT_TOPIC_NETWORK)) {
      //valid message received
      NetworkState = cmdData["Network"].as<bool>(); //Get the network state value received as JSON object
      Serial.print("Network State has been changed:");
      Serial.println(NetworkState);
      jsonReceiveDoc.clear();
    }

    else if (0 == strcmp(topic, MQTT_TOPIC_INTERVAL)) {
      //valid message received
      ReportingInterval = cmdData["Interval"].as<int32_t>(); //Get the reporting interval value received as JSON object
      Serial.print("Reporting Interval has been changed:");
      Serial.println(ReportingInterval);
      jsonReceiveDoc.clear();
    } 
    else {
      Serial.println("Unknown command received");
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  // Start serial console
  Serial.begin(115200);
  Serial.setTimeout(2000);
  while (!Serial) { }
  Serial.println();
  Serial.println("ESP8266 Sensor Application");

  // Start WiFi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");

  // Set time from NTP servers
  configTime(TZ_OFFSET * 3600, TZ_DST * 60, "1.pool.ntp.org", "0.pool.ntp.org");
  Serial.println("\nWaiting for time");
  unsigned timeout = 5000;
  unsigned start = millis();
  while (millis() - start < timeout) {
      time_t now = time(nullptr);
      if (now > (2018 - 1970) * 365 * 24 * 3600) {
          break;
      }
      delay(100);
  }
  delay(1000); // Wait for time to fully sync
  Serial.println("Time sync'd");
  
  time_t now = time(nullptr);
  globaltime = time(nullptr);
  time1 = millis();
  Serial.println(ctime(&now));

  // Connect to MQTT broker
  if (mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
    Serial.println("MQTT Connected");
    mqtt.subscribe(MQTT_TOPIC_NETWORK);
    mqtt.subscribe(MQTT_TOPIC_INTERVAL);

  } else {
    Serial.println("MQTT Failed to connect!");
    ESP.reset();
  }
  
}

void loop() {
  mqtt.loop();
    while (!mqtt.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (mqtt.connect(MQTT_DEVICEID, MQTT_USER, MQTT_TOKEN)) {
        Serial.println("MQTT Connected");
        mqtt.subscribe(MQTT_TOPIC_NETWORK);
        mqtt.subscribe(MQTT_TOPIC_INTERVAL);
        mqtt.loop();
      } else {
        Serial.println("MQTT Failed to connect!");
        delay(5000);
      }
    }

    //Read data sent over serial communication for MQTT protocol
    if (stringComplete) {
      // split the receivedString at commas and convert to integers
      int index1 = receivedString.indexOf(','); //get 1st index of the data 
      int node1 = receivedString.substring(1, index1).toInt(); //get node1 ID and convert to integer
      int index2 = receivedString.indexOf(',', index1 + 1); //get index of 2nd value of array
      int state1 = receivedString.substring(index1 + 1, index2).toInt(); //get state1 and convert to integer
      index1 = receivedString.indexOf(',', index2 + 1); //get the next data index
      int node2 = receivedString.substring(index2 + 1, index1).toInt(); // get node2 ID and convert to integer
      int state2 = receivedString.substring(index1 + 1, receivedString.length() - 1).toInt(); //get state2 and convert to integer

      // print the values
      Serial.print("Node1: ");
      Serial.println(node1);
      Serial.print("State1: ");
      Serial.println(state1);
      Serial.print("Node2: ");
      Serial.println(node2);
      Serial.print("State2: ");
      Serial.println(state2);
      int myArray[4] = {node1, state1, node2, state2}; //Array containing all data to be sent
      sendingMsg="";
      for(int i = 0; i<sizeof(myArray)/sizeof(myArray[0]);i++){
        sendingMsg+=String(myArray[i]); //Convert integer values into String
        if(i<sizeof(myArray)/sizeof(myArray[0])-1){
          sendingMsg += ",";
        }
      }
      receivedString = "";
      stringComplete = false;
    }

    if(NetworkState != NetworkState_tmp){ //Check is network state has changed
      Serial.print("Network State: ");
      Serial.print("<");
      Serial.print(NetworkState); //Write on Serial communication the <network state>
      Serial.println(">");
      NetworkState_tmp = NetworkState; //Update temporary network state value
    }

    time_t now = time(nullptr);

    // Print Message to console in JSON format
    status["data"] = sendingMsg;
    serializeJson(jsonDoc, msg, 50);
    Serial.println(msg);
    
    if (!mqtt.publish(MQTT_TOPIC, msg)) {
      Serial.println("MQTT Publish failed");
    }
  
  Serial.print("ReportingInterval :");
  Serial.print(ReportingInterval);
  Serial.println();
  for (int i = 0; i < ReportingInterval; i++) { 
    mqtt.loop();
    delay(1000);
  }
}

