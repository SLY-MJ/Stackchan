#pragma once

// 初始化 M5Stack CoreS3 的底层电源管理芯片与 IO 扩展芯片
// 解决初始化时背光不亮、电压没推过去的问题
void initCoreS3Power();

