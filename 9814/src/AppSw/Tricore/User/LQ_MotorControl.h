/*!
  * @file     LQ_MotorControl.h
  *
  * @brief    
  *
  * @company  �����������ܿƼ�
  *
  * @author   LQ-005
  *
  * @note     Tab�� 4���ո�
  *
  * @version  V1.0
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2020��6��5��
  */ 
#ifndef SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_
#define SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_

#include <stdint.h>

void MotorInit(void);
void EncInit(void);
void MotorCtrl(float motor1, float motor2, float motor3, float motor4);
void SpeedCtrl(int16_t x, int16_t y, int16_t z);


#endif /* SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_ */
