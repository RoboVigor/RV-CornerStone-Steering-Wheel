#include "unitree.h"

void Unitree_Init(Unitree_Type *unitree, USART_TypeDef *USARTx,uint8_t id, uint8_t status,float k_spd, float k_pos) {
	
    unitree->_Unitree_Bind=_Unitree_Bind;
    unitree->_Unitree_Send=_Unitree_Send;
    unitree->_Unitree_Set_K=_Unitree_Set_K;
	unitree->_Unitree_Set_Data=_Unitree_Set_Data;

    unitree->_Unitree_Bind(unitree,USARTx);
    unitree->_Unitree_Set_K(unitree, k_spd, k_pos); 
	
	unitree->Unitree_Data.head[0]=0xfe;
	unitree->Unitree_Data.head[1]=0xee;
	
	unitree->Unitree_Data.id=id;
	
	unitree->Unitree_Data.status=status;
	
	if(unitree->USARTx==USART6){
		BSP_DMA_Init(USART6_Rx, unitree->receiveBuf, Unitree_Protocol_Length);
		BSP_DMA_Init(USART6_Tx, &unitree->Unitree_Data, Unitree_Protocol_Length);
	}
}

void _Unitree_Bind(Unitree_Type *unitree,USART_TypeDef *USARTx){
    unitree->USARTx=USARTx;
}


void _Unitree_Send(Unitree_Type *unitree) {

	MIAO(unitree->k_pos,0.0f, 25.599f);
	MIAO(unitree->k_spd,0.0f, 25.599f);
	MIAO(unitree->torque,-127.99f, 127.99f);
	MIAO(unitree->velocity,-804.00f, 804.00f);
	MIAO(unitree->angle,-411774.0f, 411774.0f);

    unitree->Unitree_Data.k_pos=unitree->k_pos/ 25.6f * 32768.0f;
    unitree->Unitree_Data.k_spd=unitree->k_spd/ 25.6f * 32768.0f;
    unitree->Unitree_Data.torque=unitree->torque* 256.0f;
    unitree->Unitree_Data.velocity=unitree->velocity/ 6.28318f * 256.0f;
    unitree->Unitree_Data.angle=unitree->angle/ 6.28318f * 256.0f;	
	
    uint16_t dataCRC16= Get_CRC16_Check_Sum(&unitree->Unitree_Data, Unitree_Protocol_Length-Unitree_CRC16_Length);
	
	unitree->Unitree_Data.crc16=dataCRC16;

    DMA_Disable(USART6_Tx);
    DMA_Enable(USART6_Tx, Unitree_Protocol_Length);
	
}

void _Unitree_Set_K( Unitree_Type *unitree, float k_spd, float k_pos){
    unitree->k_spd = k_spd;
    unitree->k_pos = k_pos;

}

void _Unitree_Set_Data(Unitree_Type *unitree, float torque, float velocity,float angle){
	unitree->torque=torque;
	unitree->velocity=velocity;
	unitree->angle=angle;


}

