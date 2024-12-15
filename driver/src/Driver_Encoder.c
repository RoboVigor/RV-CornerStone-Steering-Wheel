#include "Driver_Motor.h"
#include "macro.h"
#include "Driver_Encoder.h"

void Encoder_Can_Send(CAN_TypeDef *CANx, uint8_t id, uint8_t command, uint32_t data) {
    CanTxMsg CanTxData;
    uint8_t  mailBox;

    if (id <= 0x7ff) {
        CanTxData.StdId = id;
        CanTxData.IDE   = CAN_Id_Standard;
    } else {
        CanTxData.ExtId = id;
        CanTxData.IDE   = CAN_Id_Extended;
    }

    CanTxData.RTR = CAN_RTR_Data;
    CanTxData.DLC = 0x04;


    CanTxData.Data[0]=CanTxData.DLC;
    CanTxData.Data[1]=id;
    CanTxData.Data[2]=command;
    CanTxData.Data[3]=data;
	
    // do {
    //     if (CANx->ESR) {
    //         // 可以在这里输出ESR来查看CAN错误
    //         CANx->MCR |= 0x02;
    //         CANx->MCR &= 0xFD;
    //     }
    // } while (!(CANx->TSR & 0x1C000000));

    mailBox = CAN_Transmit(CANx, &CanTxData);

    while (CAN_TransmitStatus(CANx, mailBox) != CAN_TxStatus_Ok) {
    }
}

void Encoder_Get_Data(CAN_TypeDef *CANx,uint8_t id){
	uint32_t data=0x01;
	
	//Encoder_Can_Send(CANx,id,0x01,&data,1);

}

void Encoder_Set_Bitrate(uint8_t id) {
}
void Encoder_Update(Encoder_Type *encoder, uint8_t data[8]) {
    uint32_t position = 0;
    int      i;
	
    for (i = 6; i >= 3; i--) {
        position *= 256;
        position += data[i];
    }
	
	//while(1);

    encoder->position = position;

    encoder->angle = position * 365 / 1024;

    return;
}