# Test Execution Report — Ceedling Run

**Date:** 2024-04-24
**Commit:** `fcd6aa18f00205bac6cafbee91dc0ca0ca45b4fc`
**Environment:** ESP32-S3 firmware (PVrouterBoardFirmware), ESP-IDF 5.5.0, PlatformIO

---

## Executive Summary

| Metric | Count |
|--------|-------|
| **Total Tests** | 75 |
| **Passed** | 67 |
| **Failed** | 8 |
| **Ignored** | 0 |
| **Pass Rate** | 89.3 % |

The test suite covers 15 modules. **5 modules** contain at least one failing test and require attention before the next release or merge.

---

## Failing Tests by Module

### 1. `test/app/test_app.c` (1 failure)

| Test | Line | Failure |
|------|------|---------|
| `test_init_adc_loads_calibration_params` | 119 | `Expected TRUE Was FALSE` |

**Observation:** The ADC initialisation path does not successfully load calibration parameters. This may indicate a regression in the calibration-load sequence or a stub/fake mismatch in the test harness.

---

### 2. `test/server/test_handlers.c` (3 failures)

| Test | Line | Failure |
|------|------|---------|
| `test_index_handler_with_null_session_ctx_should_create_ctx` | 63 | `Expected Non-NULL` |
| `test_index_handler_with_non_null_session_ctx_should_return_ok` | 102 | `Expected 0 Was -1` |
| `test_calibration_handler_with_null_session_ctx_should_create_ctx` | 108 | `Expected Non-NULL` |

**Observation:** URI handler tests are failing on session-context creation and return-code expectations. The handlers appear to return `NULL` or `-1` where success is expected. This likely affects the web-server’s ability to serve `index.html` and `calibration.html`.

---

### 3. `test/server/test_server.c` (1 failure)

| Test | Line | Failure |
|------|------|---------|
| `test_server_start_should_define_last_uri_as_websocket` | 69 | `Expected '/ws' Was '/script.js'` |

**Observation:** The last registered URI is `/script.js` instead of the expected `/ws` websocket endpoint. This suggests that the URI registration order changed or the websocket handler is not being appended correctly.

---

### 4. `test/server/test_websocket_handler.c` (1 failure)

| Test | Line | Failure |
|------|------|---------|
| `test_ws_handler_should_send_calibration_on_first_message` | 290 | `Expected '{"objects":[{"id":"Ugain","value":1000},...]}' Was NULL` |

**Observation:** On the first websocket message the server is expected to push the current calibration JSON payload, but `NULL` is being sent instead. This may be coupled with the calibration-load failure in `test_app.c`.

---

### 5. `test/test_nvs_driver/test_nvs_driver.c` (2 failures)

| Test | Line | Failure |
|------|------|---------|
| `test_get_first_nvs_data` | 19 | `Expected 'Mc' Was NULL` |
| `test_get_next_nvs_data` | 19 | `Expected 'Un' Was NULL` |

**Observation:** NVS iterator success-path tests expect keys from the `nvs_datas` array but receive `NULL`. The `test/app/stub_nvs.c` stub only fills entry info when `nvs_entry_info_should_fill_key` is set. Previously these tests were inadvertently passing because Ceedling compiled the wrong `stub_nvs.c` (basename collision with `test/test_nvs_driver/stub_nvs.c`).

---

## Recommended Next Steps

1. **Calibration / ADC pipeline** — Investigate `test_init_adc_loads_calibration_params` and `test_ws_handler_should_send_calibration_on_first_message` together; the root cause may be shared.
2. **Web-server handlers** — Review session-context allocation and URI registration order in `lib/server/`.
3. **NVS driver stubs** — Update `test/app/stub_nvs.c` to support the iterator data behaviour required by `test_nvs_driver.c`, or adjust the test setup to set `nvs_entry_info_should_fill_key`.

---

*Generated automatically from Ceedling output.*
