/*
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    usbsss.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "Util.h"
#include "usb_device_interface_0_cic_vcom.h"
#include "string.h"


void UART3_SERIAL_RX_TX_IRQHANDLER(void);

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t timerCounter;
volatile _rx ringRx;
volatile _tx ringTx;
_rx espRx;
_tx espTx;


uint8_t rxBuf[256], txBuf[256], rxEspBuf[256],txEspBuf[256];

/* PIT0_IRQn interrupt handler */
void PIT_CHANNEL_0_IRQHANDLER(void);
void UART3_SERIAL_RX_TX_IRQHANDLER(void);


// typedef
typedef union{
	struct{
		uint8_t b0: 1;
		uint8_t b1: 1;
		uint8_t b2: 1;
		uint8_t b3: 1;
		uint8_t b4: 1;
		uint8_t b5: 1;
		uint8_t b6: 1;
		uint8_t b7: 1;
	}bit;
	uint8_t byte;
}_sFlag;


typedef union {
    uint8_t u8[4];
    int8_t i8[4];
    uint16_t u16[2];
    int16_t i16[2];
    int32_t i32;
    uint32_t u32;
    float f;
} _sWork;


//..
// Defines
#define ALIVESENT flag1.bit.b0 //enviar alive
#define MOTORSSENT flag1.bit.b1 //Confirmacion recepcion comando motor
#define MOTORONOFFSENT flag1.bit.b2 //Confirmacion recepcion motor on/off
#define INITESP flag1.bit.b3 //iniciar ESP
#define INITESPCMD 0xC0 //Inicializar ESP
#define MOTORSCMD 0xD0 //comando motor
#define MOTORSONOCMD 0xD2 // COMANDO ON/OFF MOTOR
#define ALIVECMD 0xF0 // comando alive

// Variables Globales
volatile _sFlag flag1;
_sWork PWM1, PWM2;
uint8_t o_recibe = 0, k_recibe = 0, lastcommand = 1,sizecommand=6;


void LeerCabecera(uint8_t ind);
void RecibirDatos(uint8_t head);
void EnviarDatos(uint8_t cmd);
void ESP_UART3_SEND(void);
void BanderasComandos(void);
void DecoEsp(void);


//str
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    	ringTx.buf=txBuf;
        ringRx.buf=rxBuf;
        espTx.buf=txEspBuf;
        espRx.buf=rxEspBuf;
        ringTx.iW=0;
        ringTx.iR=0;
        ringRx.iW=0;
        ringRx.iR=0;
        espRx.iW=0;
        espRx.iR=0;
        espTx.iW=0;
        espTx.iR=0;
        espTx.buf[espTx.iW++] = 'A';
        espTx.buf[espTx.iW++] = 'T';
        espTx.buf[espTx.iW++] = 0xD;
        espTx.buf[espTx.iW++] = 0xA;
    //PRINTF("Hello World\n");

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {



    	USB_DeviceInterface0CicVcomTask();
    	BanderasComandos();

    	if (ringRx.iW != ringRx.iR) {
    		LeerCabecera(ringRx.iW);

    	}
    	if(espRx.iW != espRx.iR){
    		DecoEsp();
    		//espRx.iR++;
    	}
    	if(espTx.iW != espTx.iR){
    	   ESP_UART3_SEND();
    	}

    	i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}

void DecoEsp(void){

	switch(espRx.buf[espRx.iR+sizecommand]){
		case 'O':
			o_recibe = 1;
			espRx.iR++;
		break;
		case 'K':
			k_recibe = 1;
			espRx.iR++;
			espRx.iR += sizecommand+2;
			lastcommand = 0;
		break;
	}
}

void ESP_UART3_SEND(void){
	if(kUART_TxDataRegEmptyFlag & UART_GetStatusFlags(UART3)){
		UART_WriteByte(UART3, espTx.buf[espTx.iR++]);
	}
}

void BanderasComandos(void){

	if (ALIVESENT){
		//GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
		EnviarDatos(ALIVECMD);
	}

	if (MOTORSSENT){
	    //GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
	    //funcion motor pwm
	    EnviarDatos(MOTORSCMD);
	}

	if (MOTORONOFFSENT){
		   // GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
		    //MOTORES ON OFF
		    EnviarDatos(MOTORSONOCMD);
	}

}

