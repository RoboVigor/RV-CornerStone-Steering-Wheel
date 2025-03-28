#include "unitree.h"

void Unitree_Init(Unitree_Type *unitree,Unitree_Bridge_Type bridge,uint8_t id, uint8_t status,float k_spd, float k_pos) {
    unitree->_Unitree_Send=_Unitree_Send;
    unitree->_Unitree_Set_K=_Unitree_Set_K;
	unitree->_Unitree_Set_Data=_Unitree_Set_Data;
	unitree->_Unitree_Unpack=_Unitree_Unpack;

    unitree->_Unitree_Set_K(unitree, k_spd, k_pos); 
	unitree->_Unitree_Set_Data(unitree, 0, 0, 0);
	
	unitree->Unitree_Data=&bridge.data_t;
	
	unitree->Unitree_Data->head[0]=0xfe;
	unitree->Unitree_Data->head[1]=0xee;
	
	unitree->id=id;
	
	unitree->status=status;
	
}

void _Unitree_Bind(Unitree_Bridge_Type bridge,USART_TypeDef *USARTx){
	bridge.USARTx=USARTx;

	if(USARTx==USART6){
		BSP_DMA_Init(USART6_Rx, bridge.receiveBuf, Unitree_Protocol_Receive_Length);
		BSP_DMA_Init(USART6_Tx, &bridge.data_t, Unitree_Protocol_Send_Length);
	}
}

void _Unitree_Send(Unitree_Type *unitree) {
	uint16_t dataCRC_16;
	
	unitree->Unitree_Data->head[0]=0xfe;
	unitree->Unitree_Data->head[1]=0xee;
	
	unitree->Unitree_Data->id=unitree->id;
	
	unitree->Unitree_Data->status=unitree->status;

	MIAO(unitree->k_pos,0.0f, 25.599f);
	MIAO(unitree->k_spd,0.0f, 25.599f);
	MIAO(unitree->torque,-127.99f, 127.99f);
	MIAO(unitree->velocity,-804.00f, 804.00f);
	MIAO(unitree->angle,-411774.0f, 411774.0f);
	
    unitree->Unitree_Data->k_pos=unitree->k_pos*1280;
    unitree->Unitree_Data->k_spd=unitree->k_spd	*1280;
    unitree->Unitree_Data->torque=unitree->torque* 256.0f;
    unitree->Unitree_Data->velocity=unitree->velocity/ 6.28318f * 256.0f;
    unitree->Unitree_Data->angle=unitree->angle/ 6.28318f * 32768.0f*6.33;	
	
    dataCRC_16= Get_CRC16_Check_Sum(unitree->Unitree_Data, Unitree_Protocol_Send_Length-Unitree_CRC16_Length);
	
	unitree->Unitree_Data->crc16=dataCRC_16;

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


void _Unitree_Receive(Unitree_Bridge_Type bridge,Unitree_Type unitree_1,Unitree_Type unitree_2) {
	uint16_t tmp,deviceid;

    // clear IDLE flag
    tmp = bridge.USARTx->DR;
    tmp = bridge.USARTx->SR;

	DMA_Disable(USART6_Rx);

	deviceid=bridge.receiveBuf[2]&0x0f;
	if(deviceid==0){
		memcpy(unitree_1.receiveBuf,bridge.receiveBuf,Unitree_Protocol_Receive_Length);
		unitree_1._Unitree_Unpack(&unitree_1);
	}
	else{
		memcpy(unitree_2.receiveBuf,bridge.receiveBuf,Unitree_Protocol_Receive_Length);
		unitree_2._Unitree_Unpack(&unitree_2);

	}


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

