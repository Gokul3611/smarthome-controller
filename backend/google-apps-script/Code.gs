/**
 * Smart Home Controller - Google Apps Script Backend
 * Version: 1.0.0
 * 
 * This script provides:
 * - RESTful API for ESP32 devices
 * - Google Sheets database integration
 * - Web dashboard hosting
 * - Device state management
 * - User authentication
 * - Scheduling & scenes management
 */

// Configuration
const CONFIG = {
  SHEET_ID: PropertiesService.getScriptProperties().getProperty('SHEET_ID') || '',
  API_KEY: PropertiesService.getScriptProperties().getProperty('API_KEY') || 'smarthome-2024',
  POLL_INTERVAL: 2500, // milliseconds
  MAX_DEVICES: 50,
  VERSION: '1.0.0'
};

/**
 * Main entry point for web app
 */
function doGet(e) {
  const path = e.parameter.page || 'dashboard';
  const action = e.parameter.action || '';
  
  // API endpoints
  if (action) {
    return handleApiRequest(e);
  }
  
  // Serve web pages
  if (path === 'dashboard') {
    return HtmlService.createHtmlOutputFromFile('Dashboard')
      .setTitle('Smart Home Controller')
      .setXFrameOptionsMode(HtmlService.XFrameOptionsMode.ALLOWALL);
  }
  
  return ContentService.createTextOutput('Smart Home Controller API v' + CONFIG.VERSION);
}

/**
 * Handle POST requests (API calls from ESP32 and app)
 */
function doPost(e) {
  return handleApiRequest(e);
}

/**
 * Handle API requests
 */
function handleApiRequest(e) {
  try {
    const action = e.parameter.action || e.postData?.contents ? JSON.parse(e.postData.contents).action : '';
    const data = e.postData?.contents ? JSON.parse(e.postData.contents) : e.parameter;
    
    // Verify API key for sensitive operations
    if (!verifyApiKey(data.api_key)) {
      return jsonResponse({ success: false, error: 'Invalid API key' }, 401);
    }
    
    switch (action) {
      case 'poll':
        return handleDevicePoll(data);
      case 'register':
        return handleDeviceRegister(data);
      case 'update_state':
        return handleStateUpdate(data);
      case 'get_devices':
        return handleGetDevices(data);
      case 'update_device':
        return handleUpdateDevice(data);
      case 'get_schedules':
        return handleGetSchedules(data);
      case 'save_schedule':
        return handleSaveSchedule(data);
      case 'delete_schedule':
        return handleDeleteSchedule(data);
      case 'get_scenes':
        return handleGetScenes(data);
      case 'save_scene':
        return handleSaveScene(data);
      case 'delete_scene':
        return handleDeleteScene(data);
      case 'reset_wifi':
        return handleResetWifi(data);
      case 'login':
        return handleLogin(data);
      case 'signup':
        return handleSignup(data);
      default:
        return jsonResponse({ success: false, error: 'Unknown action' }, 400);
    }
  } catch (error) {
    Logger.log('Error: ' + error.toString());
    return jsonResponse({ success: false, error: error.toString() }, 500);
  }
}

/**
 * Verify API key
 */
function verifyApiKey(key) {
  return key === CONFIG.API_KEY || !CONFIG.API_KEY;
}

/**
 * Handle device poll (ESP32 checking for commands)
 */
function handleDevicePoll(data) {
  const uid = data.uid || data.mac;
  if (!uid) {
    return jsonResponse({ success: false, error: 'Device UID required' }, 400);
  }
  
  const sheet = getSheet('Devices');
  const device = findDevice(sheet, uid);
  
  if (!device) {
    // Auto-register new device
    registerDevice(sheet, {
      uid: uid,
      name: 'Device ' + uid.substr(-4),
      ip: data.ip || '',
      version: data.ver || '1.0',
      uptime: data.uptime || 0,
      rssi: data.rssi || 0,
      heap: data.heap || 0
    });
  } else {
    // Update device stats
    updateDeviceStats(sheet, device.row, {
      ip: data.ip,
      version: data.ver,
      uptime: data.uptime,
      rssi: data.rssi,
      heap: data.heap,
      last_seen: new Date()
    });
  }
  
  // Get device configuration and pending commands
  const config = getDeviceConfig(uid);
  const commands = getPendingCommands(uid);
  
  // Clear executed commands
  if (commands.length > 0) {
    clearCommands(uid);
  }
  
  return jsonResponse({
    success: true,
    config: config,
    commands: commands,
    ota_update: false,
    bin_url: ''
  });
}

/**
 * Handle device registration
 */
function handleDeviceRegister(data) {
  const sheet = getSheet('Devices');
  const device = {
    uid: data.uid,
    name: data.name || 'New Device',
    ip: data.ip || '',
    version: data.version || '1.0'
  };
  
  registerDevice(sheet, device);
  
  return jsonResponse({ success: true, message: 'Device registered' });
}

/**
 * Handle state update from device
 */
function handleStateUpdate(data) {
  const uid = data.uid;
  const sheet = getSheet('DeviceStates');
  
  // Update or create state record
  updateDeviceState(sheet, uid, data.states);
  
  return jsonResponse({ success: true });
}

/**
 * Handle get devices (for dashboard/app)
 */
function handleGetDevices(data) {
  const userId = data.user_id || 'default';
  const sheet = getSheet('Devices');
  const devices = getAllDevices(sheet, userId);
  
  return jsonResponse({ success: true, devices: devices });
}

/**
 * Handle update device (rename, type change, etc.)
 */
