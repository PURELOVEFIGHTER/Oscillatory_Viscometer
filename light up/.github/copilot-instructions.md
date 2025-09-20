# Copilot Instructions for Oscillating Viscometer Firmware

## 项目架构与主要组件
- 本项目为基于 STM32F103 微控制器的振荡粘度计固件，采用 STM32 HAL 库开发。
- 主要代码位于 `Core/Inc/`（头文件）和 `Core/Src/`（源文件）目录。
- 关键驱动模块：
  - `__ldc1101_driver.*`：LDC1101 电感传感器驱动
  - `drv8833_driver.*`：DRV8833 电机驱动芯片驱动
  - `oled.*`、`oledfont.*`：OLED 显示屏驱动及字库
- 主控制逻辑在 `main.c`，相关全局变量和接口在 `main.h` 声明。

## 重要开发流程
- **编译/烧录**：使用 Keil MDK-ARM 工程文件（`light up.uvprojx`），通常通过 Keil IDE 构建和下载固件。
- **CubeMX 配置**：硬件外设初始化由 CubeMX 生成，配置文件为 `light up.ioc`。
- **调试**：常用 J-Link 仿真器，调试配置见 `DebugConfig/`。

## 代码风格与约定
- 头文件统一放在 `Core/Inc/`，源文件在 `Core/Src/`。
- 所有外部接口、全局变量需在头文件中 `extern` 声明。
- 用户自定义代码区域用 `/* USER CODE BEGIN ... */` 和 `/* USER CODE END ... */` 包裹，CubeMX 生成代码会保留这些区域。
- GPIO、外设定义采用 `#define`，并在 `main.h` 统一管理。
- 频率/占空比扫描、蓝牙串口等功能有专用函数（如 `FREQ_Scan`、`DR_Scan`、`Command_Parse`）。

## 依赖与集成
- 依赖 STM32 HAL 驱动（`Drivers/STM32F1xx_HAL_Driver/`）和 CMSIS（`Drivers/CMSIS/`）。
- 外设初始化和中断处理分布在 `gpio.*`、`tim.*`、`usart.*`、`stm32f1xx_it.*` 等文件。
- 主要硬件接口：SPI（LDC1101）、PWM（DRV8833）、UART（蓝牙通信）、GPIO（LED/控制信号）。

## 典型开发模式示例
- 新增外设：CubeMX 配置后，补充相关驱动文件并在 `main.h` 注册。
- 扩展功能：在 `main.c` 添加主循环逻辑，相关变量/函数在 `main.h` 声明。
- 调试输出：可通过 UART 蓝牙串口（`Command_Parse`）进行命令交互和调试。

## 关键文件参考
- `Core/Inc/main.h`：全局定义、外设宏、全局变量、功能接口声明
- `Core/Src/main.c`：主程序入口、主循环、系统初始化
- `Core/Inc/__ldc1101_driver.h` / `Core/Src/__ldc1101_driver.c`：LDC1101 传感器驱动实现
- `Core/Inc/drv8833_driver.h` / `Core/Src/drv8833_driver.c`：DRV8833 电机驱动实现
- `Core/Inc/oled.h` / `Core/Src/oled.c`：OLED 显示驱动

---

如需自动化生成代码、添加新外设或扩展功能，请遵循上述结构和约定。遇到不明确的硬件接口或初始化流程，优先查阅 `main.h` 和 CubeMX 配置文件。