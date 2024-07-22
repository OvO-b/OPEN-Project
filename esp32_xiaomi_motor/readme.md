# 前言

鉴于项目要使用小米电机，并且要使用esp32上自带的CAN库来控制，但是没有找到合适的、能用的库，只能从现有STM32小米电机控制库的基础上进行修改。为了方便快速实现，采用Arduino的平台对esp32进行编程，对应需要修改成Arduino的库

---

# 一、前置任务


 1. [CAN通信基础](https://zhuanlan.zhihu.com/p/415418456)
 2. ESP32自带CAN控制器---TWAI：[官方文档](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/api-reference/peripherals/twai.html#id24)，[文章介绍](https://zhuanlan.zhihu.com/p/415830609)
 3. 相关教程：[CAN通信](https://lingshunlab.com/book/esp32/esp32-use-canbus-tja1051-esp32-arduino-can)，[Arduino外接CAN控制](https://blog.csdn.net/m0_60415072/article/details/136461449)，
[STM32F103用CAN通讯驱动小米电机讲解（视频）](https://www.bilibili.com/video/BV1Z1421C7dP/?spm_id_from=333.337.search-card.all.click&vd_source=8ef000d3deda5a263451f145f4a88281)

***上面这个视频是本库的关键参考，UP讲的非常细致，基本操作都是一样的，就不再赘述了（重要！！！）***

# 二、硬件配置

 1. esp32×1 
 2. TTL转CAN模块（是一种电平转换模块）×1 
 3. 小米微电机×1 杜邦线×4，CAN线×2 
 4. 24V电池×1
 5. USB转CAN模块（连接上位机查看数据，控制时不需要）

# 三、线路连接
![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/fdb5677c9d9a4140b107f4511df3eb2a.jpeg#pic_center)

 - esp32的4号引脚连接转换模块的RX，esp32的5号引脚连接转换模块的TX（esp32默认的CAN引脚）；
 - 转换模块的CANL连接电机的CANL，转换模块的CANH连接电机的CANH；
 - VCC接VCC，GND接GND； 
 - 连接24V电源；

# 四、引入底层库函数

由于esp32的Arduino库不自带CAN的库（也就是TWAI的库），需要自己去搜索下载

```c
ESP32-TWAI-CAN
```
## 1.Arduino原生ide
![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/65b8fab25d764f4eb38da7f1d7409e91.jpeg#pic_center)

## 2.VScode+PlatformIO
![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/973496d54ab64a6bae4efe97f838fb5a.jpeg#pic_center)
![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/f80f88ab2fab4aea8dd558285146cd85.jpeg#pic_center)


# 五、引入控制库
使用例程
```cpp
#include <Arduino.h>
#include "TWAI_CAN_MI_Motor.h"

MI_Motor_ M1_con;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Motor_CAN_Init();
  M1_con.Motor_Con_Init(MOTER_1_ID);
  M1_con.Motor_Set_Zero();
  M1_con.Change_Mode(SPEED_MODE);
  M1_con.Motor_Enable();
}

void loop() {
    M1_con.Motor_Data_Updata(20);
    Serial.printf("M1数据: %d,%d,%d,%d,%d,%d,%d,%d,%d,angle:%.2f,speed:%.2f,torque:%.2f,temp:%.2f,\r\n",
                                  M1_con.motor_rx_data.master_id,M1_con.motor_rx_data.motor_id,
                                  M1_con.motor_rx_data.err_sta,M1_con.motor_rx_data.HALL_err,
                                  M1_con.motor_rx_data.magnet_err,M1_con.motor_rx_data.temp_err,
                                  M1_con.motor_rx_data.current_err,M1_con.motor_rx_data.voltage_err,
                                  M1_con.motor_rx_data.mode_sta,
                                  M1_con.motor_rx_data.cur_angle,M1_con.motor_rx_data.cur_speed,
                                  M1_con.motor_rx_data.cur_torque,M1_con.motor_rx_data.cur_temp);
    M1_con.Set_SpeedMode(-0.1);
    vTaskDelay(20);
}
```



---


# 总结
CAN的数据发送应该没什么问题（需要电机先启动，再发控制指令），数据接收暂时还是轮询模式，不过配置有队列应该问题不大，有时间看看能不能改成中断处理