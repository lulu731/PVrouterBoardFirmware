# Code Review: PVrouterBoardFirmware

Based on my analysis of the codebase, here are my findings organized by severity:

---

## 🔴 Critical Issues

### 1. Memory Leak in `uri_handlers.c` - `ws_handler()`
**File:** `lib/server/uri_handlers.c`
**Issue:** The `ws_payload` is allocated in `get_ws_payload()` via `calloc()`, but there's a path where it's not freed if `json_parse_gain_object()` returns `NULL`. The check `if (obj == NULL)` frees `ws_payload`, but if `json_parse_gain_object()` returns `&null_gain_object` instead, the payload is leaked.

**Fix:** Move `free(ws_payload)` to a separate cleanup section or use a `goto cleanup` pattern.

---

### 2. Buffer Overflow in `json_parse_gain_object()`
**File:** `lib/json/json.c`
**Issue:**
```c
object->key = malloc(strlen(json_id->valuestring) + 1);
strncpy(object->key, json_id->valuestring, sizeof(object->key)); // BUG!
```
`sizeof(object->key)` is `sizeof(char*)` (8 bytes on 64-bit), not the allocated size. This truncates long strings and doesn't copy the full content.

**Fix:** Use `strlen(json_id->valuestring) + 1` as the third argument to `strncpy`.

---

### 3. Signed Integer Overflow in `get_main_real_power()`
**File:** `lib/system/system.c`
**Issue:**
```c
int16_t get_main_real_power()
{
    read_adc_register(&P_MEAN);
    if (P_MEAN.data >> 15 == 1)
        return -(~P_MEAN.data);  // Can overflow for 0x8000
    return P_MEAN.data;
}
```
When `P_MEAN.data == 0x8000`, `~0x8000 = 0x7FFF`, and `-0x7FFF = -32767` is fine. But the logic is fragile. The intent is unclear—should it return `-32768` or `32767` for `0x8000`?

**Fix:** Clarify the intended behavior and use explicit masking: `return -(int16_t)(P_MEAN.data);`

---

### 4. Hardcoded WiFi Credentials in `wifi_connect.c`
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

**Fix:** Require explicit configuration or fail compilation if not provided.

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
The code has a clear structure and good documentation in most places. However, there are several correctness issues (memory leak, buffer overflow, potential division by zero) that should be addressed. The commented-out code blocks and inconsistent naming suggest the codebase is in an active development/refactoring phase.

**Top 3 Action Items:**
1. Fix the buffer overflow in `json_parse_gain_object()` (security/correctness)
2. Add relay pulse timing in `trigger_relay()` (functional correctness)
3. Fix the division by zero in `write_PL_constant()` (robustness)