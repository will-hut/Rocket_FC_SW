/*
 * MadgwickAHRS.c
 *
 *  Created on: Feb 28, 2024
 *      Author: willh
 */

#ifndef INC_MADGWICKAHRS_C_
#define INC_MADGWICKAHRS_C_

#include "MadgwickAHRS.h"
#include <math.h>

void Madgwick_Init(Madgwick* AHRS){
	AHRS->beta = MADGWICK_BETA_DEF;
	AHRS->q0 = 1.0f;
	AHRS->q1 = 0.0f;
	AHRS->q2 = 0.0f;
	AHRS->q3 = 0.0f;
	AHRS->invSampleFreq = 1.0/MADGWICK_SAMPLE_FREQ;
}

float fastInvSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	y = y * (1.5f - (halfx * y * y));
	return y;
}

void Madgwick_ComputeAngles(Madgwick* AHRS)
{
	AHRS->Roll_Radians = atan2f(AHRS->q0*AHRS->q1 + AHRS->q2*AHRS->q3, 0.5f - AHRS->q1*AHRS->q1 - AHRS->q2*AHRS->q2);
	AHRS->Pitch_Radians = asinf(-2.0f * (AHRS->q1*AHRS->q3 - AHRS->q0*AHRS->q2));
	AHRS->Yaw_Radians = atan2f(AHRS->q1*AHRS->q2 + AHRS->q0*AHRS->q3, 0.5f - AHRS->q2*AHRS->q2 - AHRS->q3*AHRS->q3);
}

void Madgwick_UpdateIMU(Madgwick* AHRS, float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	// Convert gyroscope degrees/sec to radians/sec
	gx *= 0.0174533f;
	gy *= 0.0174533f;
	gz *= 0.0174533f;

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-AHRS->q1 * gx - AHRS->q2 * gy - AHRS->q3 * gz);
	qDot2 = 0.5f * (AHRS->q0 * gx + AHRS->q2 * gz - AHRS->q3 * gy);
	qDot3 = 0.5f * (AHRS->q0 * gy - AHRS->q1 * gz + AHRS->q3 * gx);
	qDot4 = 0.5f * (AHRS->q0 * gz + AHRS->q1 * gy - AHRS->q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = fastInvSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * AHRS->q0;
		_2q1 = 2.0f * AHRS->q1;
		_2q2 = 2.0f * AHRS->q2;
		_2q3 = 2.0f * AHRS->q3;
		_4q0 = 4.0f * AHRS->q0;
		_4q1 = 4.0f * AHRS->q1;
		_4q2 = 4.0f * AHRS->q2;
		_8q1 = 8.0f * AHRS->q1;
		_8q2 = 8.0f * AHRS->q2;
		q0q0 = AHRS->q0 * AHRS->q0;
		q1q1 = AHRS->q1 * AHRS->q1;
		q2q2 = AHRS->q2 * AHRS->q2;
		q3q3 = AHRS->q3 * AHRS->q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * AHRS->q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		s2 = 4.0f * q0q0 * AHRS->q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		s3 = 4.0f * q1q1 * AHRS->q3 - _2q1 * ax + 4.0f * q2q2 * AHRS->q3 - _2q2 * ay;
		recipNorm = fastInvSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= AHRS->beta * s0;
		qDot2 -= AHRS->beta * s1;
		qDot3 -= AHRS->beta * s2;
		qDot4 -= AHRS->beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	AHRS->q0 += qDot1 * AHRS->invSampleFreq;
	AHRS->q1 += qDot2 * AHRS->invSampleFreq;
	AHRS->q2 += qDot3 * AHRS->invSampleFreq;
	AHRS->q3 += qDot4 * AHRS->invSampleFreq;

	// Normalise quaternion
	recipNorm = fastInvSqrt(AHRS->q0 * AHRS->q0 + AHRS->q1 * AHRS->q1 + AHRS->q2 * AHRS->q2 + AHRS->q3 * AHRS->q3);
	AHRS->q0 *= recipNorm;
	AHRS->q1 *= recipNorm;
	AHRS->q2 *= recipNorm;
	AHRS->q3 *= recipNorm;

	Madgwick_ComputeAngles(AHRS);
}


#endif /* INC_MADGWICKAHRS_C_ */
