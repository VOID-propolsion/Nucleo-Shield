# Astrea

## Useful links:

https://dev.to/pgradot/cmake-on-stm32-the-beginning-3766
https://www.catsystems.io/vega
https://github.com/catsystems/cats-embedded

# Team Void - Electronics overview

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
- **Type:** STM32F7 Series
- **Description:** Serves as the primary controller for navigation, sensor data processing, and actuator management.

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

1. Daniel - Team leader and focuses on PCB design, microcontroller programming and integration.
2. Mat - Responsible for Simulation, programming and testing..
3. Ismail - Specializes in power management and programming.
4. Yassin - Programming and testing.

## Getting Started
To get started with this project, clone this repository to your local machine using:

```bash
git clone https://github.com/danielsalyi/Astrea
```

# Project Setup Guide

## Opening the Project in VS Code

1. **Launch Visual Studio Code.**
2. **Open the Project:**
   - Go to `File > Open Folder...`
   - Navigate to the directory where you've cloned the repository and click `Open`.

## Setting Up Your Environment

1. **Install the ARM Toolchain:**
   - Windows: Download and install from [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads).
   - macOS and Linux: Use package managers like Homebrew or apt to install the ARM GCC compiler.

2. **Install Required VS Code Extensions:**
   - **C/C++**: Provides IntelliSense and debugging support.
   - **Cortex-Debug**: Enables debugging STM32 devices directly from VS Code.

## Compiling and Uploading the Code

1. **Build the Project:**
   - Open a terminal in VS Code (`Terminal > New Terminal`).
   - Run the command:
     ```
     make
     ```
     This compiles the project using the Makefile.

2. **Upload the Code to the Board:**
   - Ensure your Nucleo board is connected to your PC.
   - Start OpenOCD with:
     ```
     openocd -f openocd.cfg
     ```
   - In another terminal, use GDB to load the compiled binary:
     ```
     arm-none-eabi-gdb build/your-output-file.elf
     (gdb) target extended-remote :3333
     (gdb) load
     (gdb) run
     ```
