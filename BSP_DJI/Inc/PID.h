#ifndef __PID_H__
#define __PID_H__
/**Include Header Files**/
#include <stdint.h>
#include <math.h>  
/**Enum Definition**/
typedef enum
{
    PID_POSITION = 0,
    PID_DELTA
} PID_MODE;


/**Struct Definition**/
typedef struct
{
    /* data */
	PID_MODE pidmode;

	float error[3]; //error[0]�������һ�����  error[1]������һ����� error[2]�������ϴ����
	float sum_error;
	float Dbuf[3];  //΢���� 0���� 1��һ�� 2���ϴ�

	float kp;
	float ki;
	float kd; // PIDϵ��

	float Pout;
	float Iout;
	float Dout; //���� ���� ΢�������
	float Out; //�������

	float dst;  //Ŀ��ֵ
	float real; //ʵ��ֵ

	float MaxOut; //PID������(0Ϊ������)
	float MaxIout; //������������(0Ϊ������)
	
	float MaxBlockIount;//��ת���ʱ�����б�Ļ�����������
	
	uint16_t DeadZone; //������С

} pid_type;


/**Function Delaration**/
uint8_t PID_Parameter_Init(pid_type *PID, PID_MODE mode, const float Pidpara[], uint32_t MaxOut, uint32_t MaxIout, uint16_t DeadZone);
uint8_t PID_Parameter_Update(pid_type * pid,const float Pidpara[]);
uint8_t PID_clear(pid_type *pid);
float Pid_cal(pid_type *PID, float dst, float real);

#endif
