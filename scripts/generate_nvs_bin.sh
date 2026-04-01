#!/bin/bash
#
# Generate NVS partition binary from CSV file
# Usage: ./generate_nvs_bin.sh [input_csv] [output_bin] [size]
#

set -e

# Default values
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

INPUT_CSV="${1:-$PROJECT_ROOT/src/meter_config.csv}"
OUTPUT_BIN="${2:-$PROJECT_ROOT/meter_config.bin}"
BIN_SIZE="${3:-0x3000}"

echo "=== NVS Bin Generator ==="
echo "Input CSV:  $INPUT_CSV"
echo "Output BIN: $OUTPUT_BIN"
echo "Size:       $BIN_SIZE"

# Check if input CSV exists
if [ ! -f "$INPUT_CSV" ]; then
    echo "Error: Input CSV file not found: $INPUT_CSV"
    exit 1
fi

# Run the nvs_partition_gen.py tool via PlatformIO
pio pkg exec --package "platformio/framework-espidf" -- components/nvs_flash/nvs_partition_generator/nvs_partition_gen.py generate "$INPUT_CSV" "$OUTPUT_BIN" "$BIN_SIZE"

if [ $? -eq 0 ]; then
    echo "Success! NVS binary created: $OUTPUT_BIN"
    ls -la "$OUTPUT_BIN"
else
    echo "Error: Failed to generate NVS binary"
    exit 1
fi
