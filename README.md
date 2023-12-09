# Wireless Sensor Network
This project consists on design a small sensor network based on interconnected [ESP8266](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf) micro-controllers. 

## Team Members
1. Ali Altaheri      - H00417357
2. Ángel García      - H00417698
3. Baudouin Belpaire - H00445613
4. Diego Velasquez   - H00434521

## Purpose of the system 
The system’s purpose is to develop an IoT platform monitoring the chairs status in a remote environment. The chairs are equipped with pressure mat sensors detecting the presence of a person sitting on it. Thus, each chair represents a sensing node and communicates wirelessly its status with a bridge node. The bridge node consists of a bridge in the network gathering in a single point all the data from the different places in the environment. The gathered data is meant to be transferred through wired communication to a WiFi gateway enabling to connect the local application to an Internet connected one. The connection to an Internet access point is ensured through a MQTT broker communicating thanks to the MQTT protocol. This part is crucial as it allows to access from anywhere in the world to the local environment. From there, any users can be notified on the environment status and take the control remotely.


## Hardware Design
In the coursework, the sensing nodes, the bridge, and the WiFi gateway have been implemented on 4 different ESP8266 boards. The ESP8266 board is an optimal solution for IoT systems as it embeds a microprocessor, GPIO, a WiFi antenna for wireless communications, and is energy efficient. The WiFi antenna is used by the nodes to communicates over ESP-Now technology using the same WiFi bandwidth while consuming less power. The pressure mats are replaced by push buttons returning a digital signal (0 or 1) to the GPIO 5 on the board. The wired communication between the bridge and the gateway is performed through Serial communication using the TX/RX ports. Finally, the gateway communicates with the online Moquitto Broker thanks to the MQTT protocol used on a private WiFi network provided by Heriot-Watt.

### System Architecture:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Architecture2.png" alt="Architecture">

## Software Design
Concerning the software part, the Arduino IDE was used to program the different ESP8266 nodes. The 'WiFi', 'ESP-Now', 'ArduinoJson', and 'PubSubClient' libraries were used to facilitate the code implementation. Additionally, to establish communication among the sensor nodes and the bridge is essential to get the MAC address of each board [MAC Address](https://github.com/DIEGO15457/Final-Project/blob/main/code/get_mac_address.ino). Thus, the flowcharts for the different nodes are shown below, and the codes for the [Sensing node](https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/code/sensing_node.ino), [Bridge](https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/code/bridge.ino), [WiFi Gateway](https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/code/gateway.ino) are available here.

### Sensing node flowchart:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Sensing_node_flowchart.png" alt="Sensing Flowchart" width="600" height="auto">

### Bridge flowchart:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Bridge_flowchart.png" alt="Bridge Flowchart" width="600" height="auto">

### WiFi Gateway flowchart:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Gateway_flowchart.png" alt="WF Gateway Flowchart" width="600" height="auto">

The MQTT Explorer application was also used to subscribe to the different topics used in the network allowing to debug the code implemented. \
Finally, the Node-RED tool was used to visualise the data in real-time, to store it on a local database using MongoDB application, and to modify remotely the network parameters such as the working state, or the data reporting interval.

### Node-RED Pipeline:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Node-RED Pipeline.png" alt="Node-RED Pipeline" width="600" height="auto">

## Performance Tests
The implementation of the IoT system was successful as it is possible to display the current nodes status on real-time chart with Node-RED, but also to change the network status or the reporting interval from a remote location thanks to a dashboard on Node-RED.

### Real-time charts:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Nodes_states.png" alt="Charts" width="300" height="auto">

### Dashboard:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Dashboard.png" alt="Dashboard" width="300" height="auto">

## Instructions on how to use the system.
Firstly, make sure to have all hardware units and connect them each other like the architecture mentioned and provide adequate power source for every node, in this case to feed of energy to every ESP8266 module. Take into account that push button will act as sensor to detect when the seat receives person's weight or release of it.

Secondly, download the algorithm for each module in order to process adequately the information. Every node has to be embedded by the script called [sensing_node.ino](https://github.com/DIEGO15457/Final-Project/blob/main/code/sensing_node.ino) and the [bridge node](https://github.com/DIEGO15457/Final-Project/blob/main/code/bridge.ino). So far, the nodes can pair with bridge node through the esp-now communication protocol. 

Once, the system is connected to MQTT server and established the gateway, the data which provides information about the number of node and the state of it, is visualised in real-time, stored locally on a MongoDB application, and remotely changed network characteristics like the functioning status or the data reporting interval thanks to the Node-RED tool. Thus, final users can monitor the state of the seats to detect if they are occupied or released. 

## Problems & Solutions

## Critical Analysis 

## Future Work 
This system can be adapted to different schemes where the main purpose will be monitoring states of devices with just two modes (boolean condition) like this project or to widen it with more conditions of stages. Furthermore, it may also leverage the way of communication using the esp-now protocol which offers low-power consumption of energy allowing to get extender lifetime of power source as batteries. 

Moreover, the particular approach of how to achieve of proper connection using a gateway to establish connection with a remote client to get data in real time allowing to store data through the time. Historical data like this (number of node and states) will allow to make predictions about the frequency of usage the controlled device and as consequence to establish proper actions as for example to add more seats for the space assessed and even to detect if specific links into the system is broken receiving an new state of "Link Broken"


