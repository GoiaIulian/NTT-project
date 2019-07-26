/*
 * mpu6050.h
 *
 *  Created on: Jul 25, 2019
 *      Author: yulia
 */


#define MPU_6050_H_

#define mpu_6050_smplrt_div 25
	#define mpu_6050_config 26
	#define mpu_6050_gyro_config 27
	#define mpu_6050_accel_config 28


//Accelerometer
	#define mpu_6050_accel_x_h 59
	#define mpu_6050_accel_x_l 60
	#define mpu_6050_accel_y_h 61
	#define mpu_6050_accel_y_l 62
	#define mpu_6050_accel_z_h 63
	#define mpu_6050_accel_z_l 64

//Temperature
	#define mpu_6050_temp_h 65
	#define mpu_6050_temp_l 66


//Gyroscope
	#define mpu_6050_gyro_x_h 67
	#define mpu_6050_gyro_x_l 68
	#define mpu_6050_gyro_y_h 69
	#define mpu_6050_gyro_y_l 70
	#define mpu_6050_gyro_z_h 71
	#define mpu_6050_gyro_z_l 72


//who I am
	#define mpu_6050_who_am_I 117

//Power Management ---allows the user to config the power mode and clock source
	#define mpu_6050_pwr_mgmt_1 107 0x6B

//User Control --when I2C_MST_EN=1 =I2C is enabled
	#define mpu_6050_user_ctrl 106

//Signal Path Reset for TMP, GYRO AND ACCEL
	#define mpu_6050_sig_path_rst 104

//6050 address
	#define mpu_6050_adress 0x68

	#define init_byte_107 0b00000000
	#define init_byte_106 0b00000001
	#define init_byte_104 0b00000111
