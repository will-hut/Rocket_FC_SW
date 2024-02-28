/*
 * MadgwickAHRS.h
 *
 *  Created on: Feb 28, 2024
 *      Author: willh
 */

#ifndef INC_MADGWICKAHRS_H_
#define INC_MADGWICKAHRS_H_

#define MADGWICK_SAMPLE_FREQ 	100.0  	// sample frequency in Hz
#define MADGWICK_BETA_DEF		0.01	// 2x P-gain, increase to make more responsive

typedef struct{
    float 						beta;	// algorithm gain
    float 						q0;
    float 						q1;
    float 						q2;
    float 						q3;		// quaternion of sensor frame relative to auxiliary frame
    float 						invSampleFreq;


    float 						Roll_Radians;
    float 						Pitch_Radians;
    float 						Yaw_Radians;

} Madgwick;

void Madgwick_Init(Madgwick*);
void Madgwick_ComputeAngles(Madgwick*);
void Madgwick_UpdateIMU(Madgwick*, float, float, float, float, float, float);


#endif /* INC_MADGWICKAHRS_H_ */
