#include "power_setup.h"
#include <Arduino.h>
#include <Wire.h>

// CoreS3 主板上的底侧电源管理芯片和扩展 IO 芯片的 I2C 地址
#define AXP2101_ADDR 0x34
#define AW9523_ADDR 0x58

void initCoreS3Power() {
    // 强制接管硬件层次 I2C 控制
    // CoreS3 的内部底层 I2C 总线在引脚 SDA:12, SCL:11
    Wire.begin(12, 11, 400000);

    // ==========================================
    // 1. AXP2101 电源芯片控制 - 开启所有必要的低压差线性稳压器(LDO)
    // ==========================================
    // 0x90 寄存器控制 LDOS 的开关（包括负责背光驱动的 DLDO1）
    // bit 7 设置为 1 代表开启 DLDO1
    Wire.beginTransmission(AXP2101_ADDR);
    Wire.write(0x90);
    Wire.write(0xBF); // 默认开启多数 LDO
    Wire.endTransmission();
    delay(50); // 给电源一点时间稳定

    // ==========================================
    // 2. AXP2101 电压调整 - 把背光升压驱动供电拉满
    // ==========================================
    // 0x99 寄存器控制 DLDO1 的具体输出电压
    // 范围是 0.5V 到 3.3V (最高 0x1C 即 28 steps)
    Wire.beginTransmission(AXP2101_ADDR);
    Wire.write(0x99);
    Wire.write(0x1C); // 最大 3.3V 给背光推满电压
    Wire.endTransmission();
    delay(50);

    // ==========================================
    // 3. AW9523 IO 扩展芯片控制 - 释放可能被下拉的控制引脚
    // ==========================================
    // 强制把 AW9523 的 P1_1 / P0_X 口强推高，防止它把 LCD_BL(背光控制引脚) 给关了
    Wire.beginTransmission(AW9523_ADDR);
    Wire.write(0x02); // 配置端口0
    Wire.write(0x07);
    Wire.endTransmission();

    Wire.beginTransmission(AW9523_ADDR);
    Wire.write(0x03); // 配置端口1
    Wire.write(0x83);
    Wire.endTransmission();

    // 等待所有电源逻辑最终平稳
    delay(100);
}

