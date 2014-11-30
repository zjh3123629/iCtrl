#ifndef __IMU_H
#define	__IMU_H
#include "stm32f10x.h"
#include "include.h"

#define RtA 		57.324841				
#define AtR    	0.0174533				
#define Acc_G 	0.0011963				
#define Gyro_G 	0.03051756				
#define Gyro_Gr	0.0005326

#define FILTER_ACC_NUM 10

typedef struct{
				float X;
				float Y;
				float Z;}S_FLOAT_XYZ;

typedef struct{
				S_FLOAT_XYZ ACC[FILTER_ACC_NUM];}BUF_INT16_XYZ;


extern S_INT16_XYZ ACC_AVG;

void Prepare_Data(void);
void Get_Attitude(void);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);


extern S_FLOAT_XYZ Q_ANGLE;



#endif













