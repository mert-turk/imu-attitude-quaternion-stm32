#include"quaternion.h"


const Quaternion_t quaternion_identity_val = {1.0f, 0.0f, 0.0f, 0.0f};
static float fast_copysign(float a, float b);
static void array_to_struct(const float32_t arr[4], Quaternion_t *q);
static void struct_to_array(const Quaternion_t *q, float32_t arr[4]);




/*=====================================================================*/
Quaternion_t quaternion_identity() {
    return quaternion_identity_val;
}


/*=====================================================================*/
Quaternion_t quaternion_from_gyro(float gx, float gy, float gz, float dt)
{
    float32_t norm = (gx*gx + gy*gy + gz*gz);
    arm_sqrt_f32(norm , &norm );

    if (norm == 0.0f) {
        return quaternion_identity();
    }

    float32_t theta = norm * dt;
    float32_t half_theta = theta * 0.5f;
    float32_t sin_half_theta = arm_sin_f32(half_theta);
    float32_t cos_half_theta = arm_cos_f32(half_theta);

    Quaternion_t dq;
    dq.w = cos_half_theta;
    dq.x = (gx / norm) * sin_half_theta;
    dq.y = (gy / norm) * sin_half_theta;
    dq.z = (gz / norm) * sin_half_theta;

    return dq;
}


/*=====================================================================*/
void quaternion_to_euler(Quaternion_t  q, EulerAngle_t *  Attitude)
{
	if (Attitude == NULL) return;
    // Roll (x-axis rotation)
    float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    arm_atan2_f32(sinr_cosp, cosr_cosp, &Attitude->Roll);

    // Pitch (y-axis rotation)
    float sinp = 2.0f * (q.w * q.y - q.z * q.x);
    
    if (fabsf(sinp) >= 1.0f)
        Attitude->Pitch = fast_copysign(PI / 2.0f, sinp); // 90 dereceye sabitle
    else
        Attitude->Pitch = asinf(sinp);

    // Yaw (z-axis rotation)
    float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    arm_atan2_f32(siny_cosp, cosy_cosp, &Attitude->Yaw);

}

/*=====================================================================*/
Quaternion_t quaternion_update(Quaternion_t q, float gx, float gy, float gz, float dt)
{
    Quaternion_t dq = quaternion_from_gyro(gx, gy, gz, dt);
    float32_t q_arr[4], dq_arr[4], result[4];

    struct_to_array(&q, q_arr);
    struct_to_array(&dq, dq_arr);

    arm_quaternion_product_f32(q_arr, dq_arr, result, 1);
    arm_quaternion_normalize_f32(result, result, 1);

    array_to_struct(result, &q);
    return q;
}




/*=====================================================================*/
static float fast_copysign(float a, float b) 
{
    return (b >= 0.0f) ? fabsf(a) : -fabsf(a);
}

static void struct_to_array(const Quaternion_t *q, float32_t arr[4])
{
    arr[0] = q->w;
    arr[1] = q->x;
    arr[2] = q->y;
    arr[3] = q->z;
}

static void array_to_struct(const float32_t arr[4], Quaternion_t *q)
{
    q->w = arr[0];
    q->x = arr[1];
    q->y = arr[2];
    q->z = arr[3];
}



