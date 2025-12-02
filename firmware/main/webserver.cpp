#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "webserver.h"
#include "device_control.h"

static WebServer server(80);

static void handle_status() {
  String json = "{ \"devices\":[";
  for (int i = 0; i < 4; i++) {
    DeviceState st = device_control_get(i);
    json += "{";
    json += "\"id\":" + String(i) + ",";
    json += "\"power\":" + String(st.power ? "true" : "false") + ",";
    json += "\"speed\":" + String(st.speed);
    json += "}";
    if (i < 3) json += ",";
  }
  json += "] }";

  server.send(200, "application/json", json);
}

static void handle_control() {
  if (!server.hasArg("id") || !server.hasArg("power")) {
    server.send(400, "text/plain", "Missing params");
    return;
  }

  int id = server.arg("id").toInt();
  bool p = server.arg("power") == "1";
  int speed = server.hasArg("speed") ? server.arg("speed").toInt() : 0;

  device_control_set(id, p, speed);
  server.send(200, "application/json", "{\"ok\":true}");
}

void webserver_init() {
  server.on("/status", HTTP_GET, handle_status);
  server.on("/control", HTTP_GET, handle_control);
  server.begin();
}

void webserver_loop() {
  server.handleClient();
}
