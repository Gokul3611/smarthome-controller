#include "wifi_manager.h"

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

static WebServer server(80);
static DNSServer dnsServer;
static Preferences prefs;

const char* AP_SSID = "SmartSetup";
const byte DNS_PORT = 53;
const char* PREF_NAMESPACE = "smarthome";
const char* PREF_SSID = "ssid";
const char* PREF_PWD  = "pwd";

static bool runningAP = false;

static void handleRoot() {
  String page = "<!doctype html><html><body><h3>Provision Device</h3>"
                "<form method='POST' action='/save'>"
                "SSID:<input name='ssid'><br>PWD:<input name='pwd' type='password'><br>"
                "<input type='submit' value='Save'>"
                "</form></body></html>";
  server.send(200, "text/html", page);
}

static void handleSave() {
  if (server.hasArg("ssid") && server.hasArg("pwd")) {
    String ssid = server.arg("ssid");
    String pwd  = server.arg("pwd");
    prefs.begin(PREF_NAMESPACE, false);
    prefs.putString(PREF_SSID, ssid);
    prefs.putString(PREF_PWD, pwd);
    prefs.end();
    server.send(200, "text/html", "<html><body>Saved. Rebooting...</body></html>");
    delay(800);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Missing args");
  }
}

void startAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID);
  IPAddress apIP = WiFi.softAPIP(); // usually 192.168.4.1
  dnsServer.start(DNS_PORT, "*", apIP);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.begin();
  runningAP = true;
}

void stopAP() {
  server.stop();
  dnsServer.stop();
  WiFi.softAPdisconnect(true);
  runningAP = false;
}

void trySTAConnect(const String& ssid, const String& pwd) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pwd.c_str());
  unsigned long start = millis();
  const unsigned long timeout = 15000; // 15s
  while (millis() - start < timeout) {
    if (WiFi.status() == WL_CONNECTED) break;
    delay(200);
  }
}

void wifi_manager_init() {
  prefs.begin(PREF_NAMESPACE, true); // read-only
  String saved_ssid = prefs.getString(PREF_SSID, "");
  String saved_pwd  = prefs.getString(PREF_PWD, "");
  prefs.end();

  if (saved_ssid.length() > 0) {
    trySTAConnect(saved_ssid, saved_pwd);
    if (WiFi.status() == WL_CONNECTED) {
      // Connected to home WiFi; nothing to do here now.
      return;
    }
  }
  // Fallback to AP mode / captive portal
  startAP();
}

void wifi_manager_loop() {
  if (runningAP) {
    dnsServer.processNextRequest();
    server.handleClient();
  } else {
    // Monitor STA connection; restart AP if lost
    if (WiFi.status() != WL_CONNECTED) {
      startAP();
    }
  }
}

void wifi_manager_clear_credentials() {
  prefs.begin(PREF_NAMESPACE, false);
  prefs.remove(PREF_SSID);
  prefs.remove(PREF_PWD);
  prefs.end();
}
