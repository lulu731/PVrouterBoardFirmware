#!/bin/bash
#
# Flash NVS partition binary to ESP32-S3
# Usage: ./flash_nvs_bin.sh [port] [bin_file]
#

set -e

# Default values
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Chip parameters
CHIP="esp32s3"
FLASH_MODE="qio"
FLASH_FREQ="80m"
FLASH_SIZE="4MB"
PARTITION_OFFSET="0x9000"

# Serial port (must be provided or auto-detected)
PORT="${1:-}"
BIN_FILE="${2:-$PROJECT_ROOT/meter_config.bin}"

# Auto-detect port if not provided
if [ -z "$PORT" ]; then
    echo "No port specified, attempting auto-detection..."

    # Common ESP32 serial ports on Linux/macOS
    for p in /dev/ttyUSB* /dev/ttyACM* /dev/cu.usbserial-* /dev/cu.usbmodem*; do
        if [ -e "$p" ]; then
            PORT="$p"
            break
        fi
    done

    if [ -z "$PORT" ]; then
        echo "Error: No serial port found. Please specify port manually."
        echo "Usage: $0 <port> [bin_file]"
        exit 1
    fi
    echo "Auto-detected port: $PORT"
fi

echo "=== NVS Bin Flash Script ==="
echo "Port:          $PORT"
echo "Chip:          $CHIP"
echo "BIN File:      $BIN_FILE"
echo "Partition:     $PARTITION_OFFSET"

# Check if bin file exists
if [ ! -f "$BIN_FILE" ]; then
    echo "Error: Binary file not found: $BIN_FILE"
    echo "Run generate_nvs_bin.sh first to create the binary."
    exit 1
fi

# Run esptool.py to flash the binary
pio pkg exec --package "platformio/tool-esptoolpy" -- esptool.py \
    --chip "$CHIP" \
    -p "$PORT" \
    --before=default_reset \
    --after=no_reset \
    write_flash \
    --flash_mode "$FLASH_MODE" \
    --flash_freq "$FLASH_FREQ" \
    --flash_size "$FLASH_SIZE" \
    "$PARTITION_OFFSET" "$BIN_FILE"

if [ $? -eq 0 ]; then
    echo "Success! Binary flashed to partition at offset $PARTITION_OFFSET"
else
    echo "Error: Failed to flash binary"
    exit 1
fi
