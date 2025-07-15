# Automatic-door-opening-by-face-recognition-
1. Introduction 
- This project use camera on the laptop to capture pictures and send to adafruit io ( on file subcriber.py). After that file ai_train.py receive pictures on adafruit and run AI for face recognition and publish name and flags to adafruit io. File sketch receive name and flags and run it on servor ( open or close door), l2d ( name of the qualified user or error). There is a button on yolobit to open the camera.

- This project work on Yolobit and use Teachable Machine for AI model.

- This is a downgrade version of my group project. I work on yolobit to receive and publish data to adafruit io. the others members work on web.

2. How to run
- step 1: download the code and required Library.
- step 2: set up yolo-bit and a house model.
- step 3: run files in order ( ai_train.py => subcriber.py => sketch)
- step 4: now you can open door or camera on adafruit io.

3. Adafruit io

4. Demo video


5. Required library
The library for Arduino:
- ArduinoHttpClient -> 0.6.1
- Ethernet -> 2.0.2
- WiFi101 -> 0.16.1
- AUnit -> 1.7.1
- Adafruit MQTT -> 2.5.9
- Adafruit NeoPixel -> 1.12.5
- Adafruit IO Arduino -> 4.3.0
- ESP32Servo -> 3.0.6
- WiFimanager -> 2.0.17

The library for the Python:
- adafruit-io -> 2.8.0
- keras -> 2.13.1
- Flask-SockerIO -> 5.5.1
- mqttclient -> 0.0.2
- paho-mqtt -> 2.1.0 (1.6.5 if error happen on mqtt)
- pillow ->10.4.0
- tensorflow -> 2.13.0
- wheel -> 0.45.1
