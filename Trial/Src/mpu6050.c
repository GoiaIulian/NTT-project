/*
 * mpu6050.c
 *
 *  Created on: Jul 31, 2019
 *      Author: yulia
 */

#include "mpu6050.h"



IMU_tstImuData avg()
{
	IMU_tstImuData a;
	IMU_tstImuData raw;

	float AccXDataSum = 0;
	float AccYDataSum = 0;
	float AccZDataSum = 0;
	float TempSum = 0;
	float GyroXDataSum = 0;
	float GyroYDataSum = 0;
	float GyroZDataSum = 0;

	for (int i = 0; i < 50; i++)
	{
		raw = get_DATA();
		raw.AccXData  = (raw.AccXData) / 835.040;
		raw.AccYData  = (raw.AccYData) / 835.040;
		raw.AccZData  = (raw.AccZData) / 835.040;
		raw.GyroXData = (raw.GyroXData) / 939.650784;
		raw.GyroYData = (raw.GyroYData) / 939.650784;
		raw.GyroZData = (raw.GyroZData) / 939.650784;
		raw.Temp 	  = ((raw.Temp)/340) + 36.53;

		AccXDataSum  += raw.AccXData;
		AccYDataSum  += raw.AccYData;
		AccZDataSum  += raw.AccZData;
		TempSum      += raw.Temp;
		GyroXDataSum += raw.GyroXData;
		GyroYDataSum += raw.GyroYData;
		GyroZDataSum += raw.GyroZData;
	}
	a.AccXData  = AccXDataSum / 50;
	a.AccYData  = AccYDataSum / 50;
	a.AccZData  = AccZDataSum / 50;
	a.Temp      = TempSum / 50;
	a.GyroXData = GyroXDataSum / 50;
	a.GyroYData = GyroYDataSum / 50;
	a.GyroZData = GyroZDataSum / 50;

	return a;
}

void _delay_ms(int time)
{
	volatile int i,j;

	for(i=0;i<time;i++)
	{
		j++;
	}
}

void Init__vMPU_6050()
{

	unsigned char dest;
	// reset chip 1 - exit sleep mode
	//_delay_ms(1000);
	I2C__vWriteSingleByteBuffer(mpu_6050_adress,mpu_6050_pwr_mgmnt_1,init_byte_107);
	//_delay_ms(1000);
	I2C__vReadBuffer(mpu_6050_adress,mpu_6050_pwr_mgmnt_1,&dest,1);

	if (dest == init_byte_107)
	{
		printf("\r\n MPU chip 1 reseted successfully !\r\n");

	}
	else
	{
		printf("\r\n MPU chip 1 reset failed !\r\n");
	}
	// reset chip 2
	I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_sig_path_rst,init_byte_104);
	//_delay_ms(1000);
	I2C__vReadBuffer(mpu_6050_adress,mpu_6050_sig_path_rst,&dest,1);
	if ((dest == 0x00))
	{
		// dupa ce se da reset valorile devin 0 din nou
		printf("\r\n MPU chip 2 reseted successfully !\r\n");

	}
	else
	{
		printf("\r\n MPU chip 2 reset failed !\r\n");
	}

	// sample divide rate  sample rate = ex 8kHz / (1+divide rate)  =  8000/110 = 72 Hz
	I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_smprt_div,80);
	_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_smprt_div,&dest,1);
		if ((dest == 80))
		{

			printf("\r\n MPU sample divide rate configured successfully !\r\n");
		}
		else
		{
			printf("\r\n MPU sample divide rate configuration failed !\r\n");
		}
		_delay_ms(1000);

	// digital low pass filter
		I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_config,0x00);
		_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_config,&dest,1);
		if (dest == 0x00)
		{
			printf("\r\n MPU digital low pass filter configured successfully !\r\n");

		}
		else
		{
			printf("\r\n MPU digital low pass filter configuration failed !\r\n");
		}

	// gyroscope config - gyro full scale = +/- 2000dps
		I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_gyro_config,0b00011000);
		_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_gyro_config,&dest,1);
		if (dest == 0b00011000)
		{
			printf("\r\n MPU gyroscope configured successfully !\r\n");

		}
		else
		{
			printf("\r\n MPU gyroscope configuration failed !\r\n");
		}

	// accelerometer config - accelerometer full scale = +/- 4g
		I2C__vWriteSingleByteBuffer(mpu_6050_adress, mpu_6050_accel_config,0b00001000);
		_delay_ms(1000);
		I2C__vReadBuffer(mpu_6050_adress,mpu_6050_accel_config,&dest,1);
		if (dest == 0b00001000)
		{
			printf("\r\n MPU accelerometer configured successfully !\r\n");

		}
		else
		{
			printf("\r\n MPU accelerometer configuration failed !\r\n");
		}
		_delay_ms(1000);

	return ;
}

void testChip(void)
{
	uint8_t u8destVal;

	I2C__vReadBuffer(mpu_6050_adress,117,&u8destVal,1);

	if (u8destVal == 0x68)
	{
		printf("\r\n----Who am I...--------[ OK ]");
	}
	else
	{
		printf("\r\n---nWho am I...[ NOK ]---");
	}
}

IMU_tstImuData get_DATA(){

	uint8_t AccelGyroRawData[14] = {0};
	int16_t int16FinalImuRawData[7]={0};
	IMU_tstImuData IMUstRawData;
	I2C__vReadBuffer(mpu_6050_adress,mpu_6050_accel_x_h,AccelGyroRawData,14);

	int16FinalImuRawData[0] = (AccelGyroRawData[0]<<8)|(AccelGyroRawData[1]); //acc_x
	int16FinalImuRawData[1] = (AccelGyroRawData[2]<<8)|(AccelGyroRawData[3]); //acc_y
	int16FinalImuRawData[2] = (AccelGyroRawData[4]<<8)|(AccelGyroRawData[5]); //acc_z
	int16FinalImuRawData[3] = (AccelGyroRawData[6]<<8)|(AccelGyroRawData[7]); //temperature
	int16FinalImuRawData[4] = (AccelGyroRawData[8]<<8)|(AccelGyroRawData[9]); //gyro_x
	int16FinalImuRawData[5] = (AccelGyroRawData[10]<<8)|(AccelGyroRawData[11]); //gyro_y
	int16FinalImuRawData[6] = (AccelGyroRawData[12]<<8)|(AccelGyroRawData[13]); //gyro_z

	IMUstRawData.AccXData  = (float)int16FinalImuRawData[0];
	IMUstRawData.AccYData  = (float)int16FinalImuRawData[1];
	IMUstRawData.AccZData  = (float)int16FinalImuRawData[2];
	IMUstRawData.Temp      = (float)int16FinalImuRawData[3];
	IMUstRawData.GyroXData = (float)int16FinalImuRawData[4];
	IMUstRawData.GyroYData = (float)int16FinalImuRawData[5];
	IMUstRawData.GyroZData = (float)int16FinalImuRawData[6];

	return IMUstRawData;

}
