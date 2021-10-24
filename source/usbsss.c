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

//Comando para conectarse a la red micros
const char CWJAP[]={'A','T','+','C','W','J','A','P','=','"','M','I','C','R','O','S','"',',','"','m','i','c','r','o','s','1','2','3','4','5','6','7','"','\r','\n'};
const char CWJAP2[]={'A','T','+','C','W','J','A','P','=','"','N','u','n','o','"',',','"','e','s','s','6','-','8','y','i','v','-','0','p','w','t','"','\r','\n'};
//comando CIFSR Gets the local IP Adress -- Obtener la direccion ip local
const char CIFSR[]={"AT+CIFSR\r\n"};
//Comando CIPMUX Enable single connection -- Activa una sola coneccion
const char CIPMUX[]={"AT+CIPMUX=0\r\n"};
//Comando CIPSTART Establish UDP Transmition -- Establece transmision UDP
const char CIPSTART[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
		'"',',','"','1','9','2','.','1','6','8','.','1','.','2','"',',','3','0','0','1','5',',','3','0','2','0','\r','\n'};
//CIPSTART 2
const char CIPSTART2[]={",30015,3020\r\n"};
//Comando CIPSEND Sends data -- Enviar datos
const char CIPSEND[]={"AT+CIPSEND="};
//Comando CIPCLOSE Close the UDP Connection -- Cierra la conexion UDP
const char CIPCLOSE[]={"AT+CIPCLOSE\r\n"};
//Comando CWQAP Disconnect from the AP -- Desconecta de la AP
const char CWQAP[]={"AT+CWQAP\r\n"};
//Comando CWMODE Sets the Wi-Fi mode (= 3 Station mode) -- Establece el modo Wi-Fi
const char CWMODE[]={"AT+CWMODE=3\r\n"};

//Respuesta CWQAP
const char ANS_CWQAP[]={"AT+CWQAP\r\n\r\nOK\r\n"};
//Respuesta CWMODE
const char ANS_CWMODE[]={"AT+CWMODE=3\r\n\r\nOK\r\n"};
//Respuesta CWJAP
const char ANS_CWJAP[]={'A','T','+','C','W','J','A','P','=','"','M','I','C','R','O','S','"',',','"','m','i','c','r','o','s','1','2','3','4','5','6','7',
						'"','\r','\n','W','I','F','I',' ','C','O','N','N','E','C','T','E','D','\r','\n','W','I','F','I',
						' ','G','O','T','I','P','\r','\n','\r','\n','O','K','\r','\n'};

const char ANS_CWJAP2[]={'A','T','+','C','W','J','A','P','=','"','N','u','n','o','"',',','"','e','s','s','6','-','8','y','i','v','-','0','p','w','t',
						'"','\r','\n','W','I','F','I',' ','C','O','N','N','E','C','T','E','D','\r','\n','W','I','F','I',
						' ','G','O','T','I','P','\r','\n','\r','\n','O','K','\r','\n'};

//Respuesta CIPMUX
const char ANS_CIPMUX[]={"AT+CIPMUX=0\r\n\r\nOK\r\n"};
//Respuesta CIPSTART
const char ANS_CIPSTART[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
		'"',',','"','1','9','2','.','1','6','8','.','1','.','2','"',',','3','0','0','1','5',',','3','0','2','0','\r',
		'\n','C','O','N','N','E','C','T','\r','\n','\r','\n','O','K','\r','\n'};//59
//Respuesta CIPSEND
const char ANS_CIPSEND[]={};
const char AUTOMATIC_WIFI_CONNECTED[]={"WIFI CONNECTED\r\nWIFI GOT IP\r\n"};

//OBTENER IP AUTITO
const char CIFSR_STAIP[]={"+CIFSR:STAIP,"};
//Respuesta OK
const char OK[]={"\r\nOK\r\n"};
//CIPSEND 4 BYTES
const char CIPSEND_4BYTES[]={'A','T','+','C','I','P','S','E','N','D','=','4','\r','\n','\r','\n','O','K','\r','\n','>'};
const char CIPSEND_4BYTES2[]={"Recv 4 bytes\r\n\r\nSEND OK\r\n"};//25

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
uint8_t statusAT = 0, readyToSend = 1, lIp = 0,statusESP, timeout2 = 0;
char espIP[13];
uint8_t o_recibe = 0, k_recibe = 0, lastcommand = 1,sizecommand=6;
uint8_t coincidencias = 0, statusCIFSR = 0;

