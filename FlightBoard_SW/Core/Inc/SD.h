/*
 * SD.h
 *
 *  Created on: Feb 27, 2024
 *      Author: willh
 */

#ifndef INC_SD_H_
#define INC_SD_H_

#include "main.h"
#include "FatFS.h"


#define SD_WRITE_SIZE		2048
#define BUFFER_SIZE			(SD_WRITE_SIZE*2)


typedef struct{
	// Circular buffer
	char					Buffer[BUFFER_SIZE];
	uint16_t				Head;
	uint16_t				Tail;
	uint16_t				Elements;

	// Peripheral config
	FIL						SD_File;
	FATFS					Fat_FS;
	FRESULT					Res;
	uint32_t				Bytes_Written;
	uint8_t					File_Open;

} SD_t;


void SD_Init(SD_t*);
void SD_Open(SD_t*);
void SD_Close(SD_t*);
void SD_Test(SD_t*);
void SD_QueueWrite(SD_t*, char*, uint16_t);


#endif /* INC_SD_H_ */
