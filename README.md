# Wireless Sensor Network
This project consists on designing a remote sensing network based on interconnected [ESP8266](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf) micro-controllers. 

## Team Members
1. Ali Altaheri      - H00417357 - ama2023@hw.ac.uk 
2. Ángel García      - H00417698 - ag2070@hw.ac.uk
3. Baudouin Belpaire - H00445613 - bb2043@hw.ac.uk
4. Diego Velasquez   - H00434521 - dev2000@hw.ac.uk

## Purpose of the system 
<p align="justify"> The system’s purpose is to develop an IoT platform monitoring the chairs status in a remote environment. The chairs are equipped with pressure mat sensors detecting the presence of a person sitting on it. Thus, each chair represents a sensing node and communicates wirelessly its status with a bridge node. The bridge node consists of a bridge in the network gathering in a single point all the data from the different places in the environment. The gathered data is meant to be transferred through wired communication to a WiFi gateway enabling to connect the local application to an Internet connected one. The connection to an Internet access point is ensured through a MQTT broker communicating thanks to the MQTT protocol. This part is crucial as it allows the access from anywhere in the world to the local environment. From there, any users can be notified on the environment status and take the control remotely.</p>
<p align="justify"> The behaviour of the system to implement can be described by the following flowchart diagram: </p>

<p align="center"><img src= "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Flow_chart.png"></p>



## Hardware Design
<p align="justify"> In the coursework, the sensing nodes, the bridge, and the WiFi gateway have been implemented on 4 different ESP8266 boards. The ESP8266 board is an optimal solution for IoT systems as it embeds a microprocessor, GPIOs, a WiFi antenna for wireless communications, and is also energy efficient. The WiFi antenna is used by the nodes to communicates over the ESP-Now technology using the WiFi bandwidth while consuming less power. The pressure mats are replaced by push buttons returning a digital signal (0 or 1) to the GPIO 5 on the board. The wired communication between the bridge and the gateway is performed through serial communication using the TX/RX ports. Finally, the gateway communicates with the online Mosquitto Broker thanks to the MQTT protocol used on a private WiFi network provided by Heriot-Watt University. </p>
<p align="justify"> The reason why the bridge and the gateway have been implemented in different modules is due to ESP-Now does not work simultaneously with Wi-Fi. This problem is well documented in Internet forums [2], [3]. Additionally separating the receiver (bridge) from the Wi-Fi transmitter (gateway) allows to check the serial connection works properly. Thinking in a real implementation is also more suitable to separate both functions and connect an external antenna with split connectors to both bridge and gateway to maximize the range.
Regarding the wiring between the bridge and the gateway (see figure below) is worth noting that TX/RX shall be crossed and when loading the code onto the boards it is required to disconnect.</p>

<p align="center"><img src= "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/WiFi_Tx_receiver_wiring.png" height="auto"></p>

## System Architecture:
This Architecture depicts the integrated system of the components described in previous section, Hardware Design, and how they are connected each other to perform this wireless sensor network as well as communication protocols in their different stages. 
<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Architecture2.png" alt="Architecture"> </p>

