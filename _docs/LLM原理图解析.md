# STM32MP257F-DK (`MB1605-MP257F-C01`) 硬件外设与引脚映射全集

> **数据源**：[`mb1605-mp257f-c01-schematic.pdf`](file:///c:/tzy/projects/stm32mp257f-dk-m33/00-example/_docs/mb1605-mp257f-c01-schematic.pdf)（Sheet 1–18 完整网表，Revision `C-01`）与 [`UM3385`](file:///c:/tzy/projects/stm32mp257f-dk-m33/00-example/_docs/UserManual%20um3385-discovery-kit-with-stm32mp257f-mpu-stmicroelectronics.pdf)  
> **主控芯片 (`U1`)**：`STM32MP257FAK3` (`VFBGA424`, $14\times 14\text{ mm}$, Pitch $0.5\text{ mm}$)

---

## 1. 板载 LED 与按键 / 拨码开关映射（Sheet 3, 4, 8, 14, 17, 18）

### 1.1 用户可编程 LED（全部位于 `GPIOH` / `VDDIO3` $3.3\text{ V}$ 电源域）

> **关键电源域约束**：`PH2..PH13` 由独立电源域 **`VDDIO3`**（连接板载 `VDDIO` $3.3\text{ V}$）供电。在 M33 裸机/HAL 驱动 `PH4..PH7` 前，必须通过 `PWR` 开启 `VDDIO3` 电压监控并置位 `PWR_CR1_VDDIO3SV`（如 `HAL_PWREx_EnableSupply(PWR_PVM_VDDIO3)`），且通过 `RIFSC/RCC` 使能 `GPIOH` 时钟，否则引脚无法输出。

| 板载丝印 (位号)  | 颜色          | BSP 宏名 (`Led_TypeDef`) | 原理图网络名               | MPU 引脚 (BGA Ball)  | 有效电平 (点亮条件)                        | 硬件驱动拓扑与限流电阻                                                                                                                                                           | 默认用途 / 复用冲突说明                                                                   |
|:---------- |:----------- |:---------------------- |:-------------------- |:------------------ |:---------------------------------- |:--------------------------------------------------------------------------------------------------------------------------------------------------------------------- |:------------------------------------------------------------------------------- |
| **`LD7`**  | 红色 (Red)    | `LED1` / `LED_RED`     | `LED1` / `BOOTFAILN` | **`PH4`** (`AB16`) | **低电平有效 (`0` / `GPIO_PIN_RESET`)** | `3V3` $\rightarrow$ `R136` ($360\ \Omega$) $\rightarrow$ `LD7(A)`；`LD7(K)` 直连 **`PH4`** (灌电流驱动)                                                                       | **复用冲突**：与 ROM **`BOOTFAILN`** 共用；启动失败时 BootROM 开漏拉低点亮此灯                        |
| **`LD8`**  | 绿色 (Green)  | `LED2` / `LED_GREEN`   | `LED2`               | **`PH5`** (`AG18`) | **高电平有效 (`1` / `GPIO_PIN_SET`)**   | **`PH5`** 接 N-MOS `Q3` (`Si1062X`) 栅极 + `R137` ($100\text{ k}\Omega$ 下拉至 `GND`)；`3V3` $\rightarrow$ `R138` ($270\ \Omega$) $\rightarrow$ `LD8` $\rightarrow$ `Q3(D)`  | 用户通用指示灯                                                                         |
| **`LD9`**  | 橙色 (Orange) | `LED3` / `LED_ORANGE`  | `LED3`               | **`PH6`** (`AF19`) | **高电平有效 (`1` / `GPIO_PIN_SET`)**   | **`PH6`** 接 N-MOS `Q2` (`Si1062X`) 栅极 + `R141` ($100\text{ k}\Omega$ 下拉至 `GND`)；`3V3` $\rightarrow$ `R142` ($300\ \Omega$) $\rightarrow$ `LD9` $\rightarrow$ `Q2(D)`  | STM32Cube M33 示例默认状态指示灯                                                         |
| **`LED1`** | 蓝色 (Blue)   | `LED4` / `LED_BLUE`    | `LED4`               | **`PH7`** (`AE16`) | **高电平有效 (`1` / `GPIO_PIN_SET`)**   | **`PH7`** 接 N-MOS `Q1` (`Si1062X`) 栅极 + `R143` ($100\text{ k}\Omega$ 下拉至 `GND`)；`3V3` $\rightarrow$ `R144` ($120\ \Omega$) $\rightarrow$ `LED1` $\rightarrow$ `Q1(D)` | **注意丝印陷阱**：元件位号为 **`LED1`**，但对应网络为 **`LED4`**；Linux (CA35) 默认用作系统 Heartbeat 呼吸灯 |

### 1.2 板载非 MPU 直控状态 LED（硬件 / 子芯片直驱）

| 板载丝印       | 颜色       | 所在图纸                     | 驱动源与连接关系                                                                                         | 功能说明                                            |
|:---------- |:-------- |:------------------------ |:------------------------------------------------------------------------------------------------ |:----------------------------------------------- |
| **`LD11`** | 绿色       | Sheet 4 (`Power_PMIC`)   | `5V_STLK` $\rightarrow$ `LD11` $\rightarrow$ `R210` ($270\ \Omega$) $\rightarrow$ `GND`          | 板载 $5\text{ V}$ 主电源上电指示灯                        |
| **`LD5`**  | 双色 (红/绿) | Sheet 18 (`STLINK_V3EC`) | ST-LINK 主控 `U36` (`STM32F723IEK6`) `PA10` (`LED_STLK`)                                           | ST-LINK 通信状态指示灯 (`COM`)                         |
| **`LD6`**  | 双色 (红/绿) | Sheet 18 (`STLINK_V3EC`) | ST-LINK 主控 `U36` (`STM32F723IEK6`) `PC6` (`PWR_LED`)                                             | USB Type-C 供电协商状态指示灯                            |
| **`LD2`**  | 绿色       | Sheet 8 (`ETHERNET`)     | 以太网 PHY `U38` (`RTL8211F-CG`) Pin 34 (`LED2_CFG_LDO1`) $\rightarrow$ `R151` ($270\ \Omega$)      | 千兆网口活动指示灯（另 `CN16` RJ45 自带绿灯 `LED0` 与黄灯 `LED1`） |
| **`LD4`**  | 红色       | Sheet 14 (`USB_HUB`)     | USB Hub `U34` (`USB2422`) Pin 17 (`SUSP_IND/LOCAL_PWR`) $\rightarrow$ `R96` ($1\text{ k}\Omega$) | USB 2.0 Hub 挂起/状态指示灯                            |

---

### 1.3 板载按键与拨码开关（Sheet 3, 4, 17）

| 板载丝印 (按键颜色/名称)        | BSP 宏名 (`Button_TypeDef`) | 原理图网络名                                 | MPU 引脚 (BGA Ball)               | 按下有效电平                                   | 松开默认电平 (外部上下拉)                                     | 硬件消抖与电路细节                                                                                                                                                    | 特殊事项 / 启动复用                                                                      |
|:--------------------- |:------------------------- |:-------------------------------------- |:------------------------------- |:---------------------------------------- |:-------------------------------------------------- |:------------------------------------------------------------------------------------------------------------------------------------------------------------ |:-------------------------------------------------------------------------------- |
| **`B5` (`USER1`)**    | `BUTTON_USER1`            | `BUTTON.USER1`                         | **`PC5`** (`AD10`)              | **高电平有效 (`1` / `VDDIO` $3.3\text{ V}$)** | 低电平 (`0`，外部 `R135` $100\text{ k}\Omega$ 下拉至 `GND`) | 按键接 `VDDIO`；经 `R127` ($330\ \Omega$) 进 **`PC5`**，对地并有 `C162` ($100\text{ nF}$) 硬件消抖电容 + `U27` ESD + `TP5`                                                    | EXTI 线：`EXTI2_LINE_5`；U-Boot 启动阶段按住进入 **USB DFU 烧录模式**                           |
| **`B4` (`USER2`)**    | `BUTTON_USER2`            | `BUTTON.USER2`                         | **`PC11`** (`AD8`)              | **高电平有效 (`1` / `VDDIO` $3.3\text{ V}$)** | 低电平 (`0`，外部 `R128` $100\text{ k}\Omega$ 下拉至 `GND`) | 按键接 `VDDIO`；经 `R126` ($330\ \Omega$) 进 **`PC11`**，对地并有 `C161` ($100\text{ nF}$) 硬件消抖电容 + `U25` ESD + `TP3`                                                   | EXTI 线：`EXTI2_LINE_11`；U-Boot 启动阶段按住进入 **Android Fastboot 模式**                   |
| **`B3` (`TAMPER`)**   | `BUTTON_TAMPER`           | `BUTTON.TAMP`                          | **`PZ2`** (`W5`)                | **低电平有效 (`0` / `GND`)**                  | **外部上拉 `R139` 未焊 (`DNF`)！需开启内部上拉 (`GPIO_PULLUP`)** | 按键经 `R140` ($100\ \Omega$) 接 `GND`；对地并有 `C163` ($1\text{ nF}$) + `U28` ESD + `TP6`。原理图上 `R139` ($1\text{ k}\Omega$ 至 `VBAT`) 标注 **`DNF`**                    | EXTI 线：`EXTI2_LINE_2` / `RTC_TAMP2`。**若作普通 GPIO 输入，必须配置 `GPIO_PULLUP`，否则松开时悬空！** |
| **`B2` (`WAKE UP`)**  | `BUTTON_WAKEUP`           | `PONKEYn` $\rightarrow$ `MP257_WAKEUP` | **`PA0`** (`AF5`) *(经 PMIC 转发)* | **低电平有效 (`0` / 开漏中断)**                   | 高电平 (`1`，PMIC 内部上拉 / 需配内部上拉)                       | **`B2` 并非直连 MPU**，而是接在 PMIC `U26` (`STPMIC25`) Pin 1 (`PONKEYn`) 与 `GND` 之间（并 `C219` $10\text{ nF}$）；PMIC 的 Pin 32 (`INTn`) 接 MPU **`PA0`** (`MP257_WAKEUP`) | EXTI 线：`EXTI2_LINE_0` / `PWR_WKUP1`；长按 `B2` 会触发 PMIC 硬件关机/掉电序列                   |
| **`B1` (`RESET` 黑键)** | —                         | `NRST`                                 | **`NRST`** (`AE1`)              | **低电平有效 (`0` / `GND`)**                  | 高电平 (`1`，外部 `R33` $10\text{ k}\Omega$ 上拉至 `VDDIO`) | 对地并 `C33` ($10\text{ nF}$) + `U15` ESD；通过 `R81` ($0\ \Omega$) 同步连至 PMIC `RSTn` (`U26-31`)，通过 `R212` ($0\ \Omega$) 连至 ST-LINK `T_NRST`                        | 全局硬件复位                                                                           |

#### 启动模式拨码开关 `SW1` (`BOOT0..1`) & `SW2` (`BOOT2..3`)（Sheet 3, 17）

* **引脚映射**：
  * `SW1-1` $\rightarrow$ **`BOOT0`** (Ball `Y3`，`R44` $1\text{ k}\Omega$ 串联，`TP15`)
  * `SW1-2` $\rightarrow$ **`BOOT1`** (Ball `AA3`，`R122` $1\text{ k}\Omega$ 串联，`TP16`)
  * `SW2-1` $\rightarrow$ **`BOOT2`** (Ball `AB1`，`R124` $1\text{ k}\Omega$ 串联，`TP17`)
  * `SW2-2` $\rightarrow$ **`BOOT3`** (Ball `AB2`，`R123` $1\text{ k}\Omega$ 串联，`TP18`)
* **常用拨码组合 (`BOOT3:BOOT2:BOOT1:BOOT0`)**：
  * `0001`：从 **microSD 卡 (`SDMMC1`)** 启动（Cortex-A35 主导 TD 架构）
  * `0010`：从 **板载 eMMC (`SDMMC2`)** 启动（Cortex-A35 主导）
  * `0011` 或 `1100`：**Development Boot 开发调试模式**（不跑 Flash，等待 ST-LINK 挂载调试 A35/M33）
  * `0000` 或 `1111`：**UART / USB 强制串行下载模式**（USB2 OTG `CN15` + `USART2`）
  * `0111` / `1000`：分别从 microSD / eMMC 启动（Cortex-M33 主导架构）

---

## 2. ST-LINK 虚拟串口与板载扩展排针 (`CN5`) 总线映射

### 2.1 STLINK-V3EC (`U36`, 接口 `CN21`) 双路虚拟串口 (Dual VCP)

原理图 Sheet 2、Sheet 3 与 Sheet 18 将主控 **`STM32MP257FAK3` (`U1`) 的两路独立 UART** 连接到板载 **`STLINK-V3EC` (`U36` = `STM32F723IEK6`)**，因此在 PC 端会枚举出**两个虚拟串口 (VCP)**：

| 通道定位                                                         | MPU 外设编号     | 信号方向 (以 MPU 为基准)                            | MPU 引脚 (BGA Ball)  | 复用功能 (`Alternate`) | 原理图网络名                                  | 串联/上拉电阻与测试点                                                             | 对端 ST-LINK (`U36`) 引脚          |
|:------------------------------------------------------------ |:------------ |:------------------------------------------- |:------------------ |:------------------ |:--------------------------------------- |:----------------------------------------------------------------------- |:------------------------------ |
| **VCP1：CA35 主控制台 & BootROM 串口** (`BOOT_VCPA35` / `COM_CA35`) | **`USART2`** | **`USART2_TX`** (MPU $\rightarrow$ ST-LINK) | **`PA4`** (`AG17`) | `GPIO_AF6_USART2`  | `USART2_TX` $\rightarrow$ `STLK_VCP_RX` | `R154` ($0\ \Omega$)                                                    | `U36-C10` (`VCP_RX` / `PG9`)   |
| 〃                                                            | **`USART2`** | **`USART2_RX`** (ST-LINK $\rightarrow$ MPU) | **`PA8`** (`AA17`) | `GPIO_AF8_USART2`  | `STLK_VCP_TX` $\rightarrow$ `USART2_RX` | `R134` ($33\ \Omega$) 串联 + **`R107` ($10\text{ k}\Omega$ 上拉至 `VDDIO`)** | `U36-A7` (`VCP_TX` / `PG14`)   |
| **VCP2：CM33 专属调试串口** (`VCPM33` / `COM_CM33`)                 | **`UART5`**  | **`UART5_TX`** (MPU $\rightarrow$ ST-LINK)  | **`PG9`** (`M3`)   | `GPIO_AF5_UART5`   | `STLK_VCP2_RX`                          | `R211` ($0\ \Omega$)                                                    | `U36-R13` (`UART_RX` / `PB11`) |
| 〃                                                            | **`UART5`**  | **`UART5_RX`** (ST-LINK $\rightarrow$ MPU)  | **`PG10`** (`M4`)  | `GPIO_AF5_UART5`   | `STLK_VCP2_TX`                          | `R155` ($0\ \Omega$) 串联 + **`R125` ($10\text{ k}\Omega$ 上拉至 `VDDIO`)**  | `U36-B14` (`UART_TX` / `PC10`) |

> **附：ST-LINK SWD/JTAG 调试引脚（Sheet 3 & Sheet 18）**  
> `JTMS_SWDIO` = `AE2` (`R209` $33\ \Omega$, `TP40`) | `JTCK_SWCLK` = `AA5` (`R153` $33\ \Omega$, `TP43`) | `JTDO_SWO` = `AC2` (`R129` $0\ \Omega$, `TP13`) | `JTDI` = `AE3` (`R207` $33\ \Omega$, `TP41`) | `NJTRST` = `AF3` (`TP42`)。  
> **跳线 `JP3`**：短接 `JP3` 会将 `STLK_NRST` 拉低至 `GND`，使板载 ST-LINK 进入复位高阻态。

---

### 2.2 板载 40-Pin 扩展排针 `CN5` 总线分类速查（Sheet 16 `Expansion_Connector.SchDoc`）

> **说明**：`STM32MP257F-DK` (`MB1605`) **未设 Arduino 排母**，所有用户外扩 GPIO / 总线均通过 **树莓派兼容 40-Pin ($20\times 2$, $2.54\text{ mm}$) 排针 `CN5`** 引出（共 28 个 MPU GPIO，全部工作在 $3.3\text{ V}$ `VDDIO` 电平）。

#### (1) `CN5` 引出的 UART / LPUART

| 外设编号                   | 信号名称         | MPU 引脚 (BGA Ball) | 复用功能 (AF)          | `CN5` 物理针脚号      | 原理图网络名       | 备注                                                      |
|:---------------------- |:------------ |:----------------- |:------------------ |:---------------- |:------------ |:------------------------------------------------------- |
| **`USART6`**           | `USART6_TX`  | **`PF13`** (`P2`) | `GPIO_AF6_USART6`  | **`CN5` Pin 8**  | `EXP_GPIO14` | 4 线全功能串口，独占引出至 `CN5`，无板载芯片冲突                            |
| **`USART6`**           | `USART6_RX`  | **`PF14`** (`P3`) | `GPIO_AF6_USART6`  | **`CN5` Pin 10** | `EXP_GPIO15` | 〃                                                       |
| **`USART6`**           | `USART6_RTS` | **`PG5`** (`R3`)  | `GPIO_AF6_USART6`  | **`CN5` Pin 11** | `EXP_GPIO17` | 硬件流控 RTS                                                |
| **`USART6`**           | `USART6_CTS` | **`PF15`** (`R2`) | `GPIO_AF6_USART6`  | **`CN5` Pin 36** | `EXP_GPIO16` | 硬件流控 CTS                                                |
| **`LPUART1`** *(可选复用)* | `LPUART1_TX` | **`PZ7`** (`V2`)  | `GPIO_AF6_LPUART1` | **`CN5` Pin 22** | `EXP_GPIO25` | 原理图标注为 `GPIO25`；BSP (`COM_CM0PLUS`) 将其配置为 **`LPUART1`** |
| **`LPUART1`** *(可选复用)* | `LPUART1_RX` | **`PZ8`** (`V1`)  | `GPIO_AF6_LPUART1` | **`CN5` Pin 37** | `EXP_GPIO26` | 原理图标注为 `GPIO26`；可与 `PZ7` 配对组成第二路扩展串口                    |

#### (2) `CN5` 引出的 I2C / I3C

| 外设编号                | 信号名称                    | MPU 引脚 (BGA Ball)  | 复用功能 (AF)                     | `CN5` 物理针脚号      | 原理图网络名               | 板载上拉电阻                                         | 冲突与共享警告                                                                                        |
|:------------------- |:----------------------- |:------------------ |:----------------------------- |:---------------- |:-------------------- |:---------------------------------------------- |:---------------------------------------------------------------------------------------------- |
| **`I2C8` / `I3C4`** | `I2C8_SDA` (`I3C4_SDA`) | **`PZ3`** (`Y6`)   | `GPIO_AF9_I2C8` / `AF11_I3C4` | **`CN5` Pin 3**  | `EXP_GPIO2`          | **已焊 `R246` ($1.5\text{ k}\Omega$ 至 `VDDIO`)** | **`CN5` 专属主推用户 I2C/I3C 总线**，未挂板载外设芯片                                                           |
| **`I2C8` / `I3C4`** | `I2C8_SCL` (`I3C4_SCL`) | **`PZ4`** (`V3`)   | `GPIO_AF9_I2C8` / `AF11_I3C4` | **`CN5` Pin 5**  | `EXP_GPIO3`          | **已焊 `R245` ($1.5\text{ k}\Omega$ 至 `VDDIO`)** | 〃                                                                                              |
| **`I2C2`**          | `I2C2_SDA`              | **`PF0`** (`AA15`) | `GPIO_AF9_I2C2`               | **`CN5` Pin 27** | `ID_SD` (`I2C2.SDA`) | **已焊 `R31` ($1.5\text{ k}\Omega$ 至 `VDDIO`)**  | **严重共享冲突**：同时连接板载 **HDMI 桥 `ADV7535` (`U9`)**、**LVDS 触摸屏 (`CN2`)**、**CSI 摄像头 (`CN8`)**！详见第 3 节 |
| **`I2C2`**          | `I2C2_SCL`              | **`PF2`** (`AC15`) | `GPIO_AF9_I2C2`               | **`CN5` Pin 28** | `ID_SC` (`I2C2.SCL`) | **已焊 `R32` ($1.5\text{ k}\Omega$ 至 `VDDIO`)**  | 〃                                                                                              |

#### (3) `CN5` 引出的 SPI 与 SAI 数字音频总线

| 外设编号       | 信号名称        | MPU 引脚 (BGA Ball)  | `CN5` 物理针脚号      | 原理图网络名       | 说明                         |
|:---------- |:----------- |:------------------ |:---------------- |:------------ |:-------------------------- |
| **`SPI6`** | `SPI6_MOSI` | **`PC7`** (`AF9`)  | **`CN5` Pin 19** | `EXP_GPIO10` | `CN5` 独占 4 线 SPI，无板载冲突     |
| **`SPI6`** | `SPI6_MISO` | **`PC4`** (`AC9`)  | **`CN5` Pin 21** | `EXP_GPIO9`  | 〃                          |
| **`SPI6`** | `SPI6_SCK`  | **`PF7`** (`AB8`)  | **`CN5` Pin 23** | `EXP_GPIO11` | 〃                          |
| **`SPI6`** | `SPI6_NSS`  | **`PF4`** (`AF10`) | **`CN5` Pin 24** | `EXP_GPIO8`  | 〃                          |
| **`SAI4`** | `SAI4_SCKA` | **`PD2`** (`D12`)  | **`CN5` Pin 12** | `EXP_GPIO18` | `CN5` 独占数字音频接口，亦可用作普通 GPIO |
| **`SAI4`** | `SAI4_FSA`  | **`PD0`** (`B17`)  | **`CN5` Pin 35** | `EXP_GPIO19` | 〃                          |
| **`SAI4`** | `SAI4_SDA`  | **`PD1`** (`A19`)  | **`CN5` Pin 38** | `EXP_GPIO20` | 〃                          |
| **`SAI4`** | `SAI4_SDB`  | **`PB5`** (`C14`)  | **`CN5` Pin 40** | `EXP_GPIO21` | 〃                          |

#### (4) `CN5` 40-Pin 完整物理针脚对照表（双列俯视图）

| 左列 (奇数脚) 信号 / 网络                                     | MPU 引脚 (Ball)       | **Pin #** | **Pin #** | MPU 引脚 (Ball)      | 右列 (偶数脚) 信号 / 网络                                   |
|:---------------------------------------------------- |:------------------- |:---------:|:---------:|:------------------ |:-------------------------------------------------- |
| **`3V3`** ($3.3\text{ V}$ 电源)                        | —                   | **1**     | **2**     | —                  | **`5V_STLK`** ($5\text{ V}$ 电源)                    |
| **`I2C8_SDA` / `I3C4_SDA`** (`EXP_GPIO2`, `1.5K` 上拉) | **`PZ3`** (`Y6`)    | **3**     | **4**     | —                  | **`5V_STLK`** ($5\text{ V}$ 电源)                    |
| **`I2C8_SCL` / `I3C4_SCL`** (`EXP_GPIO3`, `1.5K` 上拉) | **`PZ4`** (`V3`)    | **5**     | **6**     | —                  | **`GND`**                                          |
| **`MCO1`** (`EXP_GPIO4`, 带测试点 `TP11`)                | **`PF11`** (`AE4`)  | **7**     | **8**     | **`PF13`** (`P2`)  | **`USART6_TX`** (`EXP_GPIO14`)                     |
| **`GND`**                                            | —                   | **9**     | **10**    | **`PF14`** (`P3`)  | **`USART6_RX`** (`EXP_GPIO15`)                     |
| **`USART6_RTS`** (`EXP_GPIO17`)                      | **`PG5`** (`R3`)    | **11**    | **12**    | **`PD2`** (`D12`)  | **`SAI4_SCKA`** (`EXP_GPIO18`)                     |
| **`GPIO27`** (`EXP_GPIO27`)                          | **`PZ9`** (`U3`)    | **13**    | **14**    | —                  | **`GND`**                                          |
| **`GPIO22`** (`EXP_GPIO22`)                          | **`PZ0`** (`T4`)    | **15**    | **16**    | **`PZ1`** (`U7`)   | **`GPIO23`** (`EXP_GPIO23`)                        |
| **`3V3`** ($3.3\text{ V}$ 电源)                        | —                   | **17**    | **18**    | **`PZ6`** (`V4`)   | **`GPIO24`** (`EXP_GPIO24`)                        |
| **`SPI6_MOSI`** (`EXP_GPIO10`)                       | **`PC7`** (`AF9`)   | **19**    | **20**    | —                  | **`GND`**                                          |
| **`SPI6_MISO`** (`EXP_GPIO9`)                        | **`PC4`** (`AC9`)   | **21**    | **22**    | **`PZ7`** (`V2`)   | **`GPIO25`** (`EXP_GPIO25` / 可复用 **`LPUART1_TX`**) |
| **`SPI6_SCK`** (`EXP_GPIO11`)                        | **`PF7`** (`AB8`)   | **23**    | **24**    | **`PF4`** (`AF10`) | **`SPI6_NSS`** (`EXP_GPIO8`)                       |
| **`GND`**                                            | —                   | **25**    | **26**    | **`PZ5`** (`R5`)   | **`GPIO7`** (`EXP_GPIO7`)                          |
| **`I2C2_SDA`** (`ID_SD`, **板载多芯片共享**, `1.5K` 上拉)     | **`PF0`** (`AA15`)  | **27**    | **28**    | **`PF2`** (`AC15`) | **`I2C2_SCL`** (`ID_SC`, **板载多芯片共享**, `1.5K` 上拉)   |
| **`TIM8_CH4`** (`EXP_GPIO5`)                         | **`PC10`** (`AG10`) | **29**    | **30**    | —                  | **`GND`**                                          |
| **`TIM5_CH1`** (`EXP_GPIO6`)                         | **`PH8`** (`AD16`)  | **31**    | **32**    | **`PA5`** (`AE17`) | **`TIM2_CH4`** (`EXP_GPIO12`)                      |
| **`TIM4_CH2`** (`EXP_GPIO13`)                        | **`PA1`** (`AC19`)  | **33**    | **34**    | —                  | **`GND`**                                          |
| **`SAI4_FSA`** (`EXP_GPIO19`)                        | **`PD0`** (`B17`)   | **35**    | **36**    | **`PF15`** (`R2`)  | **`USART6_CTS`** (`EXP_GPIO16`)                    |
| **`GPIO26`** (`EXP_GPIO26` / 可复用 **`LPUART1_RX`**)   | **`PZ8`** (`V1`)    | **37**    | **38**    | **`PD1`** (`A19`)  | **`SAI4_SDA`** (`EXP_GPIO20`)                      |
| **`GND`**                                            | —                   | **39**    | **40**    | **`PB5`** (`C14`)  | **`SAI4_SDB`** (`EXP_GPIO21`)                      |

---

### 2.3 板载内部专用 UART / I2C 总线（未引出至 `CN5`，直连板载芯片）

| 总线编号                        | MPU 引脚 (BGA Ball)                                                                                                            | 原理图网络名                                                     | 板载上拉                                                                               | 连接目标芯片与地址                                                                          |
|:--------------------------- |:---------------------------------------------------------------------------------------------------------------------------- |:---------------------------------------------------------- |:---------------------------------------------------------------------------------- |:---------------------------------------------------------------------------------- |
| **`I2C7`** *(原理图标注 `PMIC`)* | **`PD14`** (`E19`) = `SDA`<br>**`PD15`** (`E15`) = `SCL`                                                                     | `I2C7.SDA`<br>`I2C7.SCL`                                   | `R260` ($1.5\text{ k}\Omega$ 至 `VDDIO`)<br>`R261` ($1.5\text{ k}\Omega$ 至 `VDDIO`) | **电源管理芯片 `STPMIC25APQR` (`U26` Pin 34/33)**，7-bit 地址 `0x33` (8-bit `0x66`)         |
| **`I2C1`**                  | **`PI1`** (`M6`) = `SDA`<br>**`PG13`** (`P1`) = `SCL`                                                                        | `I2C1.SDA`<br>`I2C1.SCL`                                   | `R228` ($1.5\text{ k}\Omega$ 至 `3V3`)<br>`R224` ($1.5\text{ k}\Omega$ 至 `3V3`)     | **USB Type-C PD 保护芯片 `TCPP03-M20` (`U50` Pin 17/18)**，7-bit 地址 `0x35` (`0110101X`) |
| **`USART1`**                | **`PG14`** (`N1`) = `TX` (`TP1`)<br>**`PG15`** (`K6`) = `RX` (`TP9`)<br>**`PI2`** (`N5`) = `RTS`<br>**`PI3`** (`N7`) = `CTS` | `USART1_TX`<br>`USART1_RX`<br>`USART1_RTS`<br>`USART1_CTS` | —                                                                                  | **Wi-Fi/BT 模组 `Murata LBEE5KL1YN` (`U40` Pin 2/3/4/5)** 蓝牙 HCI 串口                  |

---

## 3. 原理图标注的引脚复用冲突与板载芯片占用清单

### 3.1 直接存在一脚多用 / 多芯片总线共享冲突的引脚（高危避坑）

1. **`PF0` (`I2C2.SDA`) & `PF2` (`I2C2.SCL`) —— 5 路跨页共享冲突（Sheet 2, 3, 9, 10, 11, 14, 16 标注 `PERIPH`）**
   
   * **冲突详情**：虽然引出到了扩展口 **`CN5` Pin 27 (`ID_SD`) / Pin 28 (`ID_SC`)**，但同一物理网络 (`I2C2.SDA` / `I2C2.SCL`，带 `R31`/`R32` $1.5\text{ k}\Omega$ 上拉) 在板载同时并联了：
     * **HDMI 桥接芯片 `ADV7535BCBZ` (`U9` Pin E5/E6，Sheet 10)**：常驻焊接在板上，7-bit 地址 `0x3D` (8-bit `0x7A`)；
     * **LVDS 触摸屏接口 `CN2` (Pin 7/8，Sheet 9)**：接 `B-LVDS7-WSVGA` 电容触摸屏控制器；
     * **MIPI CSI 摄像头接口 `CN8` (Pin 20/21，Sheet 11)**：接 `B-CAMS-IMX` 模组上的 `IMX335` 摄像头、ToF 测距与 IMU 传感器；
     * **USB 2.0 Hub `USB2422` (`U34` Pin 13/14，Sheet 14)**：经锡桥 `SB1` / `SB2` 连接（默认 **`DNF` 断开**）。
   * **开发约束**：**严禁将 `CN5` Pin 27/28 (`PF0`/`PF2`) 当作普通 GPIO 或 PWM 翻转**，否则会锁死 `I2C2` 总线并导致 Linux HDMI 显示/摄像头/触摸驱动崩溃；M33 若需接外置 I2C 从机，应优先使用 **`CN5` Pin 3/5 (`PZ3`/`PZ4` = `I2C8`)**。

2. **`PH4` (`AB16`) —— 用户红灯 `LD7` (`LED1`) 与 BootROM `BOOTFAILN` 复用冲突（Sheet 3 `Y=360.9` 同时标注 `LED1` 与 `BOOTFAILN`）**
   
   * **冲突详情**：`PH4` 在原理图 Sheet 3 上同时挂有网络名 `LED1` 和红色高亮标注 **`BOOTFAILN`**。系统上电若 BootROM 校验失败，硬件会自动将 `PH4` 配置为开漏输出低电平，强制点亮红灯 `LD7`。

3. **`PF10` (`AE5`) —— `MCO2`、`VBUS_DIV` 与 `ADC3P2` 三重标注冲突（Sheet 3 `Y=324.4` & Sheet 13）**
   
   * **冲突详情**：Sheet 3 在 `PF10` 引脚旁同时标注了 **`MCO2`**、**`VBUS_DIV`**、**`ADC3P2`**（带测试点 `TP35`）。在 Sheet 13 (`USB_DRD_TypeC.SchDoc`) 中，`PF10` 物理连接到了 Type-C `VBUSc` 的电阻分压采样网络（`R219` $140\text{ k}\Omega$ 接 `VBUSc`，`R220` $10\text{ k}\Omega$ 接 `GND`，并 `C99` $10\text{ nF}$）。
   * **开发约束**：`PF10` 已被 USBPD 子系统固定用作 **`ADC3_INP2`** 测量 VBUS 电压，**不可配置为 `MCO2` 时钟输出**（否则推挽输出会与 $10\text{ k}\Omega/140\text{ k}\Omega$ 分压网络及 `10nF` 电容直接冲突；如需对外输出时钟请用 `CN5` Pin 7 的 `PF11` = `MCO1`）。

4. **`PA0` (`AF5`) —— `MP257_WAKEUP` 与 PMIC `INTn` / `B2` 唤醒键耦合（Sheet 3 & Sheet 4）**
   
   * **冲突详情**：`PA0` 连接 `STPMIC25` (`U26`) 的中断输出脚 `INTn` (Pin 32)。板载唤醒按键 `B2` 接在 PMIC 的 `PONKEYn` (Pin 1) 上，按下 `B2` 以及 PMIC 任何电源轨过流/欠压告警都会通过同一个 **`PA0`** 引脚触发中断。

5. **`PZ3` (`CN5-3`) & `PZ4` (`CN5-5`) —— 固定 $1.5\text{ k}\Omega$ 强上拉对 GPIO / I3C 的影响（Sheet 3 & Sheet 16）**
   
   * **冲突详情**：Sheet 3 同时标注了 `I2C8.SDA`/`I3C4_SDA` 与 `I2C8.SCL`/`I3C4_SCL`，且板上已焊接 `R246`/`R245` ($1.5\text{ k}\Omega$) 强上拉至 $3.3\text{ V}$。若把 `CN5` Pin 3/5 当作高阻输入或低功耗输出使用，需计入每个引脚拉低时 $2.2\text{ mA}$ 的灌电流。

6. **`PE0..PE5` (`VDDIO1`) 与 `PE6..PE15` (`VDDIO2`) —— 独立低电压/可变电压域限制（Sheet 3）**
   
   * **`PE0..PE5` (`SDMMC1` microSD 卡槽 `CN12`)**：位于 **`VDDIO1`** 域，由 PMIC `LDO8` (`VDDIO_SDCARD`) 供电，电压随 SD 卡模式在 **$3.3\text{ V}$ 与 $1.8\text{ V}$ 之间动态切换**。
   * **`PE6..PE15` (`SDMMC2` 板载 32Gbit eMMC `U37`)**：位于 **`VDDIO2`** 域，永久由 **`1V8` ($1.8\text{ V}$)** 供电，且全部直连板载 eMMC 芯片。

---

### 3.2 板载专用外设芯片占用的全部 MPU 引脚汇总表（M33 不可随意挪用）

当对应的板载外设（由 Cortex-A35 Linux 或板级固件驱动）工作时，以下 GPIO 引脚在物理电路上已被板载芯片独占：

| 板载外设模块 (原理图页码)                                                                  | 占用引脚数          | 涉及的 MPU GPIO 引脚与网络功能明细                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
|:------------------------------------------------------------------------------- |:--------------:|:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **1. PMIC 电源管理 `STPMIC25APQR` (`U26`, Sheet 3 & 4)**                            | 4 GPIO + 4 Sys | • **`PD14`** (`E19`): `I2C7.SDA` (`1.5K` 上拉)<br>• **`PD15`** (`E15`): `I2C7.SCL` (`1.5K` 上拉)<br>• **`PA0`** (`AF5`): `MP257_WAKEUP` (接 `U26-32` `INTn`)<br>• **`PC13`** (`AE6`): `PMIC_WAKEUPn` (经 `R46` $0\ \Omega$ 接 `U26-30` `WAKEUPn`)<br>• *专用系统脚*：`PWR_ON` (`AD3` $\rightarrow$ `PWRCTRL1`), `PWR_CPUON` (`AB4` $\rightarrow$ `PWRCTRL2`), `NRSTC1MS` (`W3` $\rightarrow$ `PWRCTRL3`), `NRST` (`AE1` $\rightarrow$ `RSTn`)                                                                                                                                                                                                                                                                                                                                             |
| **2. 千兆以太网 PHY `RTL8211F-CG` (`U38`, Sheet 3 & 8)**                             | 17             | • **控制与管理**：**`PA2`** (`AD19`, `ETH1.NRST`, `R205` $10\text{ k}\Omega$ 下拉), **`PA9`** (`AC16`, `ETH1.MDC`), **`PA10`** (`AG20`, `ETH1.MDIO`), **`PA12`** (`AD17`, `ETH1.MDINT` / `PHY_INTN`)<br>• **RGMII 发送 (均串 $22\ \Omega$)**：**`PA13`** (`AF18`, `TX_CTL`), **`PC0`** (`AG11`, `GTX_CLK`), **`PA15`** (`AD18`, `TXD0`), **`PC1`** (`AD11`, `TXD1`), **`PH10`** (`AF15`, `TXD2`), **`PH11`** (`AE14`, `TXD3`)<br>• **RGMII 接收与时钟 (带 PHY 配置上下拉)**：**`PA11`** (`AF17`, `RX_CTL`), **`PA14`** (`AE18`, `RX_CLK`), **`PF1`** (`AF12`, `RXD0`), **`PC2`** (`AE11`, `RXD1`), **`PH12`** (`AF14`, `RXD2`), **`PH13`** (`AG14`, `RXD3`), **`PH9`** (`AD15`, `CLK125`)<br>• *注*：**`PF3`** (`AF11`, `ETH1.CLK`) 经 **`R196` (`DNF` 未焊)** 接 PHY，默认断开（PHY 使用独立晶振 `X4` $25\text{ MHz}$） |
| **3. 板载 32Gbit eMMC `THGBMDG5D1LBAIL` (`U37`, Sheet 3 & 6)**                    | 10 (`1.8V` 域)  | • **`PE6`** (`D22`, `D6`), **`PE7`** (`D19`, `D7`), **`PE8`** (`D23`, `D2`), **`PE9`** (`C23`, `D5`), **`PE10`** (`C20`, `D4`), **`PE11`** (`C22`, `D1`), **`PE12`** (`A22`, `D3`), **`PE13`** (`B22`, `D0`), **`PE14`** (`B23`, `CK`), **`PE15`** (`C21`, `CMD`)<br>• *注*：eMMC 硬件复位脚 `RSTn` 经 `R86` ($0\ \Omega$) 接 PMIC `LDO2_1V8` 电源输出 (`D7` 二极管至 `NRSTC1MS` 为 `DNF`)                                                                                                                                                                                                                                                                                                                                                                                                     |
| **4. microSD 卡槽 (`CN12`, Sheet 3 & 7)**                                         | 7              | • **SDMMC1 (`VDDIO1` 动态 $3.3\text{V}/1.8\text{V}$ 域)**：**`PE0`** (`E18`, `D2`), **`PE1`** (`D18`, `D3`), **`PE2`** (`E16`, `CMD`), **`PE3`** (`D17`, `CK`), **`PE4`** (`E14`, `D0`), **`PE5`** (`E17`, `D1`)<br>• **卡插入检测 (`VDDIO` $3.3\text{V}$ 域)**：**`PD3`** (`C13`, `SDMMC1_CD`, 外部 `R87` $10\text{ k}\Omega$ 上拉至 `VDDIO`，插卡拉低至 `GND`)                                                                                                                                                                                                                                                                                                                                                                                                                                   |
| **5. Wi-Fi / 蓝牙二合一模组 `Murata LBEE5KL1YN` (`U40`, Sheet 3 & 12)**                | 19             | • **Wi-Fi (`SDMMC3` + 控制)**：**`PB13`** (`K2`, `CK`), **`PD12`** (`J4`, `CMD`), **`PB14`** (`L4`, `D0`), **`PD13`** (`L6`, `D1`), **`PB12`** (`L3`, `D2`), **`PI11`** (`K4`, `D3`), **`PG8`** (`M1`, `WL_REG_ON`), **`PG11`** (`M5`, `WL_HOST_WAKE`)<br>• **Bluetooth (`USART1` + `PCM/I2S1` + 控制)**：**`PG14`** (`N1`, `USART1_TX`), **`PG15`** (`K6`, `USART1_RX`), **`PI2`** (`N5`, `USART1_RTS`), **`PI3`** (`N7`, `USART1_CTS`), **`PG6`** (`T1`, `BT_PCM_CLK`), **`PG7`** (`T2`, `BT_PCM_WS`), **`PF12`** (`N4`, `BT_PCM_SDI`), **`PI5`** (`N6`, `BT_PCM_SDO`), **`PG4`** (`T3`, `BT_REG_ON`), **`PG2`** (`AA9`, `BT_HOST_WAKE`), **`PB15`** (`K5`, `BT_DEV_WAKE`), **`PI8`** (`V6`, `LPO_32` $32.768\text{ kHz}$ 时钟输出)                                                  |
| **6. USB Type-C PD 与高速开关 (`U50` `TCPP03-M20` & `U56` `FUSB340`, Sheet 3 & 13)** | 9              | • **TCPP03-M20 (`U50`)**：**`PG13`** (`P1`, `I2C1.SCL`), **`PI1`** (`M6`, `I2C1.SDA`), **`PA3`** (`AD20`, `TCPP_EN`), **`PG1`** (`AB9`, `TCPP_INTN`), **`PF5`** (`AG7`, `VCONN_CC1_EN`), **`PC3`** (`AC10`, `VCONN_CC2_EN`), **`PF10`** (`AE5`, `VBUS_DIV` / `ADC3P2` / `MCO2`)<br>• **USB3.0 正反插开关 `FUSB340` (`U56`)**：**`PH2`** (`AC12`, `USBSW_OE`), **`PH3`** (`AE13`, `USBSW_S`)                                                                                                                                                                                                                                                                                                                                                                                         |
| **7. 板载双数字麦克风 `IMP34DT05TR` (`U12` 右 & `U53` 左, Sheet 3 & 15)**                 | 2              | • **`PA7`** (`AB17`, `MDF.CCK` 时钟输出，带 `TP32`)<br>• **`PA6`** (`AF16`, `MDF.SDI6` 双麦立体声 PDM 数据输入，带 `TP26`)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| **8. HDMI 桥接芯片 `ADV7535BCBZ` (`U9`, Sheet 3 & 10)**                             | 5 专用 + 2 共享    | • **专用控制与音频 (`I2S2`)**：**`PB4`** (`B13`, `HDMI.INT`), **`PB6`** (`A13`, `HDMI.RST`), **`PB0`** (`B15`, `HDMI_I2S2.CK`), **`PB2`** (`B14`, `HDMI_I2S2_SDO`), **`PB3`** (`B16`, `HDMI_I2S2.WS`)<br>• **共享 I2C2**：**`PF0`** (`AA15`, `I2C2.SDA`), **`PF2`** (`AC15`, `I2C2.SCL`)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                |
| **9. LVDS 显示与触摸屏接口 (`CN2`, Sheet 3 & 9)**                                       | 4 专用 + 2 共享    | • **专用控制**：**`PI0`** (`P6`, `CTP.RST`), **`PI6`** (`P5`, `CTP.INT`), **`PI4`** (`A12`, `DISP.LED_EN`), **`PI7`** (`R6`, `DISP.BLCTRL`)<br>• **共享 I2C2**：**`PF0`** (`AA15`, `I2C2.SDA`), **`PF2`** (`AC15`, `I2C2.SCL`)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
| **10. MIPI CSI-2 摄像头接口 (`CN8`, Sheet 3 & 11)**                                  | 6 专用 + 2 共享    | • **专用控制**：**`PB1`** (`D14`, `CAM.RST`), **`PB11`** (`D13`, `CAM.EN`), **`PB7`** (`A16`, `CAM.TOF_LPn`), **`PB8`** (`C12`, `CAM.TOF_INT`), **`PB9`** (`A14`, `CAM.IMU_INT1`), **`PB10`** (`A17`, `CAM.IMU_INT2`)<br>• **共享 I2C2**：**`PF0`** (`AA15`, `I2C2.SDA`), **`PF2`** (`AC15`, `I2C2.SCL`)                                                                                                                                                                                                                                                                                                                                                                                                                                                                             |

---

## 4. 全芯片 138 个 GPIO 引脚 (`PA0..PZ9`) 极速检索总表（Sheet 3 `U1B`）

| Port     | Pin 0                                      | Pin 1                                    | Pin 2                                        | Pin 3                                         | Pin 4                                         | Pin 5                               | Pin 6                           | Pin 7                                          | Pin 8                                          | Pin 9                              | Pin 10                                  | Pin 11                                | Pin 12                      | Pin 13                            | Pin 14                             | Pin 15                              |
|:-------- |:------------------------------------------ |:---------------------------------------- |:-------------------------------------------- |:--------------------------------------------- |:--------------------------------------------- |:----------------------------------- |:------------------------------- |:---------------------------------------------- |:---------------------------------------------- |:---------------------------------- |:--------------------------------------- |:------------------------------------- |:--------------------------- |:--------------------------------- |:---------------------------------- |:----------------------------------- |
| **`PA`** | `AF5`<br>`MP257_WAKEUP` (`U26` INTn)       | `AC19`<br>**`CN5-33`** (`TIM4_CH2`)      | `AD19`<br>`ETH1.NRST`                        | `AD20`<br>`TCPP_EN` (`U50`)                   | `AG17`<br>**`USART2_TX`** (STLK VCP1)         | `AE17`<br>**`CN5-32`** (`TIM2_CH4`) | `AF16`<br>`MDF.SDI6` (麦克风)      | `AB17`<br>`MDF.CCK` (麦克风)                      | `AA17`<br>**`USART2_RX`** (STLK VCP1)          | `AC16`<br>`ETH1.MDC`               | `AG20`<br>`ETH1.MDIO`                   | `AF17`<br>`ETH1.RX_CTL`               | `AD17`<br>`ETH1.MDINT`      | `AF18`<br>`ETH1.TX_CTL`           | `AE18`<br>`ETH1.RX_CLK`            | `AD18`<br>`ETH1.TXD0`               |
| **`PB`** | `B15`<br>`HDMI_I2S2.CK`                    | `D14`<br>`CAM.RST`                       | `B14`<br>`HDMI_I2S2_SDO`                     | `B16`<br>`HDMI_I2S2.WS`                       | `B13`<br>`HDMI.INT`                           | `C14`<br>**`CN5-40`** (`SAI4_SDB`)  | `A13`<br>`HDMI.RST`             | `A16`<br>`CAM.TOF_LPn`                         | `C12`<br>`CAM.TOF_INT`                         | `A14`<br>`CAM.IMU_INT1`            | `A17`<br>`CAM.IMU_INT2`                 | `D13`<br>`CAM.EN`                     | `L3`<br>`WL.SDMMC3_D2`      | `K2`<br>`WL.SDMMC3_CK`            | `L4`<br>`WL.SDMMC3_D0`             | `K5`<br>`BT_DEV_WAKE`               |
| **`PC`** | `AG11`<br>`ETH1.GTX_CLK`                   | `AD11`<br>`ETH1.TXD1`                    | `AE11`<br>`ETH1.RXD1`                        | `AC10`<br>`VCONN_CC2_EN`                      | `AC9`<br>**`CN5-21`** (`SPI6_MISO`)           | `AD10`<br>**`B5` (`USER1`)** (高有效)  | `AB10`<br>*NC*                  | `AF9`<br>**`CN5-19`** (`SPI6_MOSI`)            | `AG9`<br>*NC*                                  | `AE9`<br>*NC*                      | `AG10`<br>**`CN5-29`** (`TIM8_CH4`)     | `AD8`<br>**`B4` (`USER2`)** (高有效)     | `AE7`<br>*NC*               | `AE6`<br>`PMIC_WAKEUPn`           | —                                  | —                                   |
| **`PD`** | `B17`<br>**`CN5-35`** (`SAI4_FSA`)         | `A19`<br>**`CN5-38`** (`SAI4_SDA`)       | `D12`<br>**`CN5-12`** (`SAI4_SCKA`)          | `C13`<br>`SDMMC1_CD` (SD卡检测)                  | `C18`<br>*NC*                                 | `B18`<br>*NC*                       | `A18`<br>*NC*                   | `D20`<br>*NC*                                  | `C16`<br>*NC*                                  | `C15`<br>*NC*                      | `C17`<br>*NC*                           | `A15`<br>*NC*                         | `J4`<br>`WL.SDMMC3_CMD`     | `L6`<br>`WL.SDMMC3_D1`            | `E19`<br>**`I2C7.SDA`** (PMIC)     | `E15`<br>**`I2C7.SCL`** (PMIC)      |
| **`PE`** | `E18`<br>`SDMMC1_D2` (`VDDIO1`)            | `D18`<br>`SDMMC1_D3` (`VDDIO1`)          | `E16`<br>`SDMMC1_CMD` (`VDDIO1`)             | `D17`<br>`SDMMC1_CK` (`VDDIO1`)               | `E14`<br>`SDMMC1_D0` (`VDDIO1`)               | `E17`<br>`SDMMC1_D1` (`VDDIO1`)     | `D22`<br>`eMMC_D6` (`1.8V`)     | `D19`<br>`eMMC_D7` (`1.8V`)                    | `D23`<br>`eMMC_D2` (`1.8V`)                    | `C23`<br>`eMMC_D5` (`1.8V`)        | `C20`<br>`eMMC_D4` (`1.8V`)             | `C22`<br>`eMMC_D1` (`1.8V`)           | `A22`<br>`eMMC_D3` (`1.8V`) | `B22`<br>`eMMC_D0` (`1.8V`)       | `B23`<br>`eMMC_CK` (`1.8V`)        | `C21`<br>`eMMC_CMD` (`1.8V`)        |
| **`PF`** | `AA15`<br>**`CN5-27`** (`I2C2.SDA` **共享**) | `AF12`<br>`ETH1.RXD0`                    | `AC15`<br>**`CN5-28`** (`I2C2.SCL` **共享**)   | `AF11`<br>`ETH1.CLK` (`R196` DNF)             | `AF10`<br>**`CN5-24`** (`SPI6_NSS`)           | `AG7`<br>`VCONN_CC1_EN`             | `AC7`<br>*NC*                   | `AB8`<br>**`CN5-23`** (`SPI6_SCK`)             | `AE10`<br>*NC*                                 | `AE8`<br>*NC*                      | `AE5`<br>`VBUS_DIV` / `ADC3P2` / `MCO2` | `AE4`<br>**`CN5-7`** (`MCO1`, `TP11`) | `N4`<br>`BT_PCM_SDI`        | `P2`<br>**`CN5-8`** (`USART6_TX`) | `P3`<br>**`CN5-10`** (`USART6_RX`) | `R2`<br>**`CN5-36`** (`USART6_CTS`) |
| **`PG`** | `AF7`<br>*NC*                              | `AB9`<br>`TCPP_INTN` (`U50`)             | `AA9`<br>`BT_HOST_WAKE`                      | `AA7`<br>*NC*                                 | `T3`<br>`BT_REG_ON`                           | `R3`<br>**`CN5-11`** (`USART6_RTS`) | `T1`<br>`BT_PCM_CLK`            | `T2`<br>`BT_PCM_WS`                            | `M1`<br>`WL_REG_ON`                            | `M3`<br>**`UART5_TX`** (STLK VCP2) | `M4`<br>**`UART5_RX`** (STLK VCP2)      | `M5`<br>`WL_HOST_WAKE`                | `N3`<br>*NC*                | `P1`<br>**`I2C1.SCL`** (`TCPP03`) | `N1`<br>**`USART1_TX`** (BT)       | `K6`<br>**`USART1_RX`** (BT)        |
| **`PH`** | `Y1`<br>`OSC_IN` (`X2` $40\text{MHz}$)     | `AA1`<br>`OSC_OUT` (`X2` $40\text{MHz}$) | `AC12`<br>`USBSW_OE` (`U56`)                 | `AE13`<br>`USBSW_S` (`U56`)                   | `AB16`<br>**`LD7` 红灯** (低有效/`BOOTFAILN`)      | `AG18`<br>**`LD8` 绿灯** (高有效)        | `AF19`<br>**`LD9` 橙灯** (高有效)    | `AE16`<br>**`LED1` 蓝灯** (`LED4`, 高有效)          | `AD16`<br>**`CN5-31`** (`TIM5_CH1`)            | `AD15`<br>`ETH1.CLK125`            | `AF15`<br>`ETH1.TXD2`                   | `AE14`<br>`ETH1.TXD3`                 | `AF14`<br>`ETH1.RXD2`       | `AG14`<br>`ETH1.RXD3`             | —                                  | —                                   |
| **`PI`** | `P6`<br>`CTP.RST`                          | `M6`<br>**`I2C1.SDA`** (`TCPP03`)        | `N5`<br>`USART1_RTS` (BT)                    | `N7`<br>`USART1_CTS` (BT)                     | `A12`<br>`DISP.LED_EN`                        | `N6`<br>`BT_PCM_SDO`                | `P5`<br>`CTP.INT`               | `R6`<br>`DISP.BLCTRL`                          | `V6`<br>`LPO_32` (BT $32\text{kHz}$)           | `U1`<br>*NC*                       | `D16`<br>*NC*                           | `K4`<br>`WL.SDMMC3_D3`                | —                           | —                                 | —                                  | —                                   |
| **`PZ`** | `T4`<br>**`CN5-15`** (`GPIO22`)            | `U7`<br>**`CN5-16`** (`GPIO23`)          | `W5`<br>**`B3` (`TAMPER`)** (低有效/`R139` DNF) | `Y6`<br>**`CN5-3`** (`I2C8_SDA` / `I3C4_SDA`) | `V3`<br>**`CN5-5`** (`I2C8_SCL` / `I3C4_SCL`) | `R5`<br>**`CN5-26`** (`GPIO7`)      | `V4`<br>**`CN5-18`** (`GPIO24`) | `V2`<br>**`CN5-22`** (`GPIO25` / `LPUART1_TX`) | `V1`<br>**`CN5-37`** (`GPIO26` / `LPUART1_RX`) | `U3`<br>**`CN5-13`** (`GPIO27`)    | —                                       | —                                     | —                           | —                                 | —                                  | —                                   |
