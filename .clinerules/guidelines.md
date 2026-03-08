# Project Guidelines

## Current Work

This is an ESP32-S3 firmware project (PVrouterBoardFirmware) for a photovoltaic router board. The project uses:
- ESP-IDF as the underlying framework
- PlatformIO as the build system
- LittleFS for file system storage
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
- **ESP-IDF**: Espressif IoT Development Framework
- **PlatformIO**: Cross-platform build system for embedded development
- **LittleFS**: Little file system for embedded devices
- **ADC**: Analog-to-Digital Converter for sensor readings
- **NVS**: Non-Volatile Storage for configuration persistence
- **FreeRTOS**: Real-time operating system for task scheduling
- **Conventional Commits**: Git commit message format (type(scope): description)
- **Cline Rules**: AI assistant configuration rules stored in .clinerules directory
