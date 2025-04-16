/*
 * mpu6050_driver.h
 *
 *  Created on: Apr 11, 2025
 *      Author: user
 */

#ifndef MPU6050_DRIVER_H_
#define MPU6050_DRIVER_H_
#include"stm32f4xx.h"
#include"stdio.h"
#include"main.h"
#include"math.h"
extern I2C_HandleTypeDef hi2c1;

#define MPU6050_SLAVE_ADDRESS		0xD0U
#define MPU6050_CONFIG_REG			0x1AU
#define MPU6050_SMPLRTDIV_REG		0x19U
#define MPU6050_GYRO_CONFIG_REG		0x1BU
#define MPU6050_ACCEL_CONFIG_REG	0x1CU
#define MPU6050_ACCELXOUT_REG		0x3BU
#define MPU6050_GYROXOUT_REG		0x43U
#define MPU6050_PWR_MGMT1_REG		0x6BU
#define MPU6050_WHO_AM_I_REG		0x75U

#define ACCEL_SCALE 16384.0f  // LSB/g
#define GYRO_SCALE  131.0f    // LSB/(°/s)
#define DEG2RAD 0.0174533f
#define G_TO_M_S2	9.80665f
#define LPF_GYRO_ALPHA 0.02


typedef struct{
	float Accel_X;
	float Accel_Y;
	float Accel_Z;
}AccelMeas;

typedef struct{
	float Gyro_X;
	float Gyro_Y;
	float Gyro_Z;
}GyroMeas;


typedef struct{
	float phi;
	float theta;
	float psi;

}Euler_Angles_t;

void mpu6050_init(void);
void mpu6050_data(void);
void LPF_Apply( GyroMeas * gyro , float gyroPrev[3] );
void calibrate_gyro_bias(void);
void i2c_reset(void);

#endif /* MPU6050_DRIVER_H_ */
