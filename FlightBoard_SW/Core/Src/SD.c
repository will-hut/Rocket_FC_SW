/*
 * SD.c
 *
 *  Created on: Feb 27, 2024
 *      Author: willh
 */


#include "SD.h"
#include <string.h>


uint16_t min(uint16_t a, uint16_t b) {
    return a < b ? a : b;
}


void SD_Init(SD_t* SD){
	SD->Head = 0;
	SD->Tail = 0;
	SD->File_Open = 0;
}

void SD_Open(SD_t* SD){
	// Mount file system
	SD->Res = f_mount(&SD->Fat_FS, (TCHAR const *)"", 1);
	if (SD->Res != FR_OK) {
		Error_Handler();
	}

	// Open file for writing (Create)
	SD->Res = f_open(&SD->SD_File, "FLIGHT.CSV", FA_CREATE_ALWAYS | FA_WRITE);
	if (SD->Res != FR_OK) {
		Error_Handler();
	}

	SD->File_Open = 1;
}

void SD_Close(SD_t* SD){
	  f_close(&SD->SD_File);
	  SD->Res = f_mount(NULL, "", 0);

	  SD->File_Open = 0;
}

void SD_Test(SD_t* SD){
	SD_Open(SD);
	SD->Bytes_Written = 0;
	SD->Res = f_write(&SD->SD_File, "Hello world!", 12, (void *)&SD->Bytes_Written);
	if ((SD->Bytes_Written == 0) || (SD->Res != FR_OK)) {
		Error_Handler();
	}
	SD_Close(SD);
}



void SD_QueueWrite(SD_t* SD, char* data, uint16_t len){
	uint16_t part1 = min(len, BUFFER_SIZE - SD->Tail); 		// either the full data written, or the part that doesn't get wrapped around
	uint16_t part2 = len - part1; 							// the part that gets wrapped around

	memcpy(SD->Buffer + SD->Tail, 	data, 		part1); 	// write the data that doesn't get wrapped
	memcpy(SD->Buffer, 				data+part1, part2);		// write the data that gets wrapped

	SD->Tail = (SD->Tail + len) % BUFFER_SIZE;

	SD->Elements += len;


	if(SD->Elements > SD_WRITE_SIZE){

		if(SD->Tail < SD_WRITE_SIZE){
			// trigger write of second half of buffer
			SD->Head = SD_WRITE_SIZE;
		} else {
			// trigger write of first half of buffer
			SD->Head = 0;
		}

		// actually write to SD card

		if(SD->File_Open){
			SD->Bytes_Written = 0;

			char* buf_start = SD->Buffer + SD->Head;

			SD->Res = f_write(&SD->SD_File, buf_start, SD_WRITE_SIZE, (void *)&SD->Bytes_Written);
			if ((SD->Bytes_Written == 0) || (SD->Res != FR_OK)) {
				Error_Handler();
			}
		}
		SD->Elements -= SD_WRITE_SIZE;
	}
}
