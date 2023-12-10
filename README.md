# Wireless Sensor Network
This project consists on design a small sensor network based on interconnected [ESP8266](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf) micro-controllers. 

## Team Members
1. Ali Altaheri      - H00417357
2. Ángel García      - H00417698
3. Baudouin Belpaire - H00445613
4. Diego Velasquez   - H00434521

## Purpose of the system 
The system’s purpose is to develop an IoT platform monitoring the chairs status in a remote environment. The chairs are equipped with pressure mat sensors detecting the presence of a person sitting on it. Thus, each chair represents a sensing node and communicates wirelessly its status with a bridge node. The bridge node consists of a bridge in the network gathering in a single point all the data from the different places in the environment. The gathered data is meant to be transferred through wired communication to a WiFi gateway enabling to connect the local application to an Internet connected one. The connection to an Internet access point is ensured through a MQTT broker communicating thanks to the MQTT protocol. This part is crucial as it allows the access from anywhere in the world to the local environment. From there, any users can be notified on the environment status and take the control remotely.

## Hardware Design
In the coursework, the sensing nodes, the bridge, and the WiFi gateway have been implemented on 4 different ESP8266 boards. The ESP8266 board is an optimal solution for IoT systems as it embeds a microprocessor, GPIOs, a WiFi antenna for wireless communications, and is also energy efficient. The WiFi antenna is used by the nodes to communicates over the ESP-Now technology using the WiFi bandwidth while consuming less power. The pressure mats are replaced by push buttons returning a digital signal (0 or 1) to the GPIO 5 on the board. The wired communication between the bridge and the gateway is performed through serial communication using the TX/RX ports. Finally, the gateway communicates with the online Mosquitto Broker thanks to the MQTT protocol used on a private WiFi network provided by Heriot-Watt University.

### System Architecture:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Architecture2.png" alt="Architecture">

