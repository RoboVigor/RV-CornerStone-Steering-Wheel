#include "unitree.h"

void Unitree_Init(Unitree_Type *unitree, uint8_t id, uint8_t motor_id, uint8_t k_spd, uint8_t k_pos) {
    unitree->id_t->id         = id;
    unitree->id_t->motor_id   = motor_id;
    unitree->k_spd           = k_spd;
    unitree->k_pos           = k_pos;
    unitree->_Unitree_Set_Id  = _Unitree_Set_Id;
    unitree->_Unitree_Set_K = _Unitree_Set_K;
    unitree->_Unitree_Send    = _Unitree_Send;
}

void _Unitree_Set_Id( Unitree_Type *unitree, uint8_t id, uint8_t motor_id) {
    unitree->id_t->id       = id;
    unitree->id_t->motor_id = motor_id;
}

void _Unitree_Send(Unitree_Type *unitree, CAN_TypeDef *CANx, uint8_t *sendbuff) {
    uint16_t id = unitree->id_t->data >>= 3;
    Can_Send_Msg(CANx, id, sendbuff,8);
}

void _Unitree_Set_K( Unitree_Type *unitree, uint8_t k_spd, uint8_t k_pos){

}