void LeerCabecera(uint8_t ind){

	while(ind != ringRx.iR)
	{
		switch (ringRx.header)
		{
			case 0:
			if (ringRx.buf[ringRx.iR] == 'U')
				ringRx.header++;
			else{
				ringRx.header = 0;
				ringRx.iR--;
			}
			break;
			case 1:
			if (ringRx.buf[ringRx.iR] == 'N')
				ringRx.header++;
			else{
				ringRx.header = 0;
				ringRx.iR--;
			}
			break;
			case 2:
			if (ringRx.buf[ringRx.iR] == 'E')
				ringRx.header++;
			else{
				ringRx.header = 0;
				ringRx.iR--;
			}
			break;
			case 3:
			if (ringRx.buf[ringRx.iR] == 'R')
				ringRx.header++;
			else{
				ringRx.header = 0;
				ringRx.iR--;
			}
			break;
			case 4:
				ringRx.nBytes = ringRx.buf[ringRx.iR];
				ringRx.header++;
			break;
			case 5:
			if (ringRx.buf[ringRx.iR] == 0x00)
				ringRx.header++;
			else{
				ringRx.header = 0;
				ringRx.iR--;
			}
			break;
			case 6:
			if (ringRx.buf[ringRx.iR] == ':')
			{
				ringRx.cks= 'U'^'N'^'E'^'R'^ringRx.nBytes^0x00^':';
				ringRx.header++;
				ringRx.iData = ringRx.iR+1;
			}
			else{
				ringRx.header = 0;
				ringRx.iR--;
			}
			break;

			case 7:
			if(ringRx.nBytes>1){
				ringRx.cks^=ringRx.buf[ringRx.iR];
			}
			ringRx.nBytes--;
			if(ringRx.nBytes==0){
				ringRx.header=0;
				if(ringRx.cks==ringRx.buf[ringRx.iR]){
					RecibirDatos(ringRx.iData);
				}
			}
			break;
			default:
				ringRx.header = 0;
			break;
		}
		ringRx.iR++;
	}
}

void RecibirDatos(uint8_t head){
	switch (ringRx.buf[head++]){
		case 0xF0:
			ALIVESENT = 1;
			//algo
		break;
		case MOTORSCMD:
			PWM1.u8[0] = ringRx.buf[head++];
			PWM1.u8[1] = ringRx.buf[head++];
			PWM1.u8[2] = ringRx.buf[head++];
			PWM1.u8[3] = ringRx.buf[head++];
			PWM2.u8[0] = ringRx.buf[head++];
			PWM2.u8[1] = ringRx.buf[head++];
			PWM2.u8[2] = ringRx.buf[head++];
			PWM2.u8[3] = ringRx.buf[head++];
			MOTORSSENT = 1; //CONFIRMACION QUE RECIBI LOS DATOS
		break;
		case MOTORSONOCMD:
			MOTORSSENT = 1;
		break;
	}
}

void EnviarDatos(uint8_t cmd){
	ringTx.buf[ringTx.iW++]='U';
	ringTx.buf[ringTx.iW++]='N';
	ringTx.buf[ringTx.iW++]='E';
	ringTx.buf[ringTx.iW++]='R';

	switch(cmd){
		case ALIVECMD:
			ringTx.buf[ringTx.iW++] = 0x02;
			ringTx.buf[ringTx.iW++] = 0x00;
			ringTx.buf[ringTx.iW++] = ':';
			ringTx.buf[ringTx.iW++] = cmd;
			ALIVESENT = 0;
		break;
		case MOTORSCMD:
			ringTx.buf[ringTx.iW++] = 0x02;
			ringTx.buf[ringTx.iW++] = 0x00;
			ringTx.buf[ringTx.iW++] = ':';
			ringTx.buf[ringTx.iW++] = cmd;
			MOTORSSENT = 0;
		break;
		case MOTORSONOCMD:
			ringTx.buf[ringTx.iW++] = 0x02;
			ringTx.buf[ringTx.iW++] = 0x00;
			ringTx.buf[ringTx.iW++] = ':';
			ringTx.buf[ringTx.iW++] = cmd;
			MOTORONOFFSENT = 0;
		break;
		default:
		break;
	}

	ringTx.cks=0;
	for(uint8_t i=ringTx.iR; i<ringTx.iW; i++) {
		ringTx.cks^=ringTx.buf[i];
		//pc.printf("%d - %x - %d   v: %d \n",i,cks,cks,tx[i]);
	}
	if(ringTx.cks>0)
		ringTx.buf[ringTx.iW++]=ringTx.cks;
}



/* UART3_RX_TX_IRQn interrupt handler */
void UART3_SERIAL_RX_TX_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status registers */
  intStatus = UART_GetStatusFlags(UART3_PERIPHERAL);

  /* Flags can be cleared by reading the status register and reading/writing data registers.
    See the reference manual for details of each flag.
    The UART_ClearStatusFlags() function can be also used for clearing of flags in case the content of data regsiter is not used.
    For example:
        status_t status;
        intStatus &= ~(kUART_RxOverrunFlag | kUART_NoiseErrorFlag | kUART_FramingErrorFlag | kUART_ParityErrorFlag);
        status = UART_ClearStatusFlags(UART3_PERIPHERAL, intStatus);
  */

  /* Place your code here */

  status_t status;
  char value;

  value = UART_ReadByte(UART3);
  espRx.buf[espRx.iW++] = value;

  status = UART_ClearStatusFlags(UART3, intStatus);


  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