void LeerCabecera(uint8_t ind);
void RecibirDatos(uint8_t head);
void EnviarDatos(uint8_t cmd);
void ESP_UART3_SEND(void);
void BanderasComandos(void);
void initESP(void);
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
    //PRINTF("Hello World\n");

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {



    	USB_DeviceInterface0CicVcomTask();
    	BanderasComandos();

    	switch(statusESP){
			case 0:
				initESP();
			break;
    	}

    	if (ringRx.iW != ringRx.iR) {
    		LeerCabecera(ringRx.iW);

    	}
    	if((espRx.iW != espRx.iR) && (!timeout2)){
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

void initESP(void){
	if(readyToSend)
		switch(statusAT){
			case 0:
				memcpy(&espTx.buf[espTx.iW], CWMODE, 13);
				espTx.iW += 13;
				timeout2 = 15;
				readyToSend = 0;
			break;
			case 1:
				memcpy(&espTx.buf[espTx.iW], CWJAP2, 34);
				espTx.iW += 34;
				timeout2 = 30;
				readyToSend = 0;
			break;
			case 2:
				memcpy(&espTx.buf[espTx.iW], CIPMUX, 13);
				espTx.iW += 13;
				timeout2 = 20;
				readyToSend = 0;
			break;
			case 3:
				memcpy(&espTx.buf[espTx.iW], CIFSR, 10);
				espTx.iW += 10;
				timeout2 = 15;
				readyToSend = 0;
			break;

		}
}

void DecoEsp(void){

	switch(statusAT){
	case 0:
		if(espRx.buf[espRx.iR]==ANS_CWMODE[coincidencias]){
			coincidencias++;
			if(coincidencias == 19){
				statusAT++;
				coincidencias = 0;
				readyToSend = 1;
			}
		}
		espRx.iR++;
	break;
	case 1:
		if(espRx.buf[espRx.iR]==ANS_CWJAP2[coincidencias]){
			coincidencias++;
			if(coincidencias == 68){
				statusAT++;
				coincidencias = 0;
				readyToSend = 1;
			}
		}
		espRx.iR++;
	break;
	case 2:
		if(espRx.buf[espRx.iR]==ANS_CIPMUX[coincidencias]){
			coincidencias++;
			if(coincidencias == 13){
				statusAT++;
				coincidencias = 0;
				readyToSend = 1;
			}
		}
		espRx.iR++;
	break;
	case 3:
		switch(statusCIFSR){
			case 0:
				if(espRx.buf[espRx.iR]==CIFSR[coincidencias]){
					coincidencias++;
					if(coincidencias == 10){
						//statusAT++;
						coincidencias = 0;
						statusCIFSR++;
						//readyToSend = 1;
					}
				}

			break;
			case 1:
				if(espRx.buf[espRx.iR]==CIFSR_STAIP[coincidencias]){
					coincidencias++;
					if(coincidencias == 13){
						//statusAT++;
						coincidencias = 0;
						statusCIFSR++;
						//readyToSend = 1;
					}
				}
			break;
			case 2:
				espIP[coincidencias] = espRx.buf[espRx.iR];
				coincidencias++;
				lIp++;
				if ((espRx.buf[espRx.iR] == '"') && (lIp>1)) {
					coincidencias = 0;
					statusCIFSR++;
				}
			break;
			case 3:
				if(espRx.buf[espRx.iR]==OK[coincidencias]){
					coincidencias++;
					if(coincidencias == 6){
						statusAT++;
						coincidencias = 0;
						statusCIFSR=0;
						readyToSend = 1;
					}
				}
			break;
		}
		espRx.iR++;
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

/* PIT0_IRQn interrupt handler */
void PIT_CHANNEL_0_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);

  /* Place your code here */

  if(timeout2){
	  timeout2--;
  }

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}
