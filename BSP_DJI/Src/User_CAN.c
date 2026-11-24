/**Include Header Files**/
#include "User_CAN.h"
#include "DJI_Motor.h"   // 引用大疆电机解析函数

// 🌟 告诉编译器，hfdcan1 在其他文件（main.c/fdcan.c）里已经定义过了
extern FDCAN_HandleTypeDef hfdcan1;

// 🌟 声明外部电机数组。请确保你在 main.c 或全局定义了这个名字！
extern motor_t Arm_Motor[]; 

void CAN_Init(FDCAN_HandleTypeDef *hfdcan)
{
  // 如果传进来的不是 CAN1，直接退出
  if (hfdcan != &hfdcan1) {
    return;
  }

  CAN_Fliter_Init(hfdcan);
  HAL_FDCAN_Start(hfdcan);
  HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}

void CAN_Fliter_Init(FDCAN_HandleTypeDef *hfdcan)
{
  FDCAN_FilterTypeDef sFilterConfig;

  if (hfdcan != &hfdcan1) {
    return;
  }

  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x00; 
  sFilterConfig.FilterID2 = 0x00; 
  
  HAL_FDCAN_ConfigFilter(hfdcan, &sFilterConfig);
  HAL_FDCAN_ConfigGlobalFilter(hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);
  HAL_FDCAN_ConfigFifoWatermark(hfdcan, FDCAN_CFG_RX_FIFO0, 1);
}

/**
 * @brief  FIFO0的CAN接收回调 (只处理 CAN1)
 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t aData[8];

  (void)RxFifo0ITs;
  
  if (hfdcan != &hfdcan1) return;

  if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, aData) == HAL_OK)
  {
    Can1Received_infoHandle(RxHeader.Identifier, aData);
  }
}

/**
 * @brief  处理 CAN1 接收到的数据 (专注大疆 3508)
 */
void Can1Received_infoHandle(uint32_t stdid, uint8_t adata[])
{
  if (adata == NULL) return;

  // 🌟 大疆 3508 反馈报文 ID 范围：0x201 ~ 0x208
  if (stdid >= 0x201 && stdid <= 0x208) 
  {
      uint8_t motor_idx = stdid - 0x201; // 计算数组下标
      DJIMotor_ParaHandle(&Arm_Motor[motor_idx], adata); // 调用解析
      return;
  }
}

// 为了满足头文件里的声明，留个空函数在这里，防止链接报错
void Can2Received_infoHandle(uint32_t stdid, uint8_t adata[])
{
  (void)stdid; 
  (void)adata;
}

void Can3Received_infoHandle(uint32_t stdid, uint8_t adata[])
{
  (void)stdid;
  (void)adata;
}