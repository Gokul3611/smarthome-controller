/**
 * Database Helper Functions
 * Google Sheets as Database Layer
 */

/**
 * Find device by UID
 */
function findDevice(sheet, uid) {
  const data = sheet.getDataRange().getValues();
  
  for (let i = 1; i < data.length; i++) {
    if (data[i][0] === uid) {
      return {
        row: i + 1,
        uid: data[i][0],
        name: data[i][1],
        type: data[i][2],
        ip: data[i][3],
        version: data[i][4],
        uptime: data[i][5],
        rssi: data[i][6],
        heap: data[i][7],
        last_seen: data[i][8],
        user_id: data[i][9],
        created: data[i][10]
      };
    }
  }
  
  return null;
}

/**
 * Register new device
 */
function registerDevice(sheet, device) {
  sheet.appendRow([
    device.uid,
    device.name,
    device.type || 'light',
    device.ip,
    device.version,
    device.uptime || 0,
    device.rssi || 0,
    device.heap || 0,
    new Date(),
    device.user_id || 'default',
    new Date()
  ]);
}

/**
 * Update device statistics
 */
function updateDeviceStats(sheet, row, stats) {
  if (stats.ip) sheet.getRange(row, 4).setValue(stats.ip);
  if (stats.version) sheet.getRange(row, 5).setValue(stats.version);
  if (stats.uptime) sheet.getRange(row, 6).setValue(stats.uptime);
  if (stats.rssi) sheet.getRange(row, 7).setValue(stats.rssi);
  if (stats.heap) sheet.getRange(row, 8).setValue(stats.heap);
  sheet.getRange(row, 9).setValue(new Date());
}

/**
 * Update device properties
 */
function updateDeviceProperties(sheet, row, properties) {
  if (properties.name) sheet.getRange(row, 2).setValue(properties.name);
  if (properties.type) sheet.getRange(row, 3).setValue(properties.type);
}

/**
 * Get all devices for a user
 */
function getAllDevices(sheet, userId) {
  const data = sheet.getDataRange().getValues();
  const devices = [];
  
  for (let i = 1; i < data.length; i++) {
    if (data[i][9] === userId || userId === 'default' || !data[i][9]) {
      devices.push({
        uid: data[i][0],
        name: data[i][1],
        type: data[i][2],
        ip: data[i][3],
        version: data[i][4],
        uptime: data[i][5],
        rssi: data[i][6],
        heap: data[i][7],
        last_seen: data[i][8],
        online: isDeviceOnline(data[i][8])
      });
    }
  }
  
  return devices;
}

/**
 * Check if device is online (seen in last 10 seconds)
 */
function isDeviceOnline(lastSeen) {
  if (!lastSeen) return false;
  const now = new Date();
  const diff = now - new Date(lastSeen);
  return diff < 10000; // 10 seconds
}

/**
 * Get device configuration
 */
function getDeviceConfig(uid) {
  const sheet = getSheet('Devices');
  const device = findDevice(sheet, uid);
  
  if (!device) return {};
  
  return {
    name: device.name,
    type: device.type
  };
}

/**
 * Update device state
 */
function updateDeviceState(sheet, uid, states) {
  // Remove old states for this device
  const data = sheet.getDataRange().getValues();
  for (let i = data.length - 1; i > 0; i--) {
    if (data[i][0] === uid) {
      sheet.deleteRow(i + 1);
    }
  }
  
  // Add new states
  if (states && Array.isArray(states)) {
    states.forEach(state => {
      sheet.appendRow([
        uid,
        state.channel,
        state.state,
        state.value || 0,
        state.type || 'light',
        new Date()
      ]);
    });
  }
}

/**
 * Get pending commands for device
 */
function getPendingCommands(uid) {
  const sheet = getSheet('Commands');
  const data = sheet.getDataRange().getValues();
  const commands = [];
  
  for (let i = 1; i < data.length; i++) {
    if (data[i][0] === uid && !data[i][3]) {
      commands.push(JSON.parse(data[i][1]));
    }
  }
  
  return commands;
}

/**
 * Queue command for device
 */
function queueCommand(uid, command) {
  const sheet = getSheet('Commands');
  sheet.appendRow([
    uid,
    JSON.stringify(command),
    new Date(),
    false
  ]);
}

/**
 * Clear executed commands
 */