## Software Design
Concerning the software part, the Arduino IDE was used to program the different ESP8266 nodes. The 'WiFi', 'ESP-Now', 'ArduinoJson', and 'PubSubClient' libraries were used to facilitate the code implementation. Additionally, by using the ESP-NOW protocol  among the sensor nodes and the bridge is essential to get the [MAC address](https://github.com/DIEGO15457/Final-Project/blob/main/code/get_mac_address.ino) of each board. Thus, the flowcharts for the different nodes are shown below, and the codes for the [sensing node](https://github.com/DIEGO15457/Final-Project/blob/main/code/sensing_node.ino), [bridge](https://github.com/DIEGO15457/Final-Project/blob/main/code/bridge.ino), and [WiFi gateway](https://github.com/DIEGO15457/Final-Project/blob/main/code/gateway.ino) are available here.
The sensing nodes code was designed to be energy efficient and so to send a notification only when the chair state changes (i.e the user sits down or stands up) instead of every time.

### Sensing node flowchart:
<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Sensing_node_flowchart.png" alt="Sensing Flowchart" width="600" height="auto"> </p>

### Bridge flowchart:
<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Bridge_flowchart.png" alt="Bridge Flowchart" width="600" height="auto"> </p>

### WiFi Gateway flowchart:
<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Gateway_flowchart.png" alt="WF Gateway Flowchart" width="600" height="auto"> </p>

<p align="justify"> The MQTT Explorer application was also used to subscribe to the different topics used in the network allowing to debug the code implemented.

Finally, the Node-RED tool was used to visualise the data in real-time, to store it on a local database using MongoDB application, and to modify remotely the network parameters such as the working state, or the data reporting interval. </p>

### Node-RED Pipeline:
<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Node-RED Pipeline.png" alt="Node-RED Pipeline" width="600" height="auto"> </p>

## Performance Tests
<p align="justify"> The implementation of the IoT system was successful as it is possible to transmit the nodes status, to display their current status on real-time charts with Node-RED, but also to change the network status or the reporting interval from a remote location thanks to a dashboard on Node-RED. </p>

### Data transmission:
<p align="justify"> As follow is the bridge output terminal receiving a notification form the node #2 where the state has changed to 0. The transmitted data buffer is defined as [ID Node#1, State1, ID Node#2, State2]. The buffer is updated each time a notification is received from a sensing ndoe, otherwise the values are unchanged and sent over the wire every 10s. </p>

<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Bridge_outputs.png" alt="Bridge outputs" width="200" height="auto"> </p>

<p align="justify"> On the gateway side, the message is received and decoded through the wired connection. Thus, the results dsiplayed on the monitor are the data sent over the MQTT protocol, the reporting interval, and the buffer data received. </p>

<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Gateway_outputs.png" alt="Gateway outputs" width="200" height="auto"> </p>

### Real-time charts:
<p align="justify"> In this case, a user is sitting on the chair #1 while the chair #2 is free to seat. The time-axis was set to 1 minute but this can be easily changed on Node-RED to extend the time scale for a wider time-window on the chairs status. </p>

<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Nodes_states.png" alt="Charts" width="300" height="auto"> </p>

### Dashboard:
<p align="justify"> In this case, the network state slider is set to 1 meaning the sensing nodes are actives. Furthermore, the reporting interval option is set to the default value of 10s but can be easily changed thanks to the input frame accepting any positive integers. </p>

<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Dashboard.png" alt="Dashboard" width="300" height="auto"> </p>

<p align="justify"> Once the data is transmitted back from Node-RED to the gateway, a notification shows a message received on both Network and Interval topics. </p>

<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Gateway_network_changes.png" alt="Network changes topics" width="500" height="auto"> </p>

## Instructions on how to use the system
<p align="justify"> Firstly, make sure to have all hardware units, connect them as shown on the architecture image and provide adequate power source for each board. In this case, connect the ESP8266 module on a battery or via a USB-cable. As previously mentioned, it must be taken into account that push buttons will act as sensor to detect when the seat senses a person's weight or release. </p>

Secondly, download the algorithm for each module in order to process adequately the information for each parts. Every node has to be uploaded with the script called [sensing_node.ino](https://github.com/DIEGO15457/Final-Project/blob/main/code/sensing_node.ino), [bridge.ino](https://github.com/DIEGO15457/Final-Project/blob/main/code/bridge.ino), and [gateway.ino](https://github.com/DIEGO15457/Final-Project/blob/main/code/gateway.ino). So far, the nodes can pair with bridge node through the esp-now communication protocol, and the bridge with the gateway through serial communication.

<p align="justify">  Once, the system is connected to the MQTT server and the gateway successfully transmitting, the data provides information about the node ID and its current state. This is visualised in real-time, stored locally on a MongoDB application. The network characteristics can be remotely changed on the Node-RED application as the functioning status or the data reporting interval. The remote control tools can be found on the Dashboard pannel. From there, the slider network state allows to change the network operation mode, and the reporting interval value changes the transmission data rate. This one takes as input positive integer values. Thus, final users can remotely monitor the state of the seats to detect if they are occupied or not, and collect data over time to perform further data analytics. </p> 
The [CSV file](https://github.com/DIEGO15457/Final-Project/blob/main/assets/Sensing_nodes_data.csv) obtained from the data collection with MongoDB is also accessible here.  

## Problems & Solutions
<p align="justify"> The major problems encountered were during the data transmission between the bridge and gateway through the serial communication and with the deep sleep mode. </p> 
<p align="justify"> Concerning the serial communication, the Serial.print() function was used to send data over serial communication but it created interferences with the message displayed on the monitor to debug the code. Thus, the data transmitted had to be encoded between '<' and '>' to inform the receiver the beginning and the end of the transmission. Thus, the receiver starts recording the data after reading a '<' and keeps the data until '>'. This encryption method is valid for both way to transmit sensor data from the brigde to the node, but also to transmit the network state change from the gateway to the bridge. </p>

<p align="justify"> Concerning the sleep mode, it was tried to replace the delay() function at the end of the loop by a sleep mode to make the sensing node more energy efficient. However, this step failed the process as it was no longer able to send the data back to the bridge after waking up. Thus, this part was left on the side to concentrate on the main code. Furthermore, during the sleep mode, all components are switched off meaning that the node has a high probability to miss the change of state sent by the bridge. The trials on the sleep mode implementation are further detailed in the next section. </p>

## Critical Analysis 
The system complies with all the requirements specified in the coursework statement, but not the sleep mode:
- It captures all the push events from the remote devices.
- It implements the communication from sensors to gateway via ESP-Now.
- It can establish a remote MQTT connection via Wi-Fi and allows a client to subscribe to change the reporting interval and switching on/off the sensing nodes.

However, when trying to implement the sleep-mode it was not successful and due to the lack of time we could not try different solutions.

Going deeper through the sleep mode for ESP8266[4], to implement the sleep mode in the sender code, it was tried to use the ESP.deepSleep(time[microseconds]) function. The following two options were both tested:

- Automatic Wake-Up (Timer Wake-Up): The ESP8266 automatically wakes up after the sleep time has elapsed. This is done by setting a sleep time when calling the ESP.deepSleep(time[microseconds) function. After this duration, the ESP8266 will wake up.

<p align="center"> <img src= "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Automatic_Wake_Up.png"> </p>

- External Wake-Up: The ESP8266 can be woken up by an external event, such as the press of a button or a signal from a sensor. To enable the external wake-up source, the RST pin of the ESP8266 shall be connected to the button. Once the RST pin receives a LOW signal, the ESP8266 wakes up.

<p align="center"> <img src= "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/RST_Wake_Up.png"> </p>

<p align="justify"> The first approach is to put the ESP8266 into sleep mode after the data is sent. Once the ESP8266 wakes up from deep sleep, it is reset, which means that it starts executing from the beginning of the code again, not from where it left off before going to sleep. As we have not specified a channel, it automatically chooses a new one, different to the one it had before, which is the one where the receiver is still anchored. This is the reason why trying to implement the ESP.deepSleep() did not work. </p>

<p align="justify"> To address this issue, the best thing to do is making sure ESP-Now is connected to the same WiFi channel all the time and for the whole system (all sensors and receiver), forcing the connection to it when initializing the system and the sensors after waking-up. However, there are other options such as resetting every connection on the receiver every time a sensor wakes up. </p>

## Energy Consumption
<p align="justify"> Regarding the energy consumption, it is assumed the scenario with no sleep mode implemented. Thus, the duty cycle of the system is 1 as the current drawn over the cycle does not vary, which is due to the CPU and antennas that are continuously running.
Measuring the voltage and current directly from the sensing unit gives the following results: </p>

<p align="center"> <img src="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/20231128_103304.jpg" width="500"> </p>

Increasing the voltage from 3.30Vdc (minimum for ESP8266) to 3.7Vdc first and 5Vdc later, the current came up to 80mA, which could be assumed as almost constant, so the limitation in current consumption is between 76mA and 80mA.

<p align="center"> <img src ="https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Energy_table.png"> </p>

The increase in power for 5Vdc matches the rate conversion on the ESP8266 to convert the 5Vdc to 3Vdc.

<p align="center"> <img src = "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/EnergyRate_Equation.png"> </p>

Searching on the Internet for a battery model in the range of 3.3Vdc to 3.7Vdc, and with the maximum current in ampere-hours to maximize the life-cycle per sensor, a good option offering 3000mAh is available on [Amazon](https://www.amazon.co.uk/Rechargeable-REACELL-Headlamp-Flashlight-Electronic/dp/B0BZJ2NLQY).

Getting the minimum voltage that makes the ESP8266 work-76mA of continous current consumption, the life-cycle would be:

<p align="center"> <img src= "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/Sensor-life-cycle.png"> </p>

For the improvement of the system energy efficiency, the following method could be implemented to reduce the time for which the CPU and the WiFi antenna are in full operation mode. 

<p align="center"> <img src= "https://raw.githubusercontent.com/DIEGO15457/Final-Project/main/assets/New_energy_consumption.png" width="600" height="auto"> </p>

<p align="justify"> It starts when the device wakes up from sleep mode and start sensing the sensor state for 5µs. Then, it activates the WiFi antenna to listen for potential incoming message during 10ms concerning the network state. In the case the chair status has changed the ESP would have to submit a message containing two bytes (for the 2 different node information) at a minimal rate of 250bytes/s. The time taken would be then of 8µs for sending the data. Finally, the device enters in deep sleep mode for a duration of 5s. The enabling and disabling steps are assumed to be negligible. The WiFi antenna and the CPU are active at almost the same time and only the current drawn from both process active is known. </p>

|Process Description | Process Time | Background|
|:--------------------:|:---------:|:---------:|
| WiFi active    | 10.008 ms     | 10 ms + 8us |
| CPU active     | 10.031 ms     | 5us + 8us + 10ms |
| Sleep time     | 5s            | - |
| Total          | 5.010031 s    | CPU + Sleep time |

Thus, the duty cicles are calculated as follows:

<p align="center"> DC(active) = 0.010031 / 5.010031 </p>

<p align="center"> DC(active) = 0.002 </p>

<p align="center"> DC(sleep mode) = 0.998 </p>

The power consumption is then calculated from the active time, taking into account according to ESP guide that the current drawn in sleep mode 20µA, and the current drawn in full operation mode 76mA.

<p align="center"> P = 0.002 * 76 + 0.998*0.020 = 0.171mA </p>

and the running time,

<p align="center"> B = 3000 / 0.171 = 17544 hours = 730 days = 2 years </p>

*This needs to be implemented and tested to verify the values are accurate.*

## Future Work 
<p align="justify"> This system can be adapted to different schemes where the main purpose will be monitoring states of devices with just two modes (boolean condition) like this project or to widen it with more conditions of stages. Furthermore, it may also leverage the way of communication using the ESP-Now protocol which offers low-power consumption of energy allowing to get extender lifetime of power source as batteries. </p>
<p align="justify">Moreover, the particular approach of how to achieve of proper connection using a gateway to establish connection with a remote client to get data in real time allowing to store data through the time. Historical data like this (number of node and states) will allow to make predictions about the frequency of usage the controlled device and as consequence to establish proper actions as for example to add more seats for the assessed space if they keep occupied for long time, to enable/disable the full system if it is not required or affect critically its performance or even to detect if specific links into the system are broken receiving an new state of "Link Broken". </p>
<p align="justify">As it was mentioned, the Sleep mode will be implemented in next stage, so this network would be more efficient by managing to maximise the power supplier. Furthermore, another step would be to deploy a security layer in order to reduce external risks and control the confidentiality of stored data preventing erroneous manipulation on it either by unauthorized users or non trained personal depending on the complexity of the data.  </p> 

## References

[1] https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf (Access Nov-2023).

[2] https://espressif-docs.readthedocs-hosted.com/projects/esp-faq/en/latest/application-solution/esp-now.html#:~:text=Yes%2C%20but%20it%20should%20be%20noted%20that%20the,the%20same%20as%20that%20of%20the%20connected%20AP. (Access Nov 2023).

[3] https://github.com/espressif/arduino-esp32/issues/878 (Access Nov-2023).

[4] https://microcontrollerslab.com/esp8266-deep-sleep-wake-up-sources-arduino-ide/ (Access Nov 2023).






