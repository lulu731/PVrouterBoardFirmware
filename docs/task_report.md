# Task Report: Periodic WebSocket JSON Broadcasting

**Date:** 2026-03-12
**Status:** ✅ COMPLETED

## Task Description
Modify the server handlers to automatically send a JSON object every 2 seconds to connected WebSocket clients.

### Requirements
- Send JSON object: `{"objects":[{"id":"Umain","value":230}, {"id":"IL","value":2}, {"id":"IN","value":3}]}`
- Send interval: Every 2 seconds
- Only send when clients are connected (check before sending)

---

## Implementation Summary

### Files Modified

| File | Changes |
|------|---------|
| `lib/server/server.c` | Added esp_timer-based periodic broadcast with 2-second interval |
| `lib/server/server.h` | Added function declarations for timer control |
| `lib/app/app.c` | Integrated timer start with server lifecycle |
| `docs/decisions.md` | Documented architectural decision |
| `docs/task_in_progress.md` | Contains implementation plan |

### Implementation Details
- Uses ESP-IDF's `esp_timer` for reliable periodic timing
- Checks client list before sending using `httpd_get_client_list()`
- Only broadcasts when clients are connected
- Timer callback: `periodic_broadcast_callback()`
- Timer control functions: `server_start_periodic_broadcast()`, `server_stop_periodic_broadcast()`

---

## Architectural Decision

### Option 1: Check Client Count Before Sending (CHOSEN) ✅
- Timer runs every 2 seconds
- Check if any clients are connected using `httpd_get_client_list()`
- Only send JSON if clients exist

### Option 2: Start/Stop Timer on Connect/Disconnect
- Start timer when first WebSocket client connects
- Stop timer when last client disconnects

### Rationale for Choosing Option 1
1. **Simplicity:** Easier to implement and maintain
2. **Robustness:** No need to handle complex WebSocket disconnect detection
3. **Lightweight:** Timer overhead is minimal (just checks client count)
4. **ESP-IDF Limitation:** No built-in WebSocket disconnect callback exists

---

## Build Status
- ✅ Build successful for esp32-s3-devkitc-1
- RAM usage: 4.9% (16204 bytes / 327680 bytes)
- Flash usage: 41.0% (429725 bytes / 1048576 bytes)

---

## Git Commit
✅ Committed: `docs(docs): follow decisions and current task in 2 files`