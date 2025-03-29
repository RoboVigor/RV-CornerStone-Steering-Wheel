#include "Driver_Arm.h"
#include "macro.h"
#include "config.h"
#include "handle.h"
#include "vegmath.h"
#include "math.h"


void Arm_Init(ArmData_Type *ad){


}

void Arm_Update(ArmData_Type *ad,float q1,float q2,float q3,float q4,float q5,float q6){
    ad->jointAngle[0]=q1;
    ad->jointAngle[1]=q2;
    ad->jointAngle[2]=q3;
    ad->jointAngle[3]=q4;
    ad->jointAngle[4]=q5;
    ad->jointAngle[5]=q6;

    ad->x=LINK1_LENGTH*vegcos(ad->jointAngle[1])+LINK2_LENGTH*vegcos(ad->jointAngle[1]+ad->jointAngle[2])+LINK3_LENGTH*vegcos(ad->jointAngle[1]+ad->jointAngle[2]+ad->jointAngle[3]);
    ad->y=LINK1_LENGTH*vegsin(ad->jointAngle[1])+LINK2_LENGTH*vegsin(ad->jointAngle[1]+ad->jointAngle[2])+LINK3_LENGTH*vegsin(ad->jointAngle[1]+ad->jointAngle[2]+ad->jointAngle[3]);
    ad->theta=ad->jointAngle[1]+ad->jointAngle[2]+ad->jointAngle[3];

}

void Arm_Calculate_Joint_Angle(ArmData_Type *ArmData,int16_t endX,int16_t endY,float theta){
	float  u,v;
    u= (2*(endX-LINK3_LENGTH*vegcos(theta))*(endX-LINK3_LENGTH*vegcos(theta)) - LINK1_LENGTH*LINK1_LENGTH - LINK2_LENGTH*LINK2_LENGTH) / (2*LINK1_LENGTH*LINK2_LENGTH);
	
}
