# Zigbee-CC2530-indoor-env-monitoring-system （基于Zigbee技术CC2530单片机的室内环境监测系统）

## 如何运行 (How to run)
Copy to a new zstack project to run.
## 描述 (Discriptions)
One Coordinator and Four EndPoints struct this system whose parts connect with sensors or computer.

## 硬件列表 (Hardware List)
- CC2530 Core Board (核心板)								 X 5 
- CC2530 EX Board (底板)									 X 5
- DHT11 Temperature Sensor  (温湿度)				 X 1
- MQ-2 Smoke Sensor  (一氧化碳等粉尘)			 X 1
- Light Resistance Sensor  (光敏电阻感应)   X 1
- PM2.5 Smoke Sensor  (PM2.5粉尘)					 X 1

## 软件列表 (Software List)
- IAR Software For CC2530
- Texas Instruments Z-Stack Project Template Documents
- Serial Test Tool (SCOM)

## 功能逻辑 (Function Logic)
There is a serial winform software which contains temperature, humidity, CO analogue data, Light analogue data, PM2.5 serial-type receive order and charts for these environmental factors. 

## 提升点 (Improvements)
 - Add 4G comm, Wi-Fi comm, Bluetooth Comm or Modbus Comm to expand the data of this sensor system to a higher platform.
 - Code need to be simplified, because I am a freshman doing this project of using zstack project.
 - If I have next time to finish this, I should use EDA to draw another EX Board to create my own board.
 - To control my cost, I chose cheap sensors.So more expensive, more accuracy.

## 致敬 (Salute)
Master Gao is the teacher who helped me fix my data frame of this project, appreciate him a lot.
