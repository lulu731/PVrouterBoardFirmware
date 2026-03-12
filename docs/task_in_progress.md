# Task In Progress Summary

## Task Description
Modify the server handlers in `lib/server/uri_handlers.c` to automatically send a JSON object every 2 seconds to connected clients.

## Requirements
- Send JSON object: `{"objects":[{"id":"Umain","value":230}, {"id":"IL","value":2}, {"id":"IN","value":3}]}`
- Send interval: Every 2 seconds
- Only send when clients are connected (check before sending)

## Implementation Plan

### Step 1: Modify `lib/server/server.c`
- Add include for `esp_timer.h`
- Create a periodic timer using `esp_timer_create()` that fires every 2 seconds
- In the timer callback:
  - Check if any clients are connected using `httpd_get_client_list()`
  - Only send JSON message if clients exist
- Add `server_start_periodic_broadcast()` function to start the timer
- Add `server_stop_periodic_broadcast()` function to stop the timer

### Step 2: Modify `lib/server/server.h`
- Add declarations:
  - `server_err_t server_start_periodic_broadcast(void);`
  - `server_err_t server_stop_periodic_broadcast(void);`

### Step 3: Modify `lib/app/app.c`
- Call `server_start_periodic_broadcast()` after successful `server_start()`
- Call `server_stop_periodic_broadcast()` before `server_stop()`

### Step 4: Create `docs/decisions.md`
- Document the architectural decision (Option 1 vs Option 2)

## Architecture Decision
- **Option 1 (Chosen)**: Check client count before sending - timer runs every 2 seconds but only sends when clients are connected
- **Option 2**: Start/stop timer on connect/disconnect - more efficient but complex due to WebSocket disconnect detection challenges
- **Reason for Choice**: Simpler implementation, robust, avoids complex disconnect detection logic