function handleUpdateDevice(data) {
  const sheet = getSheet('Devices');
  const device = findDevice(sheet, data.uid);
  
  if (!device) {
    return jsonResponse({ success: false, error: 'Device not found' }, 404);
  }
  
  // Update device properties
  updateDeviceProperties(sheet, device.row, {
    name: data.name,
    type: data.type,
    channel: data.channel
  });
  
  return jsonResponse({ success: true, message: 'Device updated' });
}

/**
 * Handle get schedules
 */
function handleGetSchedules(data) {
  const sheet = getSheet('Schedules');
  const schedules = getAllSchedules(sheet, data.user_id || 'default');
  
  return jsonResponse({ success: true, schedules: schedules });
}

/**
 * Handle save schedule
 */
function handleSaveSchedule(data) {
  const sheet = getSheet('Schedules');
  saveSchedule(sheet, data.schedule);
  
  return jsonResponse({ success: true, message: 'Schedule saved' });
}

/**
 * Handle delete schedule
 */
function handleDeleteSchedule(data) {
  const sheet = getSheet('Schedules');
  deleteSchedule(sheet, data.schedule_id);
  
  return jsonResponse({ success: true, message: 'Schedule deleted' });
}

/**
 * Handle get scenes
 */
function handleGetScenes(data) {
  const sheet = getSheet('Scenes');
  const scenes = getAllScenes(sheet, data.user_id || 'default');
  
  return jsonResponse({ success: true, scenes: scenes });
}

/**
 * Handle save scene
 */
function handleSaveScene(data) {
  const sheet = getSheet('Scenes');
  saveScene(sheet, data.scene);
  
  return jsonResponse({ success: true, message: 'Scene saved' });
}

/**
 * Handle delete scene
 */
function handleDeleteScene(data) {
  const sheet = getSheet('Scenes');
  deleteScene(sheet, data.scene_id);
  
  return jsonResponse({ success: true, message: 'Scene deleted' });
}

/**
 * Handle WiFi reset
 */
function handleResetWifi(data) {
  const sheet = getSheet('Devices');
  const device = findDevice(sheet, data.uid);
  
  if (!device) {
    return jsonResponse({ success: false, error: 'Device not found' }, 404);
  }
  
  // Queue WiFi reset command
  queueCommand(data.uid, { action: 'reset_wifi' });
  
  return jsonResponse({ success: true, message: 'WiFi reset queued' });
}

/**
 * Handle user login
 */
function handleLogin(data) {
  const sheet = getSheet('Users');
  const user = findUser(sheet, data.email);
  
  if (!user || user.password !== hashPassword(data.password)) {
    return jsonResponse({ success: false, error: 'Invalid credentials' }, 401);
  }
  
  const token = generateToken(user.id);
  
  return jsonResponse({
    success: true,
    user: {
      id: user.id,
      name: user.name,
      email: user.email,
      token: token
    }
  });
}

/**
 * Handle user signup
 */
function handleSignup(data) {
  const sheet = getSheet('Users');
  const existingUser = findUser(sheet, data.email);
  
  if (existingUser) {
    return jsonResponse({ success: false, error: 'Email already registered' }, 400);
  }
  
  const userId = Utilities.getUuid();
  const user = {
    id: userId,
    name: data.name,
    email: data.email,
    password: hashPassword(data.password),
    created: new Date()
  };
  
  addUser(sheet, user);
  const token = generateToken(userId);
  
  return jsonResponse({
    success: true,
    user: {
      id: userId,
      name: user.name,
      email: user.email,
      token: token
    }
  });
}

/**
 * Helper: Create JSON response
 */
function jsonResponse(data, status = 200) {
  const output = ContentService.createTextOutput(JSON.stringify(data));
  output.setMimeType(ContentService.MimeType.JSON);
  return output;
}

/**
 * Helper: Get or create sheet
 */
function getSheet(sheetName) {
  const ss = SpreadsheetApp.openById(CONFIG.SHEET_ID || SpreadsheetApp.getActiveSpreadsheet().getId());
  let sheet = ss.getSheetByName(sheetName);
  
  if (!sheet) {
    sheet = ss.insertSheet(sheetName);
    initializeSheet(sheet, sheetName);
  }
  
  return sheet;
}

/**
 * Initialize sheet with headers
 */
function initializeSheet(sheet, sheetName) {
  switch (sheetName) {
    case 'Devices':
      sheet.appendRow(['UID', 'Name', 'Type', 'IP', 'Version', 'Uptime', 'RSSI', 'Heap', 'Last Seen', 'User ID', 'Created']);
      break;
    case 'DeviceStates':
      sheet.appendRow(['UID', 'Channel', 'State', 'Value', 'Type', 'Timestamp']);
      break;
    case 'Schedules':
      sheet.appendRow(['ID', 'User ID', 'Device UID', 'Channel', 'Name', 'Start Time', 'End Time', 'Days', 'Action', 'Value', 'Enabled', 'Created']);
      break;
    case 'Scenes':
      sheet.appendRow(['ID', 'User ID', 'Name', 'Devices', 'Created']);
      break;
    case 'Users':
      sheet.appendRow(['ID', 'Name', 'Email', 'Password', 'Created']);
      break;
    case 'Commands':
      sheet.appendRow(['UID', 'Command', 'Timestamp', 'Executed']);
      break;
  }
}

/**
 * Helper: Hash password (simple hash for demo)
 */
function hashPassword(password) {
  return Utilities.base64Encode(Utilities.computeDigest(Utilities.DigestAlgorithm.SHA_256, password));
}

/**
 * Helper: Generate auth token
 */
function generateToken(userId) {
  const timestamp = new Date().getTime();
  const data = userId + ':' + timestamp;
  return Utilities.base64Encode(data);
}

// Additional helper functions would be implemented here
// (findDevice, registerDevice, updateDeviceStats, etc.)
// For brevity, showing the structure
