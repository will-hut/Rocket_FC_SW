/*
 * GPS.c
 *
 *  Created on: Feb 27, 2024
 *      Author: willh
 */

#include "GPS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void GPS_DMA_Rx(GPS_t* GPS){
	// reads either MAX_NMEA_SIZE bytes into the buffer, or reads up until RX line goes idle
	HAL_UART_Receive_DMA(&GPS->UART, (uint8_t *)GPS->DMA_Buffer, MAX_BUF_SIZE);
}

void GPS_Init(GPS_t* GPS){
	GPS->Data_Ready = 0;
	GPS->Data_Valid = 0;
	GPS_DMA_Rx(GPS);
}


void GPS_DMA_Callback(GPS_t* GPS){
	// DMA has read more data from UART. If we need new data, copy it to the data buffer and set flag
	if(!GPS->Data_Ready){
		memcpy(GPS->Data_Buffer, GPS->DMA_Buffer, MAX_BUF_SIZE);
		GPS->Data_Ready = 1;
	}
}

float GPS_nmea_to_dec(float deg_coord, char nsew) {
    int degree = (int)(deg_coord/100);
    float minutes = deg_coord - degree*100;
    float dec_deg = minutes / 60;
    float decimal = degree + dec_deg;
    if (nsew == 'S' || nsew == 'W') { // return negative
        decimal *= -1;
    }
    return decimal;
}

void GPS_Parse(GPS_t* GPS){
	if(!GPS->Data_Ready) return;

	// locate position message
	char* loc = strstr((char *)GPS->Data_Buffer, "GPRMC");
	if(*loc != 'G') return;

	int ret = sscanf(loc, "GPRMC,%f,A,%f,%c,%f,%c,", &GPS->Time_UTC, &GPS->Latitude_Raw, &GPS->LatChar, &GPS->Longitude_Raw, &GPS->LongChar);
	if (ret != 5) return;

	GPS->Latitude = GPS_nmea_to_dec(GPS->Latitude_Raw, GPS->LatChar);
	GPS->Longitude = GPS_nmea_to_dec(GPS->Longitude_Raw, GPS->LongChar);

	GPS->Data_Valid = 1; // data is valid
	GPS->Data_Ready = 0; // request more data
}
