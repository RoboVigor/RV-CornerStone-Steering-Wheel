#include "unitree.h"

void Unitree_Init(Unitree_Type *unitree, CAN_TypeDef *CANx,uint8_t id, uint8_t motor_id, uint8_t k_spd, uint8_t k_pos) {
	
	unitree_id_type id_instance;
	unitree_data_type data_instance;
	unitree->id_t = &id_instance;
	unitree->data_t = &data_instance;
	
    unitree->_Unitree_Bind=_Unitree_Bind;
    unitree->_Unitree_Set_Id=_Unitree_Set_Id;
    unitree->_Unitree_Send=_Unitree_Send;
    unitree->_Unitree_Set_K=_Unitree_Set_K;
    unitree->_Unitree_Send_Data=_Unitree_Send_Data;

    unitree->_Unitree_Bind(unitree,CANx);
    unitree->_Unitree_Set_Id(unitree, id, motor_id);   
    unitree->_Unitree_Set_K(unitree, k_spd, k_pos); 
}

void _Unitree_Bind(Unitree_Type *unitree,CAN_TypeDef *CANx){
    unitree->CANx=CANx;
}

void _Unitree_Set_Id( Unitree_Type *unitree, uint8_t id, uint8_t motor_id) {
    unitree->id_t->id       = id;
    unitree->id_t->motor_id = motor_id;
}

void _Unitree_Send(Unitree_Type *unitree, uint8_t *sendbuff,uint8_t dlc) {
    unitree->id_t->status=Unitree_W;
    uint16_t id = unitree->id_t->data >>= 3;
    Can_Send_Msg(unitree->CANx, id, sendbuff,dlc);
}

void _Unitree_Set_K( Unitree_Type *unitree, uint8_t k_spd, uint8_t k_pos){
    unitree->k_spd = k_spd;
    unitree->k_pos = k_pos;
    unitree->id_t->mode=Unitree_Send_k;

    uint8_t data[4];
    data[0] = k_spd;
    data[1] = k_spd>>8;
    data[2] = k_pos;
    data[3] = k_pos>>8;
    unitree->_Unitree_Send(unitree,data,4);
}

void _Unitree_Send_Data(Unitree_Type *unitree,uint8_t angle,uint8_t velocity,uint8_t torque) {
    unitree->id_t->mode=Unitree_Mode1;
}