function clearCommands(uid) {
  const sheet = getSheet('Commands');
  const data = sheet.getDataRange().getValues();
  
  for (let i = data.length - 1; i > 0; i--) {
    if (data[i][0] === uid) {
      sheet.getRange(i + 1, 4).setValue(true);
    }
  }
}

/**
 * Get all schedules for user
 */
function getAllSchedules(sheet, userId) {
  const data = sheet.getDataRange().getValues();
  const schedules = [];
  
  for (let i = 1; i < data.length; i++) {
    if (data[i][1] === userId || userId === 'default') {
      schedules.push({
        id: data[i][0],
        device_uid: data[i][2],
        channel: data[i][3],
        name: data[i][4],
        start_time: data[i][5],
        end_time: data[i][6],
        days: data[i][7],
        action: data[i][8],
        value: data[i][9],
        enabled: data[i][10]
      });
    }
  }
  
  return schedules;
}

/**
 * Save schedule
 */
function saveSchedule(sheet, schedule) {
  const id = schedule.id || Utilities.getUuid();
  
  // Check if schedule exists
  const data = sheet.getDataRange().getValues();
  let row = -1;
  
  for (let i = 1; i < data.length; i++) {
    if (data[i][0] === id) {
      row = i + 1;
      break;
    }
  }
  
  if (row > 0) {
    // Update existing
    sheet.getRange(row, 1, 1, 11).setValues([[
      id,
      schedule.user_id,
      schedule.device_uid,
      schedule.channel,
      schedule.name,
      schedule.start_time,
      schedule.end_time,
      schedule.days,
      schedule.action,
      schedule.value,
      schedule.enabled
    ]]);
  } else {
    // Add new
    sheet.appendRow([
      id,
      schedule.user_id,
      schedule.device_uid,
      schedule.channel,
      schedule.name,
      schedule.start_time,
      schedule.end_time,
      schedule.days,
      schedule.action,
      schedule.value,
      schedule.enabled,
      new Date()
    ]);
  }
}

/**
 * Delete schedule
 */
function deleteSchedule(sheet, scheduleId) {
  const data = sheet.getDataRange().getValues();
  
  for (let i = data.length - 1; i > 0; i--) {
    if (data[i][0] === scheduleId) {
      sheet.deleteRow(i + 1);
      break;
    }
  }
}

/**
 * Get all scenes for user
 */
function getAllScenes(sheet, userId) {
  const data = sheet.getDataRange().getValues();
  const scenes = [];
  
  for (let i = 1; i < data.length; i++) {
    if (data[i][1] === userId || userId === 'default') {
      scenes.push({
        id: data[i][0],
        name: data[i][2],
        devices: JSON.parse(data[i][3] || '[]')
      });
    }
  }
  
  return scenes;
}

/**
 * Save scene
 */
function saveScene(sheet, scene) {
  const id = scene.id || Utilities.getUuid();
  
  // Check if scene exists
  const data = sheet.getDataRange().getValues();
  let row = -1;
  
  for (let i = 1; i < data.length; i++) {
    if (data[i][0] === id) {
      row = i + 1;
      break;
    }
  }
  
  if (row > 0) {
    // Update existing
    sheet.getRange(row, 1, 1, 4).setValues([[
      id,
      scene.user_id,
      scene.name,
      JSON.stringify(scene.devices)
    ]]);
  } else {
    // Add new
    sheet.appendRow([
      id,
      scene.user_id,
      scene.name,
      JSON.stringify(scene.devices),
      new Date()
    ]);
  }
}

/**
 * Delete scene
 */
function deleteScene(sheet, sceneId) {
  const data = sheet.getDataRange().getValues();
  
  for (let i = data.length - 1; i > 0; i--) {
    if (data[i][0] === sceneId) {
      sheet.deleteRow(i + 1);
      break;
    }
  }
}

/**
 * Find user by email
 */
function findUser(sheet, email) {
  const data = sheet.getDataRange().getValues();
  
  for (let i = 1; i < data.length; i++) {
    if (data[i][2] === email) {
      return {
        id: data[i][0],
        name: data[i][1],
        email: data[i][2],
        password: data[i][3],
        created: data[i][4]
      };
    }
  }
  
  return null;
}

/**
 * Add new user
 */
function addUser(sheet, user) {
  sheet.appendRow([
    user.id,
    user.name,
    user.email,
    user.password,
    user.created
  ]);
}
