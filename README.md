# CyberSandglass_on_STM32F103_with_SPL
**课程作业 (Class Homework)**

**简陋，且施工中（Shabby and in progress）**

A combination of porting and new-developed elements to the realization of CyberSandglass on STM32F103 platform

基于STM32F103C8T6标准库的，参照开源程序的，自己编写的，适合外设的赛博沙漏模拟器

感谢核心程序来源于：*Core computing algorithm from*

[Dog1027/Hourglass](https://github.com/Dog1027/hourglass/blob/master/Hourglass.ino)

## 硬件（HARDWARE）
### 核心板
* STM32F103C8T6核心板（20脚），来自普中科技，附SWD/JWD调试接口（20脚）及ST-LINK下载器（10脚）
### 开发板
* 普中科技HC6800-EM3 V3.0。非必要，但是在开发过程中起到了很大的作用
### 外设
* LED阵列，任意一家的可用的MAX7219芯片驱动的2片8\*8阵列（共阴），或者2片16\*16阵列（<font color="red">注意</font>除非你掌握了基本的1焊接和跳线技能，否则不要尝试用四片8*8焊接）。供电电压3.3V即可
* 蓝牙模块：封装了HC-05的ZS-040模块，注意与核心板共地，供电电压3.3-5V。
* MPU：封装了MPU6050的模块，供电电压5V。
* 按键：任意按键
* OLED：任意3.96尺寸的OLED模块。
### 供电
* USB连接核心板供电
* 外部电源连接核心板5V端口供电
### 外壳及用作支撑的PCB
* STL+DWG，注意尺寸

## 软件 (SOFTWARE)
### SPL库
* 未优化
* 引用和嵌套仍然混乱
### UCOS II
