#include<ESP8266WiFi.h>

void setup() {
  //Setup code here:
  Serial.begin(9600);
  delay(500);
  Serial.print("Mac Address of Node MCU is");
  Serial.println(WiFi.macAddress());
}

void loop() {
  // to add main code
}
