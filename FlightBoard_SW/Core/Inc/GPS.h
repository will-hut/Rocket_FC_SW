/*
 * GPS.h
 *
 *  Created on: Feb 27, 2024
 *      Author: willh
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

#include "main.h"

#define MAX_BUF_SIZE 		256

typedef struct{
	// peripheral config
	UART_HandleTypeDef		UART;
	DMA_HandleTypeDef		DMA;
	uint8_t					DMA_Buffer[MAX_BUF_SIZE];
	uint8_t					Data_Buffer[MAX_BUF_SIZE];
	uint8_t					Data_Ready;
	uint8_t					Data_Valid;

	float					Time_UTC;
	float					Latitude_Raw; 	// value in NMEA coordinate format (dddmm.mmmm)
	float					Longitude_Raw;
	char					LatChar; 		// N/S
	char					LongChar; 		// E/W

	float					Latitude; 		// values as positive/negative float (use these)
	float					Longitude;


} GPS_t;


void GPS_DMA_Rx(GPS_t*);
void GPS_Init(GPS_t*);
void GPS_DMA_Callback(GPS_t*);
void GPS_Parse(GPS_t*);


#endif /* INC_GPS_H_ */
