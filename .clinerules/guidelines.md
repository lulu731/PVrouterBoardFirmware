---
  description: Project guidelines and key technical concepts for PVrouterBoardFirmware.
  priority: high
  version: 1.0
---

# Project Guidelines

## Current Work

This is an ESP32-S3 firmware project (PVrouterBoardFirmware) for a photovoltaic router board. The project uses:
- ESP-IDF 5.5.0 as the underlying framework
- PlatformIO as the build system
- LittleFS for file system storage
- Calibration system for ADC measurements
- Web server for configuration and monitoring
- NVS (Non-Volatile Storage) for persistent configuration

The firmware handles:
- ADC measurements for voltage and current monitoring
- Relay control for switching
- WiFi connectivity
- Web-based user interface
- Calibration parameters storage

## Key Technical Concepts

- **ESP32-S3**: Xtensa dual-core 32-bit LX7 microcontroller
- **ESP-IDF 5.5.0**: Espressif IoT Development Framework
- **PlatformIO**: Cross-platform build system for embedded development
- **LittleFS**: Little file system for embedded devices
- **ADC**: Analog-to-Digital Converter for sensor readings
- **NVS**: Non-Volatile Storage for configuration persistence
- **FreeRTOS**: Real-time operating system for task scheduling
- **Conventional Commits**: Git commit message format (type(scope): description)
- **Cline Rules**: AI assistant configuration rules stored in .clinerules directory

## Tests

### Writing tests
You should use TDD as possible:
- first, write a test;
- execute the test to fail;
- implement the code to test;
- the test should succeed.
When needed, mocks, stubs and fakes should be implemented as lightly as possible, without using CMock.

### Executing tests
You should use ceedling. Examples:
- command "ceedling", to exexcute all tests;
- command "ceedling test:app", to test the app module in test/app/test_app.c

## compiling project
Use the command "pio run -e esp32-s3-devkitc-1".

## Before committing
When the user wants you to git commit, compile the project, then execute all the tests.
In case of errors, make proposals to solve the errors, wait for the user.
If there are no error, execute the command to commit code.

## User requests
Ask question if something is not clear.