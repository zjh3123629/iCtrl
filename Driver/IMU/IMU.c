#include "IMU.h"
#include "math.h"
#include "MPU6050.h"

S_INT16_XYZ ACC_AVG;		   //平均值滤波后的ACC  
BUF_INT16_XYZ  FILT_BUF;
S_FLOAT_XYZ Q_ANGLE;			//四元数计算出的角度
S_FLOAT_XYZ GYRO_I;			   //陀螺仪积分

void Prepare_Data(void)
{
	static uint8_t filter_cnt_acc=0;
	int32_t temp1=0,temp2=0,temp3=0;
	uint8_t i;

	FILT_BUF.ACC[filter_cnt_acc].X = MPU6050_ACC_LAST.X;//更新滑动窗口数组
	FILT_BUF.ACC[filter_cnt_acc].Y = MPU6050_ACC_LAST.Y;
	FILT_BUF.ACC[filter_cnt_acc].Z = MPU6050_ACC_LAST.Z;
	for(i=0;i<FILTER_ACC_NUM;i++)
	{
		temp1 += FILT_BUF.ACC[i].X;
		temp2 += FILT_BUF.ACC[i].Y;
		temp3 += FILT_BUF.ACC[i].Z;
	}
	ACC_AVG.X = temp1 / FILTER_ACC_NUM;
	ACC_AVG.Y = temp2 / FILTER_ACC_NUM;
	ACC_AVG.Z = temp3 / FILTER_ACC_NUM;
	filter_cnt_acc++;
	if(filter_cnt_acc==FILTER_ACC_NUM)	filter_cnt_acc=0;
	
}

/**************************实现函数********************************************
*函数原型:	   Q_rsqrt(float x)
*功　　能:	   快速计算 1/Sqrt(x) 	
输入参数： 要计算的值
输出参数： 结果
*******************************************************************************/
float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
 
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                      
	i  = 0x5f3759df - ( i >> 1 );               
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration （第一次牛顿迭代）
	return y;
}

float FL_ABS(float x)
{
   if(x < 0)  return -x;
	 else return x; 
}


#define Kp 2.5f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.002f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.00125f                   // half the sample period采样周期的一半


/**************************************
 * 函数名：Get_Attitude
 * 描述  ：得到当前姿态
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 *************************************/
void Get_Attitude(void)
{
	IMUupdate(MPU6050_GYRO_Gr.X,
						MPU6050_GYRO_Gr.Y,
						MPU6050_GYRO_Gr.Z,
						ACC_AVG.X,ACC_AVG.Y,ACC_AVG.Z);	//*0.0174转成弧度
}

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
  float norm;
//	int16_t Xr,Yr;
//  float hx, hy, hz, bx, bz;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;

  // 先把这些用得到的值算好
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
//  float q0q3 = q0*q3;//
  float q1q1 = q1*q1;
//  float q1q2 = q1*q2;//
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
	
	if(ax*ay*az==0)
 		return;
		
  norm = Q_rsqrt(ax*ax + ay*ay + az*az);       //acc数据归一化
  ax = ax *norm;
  ay = ay * norm;
  az = az * norm;


  // estimated direction of gravity and flux (v and w)              估计重力方向和流量/变迁
  vx = 2*(q1q3 - q0q2);												//四元素中xyz的表示
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                           					 //向量外积在相减得到差分就是误差
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki;								  //对误差进行积分
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;
// adjusted gyroscope measurements
	if(FL_ABS(ay) < 0.9 && FL_ABS(az-1) < 0.9)
	{
    gx = gx + Kp*ex + exInt;					   							//将误差PI后补偿到陀螺仪，即补偿零点漂移
  }
	else
	{
    gx = gx; 
  }
	if(FL_ABS(ax) < 0.9 && FL_ABS(az-1) < 0.9)
	{
    gy = gy + Kp*ey + eyInt;				   							
  }
	else
	{
    gy = gy; 
  }
	if(FL_ABS(ax)< 0.9 && FL_ABS(ay) < 0.9)
	{
     gz = gz + Kp*ez + ezInt;					   							
  }
	else
	{
    gz = gz; 
  }
		
  //gz = gz + Kp*ez + ezInt;				   							//这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减
	
  // integrate quaternion rate and normalise						   //四元素的微分方程
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // normalise quaternion
  norm = Q_rsqrt(q0q0 + q1q1 + q2q2 + q3q3);
  q0 = q0 * norm;
  q1 = q1 * norm;
  q2 = q2 * norm;
  q3 = q3 * norm;

  Q_ANGLE.X = atan2(2*q2q3 + 2*q0q1, -2*q1q1 - 2*q2q2 + 1) * RtA; // roll
	Q_ANGLE.Y = asin(-2*q1q3 + 2*q0q2) * RtA; // pitch
}














