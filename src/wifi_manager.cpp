#include "wifi_manager.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <M5Unified.h>
#include "config.h"

Preferences prefs;
WebServer configServer(80);
bool inConfigMode = false;

bool checkWiFiConfig() {
    prefs.begin("wifi", true);
    String ssid = prefs.getString("ssid", "");
    String pass = prefs.getString("pass", "");
    prefs.end();
    return ssid.length() > 0;
}

bool connectWiFi() {
    prefs.begin("wifi", true);
    String ssid = prefs.getString("ssid", "");
    String pass = prefs.getString("pass", "");
    prefs.end();

    if (ssid.length() == 0) return false;

    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("Connecting to:\n%s", ssid.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());

    int timeout = 20; // 10 seconds
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
        delay(500);
        M5.Lcd.print(".");
        timeout--;
    }

    if (WiFi.status() == WL_CONNECTED) {
        M5.Lcd.println("\nConnected!");
        M5.Lcd.println(WiFi.localIP().toString());
        delay(2000);
        return true;
    }

    M5.Lcd.println("\nFailed!");
    delay(2000);
    return false;
}

void handleRoot() {
    String html = "<html><body><h2>WiFi Setup</h2>"
                  "<form action='/save' method='POST'>"
                  "SSID: <input type='text' name='ssid'><br>"
                  "PASS: <input type='password' name='pass'><br>"
                  "<input type='submit' value='Save'>"
                  "</form></body></html>";
    configServer.send(200, "text/html", html);
}

void handleSave() {
    String ssid = configServer.arg("ssid");
    String pass = configServer.arg("pass");

    prefs.begin("wifi", false);
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.end();

    configServer.send(200, "text/html", "Saved! Rebooting...");
    delay(1000);
    ESP.restart();
}

void startWiFiConfigPortal() {
    inConfigMode = true;
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID);

    M5.Lcd.fillScreen(TFT_BLUE);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("AP Mode Active");
    M5.Lcd.println("Connect to:");
    M5.Lcd.println(AP_SSID);
    M5.Lcd.println("Go to: 192.168.4.1");

    configServer.on("/", handleRoot);
    configServer.on("/save", HTTP_POST, handleSave);
    configServer.begin();
}

void updateWiFiConfigPortal() {
    if (inConfigMode) {
        configServer.handleClient();
    }
}

