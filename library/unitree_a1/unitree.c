#include "unitree.h"

void Unitree_Init(Unitree_Type *unitree, USART_TypeDef *USARTx,uint8_t id, uint16_t k_spd, uint16_t k_pos) {
	
    unitree->_Unitree_Bind=_Unitree_Bind;
    unitree->_Unitree_Send=_Unitree_Send;
    unitree->_Unitree_Set_K=_Unitree_Set_K;
	unitree->_Unitree_Set_Data=_Unitree_Set_Data;

    unitree->_Unitree_Bind(unitree,USARTx);
    unitree->_Unitree_Set_K(unitree, k_spd, k_pos); 
	
	unitree->Unitree_Data.head[0]=0xfe;
	unitree->Unitree_Data.head[1]=0xee;
	
	unitree->Unitree_Data.id=id;
	
	unitree->Unitree_Data.status=1;
	
	if(unitree->USARTx==USART6){
		BSP_DMA_Init(USART6_Rx, unitree->receiveBuf, Unitree_Protocol_Length);
		BSP_DMA_Init(USART6_Tx, &unitree->Unitree_Data, Unitree_Protocol_Length);
	}
}

void _Unitree_Bind(Unitree_Type *unitree,USART_TypeDef *USARTx){
    unitree->USARTx=USARTx;
}


void _Unitree_Send(Unitree_Type *unitree) {
    uint16_t dataCRC16= Get_CRC16_Check_Sum(&unitree->Unitree_Data, Unitree_Protocol_Length-Unitree_CRC16_Length);
	
	unitree->Unitree_Data.crc16=dataCRC16;

    DMA_Disable(USART6_Tx);
    DMA_Enable(USART6_Tx, Unitree_Protocol_Length);
	
}

void _Unitree_Set_K( Unitree_Type *unitree, uint16_t k_spd, uint16_t k_pos){
    unitree->Unitree_Data.k_spd = k_spd;
    unitree->Unitree_Data.k_pos = k_pos;

}

void _Unitree_Set_Data(Unitree_Type *unitree, uint16_t torque, uint16_t velocity,uint32_t angle){
	unitree->Unitree_Data.torque=torque;
	unitree->Unitree_Data.velocity=velocity;
	unitree->Unitree_Data.angle=angle;


}

