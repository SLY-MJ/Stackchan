#include <Arduino.h>
#include <M5Unified.h>

// 引入刚刚新建的底层电源管理文件和各种原有的模块
#include "power_setup.h"
#include "face.h"
#include "servo.h"
#include "network.h"
#include "wifi_manager.h"

// 记录当前是否处于网页配网模式
bool configMode = false;

void setup() {
    // 1. 初始化 M5 基础架构
    auto cfg = M5.config();
    // 强制声明为 CoreS3，防止底层误判导致部分管脚失效
    cfg.fallback_board = m5::board_t::board_M5StackCoreS3;
    M5.begin(cfg);

    // 2. 强制初始化底层的电源，确保背光通电
    initCoreS3Power();

    // 3. 将背光亮度拉到最高 (0-255)
    M5.Lcd.setBrightness(255);

    // 4. 初始化舵机控制模块
    initServo();

    // 5. 检查 WiFi 配置并尝试连接
    //    这里的 checkWiFiConfig/connectWiFi 会去读 Preferences 里你网页输入过的密码
    if (!checkWiFiConfig() || !connectWiFi()) {
        // 无可用 WiFi，启动 AP 热点配网界面 (192.168.4.1)
        startWiFiConfigPortal();
        configMode = true;
    } else {
        // WiFi 连接成功，启动标准固件流程
        initNetwork(); // 开启 HTTP 服务监听 (/face /servo)
        initFace();    // 在屏幕上画出默认表情
    }
}

void loop() {
    // M5 官方规定的底层刷新 (用来读取触摸、按键、扬声器状态等)
    M5.update();

    if (configMode) {
        // 如果在配网模式，就在死循环里只处理网页配网请求
        updateWiFiConfigPortal();
    } else {
        // 正常运行模式下的工作管线
        updateNetwork();  // 监听并执行发来的 HTTP 请求
        updateFace();     // 更新屏幕动画（比如让小球眨眼）
        updateServo();    // 更新舵机动作（旋转过程中的平滑插值）
    }
}