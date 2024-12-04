#include "Driver_Motor.h"
#include "macro.h"

#include "Driver_Encoder.h"

void Encoder_Can_Send(CAN_TypeDef *CANx,uint8_t id, uint8_t command,uint8_t data){
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
    CanTxData.DLC = 0x08;


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

void Encoder_Set_Bitrate(uint8_t id){
    // GPIO 初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    // CAN2
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); // 使能 GPIOB 时钟
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;          // 复用模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         // 推挽输出
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13; // CAN TX, RX (PB5: TX, PB6: RX)
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;          // 上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     // 速度
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2); // 设置复用
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2); // 设置复用

    // CAN 初始化
    CAN_InitTypeDef CAN_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE); // 使能 CAN2 时钟

    // 配置 CAN
    CAN_InitStructure.CAN_ABOM = ENABLE;                  // 自动离线管理
    CAN_InitStructure.CAN_AWUM = DISABLE;                  // 自动唤醒
    CAN_InitStructure.CAN_BS1 = CAN_BS1_13tq;              // 选择时间段 1
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;               // 选择时间段 2
    CAN_InitStructure.CAN_Mode      = CAN_Mode_Normal;
    CAN_InitStructure.CAN_NART      = DISABLE;
    CAN_InitStructure.CAN_Prescaler = 3;
    CAN_InitStructure.CAN_RFLM      = DISABLE;
    CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;
    CAN_InitStructure.CAN_TTCM      = DISABLE;
    CAN_InitStructure.CAN_TXFP      = DISABLE;
    CAN_Init(CAN2, &CAN_InitStructure);

    
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterNumber         = 14;
    CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit;
    CAN_FilterInit(&CAN_FilterInitStructure);

    // NVIC
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel                   = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
	
	//while(1)
	Encoder_Can_Send(CAN2, id, 0x03, BR_1M);
}
void Encoder_Update(Encoder_Type *encoder, uint8_t data[8])
{

}