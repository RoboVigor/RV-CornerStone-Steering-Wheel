#ifndef __UNITREE_H
#define __UNITREE_H

#include "stm32f4xx.h"
#include "Driver_CAN.h"
#include "Driver_BSP.h"

#define Unitree_Protocol_Ledgth 17

typedef struct {
    DFLT, FOC, CAL
} motor_status_type;

typedef union{
	struct{
		uint16_t head;
		
		uint8_t id:4;
		uint8_t status:3;
		uint8_t nc:1;
		
		uint16_t torque;
		uint16_t velocity;
		uint16_t angle;

		uint8_t  k_spd;
		uint8_t  k_pos;
		
		uint16_t crc16;
	};
	uint8_t data[Unitree_Protocol_Ledgth];
}Unitree_Data_Type;


typedef struct{
	USART_TypeDef *USARTx;
	
	Unitree_Data_Type Unitree_Data;
	
	uint8_t receiveBuf[Unitree_Protocol_Ledgth];
	
    void (*_Unitree_Init)(struct Unitree_Type *unitree, USART_TypeDef *USARTx,uint8_t id, uint8_t k_spd, uint8_t k_pos);
    void (*_Unitree_Bind)(struct Unitree_Type *unitree,USART_TypeDef *USARTx);

    void (*_Unitree_Send)(struct Unitree_Type *unitree);
    void (*_Unitree_Set_K)(struct Unitree_Type *unitree, uint8_t k_spd, uint8_t k_pos);

} Unitree_Type;

void _Unitree_Init(Unitree_Type *unitree, USART_TypeDef *USARTx,uint8_t id, uint8_t k_spd, uint8_t k_pos);
void _Unitree_Bind(Unitree_Type *unitree,USART_TypeDef *USARTx);


void _Unitree_Send(Unitree_Type *unitree) ;
void _Unitree_Set_K( Unitree_Type *unitree, uint8_t k_spd, uint8_t k_pos);

#endif

