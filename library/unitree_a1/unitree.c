#include "unitree.h"

void Unitree_Init(Unitree_Type *unitree, uint8_t id, uint8_t motor_id, uint8_t k_spd, uint8_t k_pos) {
    unitree->id_t.id         = id;
    unitree->id_t.motor_id   = motor_id;
    unitree->k_spd           = k_spd;
    unitree->k_pos           = k_pos;
    unitree->Unitree_Set_Id  = Unitree_Set_Id;
    unitree->_Unitree_Send_K = Unitree_Send_k;
    unitree->Unitree_Send    = Unitree_Send;
}

void Unitree_Send(struct Unitree_Type *unitree, CAN_TypeDef *CANx, uint8_t *sendbuff) {
    unitree->uint16_t id = unitree->id_t.data >>= 3;
    Can_Send_Msg(CANx, id, sendbuff);
}
