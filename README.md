# Smart Glove

This project implements a wearable **Smart Glove** using an **ESP32**, **five flex sensors**, and an **MPU6050 IMU** to read hand gestures and potentially translate them into speech. The glove can be used for applications such as **sign language interpretation**, **gesture-controlled interfaces**, or **rehabilitation monitoring**.

---

## Features

- Reads analog values from **5 flex sensors** (one for each finger)
- Reads accelerometer and gyroscope data from **MPU6050**
- Uses **ESP32** for processing and communication
- Serial monitor prints mapped values and gesture classification
- Values recognized and mapped to character
- Using Wi-Fi module on **ESP32** the character is sent to **Flutter Application- SignWave** 
- The application using google text to speech converter to convert the received character to speech


---

## Hardware Used

- ESP32 DevKit V1 (or similar)
- MPU6050 6-axis accelerometer + gyroscope
- 5x Flex Sensors (resistive bend sensors)
- 10kΩ resistors (for voltage dividers with flex sensors)
- LiPo Battery (3.7V) with TP4056 charging module (for portability)
- Breadboard / PCB
- Wires and a glove

---

##  Pin Connections

 - As shown in the attached circuit diagram

---

##  Libraries Required

Install the following libraries via Arduino Library Manager:

- `Adafruit MPU6050`
- `Adafruit Unified Sensor`
- `Wire`

## Note:
- the program Flex_Reading directly reads the flex sensors and sends the data to the device.
- before using please put appropriate Wi-Fi SSID and Password
- the program interpreter involves MPU values which might vary from place to place, caliberate your MPU before using
---



