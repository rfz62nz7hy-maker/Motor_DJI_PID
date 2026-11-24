### 电机PID控制代码



#### 控制电机：大疆3508电机

#### BSP文件：SEU BSP文件

#### RTOS：FreeRTOS

#### 电机控制：角度 和 速度

##### &#x09;	SITUATION == 1 控制角度；SITUATION == 0 控制速度

#### 基本思路：

##### 接受处理CAN信号 —> 数据储存在 motor\_t Arm\_Motor\[7] 中 

##### —> PID函数调取结构体数据计算 —> 数据存在 QueueHandle\_t Queue\_CANSend  

##### —> 将列队中的数据处理发送CAN信号

#### flash.md 用于烧录




