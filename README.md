# StackChan - M5Stack CoreS3

这是一个基于 M5Stack CoreS3 和 Arduino 框架开发的 StackChan 桌面机器人项目。使用了官方的 `M5Unified` 库进行屏幕和底层硬件驱动，代码结构完全模块化。

## ⚙️ 硬件参数

*   **主控**: M5Stack CoreS3 (ESP32-S3)
*   **屏幕驱动**: M5Unified (`M5.Lcd`)
*   **舵机引脚**: (目前配置于 `src/config.h`)
    *   PAN (摇头): GPIO 8
    *   TILT (点头): GPIO 9
*   **WiFi 配网**: 智能热点模式 (默认无网络时放出 AP)
    *   热点名称: `StackChan_Config`
    *   配网地址: `192.168.4.1`
*   **表情颜色**:
    *   背景色 (COLOR_BG): 白色
    *   前景色 (COLOR_FG): 黑色

## 📂 文件目录说明

项目位于 `src/` 目录下，按照不同功能模块进行了清晰的拆分：

*   **`main.cpp`**
    主入口程序。包含系统的 `setup()` 初始化主轴与 `loop()` 循环核心管线。负责按顺序调用电源、显示、舵机和网络模块。
*   **`config.h`**
    全局配置文件。集中管理引脚定义（舵机针脚）、颜色配置参数以及热点名称设定。
*   **`power_setup.h` & `power_setup.cpp`**
    底层电源模块方案。专用于解决 CoreS3 屏幕背光不亮的兼容性痛点。通过底层 I2C 接管 AXP2101 及 AW9523 芯片，强制升压推平 LCD 背光供电。
*   **`face.h` & `face.cpp`**
    表情绘制与动画逻辑模块。纯基于 `M5.Lcd` 实现，不依赖外部图片。支持普通、开心 (挖空画法)、眨眼 (状态机时间驱动)、向左看和向右看等状态。
*   **`servo.h` & `servo.cpp`**
    舵机驱动引擎。基于 `ESP32Servo`。将动作解耦为有限状态机，负责点头 (`nod`)、摇头 (`shake`) 及复位 (`reset`) 等非阻塞式平滑动作控制。
*   **`wifi_manager.h` & `wifi_manager.cpp`**
    网络配置向导。使用 `Preferences` 在闪存中持久化存取 SSID 和密码。连接失败自动 fallback 进 Web 网页配置模式进行可视化的 WiFi 录入。
*   **`network.h` & `network.cpp`**
    HTTP 服务器与 API 接口器。入网成功后开始接管 80 端口，对外提供控制机器人的 REST API。

## 🔌 API 控制接口

机器人连上你家里的 WiFi 后，你可以直接在浏览器或者程序里调用以下 GET 接口控制它：

*   **表情控制 `GET /face?mode={模式}`**
    *   `normal` - 默认表情
    *   `happy` - 开心笑脸
    *   `blink` - 自动眨��模式
    *   `left` - 向左看
    *   `right` - 向右看
*   **舵机控制 `GET /servo?action={动作}`**
    *   `reset` - 回归正脸中心位
    *   `nod` - 点头
    *   `shake` - 摇头

## 🚀 编译与烧录

本仓库基于 **PlatformIO** 构建：
1. 使用 CLion、VSCode 或终端打开本工程。
2. 确保已连接 M5Stack CoreS3 到电脑 USB。
3. 点击 PlatformIO 的 `Upload` 进行编译及烧录。

