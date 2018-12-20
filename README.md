# IoTBroker.Cloud C++ Client

### Project description

IoTBroker.cloud С++ Client is an application that allows you to connect to the server using MQTT, MQTT-SN, 
AMQP or COAP protocols. IoTBroker.cloud С++ Client gives the opportunity to exchange messages using protocols 
mentioned above. Your data can be also encrypted with **TLS** or **DTLS** secure protocols.   

Below you can find a brief description of each protocol that can help you make your choice. 
If you need to get more information, you can find it in our [blog](https://www.iotbroker.cloud/clientApps/C++/MQTT/C++).
 
**MQTT** is a lightweight publish-subscribe based messaging protocol built for use over TCP/IP.  
MQTT was designed to provide devices with limited resources an easy way to communicate effectively. 
You need to familiarize yourself with the following MQTT features such as frequent communication drops, low bandwidth, 
low storage and processing capabilities of devices. 

Frankly, **MQTT-SN** is very similar to MQTT, but it was created for avoiding the potential problems that may occur at WSNs. 

Creating large and complex systems is always associated with solving data exchange problems between their various nodes. 
Additional difficulties are brought by such factors as the requirements for fault tolerance, 
he geographical diversity of subsystems, the presence a lot of nodes interacting with each others. 
The **AMQP** protocol was developed to solve all these problems, which has three basic concepts: 
exchange, queue and routing key. 

If you need to find a simple solution, it is recommended to choose the **COAP** protocol. 
The CoAP is a specialized web transfer protocol for use with constrained nodes and constrained (e.g., low-power, lossy) 
networks. It was developed to be used in very simple electronics devices that allows them to communicate interactively 
over the Internet. It is particularly targeted for small low power sensors, switches, valves and similar components 
that need to be controlled or supervised remotely, through standard Internet networks.   
 
### Prerequisites 
[Qt Creator](https://www.qt.io/download) should be installed before starting to clone IoTBroker.Cloud C++ Client. Note that version of Qt Creator IDE should be at least 4.7.2, version of QT should be 5.11.0 or hire.. 

### Installation 
* First, you should clone IotBroker.Cloud C++ Client.
* Next you should install wolfSS library following instructions. Note that you need to download version 3.15.0.
* Then you need to check whether OpenSSLtoolkit is installed on you PC and which version is. You must be sure that you have 1.0.2g version.
* In order to open IoTBroker.Cloud C++ client in Qt Creator, you should press **New project button**. Then **choose Import project and Import existing project**.
* Then insert the project name, set the root directory of project and press **Next. 
* You can open the project by Ctrl+O shortcut or just **go to File - Open File or Project and choose the file with .pro extension.
* Finally you should press **Run sign** to run the project. If the procedure is successful, you will see the Login page in the form of pop-up window. 

Now you are able to log in and to start exchanging messages with server.
Please note that at this stage it is not possible to register as a client. You can only log in to your existing account.

IoTBroker.Cloud C++ Client is developed by [Mobius Software](http://mobius-software.com).

## [License](LICENSE.md)
