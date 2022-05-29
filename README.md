# msp430_mqtt

connecting msp430 to mqtt broker using esp8266 
# Connection of ESP8266 Wi-Fi Module

In this project, to be able to connect to internet, ESP8266-01 wi-fi module is used. AT firmware is loaded into that wi-fi module. Through UART communication, AT commands are sent to wi-fi module. Those commands help to connect Wi-Fi module to an access point(modem) and a server. After the connection with server is done, packets are sent to MQTT server. 

To give some example of AT commands:

**AT+CWDHCP=1,1** puts ESP into STA mode, which is station mode, so it can connect to a wi-fi

**AT+CWJAP="ssid"," password"** helps to connect to an access point (modem or mobile hotspot)

**AT+CIPSEND=?** is used to send data to a server. First send this command to tell the ESP you will send a data to server. After this command send the string to ESP then ESP will send this data to connected server.

**"AT+CIPSTART=\"TCP\",\"iot.ee.hacettepe.edu.tr\",1883\r\n"** this AT command connects our ESP to server that runs MQTT sever. Again, connection to server not to MQTT broker. 

Functions

|<p>**#define** buff\_s 100</p><p>**extern** **char** receivedData[buff\_s] = {};</p><p></p><p>**void** **put\_uart\_array**(**char** \*,**unsigned** **int** size);//send string via uart</p><p>//send command via uart and wait for response then check response</p><p>**void** **esp\_command**(**char** \*command,**unsigned** **int** size\_arr,**char** \*pos\_response,**unsigned** **int** delay);</p><p>//clear receive buffer</p><p>**void** **clear\_buffer**();</p><p>//delay function waits in LPM0</p><p>**void** **delay\_sleep**(**unsigned** **int** sleep\_sec);</p><p>//put received char to buffer</p><p>**void** **get\_char\_from\_uart**(**char** );</p><p>//check the response from the esp</p><p>**int** **check\_response**(**char** \*response);</p><p></p>|
| :- |
` `below were written to be able to communicate with ESP8266-01 module.

**put\_uart\_array** function sends given string through UART.

**esp\_command** function sends command to ESP by using **put\_uart\_array** then waits for response and checks the response if the given AT command has successfully executed by using **check\_response**.

**Clear\_buffer** function clears the receive buffer. Use this function before sending AT command so it will contain only the new response of ESP.

**Get\_char\_from\_uart** places received character from UART into the receive buffer.

**Check\_response** cheks the response of the ESP module against to given command. If the given job successfully completed or failed.

Until now, ESP has connected to internet and server. Also, ESP can send data to that server.
# Connecting to MQTT broker
` `MQTT is an OASIS standard messaging protocol for the Internet of Things (IoT).  An IoT device that is connected to MQTT broker can publish message and subscribe to a topic. 

In this project first MSP creates a topic called “temperature\_humidity” and publish measured temperature as message to that topic. Second MSP subscribes “temperature\_humidity” topic to be able to receive temperature data as the first MSP publish. After receiving temperature and humidity data, second MSP runs KNN algorithm to predict the probability of rain. Then, second MSP publishes this rain probability, temperature, and humidity. Third MSP receives these three parameters and displays it.
# Communication with MQTT Broker
To be able to connect to a MQTT broker, to publish message and to subscribe to a topic, different packet formats should be sent to MQTT broker. These packet formats are created as a string then sent to MQTT broker. Belove, packet formats and functions that creates these packets are presented.

**Packet to Connect to MQTT Broker**

|Byte|1|2|3|4|5|6|7|8|9|7|8|9|10|11|12|13|14|15|
| :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- |
|meaning|Header|<p>Remaining</p><p>Length</p>|Length of Protocol Name|<p></p><p></p><p>Protocol Name</p>|<p>Connect</p><p>Flags</p>|Keep Alive|length|Client ID|
|hex|0x10||0x0|0x4|||||0x2|0x0|0x3c|0x0|0x5||||||
|ASCII|||0|4|M|Q|T|T|||60||5|g|3|4|d|p|


|16|17|18|19|20|21|22|23|24|25|26|27|28|29|
| :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- | :- |
|length|Username|length|password|
|0x0|0x5||||||0x0|0x5||||||
||5|g|3|4|d|p||5|x|x|x|x|x|

When this packet structure is created and sent to MQTT broker, connection with MQTT broker is done and IoT module can publish message and subscribe to topics.

**Packet to Subscribe a Topic**

|Subscribe|Remaining Length|Package ID|Topic Length|Subscribe Topic Name|Qos(quality of service)|
| :- | :- | :- | :- | :- | :- |
|0x82|0x08|0x00|0x01|0x00|0xA|t|e|m|p|e|r|a|t|u|r|e|0|

**Packet to Publish a Message**

|Publish|Remaining Length|Topic Length|Topic Name|Message|
| :- | :- | :- | :- | :- |
|0x30||0x00|0xA|t|e|m|p|e|r|a|t|u|r|e|t|e|m|P|:|2|5|


These functions were written to provide functionality expressed above. Below functions creates the packet structures expressed above as string and sends them to MQTT broker.

|<p>/\*</p><p>` `\* mqtt.h</p><p>` `\*</p><p>` `\*  Created on: 25 Mar 2022</p><p>` `\*      Author: deniz pelen</p><p>` `\*/</p><p></p><p>**#ifndef** MQTT\_H\_</p><p>**#define** MQTT\_H\_</p><p></p><p>**void** **connect\_broker**();</p><p>**void** **Publish\_msg**(**char** \*topic, **char** \*message);</p><p>**void** **subscribe\_to\_topic**(**char** \*topic);</p><p>**#endif** /\* MQTT\_H\_ \*/</p><p></p>|
| :- |








