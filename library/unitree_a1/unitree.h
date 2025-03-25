#ifndef __UNITREE_H
#define __UNITREE_H

#include "stm32f4xx.h"
#include "Driver_CAN.h"
#include "Driver_BSP.h"
#include "vegmath.h"
#include "macro.h"

#define Unitree_Protocol_Send_Length 17
#define Unitree_Protocol_Receive_Length 16
#define Unitree_CRC16_Length 2

typedef struct {
    DFLT, FOC, CAL
} motor_status_type;

typedef union{
#pragma pack(push, 1)  
	struct{
		uint8_t head[2];//fdee
		
		uint8_t id:4;
		uint8_t status:3;
		uint8_t nc:1;
		
		int16_t  torque;
		int16_t velocity;
		int32_t angle;

		uint16_t  k_spd;
		uint16_t  k_pos;
		
		uint16_t crc16;
	};
#pragma pack(pop)
	uint8_t data[Unitree_Protocol_Send_Length];
}Unitree_Data_Type;


typedef struct{
	USART_TypeDef *USARTx;
	
	float torque;		//(N*m)
	float velocity;		//(ras/s)
	float angle;		//(rad)
	
	float k_spd;
	float k_pos;
	
	float torque_r;
	float velocity_r;
	float angle_r;

	float angleBias;
	float angleCalibrated;
	
	Unitree_Data_Type Unitree_Data;
	uint8_t receiveBuf[Unitree_Protocol_Receive_Length];
	
    void (*_Unitree_Init)(struct Unitree_Type *unitree, USART_TypeDef *USARTx,uint8_t id,  uint8_t status,float k_spd, float k_pos);
    void (*_Unitree_Bind)(struct Unitree_Type *unitree,USART_TypeDef *USARTx);

    void (*_Unitree_Send)(struct Unitree_Type *unitree);
    void (*_Unitree_Set_K)(struct Unitree_Type *unitree, float k_spd, float k_pos);
	void (*_Unitree_Set_Data)(struct Unitree_Type *unitree, float torque, float velocity,float angle);

	void (*_Unitree_Receive)(struct Unitree_Type *unitree);
	void (*_Unitree_Unpack)(struct Unitree_Type *unitree);

} Unitree_Type;

void Unitree_Init(Unitree_Type *unitree, USART_TypeDef *USARTx,uint8_t id, uint8_t status, float k_spd, float k_pos);
void _Unitree_Bind(Unitree_Type *unitree,USART_TypeDef *USARTx);

void _Unitree_Send(Unitree_Type *unitree) ;
void _Unitree_Set_K( Unitree_Type *unitree, float k_spd, float k_pos);
void _Unitree_Set_Data(Unitree_Type *unitree, float torque, float velocity,float angle);

void _Unitree_Receive(Unitree_Type *unitree);
void _Unitree_Unpack(Unitree_Type *unitree);

#endif

