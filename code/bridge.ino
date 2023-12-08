#include <espnow.h>
#include <ESP8266WiFi.h>

//Baudouin's MAC Adress: 08:3A:8D:E3:B3:AF
//Diego's MAC Address : 34:94:54:97:52:78
//Ali's MAC Address : 08:3A:8D:EF:09:D8

// REPLACE WITH THE MAC Address of your receiver 
uint8_t node1[] = {0x34, 0x94, 0x54, 0x97, 0x52, 0x78}; //Diego
uint8_t node2[] = {0x08, 0x3A, 0x8D, 0xEF, 0x09, 0xD8}; //Ali

// Define variables to store local button state
bool remote_button;

int numberNode;
long time1;

//Initial node states
bool state1=1; 
bool state2=1;
int value = 1;
int NetworkStateTmp = 1;
String myString;

// Define variables to store incoming readings
bool send_state;

// Variable to store if sending data was successful
String success;

char user_input;

//Structure example to send data
//Message structure at sending
typedef struct struct_message_send {
  bool state;
} struct_message_send;

//Message structure at arrival
typedef struct struct_message_rcv {
  bool state;
  int node;
} struct_message_rcv;

//Msg structure to send network state
struct_message_send stateMode;

// Create a struct_message to hold the button state
struct_message_rcv incomingReadings;

// Callback function called when data is sent
void OnDataSent(uint8_t * mac_addr, uint8_t status) {
 Serial.print("\r\nLast Packet Send Status:\t");
 Serial.println(status == 0 ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  remote_button = incomingReadings.state;
  numberNode = incomingReadings.node;
  Serial.print("Sensing node : ");
  Serial.println(numberNode);
  Serial.print("Remote button state : ");
  Serial.println(remote_button);
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Define the bridge as a master and slave node
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Add peer        
  if (esp_now_add_peer(node1, ESP_NOW_ROLE_COMBO, 1, NULL, 0) != 0){
   Serial.println("Failed to add peer");
   return;
  }

  if (esp_now_add_peer(node2, ESP_NOW_ROLE_COMBO, 1, NULL, 0) != 0){
   Serial.println("Failed to add peer");
   return;
  }

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  time1 = millis();

}
 
void loop() {

  if(numberNode == 1){
    state1 = remote_button;}
  if(numberNode == 2){
    state2 = remote_button;}

  //Get the network state from Serial communication
  while(Serial.available()){ 
    char inChar = Serial.read();
    myString = "";
    if(inChar == '<'){
      myString = Serial.readString();
      value = myString[0]-48; //Convert ASCII to DEC
      break;
    }
  }
  
  //Check for network state updates, if so send message to sensing nodes
  if(NetworkStateTmp != value){
    Serial.print("Network state to : ");
    Serial.println(value);
    stateMode.state=value;
    NetworkStateTmp = value;
    esp_now_send(node1, (uint8_t *) &stateMode, sizeof(stateMode));
    esp_now_send(node2, (uint8_t *) &stateMode, sizeof(stateMode));
  }

  //Send the node state data over serial communication to WiFi Gateway
  if(millis()-time1>1000){
    time1=millis();
    Serial.print('<');
    Serial.print(static_cast<char>('0' + 1));
    Serial.print(",");
    Serial.print(state1 ? '1' : '0');
    Serial.print(",");
    Serial.print(static_cast<char>('0' + 2));
    Serial.print(",");
    Serial.print(state2 ? '1' : '0');
    Serial.println('>');
  }
}