## Software Design
Concerning the software part, the Arduino IDE was used to program the different ESP8266 nodes. The 'WiFi', 'ESP-Now', 'ArduinoJson', and 'PubSubClient' libraries were used to facilitate the code implementation. Additionally, to establish communication among the sensor nodes and the bridge is essential to get the [MAC address](https://github.com/DIEGO15457/Final-Project/blob/main/code/get_mac_address.ino) of each board. Thus, the flowcharts for the different nodes are shown below, and the codes for the [sensing node](https://github.com/DIEGO15457/Final-Project/blob/main/code/sensing_node.ino), [bridge](https://github.com/DIEGO15457/Final-Project/blob/main/code/bridge.ino), and [WiFi gateway](https://github.com/DIEGO15457/Final-Project/blob/main/code/gateway.ino) are available here.
The sensing nodes code was designed to be energy efficient and so to send a notification only when the chair state changes (i.e the user sits down or stands up) instead of every time.

### Sensing node flowchart:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Sensing_node_flowchart.png" alt="Sensing Flowchart" width="600" height="auto">

### Bridge flowchart:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Bridge_flowchart.png" alt="Bridge Flowchart" width="600" height="auto">

### WiFi Gateway flowchart:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Gateway_flowchart.png" alt="WF Gateway Flowchart" width="600" height="auto">

The MQTT Explorer application was also used to subscribe to the different topics used in the network allowing to debug the code implemented.

Finally, the Node-RED tool was used to visualise the data in real-time, to store it on a local database using MongoDB application, and to modify remotely the network parameters such as the working state, or the data reporting interval.

### Node-RED Pipeline:
<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Node-RED Pipeline.png" alt="Node-RED Pipeline" width="600" height="auto">

## Performance Tests
The implementation of the IoT system was successful as it is possible to transmit the nodes status, to display their current status on real-time charts with Node-RED, but also to change the network status or the reporting interval from a remote location thanks to a dashboard on Node-RED.

### Data transmission:
As follow is the bridge output terminal receiving a notification form the node #2 where the state has changed to 0. The transmitted data buffer is defined as [ID Node#1, State1, ID Node#2, State2]. The buffer is updated each time a notification is received from a sensing ndoe, otherwise the values are unchanged and sent over the wire every 10s. 

<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Bridge_outputs.png" alt="Bridge outputs">

On the gateway side, the message is received and decoded through the wired connection. Thus, the results dsiplayed on the monitor are the data sent over the MQTT protocol, the reporting interval, and the buffer data received.

<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Gateway_outputs.png" alt="Gateway outputs">

### Real-time charts:
In this case, a user is sitting on the chair #1 while the chair #2 is free to seat. The time-axis was set to 1 minute but this can be easily changed on Node-RED to extend the time scale for a wider time-window on the chairs status.

<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Nodes_states.png" alt="Charts" width="300" height="auto">

### Dashboard:
In this case, the network state slider is set to 1 meaning the sensing nodes are actives. Furthermore, the reporting interval option is set to the default value of 10s but can be easily changed thanks to the input frame accepting any positive integers.

<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Dashboard.png" alt="Dashboard" width="300" height="auto">

Once the data is transmitted back from Node-RED to the gateway, a notification shows a message received on both Network and Interval topics.

<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Gateway_network_changes.png" alt="Network changes topics">

## Instructions on how to use the system
Firstly, make sure to have all hardware units, connect them as shown on the architecture image and provide adequate power source for each board. In this case, connect the ESP8266 module on a battery or via a USB-cable. As previously mentioned, it must be taken into account that push buttons will act as sensor to detect when the seat senses a person's weight or release.

Secondly, download the algorithm for each module in order to process adequately the information for each parts. Every node has to be uploaded with the script called [sensing_node.ino](https://github.com/DIEGO15457/Final-Project/blob/main/code/sensing_node.ino), [bridge.ino](https://github.com/DIEGO15457/Final-Project/blob/main/code/bridge.ino), and [gateway.ino](https://github.com/DIEGO15457/Final-Project/blob/main/code/gateway.ino). So far, the nodes can pair with bridge node through the esp-now communication protocol, and the bridge with the gateway through serial communication.

Once, the system is connected to the MQTT server and the gateway successfully transmitting, the data provides information about the node ID and its current state. This is visualised in real-time, stored locally on a MongoDB application. The network characteristics can be remotely changed on the Node-RED application as the functioning status or the data reporting interval. The remote control tools can be found on the Dashboard pannel. From there, the slider network state allows to change the network operation mode, and the reporting interval value changes the transmission data rate. This one takes as input positive integer values. Thus, final users can remotely monitor the state of the seats to detect if they are occupied or not, and collect data over time to perform further data analytics.

## Problems & Solutions

## Critical Analysis 
The system complies with all the requirements specified in the coursework statement, but not the sleep mode:
- It captures all the push events from the remote devices.
- It implements the communication from sensors to gateway via ESP-Now.
- It can establish a remote MQTT connection via Wi-Fi and allows a client to subscribe to change the reporting interval and switching on/off the sensing nodes.

However, when trying to implement the sleep-mode it was not successful and due to the lack of time we could not try different solutions.

Going deeper through the sleep mode for ESP8266[4], to implement the sleep mode in the sender code, it was tried to use the ESP.deepSleep(time[microseconds]) function. The following two options were both tested:

- Automatic Wake-Up (Timer Wake-Up): The ESP8266 automatically wakes up after the sleep time has elapsed. This is done by setting a sleep time when calling the ESP.deepSleep(time[microseconds) function. After this duration, the ESP8266 will wake up.

<img src= "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Automatic_Wake_Up.png">

- External Wake-Up: The ESP8266 can be woken up by an external event, such as the press of a button or a signal from a sensor. To enable the external wake-up source, the RST pin of the ESP8266 shall be connected to the button. Once the RST pin receives a LOW signal, the ESP8266 wakes up.

<img src= "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/RST_Wake_Up.png">

The first approach is to put the ESP8266 into sleep mode after the data is sent. Once the ESP8266 wakes up from deep sleep, it is reset, which means that it starts executing from the beginning of the code again, not from where it left off before going to sleep. As we have not specified a channel, it automatically chooses a new one, different to the one it had before, which is the one where the receiver is still anchored. This is the reason why trying to implement the ESP.deepSleep() did not work.

To address this issue, the best thing to do is making sure ESP-Now is connected to the same WiFi channel all the time and for the whole system (all sensors and receiver), forcing the connection to it when initializing the system and the sensors after waking-up. However, there are other options such as resetting every connection on the receiver every time a sensor wakes up. 

### Energy Consumption
Regarding the energy consumption, it is assumed the scenario with no sleep mode implemented.
Measuring the voltage and current directly from  the sensing unit, it gets:

<img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/20231128_103304.jpg" width="500">

Increasing the voltage from 3.30Vdc (minimum for ESP8266) to 3.7Vdc first and 5Vdc later, the current came up to 80mA, which could be assumed as almost constant, so the limitation in current consumption is between 76mA and 80mA.

<img src ="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Energy_table.png">

The increase in power for 5Vdc matches the rate conversion on the ESP8266 to convert the 5Vdc to 3Vdc.

<img src = "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/EnergyRate_Equation.png">

Searching on the Internet for a battery model in the range of 3.3Vdc to 3.7Vdc, and with the maximum current in ampere-hours to maximize the life-cycle per sensor, a good option offering 3000mAh is available on [Amazon](https://www.amazon.co.uk/Rechargeable-REACELL-Headlamp-Flashlight-Electronic/dp/B0BZJ2NLQY).

Getting the minimum voltage that makes the ESP8266 work-76mA of continous current consumption, the life-cycle would be:

<img src= "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Sensor-life-cycle.png">


## Future Work 
This system can be adapted to different schemes where the main purpose will be monitoring states of devices with just two modes (boolean condition) like this project or to widen it with more conditions of stages. Furthermore, it may also leverage the way of communication using the ESP-Now protocol which offers low-power consumption of energy allowing to get extender lifetime of power source as batteries.

Moreover, the particular approach of how to achieve of proper connection using a gateway to establish connection with a remote client to get data in real time allowing to store data through the time. Historical data like this (number of node and states) will allow to make predictions about the frequency of usage the controlled device and as consequence to establish proper actions as for example to add more seats for the space assessed and even to detect if specific links into the system is broken receiving an new state of "Link Broken"

## References

[1] https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf (Access Nov-2023).

[2] https://espressif-docs.readthedocs-hosted.com/projects/esp-faq/en/latest/application-solution/esp-now.html#:~:text=Yes%2C%20but%20it%20should%20be%20noted%20that%20the,the%20same%20as%20that%20of%20the%20connected%20AP. (Access Nov 2023).

[3] https://github.com/espressif/arduino-esp32/issues/878 (Access Nov-2023).

[4] https://microcontrollerslab.com/esp8266-deep-sleep-wake-up-sources-arduino-ide/ (Access Nov 2023).



