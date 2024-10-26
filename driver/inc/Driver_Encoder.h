/**
 * @file Driver_Motor.h
 * @brief 电机驱动
 * @note 新增电机应在handle.h和handle.c中声明并初始化,在stm32f4xx_it.h中设置对应电调ID
 * @version 0.8
 */

#ifndef __DRIVER_Encoder_H
#define __DRIVER_Encoder_H

#include "stm32f4xx.h"

typedef struct {
    int16_t  position;      // 转子位置(电调机械角度值), 0-8191
    int16_t  lastPosition;  // 上一次的转子位置
    int16_t  positionDiff;  // 转子位置差值
    int16_t  speed;         // 转子转速值, RPM
} Encoder_Type;


