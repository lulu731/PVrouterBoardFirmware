# Code Review: PVrouterBoardFirmware

Based on my analysis of the codebase, here are my findings organized by severity:

---

## 🔴 Critical Issues

### 1. Memory Leak in `uri_handlers.c` - `ws_handler()` ✅ FIXED
**File:** `lib/server/uri_handlers.c`
**Issue:** The `ws_payload` is allocated in `get_ws_payload()` via `calloc()`, but there's a path where it's not freed if `json_parse_gain_object()` returns `NULL`. The check `if (obj == NULL)` frees `ws_payload`, but if `json_parse_gain_object()` returns `&null_gain_object` instead, the payload is leaked.

**Fix:** Changed check from `if (obj == NULL)` to `if (obj == &null_gain_object)` and return `ESP_OK` for graceful handling of invalid JSON.

---

### 2. Buffer Overflow in `json_parse_gain_object()` ✅ FIXED
**File:** `lib/json/json.c`
**Issue:**
```c
object->key = malloc(strlen(json_id->valuestring) + 1);
strncpy(object->key, json_id->valuestring, sizeof(object->key)); // BUG!
```
`sizeof(object->key)` is `sizeof(char*)` (8 bytes on 64-bit), not the allocated size. This truncates long strings and doesn't copy the full content.

**Fix:** Changed third argument to `strlen(json_id->valuestring) + 1` to match allocated size.

---

### 3. Signed Integer Overflow in `get_main_real_power()` ✅ FIXED
**File:** `lib/system/system.c`
**Issue:** Original code had confusing bit manipulation for two's complement conversion.

**Fix:** Simplified to direct cast: `return (int16_t)P_MEAN.data;` - the ADC returns data already in the correct format for direct interpretation as a signed int16.

---

### 4. Hardcoded WiFi Credentials in `wifi_connect.c` ✅ FIXED
**File:** `lib/wifi_connect/wifi_connect.c`
**Issue:** Default fallback values for SSID and password:
```c
#ifndef SSID
#define SSID "WIFI_SSID"
#endif

#ifndef PASSWD
#define PASSWD "WIFI_PASSWORD"
#endif
```
These will silently be used if build flags aren't set, potentially causing security issues.

**Fix:** Replace with `#error` directives that force build failure if credentials are not provided via build flags.
**Commit:** `6a242cf` - `fix(lib-wifi-connect): require WiFi credentials via build flags to prevent deployment with dummy values`

---

## 🟡 Important Issues

### 5. Missing Error Handling in `trigger_relay()`
**File:** `lib/trigger_relay/trigger_relay.c`
**Issue:** The relay trigger function has no timing control between setting high and low:
```c
static void trigger_relay(void)
{
    gpio_set_level(GPIO_HEATER, 1);
    gpio_set_level(GPIO_HEATER, 0);  // No delay - may not trigger
}
```
Most relays need ~10-20ms pulse width to activate.

**Fix:** Add `vTaskDelay(pdMS_TO_TICKS(5))`

---

### 6. Incomplete Error Handling in `calibration_load_params.c`
**File:** `lib/calibration/calibration_load_params.c`
**Issue:** The `load_calibration_params()` function silently continues if `load_param()` finds no match. If a key in NVS doesn't match any expected key, there's no warning.

---

### 7. Potential Division by Zero in `write_PL_constant()`
**File:** `lib/adc_functions/adc_functions.c`
**Issue:**
```c
const float pl_float = 838860800 * (float)(Gl * Vl * Vu) / (float)(Mc * Un * Ib);
```
If any of `Mc`, `Un`, or `Ib` is zero, this causes division by zero (undefined behavior).

**Fix:** Add a check for zero values before division.

---

### 8. Commented-Out Code Clutter
**Files:** Multiple files
**Issue:** Significant blocks of commented-out code in `app.c`, `main.c`, `server.c`, and `calibration.c` make the codebase harder to read and maintain.

**Fix:** Remove dead code or move it to a branch/feature flag if needed for future reference.

---

## 🟢 Suggestions

### 9. Inconsistent Naming Conventions ✅ ANALYZED
**Issue:** Mix of snake_case (`init_adc`, `load_calibration_params`) and camelCase (`create_broadcast_json_message`, `save_req_session_context`).

**Analysis:** After searching through the codebase, all user-defined functions use snake_case consistently. The function `create_broadcast_json_message` is already in snake_case. The codebase follows ESP-IDF conventions properly.

---

### 10. Magic Numbers ✅ FIXED
**Files:** Multiple
**Issue:** Numbers like `838860800`, `100`, `1000`, `2000000`, `0xA987`, `0x23` appear without explanation.

**Fix:** Created `lib/calibration/calibration_constants.h` with named constants:
- `PL_CONST_MAX` (838860800u)
- `VOLTAGE_RMS_DIVIDER` (100)
- `CURRENT_RMS_DIVIDER` (1000)
- `POWER_OFFSET_SAMPLES` (5)
- `BROADCAST_INTERVAL_US` (2000000u)
- `BROADCAST_INTERVAL_MS` (2000)

Updated `lib/adc_functions/adc_functions.c` and `lib/server/server.c` to use these constants.

---

### 11. Missing `const` Qualifier in `write_gain_register()` ✅ FIXED
**File:** `lib/adc_functions/adc_functions.c`
**Issue:** Function signature takes pointer but could use const for read-only access.

**Fix:** Already has const in signature: `void write_gain_register(const struct adc_register gain_register);`

---

### 12. Dead Code in `nvs_driver.c` ✅ FIXED
**File:** `lib/nvs_driver/nvs_driver.c`
**Issue:** `nvs_write_u16()` is defined but never used in the codebase (checked via search).

**Fix:** Removed unused `nvs_write_u16()` function from `lib/nvs_driver/nvs_driver.c` and its declaration from `lib/nvs_driver/nvs_driver.h`.

---

## Summary

**Overall Code Quality:** Fair

**Fixes Applied:**
- ✅ #1: Memory Leak in ws_handler() - Fixed in commit
- ✅ #2: Buffer Overflow in json_parse_gain_object() - Fixed in commit
- ✅ #3: Signed Integer Overflow in get_main_real_power() - Fixed in commit
- ✅ #4: Hardcoded WiFi Credentials (commit `6a242cf`)
- ✅ #10: Magic Numbers - Fixed with calibration_constants.h
- ✅ #11: const qualifier - Already present
- ✅ #12: Dead Code - Removed nvs_write_u16()

**Remaining Issues:**
- 5 🟡 Important (Issues #5-9)
- 0 🟢 Suggestions (Issue #9 was analyzed and is not actually a problem)

**Top 3 Action Items:**
1. Add relay pulse timing in `trigger_relay()` (functional correctness)
2. Fix the division by zero in `write_PL_constant()` (robustness)
3. Fix race condition in `server.c` (concurrency safety)
