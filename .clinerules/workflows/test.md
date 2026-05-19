# PVrouterBoardFirmware Workflow

Build and test workflow using PlatformIO.

## Step 1: Complete Workflow

To run the complete build and test workflow using PlatformIO:

```bash
#!/bin/bash

# Build
echo "=== Building for esp32-s3-devkitc-1 ==="
pio run -e esp32-s3-devkitc-1
BUILD_STATUS=$?

if [ $BUILD_STATUS -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Test
echo "=== Running Ceedling tests ==="
ceedling test:all
TEST_STATUS=$?

if [ $TEST_STATUS -ne 0 ]; then
    echo "Tests failed!"
    exit 1
fi

echo "=== Build and tests completed successfully ==="
exit 0
```
