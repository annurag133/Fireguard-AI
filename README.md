FireGuard AI

Fire Detection and Prevention System

FireGuard AI is a fire detection and prevention project developed to identify fire and smoke using computer vision and IoT technologies. The system combines a YOLOv8-based object detection model, camera streaming, environmental sensors, and a web-based monitoring application.

The main objective is to detect fire and smoke at an early stage, notify users about potential fire hazards, and support automated fire suppression through an IoT-controlled mechanism.

Features

* Real-time fire and smoke detection using a custom-trained YOLOv8 model.
* Live camera streaming using ESP32-CAM.
* Detection of three classes: Fire, Smoke, and Non-Fire.
* Web-based monitoring dashboard built with Flask.
* Real-time detection visualization with bounding boxes.
* Fire alerts through Telegram bot integration.
* Location visualization using Leaflet and OpenStreetMap.
* Flame sensor integration for additional fire monitoring.
* Servo-controlled water pipe direction.
* Water pump control for fire suppression.

System Architecture

The project consists of three main modules:

1. Fire Detection Module

The detection module processes camera frames using a custom-trained YOLOv8 model. It identifies fire, smoke, and non-fire objects and displays the detection results with bounding boxes.

2. IoT Monitoring and Control Module

The IoT module uses microcontrollers and sensors to monitor the surrounding environment.

The flame sensor provides additional fire detection capability. The ESP8266 NodeMCU handles sensor-related operations and controls the connected hardware components.

A servo motor controls the direction of the water pipe, while a DC water pump is used for the water spraying mechanism.

3. Web Monitoring Module

The web application is developed using Flask and provides a centralized interface for monitoring the system.

It includes:

* Live camera feed.
* Fire and smoke detection results.
* Detection status.
* Location visualization.
* Telegram notification integration.

Technologies Used

Category	Technology
Programming Language	Python
Object Detection	YOLOv8
Deep Learning Framework	PyTorch
Model Framework	Ultralytics
Backend	Flask
Computer Vision	OpenCV
Frontend	HTML, CSS, JavaScript
Mapping	Leaflet
Map Data	OpenStreetMap
Notifications	Telegram Bot API
Microcontroller	ESP8266 NodeMCU
Camera Module	ESP32-CAM
Servo Control	SG90 Servo Motor

Hardware Components

Component	Description
ESP32-CAM	Captures and streams live video
ESP8266 NodeMCU	Controls sensors and hardware
Flame Sensor	Detects flame radiation
HW-201 Sensor	Additional environmental sensing
SG90 Servo Motor	Controls water pipe direction
Relay Module	Controls the water pump
DC Water Pump	Pumps water through the pipe
Lithium Battery	Provides power
TP4056 Module	Lithium battery charging
Battery Holder	Holds the power source
Rubber Pipe	Carries water for suppression

Machine Learning Model

A custom dataset containing approximately 3,527 images was prepared for training the YOLOv8 model.

The model was trained to recognize the following classes:

Class	Description
Fire	Identifies visible flames
Smoke	Identifies smoke
Non-Fire	Identifies scenes without fire

Training Configuration

* Model: YOLOv8s
* Framework: Ultralytics
* Training Platform: Google Colab
* GPU: NVIDIA Tesla T4
* Training Epochs: 50
* Dataset Size: Approximately 3,527 images
* Model Weights: my_trained_model.pt

Initial Training Results

Evaluation Metric	Result
mAP@50	0.451
mAP@50-95	0.208

These results represent the initial training experiment. Further dataset improvements and model optimization are required to improve detection performance.

Project Workflow

1. The ESP32-CAM captures the surrounding environment and provides a live video stream.
2. The Flask application receives the camera stream.
3. Video frames are processed using the trained YOLOv8 model.
4. The model identifies fire, smoke, or non-fire objects.
5. Detection results are displayed on the web dashboard.
6. When fire or smoke is detected, a notification is sent through the Telegram bot.
7. The flame sensor provides an additional source of fire detection.
8. The microcontroller controls the servo motor and water pump for the suppression mechanism.

Project Structure

FireGuard-AI/
│
├── dataset/
│   ├── images/
│   │   ├── train/
│   │   └── val/
│   └── labels/
│       ├── train/
│       └── val/
│
├── models/
│   └── my_trained_model.pt
│
├── templates/
│   └── index.html
│
├── static/
│   ├── css/
│   ├── js/
│   └── images/
│
├── esp32_cam/
│   └── camera_stream.ino
│
├── esp8266/
│   └── fire_sensor_control.ino
│
├── app.py
├── requirements.txt
└── README.md

The structure above is a reference structure. Adjust filenames and folders according to the actual repository.

Installation and Setup

1. Clone the Repository

git clone https://github.com/your-username/FireGuard-AI.git
cd FireGuard-AI

2. Create a Virtual Environment

python -m venv venv

Activate the environment:

Windows:

venv\Scripts\activate

Linux / macOS:

source venv/bin/activate

3. Install Dependencies

pip install -r requirements.txt

4. Configure the Model

Place the trained model file inside the models directory:

models/my_trained_model.pt

Update the model path in the application if required.

5. Configure Telegram Notifications

Create a Telegram bot using BotFather and configure the bot token and chat ID in the application.

Keep credentials in environment variables rather than committing them to GitHub.

6. Configure the Hardware

* Connect the ESP32-CAM to the network.
* Configure the camera stream.
* Connect the flame sensor to the ESP8266.
* Connect the relay module, servo motor, and water pump.
* Verify the power supply and hardware connections.

7. Run the Application

python app.py

Open the application in your browser using the local address displayed by Flask.

Future Improvements

* Improve model accuracy using a larger and more diverse dataset.
* Reduce false-positive and false-negative detections.
* Improve detection speed for real-time operation.
* Add additional environmental sensors.
* Improve coordination between camera detection and flame sensor readings.
* Implement automatic fire suppression with improved directional control.
* Add detection history and event logging.
* Improve the web dashboard and monitoring interface.

Disclaimer

FireGuard AI is an academic and experimental project. It is intended for demonstration and research purposes. It should not be considered a replacement for certified fire detection, alarm, or suppression systems.

Author

Anurag Tiwari

Computer Science and Engineering

Project: FireGuard AI

⸻
