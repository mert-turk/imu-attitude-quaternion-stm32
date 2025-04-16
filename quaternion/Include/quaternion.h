/*
 * quaternion.h
 *
 *  Created on: Apr 13, 2025
 *      Author: user
 */

 #ifndef INC_QUATERNION_H_
 #define INC_QUATERNION_H_
 
 #include"arm_math.h"
 

 /*********************************************************/
 typedef struct {
     float32_t w;
     float32_t x;
     float32_t y;
     float32_t z;
 } Quaternion_t;
 /*********************************************************/
 typedef struct{
    float32_t Roll;
    float32_t Pitch;
    float32_t Yaw;
 }EulerAngle_t;
 
 
 
 /*********************************************************/
 /* Function Declarations*/
 /*********************************************************/
 
 Quaternion_t quaternion_identity();
 /*********************************************************/

 /*********************************************************/
 Quaternion_t quaternion_from_gyro(float gx, float gy, float gz, float dt);
 /*********************************************************/
 Quaternion_t quaternion_update(Quaternion_t q, float gx, float gy, float gz, float dt);
 /*********************************************************/
 void quaternion_to_euler(Quaternion_t  q, EulerAngle_t *  Attitude);
 
 
 
 
 
 
 
 
 
 #endif /* INC_QUATERNION_H_ */
 