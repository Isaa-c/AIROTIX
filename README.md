# AIROTIX

Airotix is a real-time IoT-based air quality monitoring system designed to track key environmental pollutants contributing to respiratory illnesses such as Chronic Obstructive Pulmonary Disease (COPD). Built with the Arduino UNO R4 WiFi microcontroller, Airotix uses multiple air sensors to collect and transmit particulate and gas data to the Arduino IoT Cloud. The system supports threshold alerting, remote monitoring, and solar-powered deployment, making it suitable for both individual health users and environmental agencies.

Features
Real-time monitoring of PM1.0, PM2.5, PM10, NO₂, ozone, voltage, current, and power

Threshold-based Boolean flags for warning and abnormal conditions

Dual cloud dashboard design for agencies (admin-level) and individuals (read-only)

Solar-powered with real-time power tracking via INA219 sensor

Modular internal design with removable Arduino and sensor terminals

Developer API support for third-party applications

Data logging and export for analysis and reporting


Hardware Components
Arduino UNO R4 WiFi (core microcontroller)

PMS5003 sensor (particulate matter)

MiCS-4514 sensor (NO₂ and CO detection)

MQ135 sensor (VOC, ammonia, CO₂, benzene, etc.)

Analog ozone sensor (ozone gas detection)

INA219 (voltage, current, power monitoring)

Solar panel (18V oversized to charge 12V system)

Terminal blocks and zero PCB for modular wiring

Custom enclosure with airflow channels and tripod mount


Functional Overview
The device reads environmental and power parameters through its sensors and periodically updates their values to the Arduino IoT Cloud. Threshold values for each pollutant can be set by authorized agencies. When a pollutant level crosses either a warning or abnormal threshold, corresponding Boolean variables are updated and used to trigger alerts. Individuals accessing the read-only dashboard are able to view current readings, graphical trends, and notifications.


Cloud Variables and Structure
Airotix uses a structured cloud variable system, including:

Sensor values (float): e.g., pm2_5, ozonePinInput, no2ChannelPin, powerSensorPin

Threshold values (float): e.g., pm2_5WarningThreshold, ozoneAbnormalThreshold

Status booleans (bool): e.g., pm2_5AbnormalReached, no2WarningReached

Thresholds are only configurable from the agency dashboard, ensuring individuals cannot tamper with alert criteria.


API Access
The Airotix system includes a RESTful API for developer integration. Authorized applications can access live and historical sensor data, including threshold states. JSON-format responses are available for real-time integration with mobile or web dashboards. Authentication is token-based.

Dashboard Design
The system uses two dedicated dashboards:

Agency dashboard: Full access to data, threshold configuration, download logs, and control alerts.

Individual dashboard: Read-only access to sensor values, Boolean alert statuses, and historical charts. Intended for community visibility and personal awareness.

Calibration
PMS5003: Factory calibrated; placement should ensure continuous airflow.

MiCS-4514: 90-second preheat followed by 30-second NO₂ baseline calibration in clean air.

MQ135: Requires outdoor calibration to determine R₀ baseline in clean conditions.

Power Management
The device is fully solar-powered and designed for off-grid operation. A robust 18V solar panel (oversized intentionally) charges a 12V battery via a solar charge controller. The INA219 sensor monitors current, voltage, and power usage, and uploads these parameters to the cloud for energy diagnostics.

Internal Layout
All sensor wires terminate at terminal blocks mounted on a zero PCB. The Arduino UNO R4 is inserted into male header pins, allowing it to be swapped out easily if needed. Sensors are externally exposed through holes cut into the enclosure, allowing direct ambient sampling without airflow obstruction.

How to Deploy
Assemble the components on a zero PCB using terminal blocks and header pins.

Flash the firmware from this repository onto the Arduino UNO R4 WiFi.

Configure cloud variables on Arduino IoT Cloud exactly as listed in the documentation.

Mount the system in an urban area with unobstructed airflow and solar exposure.

Connect the system to Wi-Fi using the credentials in thingProperties.h.



Acknowledgements
This project was developed with the goal of supporting public health awareness and environmental transparency. It is designed for scalability, educational use, and institutional deployment.
