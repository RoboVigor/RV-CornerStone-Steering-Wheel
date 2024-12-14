#include "Driver_Motor.h"
#include "macro.h"
#include "Driver_Encoder.h"

void Encoder_Can_Send(CAN_TypeDef *CANx, uint8_t id, uint8_t command, uint32_t *data, uint8_t length) {
    uint8_t  data_t[8];
    uint8_t  dlc;
    uint8_t *data_l = (uint8_t *) data;
    int      i;
	
    dlc = length + 3;
	
    data_t[0] = dlc;
    data[1]   = id;
    data[2]   = command;
    for (i = 3; i < dlc; i++) {
        data[i] = data_l[3 + i - 6];
	}
	
    Can_Send_Msg(CANx, id, data_t, dlc);
	
	return;
	
}

void Encoder_Set_Bitrate(uint8_t id){
}
void Encoder_Update(Encoder_Type *encoder, uint8_t *data)
{
	uint32_t position=0;
	int i;
	
	for(i=6;i>=3;i++){
		position<<=8;
		position|=data[i];
	}
	
	encoder->position=position;
	
	return;

}