#ifndef __MPU6050_H_
#define __MPU6050_H_
#include<stdint.h>
#include "stm32f10x.h"


//****************************************
// 定义MPU6050内部地址
//****************************************

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIGL			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
#define MPU6050_ADDRESS 0xD0    //IIC写入时的地址字节数据，+1为读取

typedef struct{
				int16_t X;
				int16_t Y;
				int16_t Z;}S_INT16_XYZ;

typedef struct{
				float X;
				float Y;
				float Z;}FLOAT_XYZ;


extern S_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//最新一次读取值
extern FLOAT_XYZ      MPU6050_GYRO_Gr; 
extern S_INT16_XYZ		MPU6050_GYRO_LAST_OLD;     
extern S_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//零漂
extern float   Acc_angle,Gry_vivi;	

u8 InitMPU6050(void);					    //初始化MPU6050
void MPU6050_Read(void);
void MPU6050_Dataanl(void);
void UART1_ReportIMU(void);
void Gyro_OFFEST(void);

#endif // __MPU6050_H__
