#include"mpu6050_driver.h"

AccelMeas accel_data;
GyroMeas gyro_data;
const float gyro_X_Bias =  0.040737316;
const float gyro_Y_Bias = -0.0186734423;
const float gyro_Z_Bias =  0.0116336327;

void mpu6050_init(void)
{
	uint8_t error_counter=0;
	HAL_StatusTypeDef stat;
	uint8_t data;
	stat = HAL_I2C_Mem_Read(&hi2c1, MPU6050_SLAVE_ADDRESS, MPU6050_WHO_AM_I_REG, 1 , &data, 1 , 100);
	if (stat != HAL_OK) {
	    printf("I2C read failed!\n");
	    error_counter++;
	    return;
	}

	if(data == 0x68)
	{
		printf("device is correct \n");
	}else
	{
		printf("device is uncorrect \n");
	}

	// Reseting device
	data = 0x80U;
	stat = HAL_I2C_Mem_Write(&hi2c1, MPU6050_SLAVE_ADDRESS, MPU6050_PWR_MGMT1_REG , 1 , &data, 1 , 100);
	if(stat != HAL_OK) {
	    printf("Failed to write to PWR_MGMT1_REG (reset)\n");
	    error_counter++;
	}
	HAL_Delay(100);

	// Make sure sleep mode is off
	data = 0x00U;
	stat = HAL_I2C_Mem_Write(&hi2c1, MPU6050_SLAVE_ADDRESS, MPU6050_PWR_MGMT1_REG , 1 , &data, 1 , 100);
	if(stat != HAL_OK) {
	    printf("Failed to write to PWR_MGMT1_REG (reset)\n");
	    error_counter++;
	}


	// Gyro config 250 degree
	data = 0x00U;
	stat = HAL_I2C_Mem_Write(&hi2c1, MPU6050_SLAVE_ADDRESS, MPU6050_GYRO_CONFIG_REG , 1 , &data, 1 , 100);
	if(stat != HAL_OK) {
	    printf("Failed to write to PWR_MGMT1_REG (reset)\n");
	    error_counter++;
	}


	// Accel config 250 degree
	data = 0x00U;
	stat = HAL_I2C_Mem_Write(&hi2c1, MPU6050_SLAVE_ADDRESS, MPU6050_ACCEL_CONFIG_REG, 1 , &data, 1 , 100);
	if(stat != HAL_OK) {
	    printf("Failed to write to PWR_MGMT1_REG (reset)\n");
	    error_counter++;
	}


	// DLPF as
	data = 0x01U;
	stat = HAL_I2C_Mem_Write(&hi2c1, MPU6050_SLAVE_ADDRESS, MPU6050_CONFIG_REG, 1 , &data, 1 , 100);
	if(stat != HAL_OK) {
	    printf("Failed to write to PWR_MGMT1_REG (reset)\n");
	    error_counter++;
	}


	printf("error counter value is : %d \n " , error_counter);
}

void mpu6050_data(void)
{
	HAL_StatusTypeDef stat;
	uint8_t AccelRaw[6];
	uint8_t GyroRaw[6];

	stat = HAL_I2C_Mem_Read(&hi2c1, MPU6050_SLAVE_ADDRESS , MPU6050_ACCELXOUT_REG, 1 , AccelRaw , 6 , 100);
	if(stat != HAL_OK) {
	    printf("Failed to write to PWR_MGMT1_REG (reset)\n");
	    //error_counter++;
	}

	stat = HAL_I2C_Mem_Read(&hi2c1, MPU6050_SLAVE_ADDRESS , MPU6050_GYROXOUT_REG, 1 , GyroRaw , 6 , 100);
	if(stat != HAL_OK) {
	    printf("Failed to write to PWR_MGMT1_REG (reset)\n");
	    //error_counter++;
	}

	accel_data.Accel_X = (int16_t)((AccelRaw[0] << 8) | AccelRaw[1]); // MPU6050 HAS X AND Y CORDINATE CONFLICT !! CHECK IT OUT !!
	accel_data.Accel_Y = (int16_t)((AccelRaw[2] << 8) | AccelRaw[3]);
	accel_data.Accel_Z = (int16_t)((AccelRaw[4] << 8) | AccelRaw[5]);

	accel_data.Accel_X = -(( accel_data.Accel_X / ACCEL_SCALE ) * G_TO_M_S2);
	accel_data.Accel_Y = -(( accel_data.Accel_Y / ACCEL_SCALE ) * G_TO_M_S2);
	accel_data.Accel_Z = -((( accel_data.Accel_Z / ACCEL_SCALE ) * G_TO_M_S2 ) - G_TO_M_S2);


	gyro_data.Gyro_X = (int16_t)((GyroRaw[2] << 8)  | GyroRaw[3]);
	gyro_data.Gyro_Y = (int16_t)((GyroRaw[0] << 8)  | GyroRaw[1]);
	gyro_data.Gyro_Z = (int16_t)((GyroRaw[4] << 8)  | GyroRaw[5]);

	gyro_data.Gyro_X = (( gyro_data.Gyro_X / GYRO_SCALE ) * DEG2RAD) - gyro_X_Bias ;
	gyro_data.Gyro_Y = (( gyro_data.Gyro_Y / GYRO_SCALE ) * DEG2RAD) - gyro_Y_Bias ;
	gyro_data.Gyro_Z = (( gyro_data.Gyro_Z / GYRO_SCALE ) * DEG2RAD) - gyro_Z_Bias ;

}


void LPF_Apply( GyroMeas * gyro , float gyroPrev[3] )
{
	gyro->Gyro_X = LPF_GYRO_ALPHA * gyroPrev[0] + (1 - LPF_GYRO_ALPHA) * gyro->Gyro_X;
	gyro->Gyro_Y = LPF_GYRO_ALPHA * gyroPrev[1] + (1 - LPF_GYRO_ALPHA) * gyro->Gyro_Y;
	gyro->Gyro_Z = LPF_GYRO_ALPHA * gyroPrev[2] + (1 - LPF_GYRO_ALPHA) * gyro->Gyro_Z;

}

void i2c_reset() {

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // SCL ve SDA'yı çıkış olarak ayarla
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Clock sinyali göndererek I2C hattını serbest bırakmaya çalış
    for (int i = 0; i < 15; i++) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);  // SCL HIGH
        HAL_Delay(2);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); // SCL LOW
        HAL_Delay(2);
    }
    // Keep SCL high
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    // SDA'yı HIGH yap
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
    HAL_Delay(5);

    // GPIO'ları tekrar I2C modu için ayarla
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);

}









