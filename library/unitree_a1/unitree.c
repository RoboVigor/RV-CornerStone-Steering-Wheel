#include "unitree.h"

void Unitree_Init(Unitree_Type *unitree, USART_TypeDef *USARTx,uint8_t id, uint8_t k_spd, uint8_t k_pos) {
	
    unitree->_Unitree_Bind=_Unitree_Bind;
    unitree->_Unitree_Send=_Unitree_Send;
    unitree->_Unitree_Set_K=_Unitree_Set_K;

    unitree->_Unitree_Bind(unitree,USARTx);
    unitree->_Unitree_Set_K(unitree, k_spd, k_pos); 
	
	unitree->Unitree_Data.head=0xfdee;
	
	unitree->Unitree_Data.id=id;
	
	if(unitree->USARTx==USART6){
		BSP_DMA_Init(USART6_Rx, unitree->receiveBuf, Unitree_Protocol_Ledgth);
		BSP_DMA_Init(USART6_Tx, &unitree->Unitree_Data, Unitree_Protocol_Ledgth);
	}
}

void _Unitree_Bind(Unitree_Type *unitree,USART_TypeDef *USARTx){
    unitree->USARTx=USARTx;
}


void _Unitree_Send(Unitree_Type *unitree) {
	
    DMA_Disable(USART6_Tx);
    DMA_Enable(USART6_Tx, Unitree_Protocol_Ledgth);
	
	
}

void _Unitree_Set_K( Unitree_Type *unitree, uint8_t k_spd, uint8_t k_pos){
    unitree->Unitree_Data.k_spd = k_spd;
    unitree->Unitree_Data.k_pos = k_pos;

}

