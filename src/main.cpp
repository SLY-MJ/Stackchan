#include <Arduino.h>
#include <M5Unified.h>
#include "face.h"
#include "servo.h"
#include "network.h"
#include "wifi_manager.h"

bool configMode = false;

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);

    initServo();

    if (!checkWiFiConfig() || !connectWiFi()) {
        startWiFiConfigPortal();
        configMode = true;
    } else {
        initNetwork();
        initFace();
    }
}

void loop() {
    M5.update();

    if (configMode) {
        updateWiFiConfigPortal();
    } else {
        updateNetwork();
        updateFace();
        updateServo();
    }
}