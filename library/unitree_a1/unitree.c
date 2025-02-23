#include "unitree.h"

void Unitree_Init(Unitree_Type *unitree ,uint8_t id , uint8_t motor_id,uint8_t k_spd,uint8_t k_pos){
    unitree->id_t.id = id;
    unitree->id_t.motor_id = motor_id;
    unitree->k_spd = k_spd;
    unitree->k_pos = k_pos;

}

