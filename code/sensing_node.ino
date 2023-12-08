#include <espnow.h>
#include <ESP8266WiFi.h>

//Baudouin's MAC Adress: 08:3A:8D:E3:B3:AF
//Diego's MAC Address : 34:94:54:97:52:78
//Ali's MAC Address : 08:3A:8D:EF:09:D8$
//receiver mac adress: 0x34, 0x94, 0x54, 0x8D, 0x04, 0x20

// REPLACE WITH THE MAC Address of your receiver 
uint8_t bridge[] = {0x34, 0x94, 0x54, 0x8D, 0x04, 0x20}; //Receiver

// Define variables to store local button state
bool local_button;
bool tmp_local_button = 0;

// Define variables to store incoming readings
bool received_state=1;

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message_rcv {
  bool state;
} struct_message_rcv;

typedef struct struct_message_send {
  bool state;
  int node;
} struct_message_send;

//Message structure for sending message from push button
struct_message_send localReadings;

//Message structure to receive network state
struct_message_rcv incomingReadings;

//esp_now_peer_info_t peerInfo;

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
  received_state = incomingReadings.state;
  Serial.print("ESP state : ");
  Serial.println(received_state);
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

  // Define the node1 as a master and slave node
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Add peer        
  if (esp_now_add_peer(bridge, ESP_NOW_ROLE_COMBO, 1, NULL, 0) != 0){
   Serial.println("Failed to add peer");
   return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(5, INPUT_PULLUP);
}
 
void loop() {
  //Put the device out of service if network state is 0 until receives 1
  while(received_state==1){
    local_button = digitalRead(5); //Read digital signal from GPIO 5
    if (local_button!=tmp_local_button){ //If push button state has changed, notify the bridge
      // Set values to send
      localReadings.state = local_button;
      localReadings.node = 1;
      // Send message via ESP-NOW
      esp_now_send(bridge, (uint8_t *) &localReadings, sizeof(localReadings));
      tmp_local_button = local_button;
    }
    //Delay for 5s
    delay(5000);
  }
}
