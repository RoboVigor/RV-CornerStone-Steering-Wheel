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

}

void Arm_Calculate_EOA(ArmData_Type *ad){
    ad->x=LINK1_LENGTH*vegcos(ad->jointAngle[1])+LINK2_LENGTH*vegcos(ad->jointAngle[1]+ad->jointAngle[2])+LINK3_LENGTH*vegcos(ad->jointAngle[1]+ad->jointAngle[2]+ad->jointAngle[3]);
    ad->y=LINK1_LENGTH*vegsin(ad->jointAngle[1])+LINK2_LENGTH*vegsin(ad->jointAngle[1]+ad->jointAngle[2])+LINK3_LENGTH*vegsin(ad->jointAngle[1]+ad->jointAngle[2]+ad->jointAngle[3]);
    ad->theta=ad->jointAngle[1]+ad->jointAngle[2]+ad->jointAngle[3];
}

void Arm_Calculate_Joint_Angle(ArmData_Type *ad,float endX,float endY,float theta){
	float  u,v,w;
	float q1,q2,q3;
	float c1,c2;
	float l1=LINK1_LENGTH,l2=LINK2_LENGTH,l3=LINK3_LENGTH;
	    
	c1=endX-l3*vegcos(theta);
	c2=endY-l3*vegsin(theta);
	
	float d_sq = c1*c1 + c2*c2;
    if (d_sq > (l1+l2)*(l1+l2) || d_sq < (l1-l2)*(l1-l2)) {
        printf("Unreachable position\n");
        return;
    }
	
    u= (c1*c1+c2*c2 - l1*l1 - l2*l2) / (2*l1*l2);
	float q2_rad=acos(u);
	if(q2_rad>0) q2_rad*=-1;
	q2=q2_rad/3.14*180;
	ad->jointAngle[2]=q2;
	
	float alpha = atan2(c2, c1);

    float beta_numerator = l2 * sin(q2_rad);
    float beta_denominator = l1 + l2 * cos(q2_rad);
    float beta = atan2(beta_numerator, beta_denominator);
	
	v=alpha-beta;
	q1=v/3.14*180;
	q3=theta-q1-q2;
	
	ad->jointAngle[1]=q1;
	ad->jointAngle[3]=q3;
}
