# Astrea - Team Void

## Project Overview
This repository contains the code for Team Void's rocket electronics system, designed for the European Rocketry Challenge (EuRoC). The system manages navigation, power, and communication functionalities essential for rocket operation.

## System Architecture

```mermaid


graph TB
    MCU["Microcontroller Unit (STM32F7 Series)"]

        MCU -->|Controls| COM["Communication (UHF Transceiver, GPS)"]
        MCU -->|Manages Power| PM["Power Management (LiPo Battery, Power Board)"]
        MCU -->|Processes Data from| SEN["Sensors (Barometric, IMU, Optional Fuel Level Indicator)"]
        MCU -->|Controls| ACT["Actuators (Electric Match Ignition)"]
        MCU -->|Operates| SER["Servo System (Valve Operation)"]
        MCU -->|Monitors| RED["Redundant System (Eggfinder TRS)"]

        COM -->|Links to| CHG["Charging System (On-board Interface)"]
        PM -->|Supplies Power To| SEN
        PM -->|Supplies Power To| ACT
        PM -->|Supplies Power To| SER
        PM -->|Supplies Power To| RED
        PM -->|Supplies Power To| OUT["Output Indicators (LEDs)"]
        PM -->|Supplies Power To| CHG
        
        SEN -->|Data to| ACT
        ACT -->|Trigger| SER
        RED -->|Backup to| SER
        
        CHG -->|Charges| BAT["Batteries"]
        BAT -->|Provides Backup Power| RED
        BAT -->|Provides Backup Power| ACT
        BAT -->|Provides Backup Power| SEN

        OUT -->|Indicates Status| PM
        OUT -->|Indicates Status| MCU
        OUT -->|Indicates Status| COM


```


Our rocket's electronics system centers around a main microcontroller unit (MCU) with a backup redundant controller and with several modules:

### Microcontroller Unit (MCU)
- **Type:** STM32F466
- **Description:** Serves as the primary micro-controller.

### Communication
- **Components:** UHF Transceiver (LoRa), GPS Tracking (Ublox NEO-6M)
- **Function:** Provides long-range telemetry and precise GPS positioning.

### Power Management
- **Components:** LiPo Battery, Power Distribution Board
- **Function:** Manages power distribution and voltage regulation to ensure stable operation.

### Sensors
- **Components:** Barometric Sensor (BMP280), IMU (MPU-6050), Optional Fuel Level Indicator
- **Function:** Vital for determining altitude and maintaining vehicle stability.

### Actuators
- **Components:** Electric match ignition system
- **Function:** Activates the parachute deployment at predetermined altitudes for safe recovery.

### Redundant System
- **Components:** Eggfinder TRS
- **Function:** Enhances mission reliability with backup GPS and altitude logging.

### Batteries
- **Description:** Extra battery packs to support extended operations and emergency scenarios.

### Charging System
- **Description:** Facilitates on-board charging of the main and auxiliary batteries.

### Output Indicators
- **Description:** LEDs that provide visual feedback on the system's operational status.

## Team Members

1. Daniel 
2. Mat 
3. Ismail 
4. Yassin 


# Project Setup Guide

## URefrences and detailed guides could be found here:

https://dev.to/pgradot/cmake-on-stm32-the-beginning-3766
https://www.catsystems.io/vega
https://github.com/catsystems/cats-embedded

For quick setup and to get started with this project using VS code, clone this repository to your local machine using:

```bash
git clone https://github.com/danielsalyi/Astrea
```

## Setting Up Your Environment
1. Download VScode and cubeMX
2. Install stm32-for-vscode extension, click automatic install or something?
3. Build and flash the project 

## Useful links:

https://dev.to/pgradot/cmake-on-stm32-the-beginning-3766
https://www.catsystems.io/vega
https://github.com/catsystems/cats-embedded
