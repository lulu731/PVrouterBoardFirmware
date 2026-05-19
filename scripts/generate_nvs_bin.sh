#!/bin/bash
#
# Generate NVS partition binary from CSV file
# Usage: ./generate_nvs_bin.sh [input_csv] [output_bin] [size]
#

set -e

# Default values
export INPUT_CSV="${1:-src/meter_config.csv}"
export OUTPUT_BIN="${2:-meter_config.bin}"
export BIN_SIZE="${3:-0x3000}"

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
bash -c 'podman run -i --rm -v $(pwd):/home/dev/project -v pio_jenkins:/root pio_run_e_jenkins:1.0.0 \
pkg exec --package "platformio/framework-espidf" -- components/nvs_flash/nvs_partition_generator/nvs_partition_gen.py generate "$INPUT_CSV" "$OUTPUT_BIN" "$BIN_SIZE"'

if [ $? -eq 0 ]; then
    echo "Success! NVS binary created: $OUTPUT_BIN"
    ls -la "$OUTPUT_BIN"
else
    echo "Error: Failed to generate NVS binary"
    exit 1
fi
