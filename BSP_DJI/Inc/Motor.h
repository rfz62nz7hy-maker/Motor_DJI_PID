#ifndef __MOTOR_H__
#define __MOTOR_H__


/**Include Header Files**/
#include "sysconfig.h"
#include "PID.h"

#define MOTOR_MAX_BLOCKTIME	(600)
#define ARM_MAX_BLOCKTIME	(1000)
#define MNUM 7

#define MOTOR_ERR_UNDER_VOLTAGE        (1u << 0)   // æ¬ åŽ‹
#define MOTOR_ERR_OVER_VOLTAGE         (1u << 1)   // è¿‡åŽ‹
#define MOTOR_ERR_OVER_CURRENT         (1u << 2)   // è¿‡æµ / çŸ­è·¯
#define MOTOR_ERR_DRIVER_OVER_TEMP     (1u << 3)   // é©±åŠ¨ / MOS è¿‡æ¸©
#define MOTOR_ERR_MOTOR_OVER_TEMP      (1u << 4)   // ç”µæœº / çº¿åœˆè¿‡æ¸©
#define MOTOR_ERR_COMM_LOST            (1u << 5)   // é€šè®¯ / è¾“å…¥ä¿¡å·ä¸¢å¤±
#define MOTOR_ERR_OVERLOAD_STALL       (1u << 6)   // å µè½¬ / è¿‡è½½
#define MOTOR_ERR_SENSOR_CALIB_FAULT   (1u << 7)   // ç¼–ç å™¨ / ä¼ æ„Ÿå™¨ / æ ‡å®š / ç¡¬ä»¶å¼‚å¸¸

#define RS_ABNORMAL_STATUS_STDID        0x0FD
#define RS_FAULT_MOTOR_OVER_TEMP        (1UL << 0)
#define RS_FAULT_DRIVER_CHIP            (1UL << 1)
#define RS_FAULT_UNDER_VOLTAGE          (1UL << 2)
#define RS_FAULT_OVER_VOLTAGE           (1UL << 3)
#define RS_FAULT_PHASE_B_OVERCURRENT    (1UL << 4)
#define RS_FAULT_PHASE_C_OVERCURRENT    (1UL << 5)
#define RS_FAULT_ENCODER_UNCALIBRATED   (1UL << 7)
#define RS_FAULT_HARDWARE_IDENTIFY      (1UL << 8)
#define RS_FAULT_POSITION_INIT          (1UL << 9)
#define RS_FAULT_STALL_OVERLOAD         (1UL << 14)
#define RS_FAULT_PHASE_A_OVERCURRENT    (1UL << 16)


/**Enum Definition**/
typedef enum
{
    noblock = 0,
    isblock,
    BlockState_Counter
}BlockState_t;

typedef enum
{
    MOTOR_NONE,
	
		MOTOR_2006,
		MOTOR_3508,
		MOTOR_6020,
		
		MOTOR_DM4310,
		MOTOR_DM4340,
		MOTOR_DM6248,
		MOTOR_DM8009,
		MOTOR_DM10010,
		
	  MOTOR_RS03,
	  MOTOR_RS04,
	  
	  MOTOR_MG8016,
	  MOTOR_MG4005I10,
    MOTOR_TYPE_COUNTER
}motor_type;

typedef enum {
    FORWARD = 1,
    REVERSE = 0
} MotorDirection_t;

typedef enum {
    OPEN = 1,
    CLOSE = 0
} MotorState_t;

typedef enum {
    ONLINE = 1,
    OFFLINE = 0
} MotorOnline_t;
/**Struct Definition**/
typedef struct                            
{         
	uint8_t   Data[66];            
}RS485Send_Type;

typedef struct
{
	uint32_t    FrameCounter;           //Ö¡ÂÊ¼ÆÊý
	int16_t    	RealSpeed;       			  //rpm
	int16_t    	Current;
	float test;
	int16_t			Voltage;
	uint16_t    Mechanical_Angle[2];    //µç»ú·µ»Ø»úÐµ½Ç [0]:µ±Ç° [1]:ÉÏÒ»´Î
	
	int16_t Temperature;

}Motor_Feedback_Data;


typedef struct
{
	/* data */
	motor_type  type; 							//Motor_type
	uint32_t		id;									//Motor_ID
	uint8_t			Error_id;
	uint8_t			Mode;
	uint8_t     CAN_ID;
	
	MotorOnline_t     Online;
	MotorState_t     State;
	MotorDirection_t  Dir;
	
	pid_type*   PositionPID;
	pid_type*   SpeedPID;
	Motor_Feedback_Data FeedbackData;
	
	uint8_t     Reductionratio; //¼õËÙ±È
	int16_t     Oricirnum;          //¼õËÙÇ°×ª×Ó×ª¹ýÈ¦Êý
	int16_t     Realcirnum;         //Êµ¼Ê×ª¹ýÈ¦Êý
	int16_t     Targetcirnum;       //Ä¿±êÈ¦Êý

	float       Realrotationrate;   //Êµ¼Ê×ªËÙ  rps
	float       Targetrotationrate; //Ä¿±ê×ªËÙ  rps

	float   InitAngle;
	float   RealAngle;          //Êµ¼Ê½Ç¶È  µ¥Î»¡ã
	float   TargetAngle;        //Ä¿±ê½Ç¶È  µ¥Î»¡ã
	int16_t TargetMechAngle;		//Ä¿±ê»úÐµ½Ç¶È

	int16_t     	Out;                //PID¼ÆËãºóÊä³öÖµ
	
	uint8_t First_Frame;
	
	float Kp;
	float Ki;
	float Kd;
	float SumError;
	float MAX_angle;
	float MIN_angle;
		
}motor_t;

extern motor_t Arm_Motor[7];
/**Function Delaration**/
void MotortypeRegister(motor_t *mot,MotorDirection_t dir, motor_type type, pid_type *pospid, pid_type *speedpid, uint8_t Reductionratio, uint32_t Motor_id,uint8_t Can_id,float Kp,float Ki,float Kd,float MAX_angle,float MIN_angle);
uint8_t DM_ErrorCode_To_MotorErrorMask(uint8_t dm_err);
uint8_t LK_ErrorState_To_MotorErrorMask(uint8_t lk_errorState);
uint8_t RS_FaultValue_To_MotorErrorMask(uint32_t rs_fault);
uint8_t Motor_IsError(motor_t *mot);
uint8_t ArmMotor_ErrorStatePack_Get(void);
void ArmMotor_ErrorMaskArray_Get(uint8_t error_mask_array[7]);





#endif
