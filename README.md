# Wireless Sensor Network.
This project consists on design a small sensor network based on interconnected [ESP8266](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf) micro-controllers. 

## Team Members
1. Ali Altaheri      - H00417357
2. Ángel García      - H00417698
3. Baudouin Belpaire - H00445613
4. Diego Velasquez   - H00434521

## Purpose of the system 
The system’s purpose is to develop an IoT platform monitoring the chairs status in a remote environment. The chairs are equipped with pressure mat sensors detecting the presence of a person sitting on it. Thus, each chair represents a sensing node and communicates wirelessly its status with a bridge node. The bridge node consists of a bridge in the network gathering in a single point all the data from the different places in the environment. The gathered data is meant to be transferred through wired communication to a WiFi gateway enabling to connect the local application to an Internet connected one. The connection to an Internet access point is ensured through a MQTT broker communicating thanks to the MQTT protocol. This part is crucial as it allows to access from anywhere in the world to the local environment. From there, any users can be notified on the environment status and take the control remotely.

<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Architecture.png" alt="Architecture">

## Hardware Design
In the coursework, the sensing nodes, the bridge, and the WiFi gateway have been implemented on 4 different ESP8266 boards. The ESP8266 board is an optimal solution for IoT systems as it embeds a microprocessor, GPIO, a WiFi antenna for wireless communications, and is energy efficient. The WiFi antenna is used by the nodes to communicates over ESP-Now technology using the same WiFi bandwidth while consuming less power. The pressure mats are replaced by push buttons returning a digital signal (0 or 1) to the GPIO 5 on the board. The wired communication between the bridge and the gateway is performed through Serial communication using the TX/RX ports. Finally, the gateway communicates with the online Moquitto Broker thanks to the MQTT protocol used on a private WiFi network provided by Heriot-Watt.

## Software Design
Concerning the software part, the Arduino IDE was used to program the different ESP8266 nodes. The WiFi, ESP-Now, ArduinoJson, and PubSubClient libraries were used to facilitate the code implementation.

### Sensing node flowchart:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Sensing_node_flowchart.png" alt="Sensing Flowchart" width="200" height="200">

### Bridge flowchart:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Bridge_flowchart.png" alt="Bridge Flowchart">

### WiFi Gateway flowchart:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Gateway_flowchart.png" alt="WF Gateway Flowchart">

The MQTT Explorer application was also used to subscribe to the different topics used in the network allowing to debug the code implemented. 
Finally, the Node-RED tool was used to visualise the data in real-time, to store it on a local database using MongoDB application, and to modify remotely the network parameters such as the working state, or the data reporting interval.
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Node-RED Pipeline.png" alt="Node-RED Pipeline">

## Performance Tests

## Instructions for Users

## Problems & Solutions

## Critical Analysis 

## Future Work 


