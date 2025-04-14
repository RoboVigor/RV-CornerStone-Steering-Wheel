#ifndef __DRIVER_ARM_H
#define __DRIVER_ARM_H

#include "stm32f4xx.h"
#include "config.h"

typedef struct {
    // 转子转速
    int16_t jointSpeed[6];
    float jointAngle[6];    //角度制
    float x;          //unit:mm
    float y;
    float theta;
    float pitch;
    float roll;
} ArmData_Type;

void Arm_Init(ArmData_Type *ad);

void Arm_Update(ArmData_Type *ad,float q1,float q2,float q3,float q4,float q5,float q6);

void Arm_Calculate_EOA(ArmData_Type *ad);

void Arm_Calculate_Joint_Angle(ArmData_Type *ad,float endX,float endY,float theta);

#endif
