/*
 * SX1262.c
 *
 *  Created on: Feb 14, 2024
 *      Author: willh
 */


// TODO: Fix hardcoded max length and investigate LoRa explicit header (variable length)

#include "SX1262.h"

// sets the CS pin low
void SX1262_CSLow(SX1262* SX){
	HAL_GPIO_WritePin(SX->NSS_Port, (int)SX->NSS_Pin, GPIO_PIN_RESET);
}

// sets the CS pin high
void SX1262_CSHigh(SX1262* SX){
	HAL_GPIO_WritePin(SX->NSS_Port, (int)SX->NSS_Pin, GPIO_PIN_SET);
}

int SX1262_IsBusy(SX1262* SX){
	return HAL_GPIO_ReadPin(SX->Busy_Port, SX->Busy_Pin);
}

void SX1262_BusyWait(SX1262* SX){
	while(HAL_GPIO_ReadPin(SX->Busy_Port, SX->Busy_Pin)) {}
}

void SX1262_WriteReg(SX1262* SX, uint16_t addr, uint8_t val){

	SX->TX_Buf[0] = SX126X_CMD_WRITE_REGISTER;
	SX->TX_Buf[1] = (uint8_t)(addr >> 8); // address high byte
	SX->TX_Buf[2] = (uint8_t)(addr & 0xFF); // address low byte
	SX->TX_Buf[3] = val; // value to write

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 4, 100);
	SX1262_CSHigh(SX);
}

uint8_t SX1262_ReadReg(SX1262* SX, uint16_t addr){

	SX->TX_Buf[0] = SX126X_CMD_READ_REGISTER;
	SX->TX_Buf[1] = (uint8_t)(addr >> 8); // address high byte
	SX->TX_Buf[2] = (uint8_t)(addr & 0xFF); // address low byte

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_TransmitReceive(&SX->SPI, SX->TX_Buf, SX->RX_Buf, 5, 100); // 3 command bytes, 1 wait, 1 response

	SX1262_CSHigh(SX);

	return SX->RX_Buf[4];
}

void SX1262_WriteBuffer(SX1262* SX, uint8_t* data, uint8_t len){
	SX->TX_Buf[0] = SX126X_CMD_WRITE_BUFFER;
	SX->TX_Buf[1] = 0x00; // no offset, read from start

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 2, 100); // command and offset
	HAL_SPI_Transmit(&SX->SPI, data, len, 100); // buffer contents
	SX1262_CSHigh(SX);
}

uint8_t* SX1262_ReadBuffer(SX1262* SX, uint8_t len){
	SX->TX_Buf[0] = SX126X_CMD_READ_BUFFER;
	SX->TX_Buf[1] = 0x00; // no offset
	SX->TX_Buf[1] = 0x00; // dummy byte

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 3, 100); // command and offset/null
	HAL_SPI_Receive(&SX->SPI, SX->Packet_Buf, SX126X_PACKET_LEN, 100); // buffer contents
	SX1262_CSHigh(SX);

	return SX->Packet_Buf;
}


