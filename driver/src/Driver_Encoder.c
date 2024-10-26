#include "Driver_Motor.h"
#include "macro.h"

#include "Driver_Encoder.h"

void Encoder_Can_Send(CAN_TypeDef *CANx,uint8_t id, uint8_t command,uint8_t *data,uint8_t length){
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
    CanTxData.DLC = 2+length;


    CanTxData.Data[0]=CanTxData.DLC;
    CanTxData.Data[1]=id;
    CanTxData.Data[2]=command;
    for(uint8_t i=0;i<length;i++){
        CanTxData.Data[3+i]=data[i];
    }

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
