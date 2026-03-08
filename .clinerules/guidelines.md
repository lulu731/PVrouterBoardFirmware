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

## Git Commit Workflow

- Before executing git commit, ask the user to approve the commit message
- Follow conventional commits format: `<type>(<scope>): <description>`
- Use appropriate type based on changes:
  - `feat` - new features
  - `fix` - bug fixes
  - `chore` - Cline rules and auxiliary tools
  - `docs` - documentation changes
  - `refactor` - code refactoring
  - `test` - test-related changes
  - `ci` - CI/CD configuration changes
- The scope should be derived from the file path (e.g., `.clinerules`, `lib/adc`)