void SX1262_SetStandby(SX1262* SX, uint8_t mode){
	SX->State = RADIO_IDLE;

	SX->TX_Buf[0] = SX126X_CMD_SET_STANDBY;
	SX->TX_Buf[1] = mode; // mode parameter

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 2, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetRx(SX1262* SX, uint32_t timeout){
	SX->TX_Buf[0] = SX126X_CMD_SET_RX;
	SX->TX_Buf[1] = (timeout >> 16) & 0xFF;
	SX->TX_Buf[2] = (timeout >> 8) & 0xFF;
	SX->TX_Buf[3] = (timeout & 0xFF);

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 4, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetTx(SX1262* SX, uint32_t timeout){
	SX->TX_Buf[0] = SX126X_CMD_SET_TX;
	SX->TX_Buf[1] = (timeout >> 16) & 0xFF;
	SX->TX_Buf[2] = (timeout >> 8) & 0xFF;
	SX->TX_Buf[3] = (timeout & 0xFF);

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 4, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetPacketType(SX1262* SX, uint8_t type){
	SX->TX_Buf[0] = SX126X_CMD_SET_PACKET_TYPE;
	SX->TX_Buf[1] = type; // type parameter

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 2, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetRfFrequency(SX1262* SX, double freq_hz){

	uint32_t freq_raw = (uint32_t)(freq_hz/SX126X_FREQUENCY_STEP_SIZE);

	SX->TX_Buf[0] = SX126X_CMD_SET_RF_FREQUENCY;
	SX->TX_Buf[1] = (freq_raw >> 24) & 0xFF;
	SX->TX_Buf[2] = (freq_raw >> 16) & 0xFF;
	SX->TX_Buf[3] = (freq_raw >> 8) & 0xFF;
	SX->TX_Buf[4] = freq_raw & 0xFF;

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 5, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetPaConfig(SX1262* SX, uint8_t paDutyCycle, uint8_t hpMax){
	SX->TX_Buf[0] = SX126X_CMD_SET_PA_CONFIG; // set pa config command
	SX->TX_Buf[1] = paDutyCycle;
	SX->TX_Buf[2] = hpMax;
	SX->TX_Buf[3] = 0x00; // 0x00 for SX1262
	SX->TX_Buf[4] = 0x01; // reserved, always 0x01

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 5, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetTxParams(SX1262* SX, int8_t power, uint8_t rampTime){
	SX->TX_Buf[0] = SX126X_CMD_SET_TX_PARAMS; // set tx params command
	SX->TX_Buf[1] = (uint8_t)power;
	SX->TX_Buf[2] = rampTime;

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 3, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetBufferBaseAddress(SX1262* SX, uint8_t txBaseAddress, uint8_t rxBaseAddress){
	SX->TX_Buf[0] = SX126X_CMD_SET_BUFFER_BASE_ADDRESS; // set buffer bass address command
	SX->TX_Buf[1] = txBaseAddress;
	SX->TX_Buf[2] = rxBaseAddress;

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 3, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetModulationParamsLoRa(SX1262* SX, uint8_t sf, uint8_t bw, uint8_t cr, uint8_t ldr){
	SX->TX_Buf[0] = SX126X_CMD_SET_MODULATION_PARAMS; // set modulation params command
	SX->TX_Buf[1] = sf;
	SX->TX_Buf[2] = bw;
	SX->TX_Buf[3] = cr;
	SX->TX_Buf[4] = ldr;


	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 5, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetPacketParamsLoRa(SX1262* SX, uint16_t preambleLength, uint8_t headerType, uint8_t payloadLength, uint8_t crcType, uint8_t invertIQ){
	SX->TX_Buf[0] = SX126X_CMD_SET_PACKET_PARAMS; // set packet params command
	SX->TX_Buf[1] = (uint8_t)(preambleLength >> 8);
	SX->TX_Buf[2] = (uint8_t)(preambleLength & 0xFF);
	SX->TX_Buf[3] = headerType;
	SX->TX_Buf[4] = payloadLength;
	SX->TX_Buf[5] = crcType;
	SX->TX_Buf[6] = invertIQ;


	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 7, 100);
	SX1262_CSHigh(SX);
}

void SX1262_SetDioIrqParams(SX1262* SX, uint16_t irqMask, uint16_t dio1Mask){
	SX->TX_Buf[0] = SX126X_CMD_SET_DIO_IRQ_PARAMS;
	SX->TX_Buf[1] = (uint8_t)(irqMask >> 8);
	SX->TX_Buf[2] = (uint8_t)(irqMask & 0xFF);
	SX->TX_Buf[3] = (uint8_t)(dio1Mask >> 8);
	SX->TX_Buf[4] = (uint8_t)(dio1Mask & 0xFF);
	SX->TX_Buf[5] = 0x00; // DIO2 inaccessible
	SX->TX_Buf[6] = 0x00;
	SX->TX_Buf[7] = 0x00; // DIO3 inaccessible
	SX->TX_Buf[8] = 0x00;


	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 9, 100);
	SX1262_CSHigh(SX);
}

uint16_t SX1262_GetIrqStatus(SX1262* SX){

	SX->TX_Buf[0] = SX126X_CMD_GET_IRQ_STATUS; // read register command


	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_TransmitReceive(&SX->SPI, SX->TX_Buf, SX->RX_Buf, 4, 100); // 1 command byte, 1 wait, 2 response
	SX1262_CSHigh(SX);

	uint16_t result = ((uint16_t)SX->RX_Buf[2] << 8) | (uint16_t)SX->RX_Buf[3];
	return result;
}

void SX1262_ClearIrqStatus(SX1262* SX, uint16_t clearParam){
	SX->TX_Buf[0] = SX126X_CMD_CLEAR_IRQ_STATUS;
	SX->TX_Buf[1] = (uint8_t)(clearParam >> 8);
	SX->TX_Buf[2] = (uint8_t)(clearParam & 0xFF);

	SX1262_BusyWait(SX);

	SX1262_CSLow(SX);
	HAL_SPI_Transmit(&SX->SPI, SX->TX_Buf, 3, 100);
	SX1262_CSHigh(SX);
}




void SX1262_RxMode(SX1262* SX){
	SX1262_SetDioIrqParams(SX, SX126X_IRQ_RX_DONE, SX126X_IRQ_RX_DONE); // assign RX done interrupt to DIO1
	SX1262_SetRx(SX, SX126X_RX_TIMEOUT_INF); // setup rx continuous mode
	SX->State = RADIO_RX;
}

void SX1262_TxMode(SX1262* SX){
	SX1262_SetDioIrqParams(SX, SX126X_IRQ_TX_DONE, SX126X_IRQ_TX_DONE);
	SX1262_SetTx(SX, SX126X_TX_TIMEOUT_NONE); // no timeout for TX
	SX->State = RADIO_TX;
}


void SX1262_HandleCallback(SX1262* SX){
	switch(SX->State){
		case RADIO_RX:
			// New packet received
			uint16_t status = SX1262_GetIrqStatus(SX);
			SX1262_ClearIrqStatus(SX, SX126X_IRQ_ALL);
			if(!(status & SX126X_IRQ_CRC_ERR)){
				// packet not corrupted
				SX1262_ReadBuffer(SX, SX126X_PACKET_LEN);
				// call packet received callback
				(SX->RX_Callback)(SX->Packet_Buf, SX126X_PACKET_LEN);
			}
			// in RX continuous mode, no need to set back to rx mode
			break;
		case RADIO_TX:
			// Packet finished transmitting, set back to RX mode
			SX1262_RxMode(SX);
			break;
		default:
			// ????
			break;
	}
}




void SX1262_Transmit(SX1262* SX, uint8_t* data){
	// TODO: THE LENGTH IS HARDCODED!!! BAD!!
	SX1262_WriteBuffer(SX, data, SX126X_PACKET_LEN);

	SX1262_BusyWait(SX);
	SX1262_TxMode(SX);
}

void SX1262_Init(SX1262* SX){
	// Toggle reset
	HAL_GPIO_WritePin(SX->Reset_Port, SX->Reset_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(SX->Reset_Port, SX->Reset_Pin, GPIO_PIN_SET);

	SX1262_SetStandby(SX, SX126X_STANDBY_XOSC); // standby with crystal oscillator (instead of RC)
	SX1262_SetPacketType(SX, SX126X_PACKET_TYPE_LORA); // set up as lora radio (instead of GFSK)
	SX1262_SetRfFrequency(SX, 915000000); // set frequency to 915.0 MHz
	SX1262_SetPaConfig(SX, SX126X_PA_CONFIG_DUTY_CYCLE, SX126X_PA_CONFIG_HP_MAX); // from table 13-21
	SX1262_SetTxParams(SX, 22, SX126X_PA_RAMP_20U); // 22dBm, 20us ramp time
	SX1262_SetBufferBaseAddress(SX, 0x00, 0x00); // no fancy buffer, just make them both start at 0x00
	SX1262_SetModulationParamsLoRa(SX, 7, SX126X_LORA_BW_10_4, SX126X_LORA_CR_4_5, SX126X_LORA_LOW_DATA_RATE_OPTIMIZE_OFF); //SF5, 62.5khz BW, CR 4/5. no LDRO
	SX1262_SetPacketParamsLoRa(SX, 12, SX126X_LORA_HEADER_IMPLICIT, SX126X_PACKET_LEN, SX126X_LORA_CRC_ON, SX126X_LORA_IQ_STANDARD); // 12 byte preamble, fixed length message, 255 byte message, crc on, normal IQ

	SX1262_RxMode(SX); // set up IRQs and go into RX mode
}


