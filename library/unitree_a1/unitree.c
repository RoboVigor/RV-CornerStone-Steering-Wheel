#include "unitree.h"

void Unitree_Init(Unitree_Type *unitree, USART_TypeDef *USARTx,uint8_t id, uint8_t status,float k_spd, float k_pos) {
	
    unitree->_Unitree_Bind=_Unitree_Bind;
    unitree->_Unitree_Send=_Unitree_Send;
    unitree->_Unitree_Set_K=_Unitree_Set_K;
	unitree->_Unitree_Set_Data=_Unitree_Set_Data;
	unitree->_Unitree_Receive=_Unitree_Receive;
	unitree->_Unitree_Unpack=_Unitree_Unpack;

    unitree->_Unitree_Bind(unitree,USARTx);
    unitree->_Unitree_Set_K(unitree, k_spd, k_pos); 
	unitree->_Unitree_Set_Data(unitree, 0, 0, 0);
	
	unitree->Unitree_Data.head[0]=0xfe;
	unitree->Unitree_Data.head[1]=0xee;
	
	unitree->Unitree_Data.id=id;
	
	unitree->Unitree_Data.status=status;
	
	if(unitree->USARTx==USART6){
		BSP_DMA_Init(USART6_Rx, unitree->receiveBuf, Unitree_Protocol_Receive_Length);
		BSP_DMA_Init(USART6_Tx, &unitree->Unitree_Data, Unitree_Protocol_Send_Length);
	}
}

void _Unitree_Bind(Unitree_Type *unitree,USART_TypeDef *USARTx){
    unitree->USARTx=USARTx;
}

void _Unitree_Send(Unitree_Type *unitree) {
	uint16_t dataCRC_16;

	MIAO(unitree->k_pos,0.0f, 25.599f);
	MIAO(unitree->k_spd,0.0f, 25.599f);
	MIAO(unitree->torque,-127.99f, 127.99f);
	MIAO(unitree->velocity,-804.00f, 804.00f);
	MIAO(unitree->angle,-411774.0f, 411774.0f);
	
    unitree->Unitree_Data.k_pos=unitree->k_pos*1280;
    unitree->Unitree_Data.k_spd=unitree->k_spd	*1280;
    unitree->Unitree_Data.torque=unitree->torque* 256.0f;
    unitree->Unitree_Data.velocity=unitree->velocity/ 6.28318f * 256.0f;
    unitree->Unitree_Data.angle=unitree->angle/ 6.28318f * 32768.0f*6.33;	
	
    dataCRC_16= Get_CRC16_Check_Sum(&unitree->Unitree_Data, Unitree_Protocol_Send_Length-Unitree_CRC16_Length);
	
	unitree->Unitree_Data.crc16=dataCRC_16;

    DMA_Disable(USART6_Tx);
    DMA_Enable(USART6_Tx, Unitree_Protocol_Send_Length);
	
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

void _Unitree_Receive(Unitree_Type *unitree) {
	uint16_t tmp;

    // clear IDLE flag
    tmp = unitree->USARTx->DR;
    tmp = unitree->USARTx->SR;

	DMA_Disable(USART6_Rx);

	unitree->_Unitree_Unpack(unitree);

    DMA_Enable(USART6_Rx, Unitree_Protocol_Receive_Length);

}

void _Unitree_Unpack(Unitree_Type *unitree) {
	int16_t t,v;
	int32_t a;
	uint16_t CRC_16;
	unsigned int CRC_16_Check_Sum_State;

	CRC_16=unitree->receiveBuf[14]|unitree->receiveBuf[15]<<8;

	CRC_16_Check_Sum_State=Verify_CRC16_Check_Sum(unitree->receiveBuf,Unitree_Protocol_Receive_Length);

	if(!CRC_16_Check_Sum_State)
		return;

	t=unitree->receiveBuf[3]|unitree->receiveBuf[4]<<8;
	unitree->torque_r=t/256;
	v=unitree->receiveBuf[5]|unitree->receiveBuf[6]<<8;
	unitree->velocity_r=v/256*6.28;
	a=unitree->receiveBuf[7]|unitree->receiveBuf[8]<<8|unitree->receiveBuf[9]<<16|unitree->receiveBuf[10]<<24;
	unitree->angle_r=a/32768.0*6.28/6.33;

}

