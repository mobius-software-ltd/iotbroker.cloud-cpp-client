# IoTBroker.Cloud C++ Client

IoTBroker.Cloud is C++ client which allows to connect to MQTT server. IoTBroker.Cloud C++ client sticks to [MQTT 3.1.1](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.pdf) standards. 

## Features

* **Clean / persistent session.** When the client disconnects, its session state can be stored (if you set Clean session flag to false) or removed (if you set Clean session flag to true). The session state includes subscriptions and incoming QoS 1 and QoS 2 messages while the client is off.

* **Last Will and Testament.** This feature implies that if a client goes offline without sending DISCONNECT message (due to some failure), other clients will be notified about that.

* **Keep Alive.** If Keep Alive is higher than 0, the client and the server is constantly exchanging PING messages to make sure whether the opposite side is still available. 

* **Retain messages.** It allows to "attach" a message to a particular topic, so the new subscribers become immediately aware of the last known state of a topic.

* **Assured message delivery.** Each message is sent according to the level of Quality of Service (QoS). QoS has 3 levels:
- QoS 0 (At most once) — a message is sent only one time. The fastest message flow, but message loss may take place. 
- QoS 1 (At least once) — a message is sent at least one time. The message duplication may occur.  
- QoS 2 (Exactly once) — a message is sent exactly one time.  But QoS 2 message flow is the slowest one. 

## Getting Started

These instructions will help you get a copy of the project and run it.

### Prerequisites
[Qt Creator](https://www.qt.io/download) should be installed before starting to clone IoTBroker.Cloud C++ Client. 

### Installation
* To install IoTBroker.Cloud, first you should clone [IotBroker.Cloud C++ Client](https://github.com/mobius-software-ltd/iotbroker.cloud-cpp-client).

* In order to open IoTBroker.Cloud C++ client in Qt Creator, you should press **New project** button. Then choose **Import project** and **Import existing project**.

* Then insert the project name, set the root directory of project and press **Next**. 

* Now you should choose the kits you are going to build and run the project with. If you see “No valid kits found” notification, open the terminal window and run the following command:

```
sudo apt-get install qt5-default
```
* You can open the project by **Ctrl+O** shortcut or just go to *File - Open File or Project* and choose the file with **.pro** extension.

* Finally you should press **Run** sign to run the project. If the procedure is successful, you will see the Login page in the form of pop-up window. Now you are able to log in and to start exchanging messages with MQTT server.

Please note that at this stage it is not possible to register as a client. You can only log in to your existing account.

IoTBroker.Cloud C++ Client is developed by [Mobius Software](http://mobius-software.com).

