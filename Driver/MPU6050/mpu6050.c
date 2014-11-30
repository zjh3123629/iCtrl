#include "include.h"


#define UART_Put_Char(x)   UART1_Put_Char(x)  

#define ACC_GYO_OFFSET  

S_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//最新一次读取值
FLOAT_XYZ     MPU6050_GYRO_Gr;  
S_INT16_XYZ		MPU6050_GYRO_LAST_OLD;
S_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//Zero drift

u8				    mpu6050_buffer[14];					//iic读取后存放数据 	
u8						ACC_OFFSET_OK = 0;


//**************************************
//初始化MPU6050
//**************************************
u8 InitMPU6050(void)
{
	u8 date;
	date = Single_Write(MPU6050_ADDRESS, PWR_MGMT_1, 0x00);	//解除休眠状态
	date += Single_Write(MPU6050_ADDRESS, SMPLRT_DIV, 0x07);
	date += Single_Write(MPU6050_ADDRESS, CONFIGL, 0x03);
	date += Single_Write(MPU6050_ADDRESS, GYRO_CONFIG, 0x10);
	date += Single_Write(MPU6050_ADDRESS, ACCEL_CONFIG, 0x09);//+-4G
	return date;
}

//**************************实现函数********************************************
//将iic读取到得数据分拆,放入相应寄存器,更新MPU6050_Last
//******************************************************************************
void MPU6050_Read(void)
{
	mpu6050_buffer[0]=Single_Read(MPU6050_ADDRESS, 0x3B);
	mpu6050_buffer[1]=Single_Read(MPU6050_ADDRESS, 0x3C);
	mpu6050_buffer[2]=Single_Read(MPU6050_ADDRESS, 0x3D);
	mpu6050_buffer[3]=Single_Read(MPU6050_ADDRESS, 0x3E);
	mpu6050_buffer[4]=Single_Read(MPU6050_ADDRESS, 0x3F);
	mpu6050_buffer[5]=Single_Read(MPU6050_ADDRESS, 0x40);
	mpu6050_buffer[8]=Single_Read(MPU6050_ADDRESS, 0x43);
	mpu6050_buffer[9]=Single_Read(MPU6050_ADDRESS, 0x44);
	mpu6050_buffer[10]=Single_Read(MPU6050_ADDRESS, 0x45);
	mpu6050_buffer[11]=Single_Read(MPU6050_ADDRESS, 0x46);
	mpu6050_buffer[12]=Single_Read(MPU6050_ADDRESS, 0x47);
	mpu6050_buffer[13]=Single_Read(MPU6050_ADDRESS, 0x48);
	
}
/**************************实现函数********************************************
//将iic读取到得数据分拆,放入相应寄存器
*******************************************************************************/
void MPU6050_Dataanl(void)
{
	MPU6050_Read();
	MPU6050_ACC_LAST.X=((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) - ACC_OFFSET.X;
	MPU6050_ACC_LAST.Y=((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) - ACC_OFFSET.Y;
	MPU6050_ACC_LAST.Z=((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]) - ACC_OFFSET.Z;

	MPU6050_GYRO_LAST.X= ((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9]);
	MPU6050_GYRO_LAST.Y= ((((int16_t)mpu6050_buffer[10]) << 8)| mpu6050_buffer[11]);
	MPU6050_GYRO_LAST.Z= ((((int16_t)mpu6050_buffer[12]) << 8)| mpu6050_buffer[13]);
  
	MPU6050_GYRO_Gr.X = MPU6050_GYRO_LAST.X * Gyro_Gr - GYRO_OFFSET.X * Gyro_Gr;
	MPU6050_GYRO_Gr.Y = MPU6050_GYRO_LAST.Y * Gyro_Gr - GYRO_OFFSET.Y * Gyro_Gr;
	MPU6050_GYRO_Gr.Z = MPU6050_GYRO_LAST.Z * Gyro_Gr - GYRO_OFFSET.Z * Gyro_Gr;

////////////////////////////////////////////////////
//    	The calibration  of  acc        //
////////////////////////////////////////////////////	
#ifdef ACC_GYO_OFFSET
	
	 if(!ACC_OFFSET_OK)
	 {
		 static int32_t	tempax=0,tempay=0,tempaz=0;
		 static uint8_t cnt_a=0;

		
		 if(cnt_a==0)
		 {
				ACC_OFFSET.X = 0;
				ACC_OFFSET.Y = 0;
				ACC_OFFSET.Z = 0;
				tempax = 0;
				tempay = 0;
				tempaz = 0;
				cnt_a = 1;
		 }
				tempax+= MPU6050_ACC_LAST.X;
				tempay+= MPU6050_ACC_LAST.Y;
				tempaz+= MPU6050_ACC_LAST.Z;
				if(cnt_a==200)
				{
					ACC_OFFSET.X=tempax/cnt_a;
					ACC_OFFSET.Y=tempay/cnt_a;
					ACC_OFFSET.Z=0;
					cnt_a = 0;
					ACC_OFFSET_OK = 1;
				//	EE_SAVE_ACC_OFFSET();//保存数据
					return;
				}
				cnt_a++;		
			}	
	#endif 	
}



void Gyro_OFFEST(void)
{
   int cnt_g=2000;
	 int32_t  tempgx=0,tempgy=0,tempgz=0;
	 GYRO_OFFSET.X=0;
	 GYRO_OFFSET.Y=0;
	 GYRO_OFFSET.Z=0;
	 while(cnt_g--)
	 {
		  MPU6050_Read();
		 
		  MPU6050_GYRO_LAST.X= ((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9]);
	    MPU6050_GYRO_LAST.Y= ((((int16_t)mpu6050_buffer[10]) << 8)| mpu6050_buffer[11]);
	    MPU6050_GYRO_LAST.Z= ((((int16_t)mpu6050_buffer[12]) << 8)| mpu6050_buffer[13]);
      tempgx+= MPU6050_GYRO_LAST.X;
			tempgy+= MPU6050_GYRO_LAST.Y;
			tempgz+= MPU6050_GYRO_LAST.Z;
   }
	 GYRO_OFFSET.X=tempgx/2000;
	 GYRO_OFFSET.Y=tempgy/2000;
	 GYRO_OFFSET.Z=tempgz/2000;
	  
}



void UART1_ReportIMU(void)
{
  int16_t ax,ay,az,gx,gy,gz;
  int16_t hx,hy,hz;
  int16_t yaw,pitch,roll;
  uint8_t temp;
	char ctemp;
	UART_Put_Char(0x88); 
	UART_Put_Char(0xAF);  
	UART_Put_Char(0x1C);  

	ax=MPU6050_ACC_LAST.X;

	ctemp=ax>>8;
	UART_Put_Char(ctemp); 
	temp+=ctemp;
	ctemp=ax;
	UART_Put_Char(ctemp); 
	temp+=ctemp;

	ay=MPU6050_ACC_LAST.Y;

	ctemp=ay>>8;
	UART_Put_Char(ctemp); 
	temp+=ctemp;
	ctemp=ay;
	UART_Put_Char(ctemp); 
	temp+=ctemp;

	az=MPU6050_ACC_LAST.Z;

	ctemp=az>>8;
	UART_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=az;
	UART_Put_Char(ctemp);
	temp+=ctemp;

	gx=MPU6050_GYRO_LAST.X;

	ctemp=gx>>8;
	UART_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=gx;
	UART_Put_Char(ctemp);
	temp+=ctemp;

	gy=MPU6050_GYRO_LAST.Y;

	ctemp=gy>>8;
	UART_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=gy;
	UART_Put_Char(ctemp);
	temp+=ctemp;

  gz=MPU6050_GYRO_LAST.Z;

	ctemp=gz>>8;
	UART_Put_Char(ctemp); 
	temp+=ctemp;
	ctemp=gz;
	UART_Put_Char(ctemp);	
	temp+=ctemp;

	hx=0;

	ctemp=hx>>8;
	UART_Put_Char(ctemp);
	temp+=ctemp;
	ctemp=hx;
	UART_Put_Char(ctemp);
	temp+=ctemp;

	hy=0;

	ctemp=hy>>8;
	UART_Put_Char(ctemp);	
	temp+=ctemp;
	ctemp=hy;
	UART_Put_Char(ctemp);	
	temp+=ctemp;

	hz=0;

	ctemp=hz>>8;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;
	ctemp=hz;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;

	pitch = (int)(Q_ANGLE.X*100);
				   

	ctemp=pitch>>8;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;
	ctemp=pitch;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;


	roll = (int)(Q_ANGLE.Y*100);

	ctemp=roll>>8;
	UART_Put_Char(ctemp);	
	temp+=ctemp;
	ctemp=roll;
	UART_Put_Char(ctemp);	 
	temp+=ctemp;


 	yaw = (int)(Q_ANGLE.Z*10);


	ctemp=yaw>>8;
	UART_Put_Char(ctemp);	
	temp+=ctemp;
	ctemp=yaw;
	UART_Put_Char(ctemp);	
	temp+=ctemp;

	UART_Put_Char(0x00);
	UART_Put_Char(0x00);
	UART_Put_Char(0x00);
	UART_Put_Char(0x00);

	UART_Put_Char(temp); 
}


