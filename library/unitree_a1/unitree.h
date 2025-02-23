#ifndef __DRIVER_Encoder_H
#define __DRIVER_Encoder_H

#include "stm32f4xx.h"
#include "Driver_CAN.h"

typedef enum { Unitree_R, Unitree_W } status_type;

typedef enum { Unitree_Mode1 = 10, Unitree_Send_k, Unitree_Read_k, Unitree_Mode2 } mode_type;

typedef struct {
    DFLT, FOC, CAL
} motor_status_type;

typedef union {
    struct {
        uint32_t id : 2;
        uint32_t status : 3;
        uint32_t mode : 8;
        uint32_t motor_id : 4;
        uint32_t motor_status : 3;
        uint32_t nc : 12;
    };
    uint32_t data;
} unitree_id_type;

typedef union {
    struct {
        uint16_t angle;
        uint8_t  velocity;
        uint8_t torque
    };
    uint8_t data[4];
} unitree_data_type;

typedef struct {
    unitree_id_type   id_t;
    unitree_data_type data_t;
    uint8_t           k_spd;
    uint8_t           k_pos;

    void (*_Unitree_Init)(struct Unitree_Type *unitree, uint8_t id, uint8_t motor_id, uint8_t k_spd, uint8_t k_pos);

    void (*_Unitree_Set_Id)(struct Unitree_Type *unitree, uint8_t id, uint8_t motor_id);
    void (*_Unitree_Send)(struct Unitree_Type *unitree, uint8 *data);
    void (*_Unitree_Set_K)(struct Unitree_Type *unitree, uint8_t k_spd, uint8_t k_pos);

} Unitree_Type;

void _Unitree_Init(struct Unitree_Type *unitree, uint8_t id, uint8_t motor_id, uint8_t k_spd, uint8_t k_pos);

void _Unitree_Set_Id(struct Unitree_Type *unitree, uint8_t id, uint8_t motor_id);

void _Unitree_Send(struct Unitree_Type *unitree, uint8 *data);
void _Unitree_Set_K(struct Unitree_Type *unitree, uint8_t k_spd, uint8_t k_pos);

#endif