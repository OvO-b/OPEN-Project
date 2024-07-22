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