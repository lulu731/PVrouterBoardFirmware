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

**Fix:** Add `vTaskDelay(pdMS_TO_TICKS(20))` or use ets_delay_us() between the two calls.

---

### 6. Race Condition in `server.c` - `periodic_broadcast_callback()`
**File:** `lib/server/server.c`
**Issue:** The `web_server` handle is accessed without synchronization. If `server_stop()` is called from another task while the timer callback runs, this is a data race.

**Fix:** Use a mutex to protect `web_server` access, or ensure the timer is stopped before `server_stop()` is called.

---

### 7. Unused Memory Allocation in `server_send_to_all_clients()`
**File:** `lib/server/server.c`
**Issue:** `malloc(sizeof(int) * fds)` could be simplified to `malloc(sizeof(int[fds]))` or use `calloc(fds, sizeof(int))`.

---

### 8. Incomplete Error Handling in `calibration_load_params.c`
**File:** `lib/calibration/calibration_load_params.c`
**Issue:** The `load_calibration_params()` function silently continues if `load_param()` finds no match. If a key in NVS doesn't match any expected key, there's no warning.

---

### 9. Potential Division by Zero in `write_PL_constant()`
**File:** `lib/adc_functions/adc_functions.c`
**Issue:**
```c
const float pl_float = 838860800 * (float)(Gl * Vl * Vu) / (float)(Mc * Un * Ib);
```
If any of `Mc`, `Un`, or `Ib` is zero, this causes division by zero (undefined behavior).

**Fix:** Add a check for zero values before division.

---

### 10. Commented-Out Code Clutter
**Files:** Multiple files
**Issue:** Significant blocks of commented-out code in `app.c`, `main.c`, `server.c`, and `calibration.c` make the codebase harder to read and maintain.

**Fix:** Remove dead code or move it to a branch/feature flag if needed for future reference.

---

## 🟢 Suggestions

### 11. Inconsistent Naming Conventions
**Issue:** Mix of snake_case (`init_adc`, `load_calibration_params`) and camelCase (`create_broadcast_json_message`, `save_req_session_context`).

**Fix:** Pick one convention (snake_case is more common in ESP-IDF) and apply consistently.

---

### 12. Magic Numbers
**Files:** Multiple
**Issue:** Numbers like `838860800`, `100`, `1000`, `2000000`, `0xA987`, `0x23` appear without explanation.

**Fix:** Define named constants or enums with explanatory comments.

---

### 13. Missing `const` Qualifier in `write_gain_register()`
**File:** `lib/adc_functions/adc_functions.c`
**Issue:** Function signature takes pointer but could use const for read-only access.

---

### 14. Dead Code in `nvs_driver.c`
**File:** `lib/nvs_driver/nvs_driver.c`
**Issue:** `nvs_write_u16()` is defined but never used in the codebase (checked via search).

---

## Summary

**Overall Code Quality:** Fair

**Fixes Applied:**
- ✅ #1: Memory Leak in ws_handler() - Fixed in commit
- ✅ #2: Buffer Overflow in json_parse_gain_object() - Fixed in commit
- ✅ #3: Signed Integer Overflow in get_main_real_power() - Fixed in commit
- ✅ #4: Hardcoded WiFi Credentials (commit `6a242cf`)

**Remaining Issues:**
- 6 🟡 Important (Issues #5-10)
- 4 🟢 Suggestions (Issues #11-14)

**Top 3 Action Items:**
1. Add relay pulse timing in `trigger_relay()` (functional correctness)
2. Fix the division by zero in `write_PL_constant()` (robustness)
3. Fix race condition in `server.c` (concurrency safety)
