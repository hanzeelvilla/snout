# SNOUT
Snout is a smart collar designed to keep your pet safe and healthy. It uses a built-in GPS to track their location, a temperature sensor to monitor their well-being, and an IMU to follow their activity levels. This information is sent to a mobile app, where you can see a real-time view of your pet's whereabouts and a history of their activity. The collar's key feature is the "safe zone" alert system, which sends an instant notification to your phone if your pet ever wanders outside of a pre-set boundary.

# System Architecture
![System Architecture](./images/system-architecture.jpg)


## Components

### Hardware

| **Amount** |   **Name**   |    **Description**    |
|:----------:|:------------:|:---------------------:|
|      2     |     ESP32    |    Main Controller    |
|      2     | LoRa RYLR998 |     Communication     |
|      1     |    NEO-6M    |          GPS          |
|      1     |   MPU 6050   | Detect activity level |
|      1     |    DS18B20   | Measure temperature   |

FALTA PONER LA PILA

### Software
`WiFi.h`: Handles WiFi connectivity.

# Special Thanks
- Esli Castellanos
- Raymundo Buenrostro