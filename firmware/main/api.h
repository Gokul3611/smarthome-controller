/**
 * REST API Handler for Flutter App Integration
 * Provides local control endpoints when device is on same network
 */

#ifndef API_H
#define API_H

#include <WebServer.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "config.h"

// ================================================================
// GLOBAL OBJECTS
// ================================================================
extern WebServer localServer;
extern WebSocketsServer webSocket;

// ================================================================
// API INITIALIZATION
// ================================================================
void initLocalAPI();
void handleLocalAPIRequests();
void broadcastStateUpdate();

// ================================================================
// REST API ENDPOINTS
// ================================================================

/**
 * GET /status
 * Returns current state of all devices
 * Response: {
 *   "devices": [
 *     {"id": 0, "name": "Light 1", "type": 0, "state": true, "brightness": 75, "runtime": 3600},
 *     ...
 *   ]
 * }
 */
void handleGetStatus();

/**
 * POST /control
 * Control a specific device
 * Body: {
 *   "id": 0,
 *   "state": true,
 *   "brightness": 75  // optional, for dimmers/fans
 * }
 * Response: {"success": true}
 */
void handlePostControl();

/**
 * GET /info
 * Get system information
 * Response: {
 *   "name": "Living Room Hub",
 *   "firmware": "3.0",
 *   "mac": "AA:BB:CC:DD:EE:FF",
 *   "ip": "192.168.1.100",
 *   "uptime": 3600,
 *   "rssi": -45,
 *   "heap": 180000,
 *   "cloud_connected": true
 * }
 */
void handleGetInfo();

/**
 * POST /config
 * Update device configuration
 * Body: {
 *   "device_id": 0,
 *   "name": "Living Room Light",
 *   "type": 2,  // DIMMER
 *   "power_on_state": 2  // LAST
 * }
 * Response: {"success": true}
 */
void handlePostConfig();

/**
 * GET /schedules
 * List all schedules
 * Response: {
 *   "schedules": [
 *     {
 *       "id": 0,
 *       "device_id": 0,
 *       "start_time": "18:00",
 *       "end_time": "23:00",
 *       "start_brightness": 0,
 *       "end_brightness": 75,
 *       "active": true,
 *       "days": [1,2,3,4,5]  // Mon-Fri
 *     }
 *   ]
 * }
 */
void handleGetSchedules();

/**
 * POST /schedules
 * Create or update a schedule
 * Body: {
 *   "id": 0,  // -1 for new
 *   "device_id": 0,
 *   "start_time": "18:00",
 *   "end_time": "23:00",
 *   "start_brightness": 0,
 *   "end_brightness": 75,
 *   "active": true,
 *   "days": [1,2,3,4,5]
 * }
 * Response: {"success": true, "id": 0}
 */
void handlePostSchedule();

/**
 * DELETE /schedules/{id}
 * Delete a schedule
 * Response: {"success": true}
 */
void handleDeleteSchedule();

/**
 * GET /scenes
 * List all scenes
 * Response: {
 *   "scenes": [
 *     {
 *       "id": 0,
 *       "name": "Movie Time",
 *       "devices": [
 *         {"id": 0, "state": false},
 *         {"id": 1, "state": true, "brightness": 20}
 *       ]
 *     }
 *   ]
 * }
 */
void handleGetScenes();

/**
 * POST /scenes
 * Create or update a scene
 * Body: {
 *   "id": 0,  // -1 for new
 *   "name": "Movie Time",
 *   "devices": [...]
 * }
 * Response: {"success": true, "id": 0}
 */
void handlePostScene();

/**
 * POST /scenes/{id}/activate
 * Activate a scene
 * Response: {"success": true}
 */
void handleActivateScene();

/**
 * DELETE /scenes/{id}
 * Delete a scene
 * Response: {"success": true}
 */
void handleDeleteScene();

/**
 * POST /restart
 * Restart the device
 * Response: {"success": true}
 */
void handleRestart();

/**
 * POST /factory-reset
 * Factory reset the device
 * Body: {"confirm": true}
 * Response: {"success": true}
 */
void handleFactoryReset();

// ================================================================
// WEBSOCKET HANDLERS
// ================================================================

/**
 * Initialize WebSocket server
 */
void initWebSocket();

/**
 * Handle WebSocket events
 */
void handleWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

/**
 * Broadcast device state change to all connected WebSocket clients
 */
void broadcastDeviceState(int deviceId);

/**
 * Broadcast system status to all connected WebSocket clients
 */
void broadcastSystemStatus();

// ================================================================
// UTILITY FUNCTIONS
// ================================================================

/**
 * Send JSON response
 */
void sendJsonResponse(int code, const JsonDocument& doc);

/**
 * Send error response
 */
void sendErrorResponse(int code, const char* message);

/**
 * Validate device ID
 */
bool isValidDeviceId(int id);

/**
 * Parse JSON body from request
 */
bool parseJsonBody(JsonDocument& doc);

#endif // API_H
