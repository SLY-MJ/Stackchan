#include <Arduino.h>
#include <M5Unified.h>
#include "face.h"
#include "servo.h"
#include "network.h"
#include "wifi_manager.h"

bool configMode = false;

void setup() {
    auto cfg = M5.config();
    // 强制后备板型，确保某些情况下 CoreS3 屏幕背光芯片没被自动抓到的问题
    cfg.fallback_board = m5::board_t::board_M5StackCoreS3;
    M5.begin(cfg);

    // CoreS3 需要等待一小会儿电源引脚激活（AW9523/AXP2101）
    delay(100);
    M5.Lcd.setBrightness(255); // 屏幕亮度拉满

    initServo();

    // 恢复配网逻辑，优先配网失败则启动热点，配网成功则启动 HTTP 和表情
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