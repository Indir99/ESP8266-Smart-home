## ESP8266 - Smart home project

Distributed automation systems / Faculty of Electrical Engineering (University in Tuzla)

## Introduction

In this project we will try to implement a smart home system. The aim of this project is to create a system that will connect sensors (input) and actuators (output). The system will be connected to the server and we will manage the actuators from a remote location. We will collect data from the sensors and send it to the server. Further, the data will be stored in a database and based on the collected data we will create appropriate diagrams, and based on the diagrams we can draw some conclusions about changes in the system. We will manage the actuators based on the data we will get from the server, that is, specifically from our SCADA system, ie. from a remote location we will manage events inside our house (e.g. we can turn on air conditioning, lights, open a garage, etc.).

Equipment:

1. NodeMCU- ESP8266
2. Wires
3. Light sensor - photoresistor
4. Thermisor
5. Tracking sensor
6. Water level sensor
7. Ultrasonic sensor
8. LEDs
9. DC motor
10. Servos

## Equipment analysis

### 1. NodeMCU - esp8266

NodeMCU is a development board that is primarily intended for the development of IoT projects. One of the main components of this development board is the esp8266 wi-fi module. We will use the mentioned development board as a basis for the realization of this project.

### 2. Photosensor

We will use a photosensor to detect light outside the house. Depending on the light intensity on the photosensor, we will have three states:
1 - sunny day
2 - cloudy day
3 - night
Every n seconds we will send sensor data to the server.

### 3. Thermistor

We will use a thermistor to detect the temperature. We will send the temperature to the server every n seconds, and depending on the temperature we will turn on / off the fan / heating in the house.

### 4. Tracking sensor

We will use this sensor to detect the object. We will detect if the car is in the garage. If the car is in the garage, we will send a value of 1 to the server, if there is no car, we will send a value of 0 to the server.

### 5. Liquid detection sensor

We will use this sensor to detect whether it is raining or not. If it rains, we will have the opportunity to cover the pool to prevent water pollution in the pool next to the house.

### 6. Ultrasonic sensor

We will also use this sensor to detect the object. Namely, with the help of an ultrasonic sensor, we will monitor whether the car is in front of the garage and how far it is from the garage. Based on this information, we will be able to open and close the garage.

### 7. LEDs

In this project, we will use LED diodes to illuminate the rooms. We will divide the house into two rooms, a living room and a bedroom. In each room we will have one diode that will represent the light in that room. Also, one diode will replace the heater if we can't turn it on. We will receive the diode control signal from the server, ie we will control the diodes via the application / web application.

### 8. DC motor

We will use the DC motor as a fan in this project, ie when the temperature rises above the predicted, we will have the opportunity to turn on the fan, ie regardless of the temperature, if we want, we will be able to turn on the fan.

### 9. Servo motors

We will use servo motors to open or close the garage door. We will also use them to cover / uncover the pool. Like other system outputs, we will control the servo motors using an application / web application.
