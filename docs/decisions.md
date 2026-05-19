# Architectural Decisions

## Periodic WebSocket JSON Broadcasting

**Date:** 2026-03-12
**Task:** Implement automatic JSON broadcast to WebSocket clients every 2 seconds

### Decision: Client Check Before Send (Option 1)

#### Context
The task requires sending a JSON message (`{"objects":[{"id":"Umain","value":230}, {"id":"IL","value":2}, {"id":"IN","value":3}]}`) to all connected WebSocket clients every 2 seconds.

#### Options Considered

1. **Option 1: Check Client Count Before Sending**
   - Timer runs every 2 seconds
   - Check if any clients are connected using `httpd_get_client_list()`
   - Only send JSON if clients exist

2. **Option 2: Start/Stop Timer on Connect/Disconnect**
   - Start timer when first WebSocket client connects
   - Stop timer when last client disconnects
   - More efficient (timer only runs when needed)

#### Decision Made
**Option 1** was chosen.

#### Rationale
1. **Simplicity:** Easier to implement and maintain
2. **Robustness:** No need to handle complex WebSocket disconnect detection
3. **Lightweight:** Timer overhead is minimal (just checks client count)
4. **ESP-IDF Limitation:** No built-in WebSocket disconnect callback exists, making Option 2 significantly more complex

#### Implementation Details
- Uses ESP-IDF's `esp_timer` for periodic timing
- Checks client list before sending to avoid unnecessary work
- Timer runs independently of client connections
- Graceful handling if timer fails to start

#### Files Modified
- `lib/server/server.c` - Added timer implementation
- `lib/server/server.h` - Added function declarations
- `lib/app/app.c` - Integrated with app lifecycle
