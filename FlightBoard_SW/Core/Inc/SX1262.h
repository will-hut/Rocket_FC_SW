/*
 * SX1262.h
 *
 *  Created on: Feb 14, 2024
 *      Author: willh
 */

#ifndef INC_SX1262_H_
#define INC_SX1262_H_

#include "SX1262_Definitions.h"

typedef struct{
	// peripheral config
	SPI_HandleTypeDef		SPI;
	GPIO_TypeDef*			Reset_Port;
	uint16_t				Reset_Pin;
	GPIO_TypeDef*			NSS_Port;
	uint16_t				NSS_Pin;
	GPIO_TypeDef*			Busy_Port;
	uint16_t				Busy_Pin;

	SX1262_STATE			State;

	uint8_t					TX_Buf[300]; // internal buffer for SPI commands
	uint8_t					RX_Buf[300]; // internal buffer for SPI commands

	uint8_t					Packet_Buf[SX126X_MAX_PACKET_LENGTH];

	void 					(*RX_Callback)(uint8_t*, uint8_t);




} SX1262;

void SX1262_CSLow(SX1262*);
void SX1262_CSHigh(SX1262*);

void SX1262_BusyWait(SX1262*);
int  SX1262_IsBusy(SX1262*);

void SX1262_WriteReg(SX1262*, uint16_t, uint8_t);
uint8_t SX1262_ReadReg(SX1262*, uint16_t);

void SX1262_WriteBuffer(SX1262*, uint8_t*, uint8_t);
uint8_t* SX1262_ReadBuffer(SX1262*, uint8_t);

void SX1262_SetStandby(SX1262*, uint8_t);
void SX1262_SetRx(SX1262*, uint32_t);
void SX1262_SetTx(SX1262*, uint32_t);
void SX1262_SetPacketType(SX1262*, uint8_t);
void SX1262_SetRfFrequency(SX1262*, double);
void SX1262_SetPaConfig(SX1262*, uint8_t, uint8_t);
void SX1262_SetTxParams(SX1262*, int8_t, uint8_t);
void SX1262_SetBufferBaseAddress(SX1262*, uint8_t, uint8_t);
void SX1262_SetModulationParamsLoRa(SX1262*, uint8_t, uint8_t, uint8_t, uint8_t);
void SX1262_SetPacketParamsLoRa(SX1262*, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void SX1262_SetDioIrqParams(SX1262*, uint16_t, uint16_t);
uint16_t SX1262_GetIrqStatus(SX1262*);
uint8_t SX1262_GetRxBufferLen(SX1262*);

void SX1262_HandleCallback(SX1262*);
void SX1262_SetPacketLen(SX1262*, uint8_t);


void SX1262_RxMode(SX1262*);
void SX1262_Transmit(SX1262*, uint8_t*, uint8_t);
void SX1262_Init(SX1262*);


#endif /* INC_SX1262_H_ */
