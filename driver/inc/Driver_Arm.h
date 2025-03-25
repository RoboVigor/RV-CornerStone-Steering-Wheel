#ifndef __DRIVER_ARM_H
#define __DRIVER_ARM_H

#include "stm32f4xx.h"
#include "config.h"

typedef struct {
    // 转子转速
    int16_t jointSpeed[6];
    float jointAngle[6];    //角度制
    int16_t x;          //unit:mm
    int16_t y;
    float pitch;
    float roll;
} ArmData_Type;

void Arm_Init(ArmData_Type *ArmData);

void Arm_Calculate_Joint_Angle(ArmData_Type *ArmData,int16_t endX,int16_t endY);

#endif
