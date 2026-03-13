# Task In Progress Summary

## Task Description
For now, the periodic_broadcast_callback timer callback function in @/lib/server/server.c does not update the json object.

## Requirements
- The values in JSON object: `{"objects":[{"id":"Umain","value":230}, {"id":"IL","value":2}, {"id":"IN","value":3}]}` should be updated before to be brodcasted.
- I think that the id parameters should conform to registers in @/lib/adc/adc_registers.c : U_RMS, I_RMS and I_RMS_2

## Implementation Plan

### Step 1: Create Tests (TDD approach)

1. **Add tests to `test/json/test_json.c`**:
   - Add test for new JSON function that handles floating-point values
   - Test format: `{"objects":[{"id":"U_RMS","value":123.45}, ...]}`

2. **Add tests to `test/server/test_server.c`**:
   - Add tests for value conversion (raw ADC to physical units):
     - Voltage: raw / 100.0
     - Current: raw / 1000.0
   - Test the periodic broadcast functionality

### Step 2: Implement the Solution

1. **Add new function to `lib/json/json.c`**:
   - Add a struct for float values (e.g., `gain_object_float`)
   - Add function to create JSON with float values using cJSON

2. **Add declaration to `lib/json/json.h`**:
   - Declare the new struct and function

3. **Modify `lib/server/server.c`**:
   - Call the new JSON function from `periodic_broadcast_callback`
   - Read U_RMS, I_RMS, I_RMS_2 registers
   - Convert values (voltage/100, current/1000)
   - Broadcast to WebSocket clients

### JSON Output Format:
```json
{"objects":[{"id":"U_RMS","value":123.45}, {"id":"I_RMS","value":12.345}, {"id":"I_RMS_2","value":...}]}
```

### Implementation Details:
- Values will be sent as floating-point numbers
- The IDs will match the register names from `adc_registers.c`
- The callback will read actual ADC values and convert them before broadcasting
- **cJSON should only be used in `lib/json/json.c`**
- Tests for JSON function in `test/json/test_json.c`
- Tests for value conversion in `test/server/test_server.c`

## Questions
Ask questions about the task when needed for understanding or planning.
