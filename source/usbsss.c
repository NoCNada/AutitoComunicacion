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
//Comando CWJAP? Para consultar si esoy conectado o no
const char CWJAP_PREGUNTA[]={"AT+CWJAP?\r\n"};//11

//comando CIFSR Gets the local IP Adress -- Obtener la direccion ip local
const char CIFSR[]={"AT+CIFSR\r\n"};
//Comando CIPMUX Enable single connection -- Activa una sola coneccion
const char CIPMUX[]={"AT+CIPMUX=0\r\n"};
//Comando CIPSTART Establish UDP Transmition -- Establece transmision UDP
const char CIPSTART[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
		'"',',','"','1','9','2','.','1','6','8','.','1','.','4','"',',','3','0','0','1','5',',','3','0','1','5','\r','\n'};
const char CIPSTART2[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
		'"',',','"','1','9','2','.','1','6','8','.','0','.','1','3','5','"',',','3','0','0','1','5',',','3','0','1','5','\r','\n'};
//CIPSTART 2
const char CIPSTART2222[]={",30015,3020\r\n"};
//Comando CIPSEND Sends data -- Enviar datos
const char CIPSEND[]={"AT+CIPSEND="};
//Comando CIPCLOSE Close the UDP Connection -- Cierra la conexion UDP
const char CIPCLOSE[]={"AT+CIPCLOSE\r\n"};
//Comando CWQAP Disconnect from the AP -- Desconecta de la AP
const char CWQAP[]={"AT+CWQAP\r\n"};
//Comando CWMODE Sets the Wi-Fi mode (= 3 Station mode) -- Establece el modo Wi-Fi
const char CWMODE[]={"AT+CWMODE=3\r\n"};
//Comando CWLAP
const char CWLAP[]={"AT+CWLAP\r\n"};


//Respuesta CWLAP ANS_CWLAP
const char ANS_CWLAP[]={"AT+CWLAP\r\n\r\n+CWLAP:\r\n"};
//Respuesta CWQAP
const char ANS_CWQAP[]={"AT+CWQAP\r\n\r\nOK\r\n"};
const char ANS_CWQAP2[]={"AT+CWQAP\r\n\r\nOK\r\nWIFI DISCONNECT/r/n"};
//Respuesta CWMODE
const char ANS_CWMODE[]={"AT+CWMODE=3\r\n\r\nOK\r\n"};
//Respuesta CWJAP
const char ANS_CWJAP[]={'A','T','+','C','W','J','A','P','=','"','M','I','C','R','O','S','"',',','"','m','i','c','r','o','s','1','2','3','4','5','6','7',
						'"','\r','\n','W','I','F','I',' ','C','O','N','N','E','C','T','E','D','\r','\n','W','I','F','I',
						' ','G','O','T','I','P','\r','\n','\r','\n','O','K','\r','\n'};

const char ANS_CWJAP_ERROR_FAIL[]={'+','C','W','J','A','P',':','3','\r','\n','\r','\n','F','A','I','L','\r','\n'};


const char ANS_CWJAP2[]={'A','T','+','C','W','J','A','P','=','"','N','u','n','o','"',',','"','e','s','s','6','-','8','y','i','v','-','0','p','w','t',
						'"','\r','\n','W','I','F','I',' ','C','O','N','N','E','C','T','E','D','\r','\n','W','I','F','I',
						' ','G','O','T','I','P','\r','\n','\r','\n','O','K','\r','\n'};

//const char ANS_CWJAP2_ERROR_FAIL[]={'+','C','W','J','A','P',':','3','/r','/n','/r','n','F','A','I','L','/r','/n'};

//Respuesta CWJAP?
const char ANS_CWJAP_PREGUNTA[] = {"AT+CWJAP?\r\nNo AP\r\n\r\nOK\r\n"};//24
//const char ANS_CWJAP_PREGUNTA2[] = {'AT+CWJAP?\r\n+CWJAP:"Nuno"'};//24
//Respuesta CIPMUX
const char ANS_CIPMUX[]={"AT+CIPMUX=0\r\n\r\nOK\r\n"};
//Respuesta CIPSTART
const char ANS_CIPSTART[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
		'"',',','"','1','9','2','.','1','6','8','.','1','.','4','"',',','3','0','0','1','5',',','3','0','1','5','\r',
		'\n','C','O','N','N','E','C','T','\r','\n','\r','\n','O','K','\r','\n'};//59

const char ANS_CIPSTART_ERROR[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
		'"',',','"','1','9','2','.','1','6','8','.','1','.','4','"',',','3','0','0','1','5',',','3','0','1','5','\r',
		'\n','A','L','R','E','A','D','Y',' ','C','O','N','N','E','C','T','E','D','\r','\n','\r','\n','E','R','R','O','R','\r','\n'};//70


const char ANS_CIPSTART2[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
		'"',',','"','1','9','2','.','1','6','8','.','0','.','1','3','5','"',',','3','0','0','1','5',',','3','0','1','5','\r',
		'\n','C','O','N','N','E','C','T','\r','\n','\r','\n','O','K','\r','\n'};//61
const char ANS_CIPSTART2_ERROR[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
		'"',',','"','1','9','2','.','1','6','8','.','0','.','1','3','5','"',',','3','0','0','1','5',',','3','0','1','5','\r',
		'\n','A','L','R','E','A','D','Y',' ','C','O','N','N','E','C','T','E','D','\r','\n','\r','\n','E','R','R','O','R','\r','\n'};//72

//AT+CIPSTART="UDP","192.168.1.4",30015,3015/r/n
//ALREADY CONNECTED/r/n/r/nERROR/r/n

//Busy p...
const char ANS_BUSY[]={"busy"};

//Respuesta CIPSEND
const char ANS_CIPSEND[]={};
const char AUTOMATIC_WIFI_CONNECTED[]={"WIFI CONNECTED\r\nWIFI GOT IP\r\n"};
//SSID
const char SSIDNUNO[]={'"','N','u','n','o','"'};

const char SSIDMICROS[]={'"','M','I','C','R','O','S','"'};
//OBTENER IP AUTITO
const char CIFSR_STAIP[]={"+CIFSR:STAIP,"};
//Respuesta OK
const char OK[]={"\r\nOK\r\n"};
//CIPSEND 4 BYTES
const char CIPSEND_4BYTES2[]={"Recv 4 bytes\r\n\r\nSEND OK\r\n"};//25
const char CIPSEND_4BYTES55[]={"AT+CIPSEND=4\r\n\r\nOK\r\n>"};
const char CIPSEND_1[]={"AT+CIPSEND="};//
const char CIPSEND_2[]={"\r\n\r\nOK\r\n>"};//9
const char CIPSEND_3[]={"Recv "};//5
const char CIPSEND_4[]={" bytes\r\n\r\nSEND OK\r\n"};//19
const char IPD[]={'+','I','P','D',','};


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
void SysTick_Handler(void);

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
#define SSIDNuno flag1.bit.b4 //iniciar ESP
#define SSIDMicros flag1.bit.b5 //iniciar ESP
#define RESETESP flag1.bit.b6 //RESET ESP
#define DESCONECTADO flag1.bit.b7 //RESET ESP
#define PrioridadRed1 flag2.bit.b0
#define REENVIARMENSAJE flag2.bit.b1
#define PrimerMensaje flag2.bit.b1
#define ALIVESENTESP flag2.bit.b2
#define MOTORSENTESP flag2.bit.b3
#define ALIVE30S flag2.bit.b4

#define INITESPCMD 0xC0 //Inicializar ESP
#define MOTORSCMD 0xD0 //comando motor
#define MOTORSONOCMD 0xD2 // COMANDO ON/OFF MOTOR
#define ALIVECMD 0xF0 // comando alive



// Variables Globales
volatile _sFlag flag1, flag2;
_sWork PWM1, PWM2;
uint8_t statusAT = 0, readyToSend = 1, lIp = 0,statusESP,parte1 = 1,timeoutESP = 100, timeoutRead = 100, timeout3 = 0;
uint16_t timeout2 = 0, timeoutPrueba = 0;
char espIP[20],CIPSEND_NBYTES[30];
uint8_t coincidencias = 0, statusCIFSR = 0, statusDecoCIPSEND = 0,coincidencias2 = 0,statusCWQAP = 0;
uint8_t bytesToSend = 0, bytesToSend_aux = 0;
uint32_t g_systickCounter;
uint8_t test[256];
uint8_t NADADEPRUEBAS = 1;
uint8_t iii = 0;

void LeerCabecera(uint8_t ind);
void RecibirDatos(uint8_t head);
void EnviarDatos(uint8_t cmd);
void ESP_UART3_SEND(void);
void BanderasComandos(void);
void initESP(void);
void DecoEsp(void);
void CommandUdp(uint8_t comando);
void Delay_ms(uint32_t n);
uint8_t BusyESP(uint8_t c);
void resetESP(void);
void RecibirComandoESP(uint8_t comando);

void SysTick_Handler(void){
	if(g_systickCounter != 0U){
		g_systickCounter--;
	}
}

void Delay_ms(uint32_t n){
	g_systickCounter = n;
	while(g_systickCounter != 0U){
	}
}


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
    if(SysTick_Config(SystemCoreClock / 1000U)){
    	while(1){
    	}
    }

//    FTM_StopTimer(FTM0_PERIPHERAL);
//    FTM0_PERIPHERAL->CONTROLS[3].CnV = 3000;
//    FTM0_PERIPHERAL->CONTROLS[1].CnV = 3999;
//    FTM_StartTimer(FTM0_PERIPHERAL, kFTM_SystemClock);
//
//    Delay_ms(2000);
//
//    FTM_StopTimer(FTM0_PERIPHERAL);
//    FTM0_PERIPHERAL->CONTROLS[3].CnV = 0;
//    FTM0_PERIPHERAL->CONTROLS[1].CnV = 0;
//    FTM_StartTimer(FTM0_PERIPHERAL, kFTM_SystemClock);
//    Delay_ms(10000);

            /* Init output LED GPIO. */
          //  GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);
         //   GPIO_PinInit(BOARD_RST_ESP_GPIO, BOARD_RST_ESP_PIN, config)
    /* Force the counter to be placed into memory. */
    PrioridadRed1 = 1;
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {



    	USB_DeviceInterface0CicVcomTask();
    	BanderasComandos();
    	if(!timeoutESP){
        	switch(statusESP){
        		case 0:
        			resetESP();
        		break;

    			case 1:
    				initESP();
    			break;
    			case 2:
    				//lll
    				CommandUdp(0xF0);
    			break;
    			case 3:
//    				if(!timeoutPrueba && !ALIVESENTESP){
//    					CommandUdp(0xF0);
//    				}

    			break;
        	}
    	}


    	if (ringRx.iW != ringRx.iR) {
    		LeerCabecera(ringRx.iW);

    	}

    	if((espRx.iW != espRx.iR) && (!timeout2)){
    		DecoEsp();
    		//espRx.iR++;
    	}
    	if((espRx.iW == espRx.iR) && (!timeout3) && (REENVIARMENSAJE)){
    		switch(statusAT){
    		case 4:
        		coincidencias =0;
        		statusCIFSR = 0;
        		REENVIARMENSAJE = 0;
        		readyToSend = 1;
    		break;
    		case 5:
				coincidencias = 0;
				coincidencias2 = 0;
				readyToSend = 1;
				REENVIARMENSAJE = 0;
			break;
    		}



    	}
    	if(espTx.iW != espTx.iR){
    	   ESP_UART3_SEND();
    	}

//    	if(!timeoutPrueba){
//    		ALIVE30S = 1;
//    	}
    	if(ALIVESENTESP){
    		CommandUdp(0xF0);
    	}
    	if(MOTORSENTESP){
    		CommandUdp(0xF1);
    	}

    	i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}

void resetESP(void){
	//GPIO_PortToggle(BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN);
	if(!RESETESP){
		GPIO_PortToggle(BOARD_RST_ESP_GPIO, 1u << BOARD_RST_ESP_PIN);
		//GPIO_PortToggle(base, mask)
		//GPIO_PinWrite(BOARD_RST_ESP_PORT, BOARD_RST_ESP_PIN, 0);
		RESETESP = 1;
		timeoutESP = 50;
	}
	else{
		timeoutRead = 100;
		timeoutESP = 100;
		GPIO_PortToggle(BOARD_RST_ESP_GPIO, 1u << BOARD_RST_ESP_PIN);
		RESETESP = 0;
		statusESP++;
	}

}

void CommandUdp(uint8_t comando){
	if(readyToSend){
		switch(comando){
			case 0xF0:
				if(parte1){
					//memcpy(&espTx.buf[espTx.iW], CIPSEND, 11);
					espTx.buf[espTx.iW++] = 'A';
					espTx.buf[espTx.iW++] = 'T';
					espTx.buf[espTx.iW++] = '+';
					espTx.buf[espTx.iW++] = 'C';
					espTx.buf[espTx.iW++] = 'I';
					espTx.buf[espTx.iW++] = 'P';
					espTx.buf[espTx.iW++] = 'S';
					espTx.buf[espTx.iW++] = 'E';
					espTx.buf[espTx.iW++] = 'N';
					espTx.buf[espTx.iW++] = 'D';
					espTx.buf[espTx.iW++] = '=';
					espTx.buf[espTx.iW++] = '9';
					espTx.buf[espTx.iW++] = '\r';
					espTx.buf[espTx.iW++] = '\n';
					bytesToSend = 9;
					//espTx.iW += 11;
					//memcpy(&espTx.buf[espTx.iW], "4\r\n", 3);
					//espTx.iW += 3;
				//	const char CIPSEND_4BYTES2[]={"Recv 4 bytes\r\n\r\nSEND OK\r\n"};//25
				//	const char CIPSEND_4BYTES55[]={"AT+CIPSEND=4\r\n\r\nOK\r\n>"};
				//	memcpy(&CIPSEND_NBYTES,"AT+CIPSEND=4\r\n\r\nOK\r\n>",20);
				//	parte1=0;
				//	timeout2 = 10;
				//	readyToSend = 0;
				}
				else {
					//memcpy(&espTx.buf[espTx.iW], "test", 4);
					//espTx.iW += 4;
					//memcpy(&CIPSEND_NBYTES,"Recv 4 bytes\r\n\r\nSEND OK\r\n",25);
					espTx.buf[espTx.iW++] = 'U';
					espTx.buf[espTx.iW++] = 'N';
					espTx.buf[espTx.iW++] = 'E';
					espTx.buf[espTx.iW++] = 'R';
					espTx.buf[espTx.iW++] = 0x02;
					espTx.buf[espTx.iW++] = 0x00;
					espTx.buf[espTx.iW++] = ':';
					espTx.buf[espTx.iW++] = 0xF0;
					espTx.cks = 'U'^'N'^'E'^'R'^0x02^0x00^':'^0xF0;
					espTx.buf[espTx.iW++] = espTx.cks;
					readyToSend = 0;
					parte1=0;
					if(ALIVESENTESP)
						ALIVESENTESP = 0;
				//	timeout2 = 15;
				}

			break;
			case 0xF1:
				if(parte1){
					//memcpy(&espTx.buf[espTx.iW], CIPSEND, 11);
					espTx.buf[espTx.iW++] = 'A';
					espTx.buf[espTx.iW++] = 'T';
					espTx.buf[espTx.iW++] = '+';
					espTx.buf[espTx.iW++] = 'C';
					espTx.buf[espTx.iW++] = 'I';
					espTx.buf[espTx.iW++] = 'P';
					espTx.buf[espTx.iW++] = 'S';
					espTx.buf[espTx.iW++] = 'E';
					espTx.buf[espTx.iW++] = 'N';
					espTx.buf[espTx.iW++] = 'D';
					espTx.buf[espTx.iW++] = '=';
					espTx.buf[espTx.iW++] = '9';
					espTx.buf[espTx.iW++] = '\r';
					espTx.buf[espTx.iW++] = '\n';
					bytesToSend = 9;
					//espTx.iW += 11;
					//memcpy(&espTx.buf[espTx.iW], "4\r\n", 3);
					//espTx.iW += 3;
					//	const char CIPSEND_4BYTES2[]={"Recv 4 bytes\r\n\r\nSEND OK\r\n"};//25
					//	const char CIPSEND_4BYTES55[]={"AT+CIPSEND=4\r\n\r\nOK\r\n>"};
					//	memcpy(&CIPSEND_NBYTES,"AT+CIPSEND=4\r\n\r\nOK\r\n>",20);
					//	parte1=0;
					//	timeout2 = 10;
					//	readyToSend = 0;
				}
				else {
					//memcpy(&espTx.buf[espTx.iW], "test", 4);
					//espTx.iW += 4;
					//memcpy(&CIPSEND_NBYTES,"Recv 4 bytes\r\n\r\nSEND OK\r\n",25);
					espTx.buf[espTx.iW++] = 'U';
					espTx.buf[espTx.iW++] = 'N';
					espTx.buf[espTx.iW++] = 'E';
					espTx.buf[espTx.iW++] = 'R';
					espTx.buf[espTx.iW++] = 0x02;
					espTx.buf[espTx.iW++] = 0x00;
					espTx.buf[espTx.iW++] = ':';
					espTx.buf[espTx.iW++] = 0xF1;
					espTx.cks = 'U'^'N'^'E'^'R'^0x02^0x00^':'^0xF1;
					espTx.buf[espTx.iW++] = espTx.cks;
					readyToSend = 0;
					parte1=0;
					if(MOTORSENTESP)
						MOTORSENTESP = 0;
					//	timeout2 = 15;
				}

				break;
		}
		readyToSend = 0;
	}

}

void initESP(void){
	if(readyToSend)
		switch(statusAT){
			case 0:
				//memcpy(&espTx.buf[espTx.iW], CWQAP,10);
				for (uint8_t var = 0; var < 10; var++) {
					espTx.buf[espTx.iW++] = CWQAP[var];
				}
				//espTx.iW += 10;
				timeout2 = 30;
				readyToSend = 0;
			break;
//			case 1:
//				if(NADADEPRUEBAS){
//					statusAT++;
//					SSIDNuno = 1;
//					SSIDMicros = 0;
//				}
//				else{
//					memcpy(&espTx.buf[espTx.iW], CWLAP,10);
//					espTx.iW += 10;
//					timeout2 = 200;
//					readyToSend = 0;
//				}
//
//			break;
			case 1:
				//memcpy(&espTx.buf[espTx.iW], CWMODE, 13);
				//espTx.iW += 13;
				for (uint8_t var = 0; var < 13; var++) {
									espTx.buf[espTx.iW++] = CWMODE[var];
								}
				timeout2 = 30;
				readyToSend = 0;
			break;
			case 2:

				if(PrioridadRed1){
					//memcpy(&espTx.buf[espTx.iW], CWJAP2, 34);
					for (uint8_t var = 0; var < 34; var++) {
										espTx.buf[espTx.iW++] = CWJAP2[var];
									}
					//espTx.iW += 34;
					timeout2 = 100;
					readyToSend = 0;
				}
				else{
					//memcpy(&espTx.buf[espTx.iW], CWJAP, 35); //34 CASA 35 MICROS
					//espTx.iW += 35;
					for (uint8_t var = 0; var < 35; var++) {
										espTx.buf[espTx.iW++] = CWJAP[var];
									}
					timeout2 = 100;
					readyToSend = 0;
				}

			break;
			case 3:
				//memcpy(&espTx.buf[espTx.iW], CIPMUX, 13);
				//espTx.iW += 13;
				for (uint8_t var = 0; var < 13; var++) {
									espTx.buf[espTx.iW++] = CIPMUX[var];
								}
				timeout2 = 30;
				readyToSend = 0;
			break;
			case 4:
				//memcpy(&espTx.buf[espTx.iW], CIFSR, 10);
				//espTx.iW += 10;
				for (uint8_t var = 0; var < 10; var++) {
									espTx.buf[espTx.iW++] = CIFSR[var];
								}
				timeout2 = 60;
				timeout3 = 120;
				readyToSend = 0;
				REENVIARMENSAJE=1;
			break;
			case 5:
				if(SSIDNuno){
					//memcpy(&espTx.buf[espTx.iW], CIPSTART);
					//espTx.iW += 46;
					for (uint8_t var = 0; var < 46; var++) {
										espTx.buf[espTx.iW++] = CIPSTART2[var];
									}
					timeout2 = 50;
					timeout3 = 100;
					readyToSend = 0;
					REENVIARMENSAJE = 1;
				}
				else{
					if(SSIDMicros){
						//memcpy(&espTx.buf[espTx.iW], CIPSTART,44); //44 micros 46 casa
						//espTx.iW += 44;
						for (uint8_t var = 0; var < 44; var++) {
											espTx.buf[espTx.iW++] = CIPSTART[var];
										}
						timeout2 = 50;
						timeout3 = 100;
						readyToSend = 0;
						REENVIARMENSAJE = 1;
					}
				}
			break;
		}
}

uint8_t BusyESP(uint8_t c){
	static uint8_t epic = 0;
	if(c==ANS_BUSY[epic]){
		epic++;
		if(epic == 4){
			//statusAT++;
			epic = 0;
			return 1;
		}
	}
	else{
		return 0;
	}
}

void DecoEsp(void){

	switch(statusAT){
	case 0:
		if(espRx.buf[espRx.iR]==ANS_CWQAP[coincidencias]){
			coincidencias++;
			if(coincidencias == 15){
				statusAT++;
				coincidencias = 0;
				readyToSend = 1;
				DESCONECTADO = 1;
			}
		}
		//test[iii++] = espRx.buf[espRx.iR];
		espRx.iR++;
		if((espRx.iR==espRx.iW) && (!DESCONECTADO)){
			coincidencias = 0;
			DESCONECTADO = 0;
			readyToSend=1;
		}
	break;
//	case 1:
//		if(NADADEPRUEBAS){
//			statusCWQAP=0;
//			statusAT++;
//			coincidencias = 0;
//			coincidencias2 = 0;
//			readyToSend=1;
//			SSIDNuno = 1;
//			SSIDMicros = 0;
//		}
//		else{
//
//		}
//
//		test[iii++] = espRx.buf[espRx.iR];
//		espRx.iR++;
//		if((espRx.iR==espRx.iW) && (!SSIDMicros && !SSIDNuno)){
//			coincidencias2 = 0;
//			statusCWQAP=0;
//			coincidencias = 0;
//			readyToSend=1;
//		}
//	break;
	case 1:
		if(espRx.buf[espRx.iR]==ANS_CWMODE[coincidencias]){
			coincidencias++;
			if(coincidencias == 19){
				statusAT++;
				coincidencias = 0;
				readyToSend = 1;
			}
		}
		//test[iii++] = espRx.buf[espRx.iR];
		espRx.iR++;
	break;
	case 2:
		if(PrioridadRed1){
			if(espRx.buf[espRx.iR]==ANS_CWJAP2[coincidencias]){
				coincidencias++;
				if(coincidencias == 68){
					statusAT++;
					coincidencias = 0;
					coincidencias2 = 0;
					readyToSend = 1;
					SSIDNuno = 1;
					SSIDMicros = 0;
				}
			}
		}
		else{
			if(espRx.buf[espRx.iR]==ANS_CWJAP[coincidencias]){
				coincidencias++;
				if(coincidencias == 69){
					statusAT++;
					coincidencias = 0;
					coincidencias2 = 0;
					readyToSend = 1;
					SSIDNuno = 0;
					SSIDMicros = 1;
				}
			}
		}
		if(espRx.buf[espRx.iR]==ANS_CWJAP_ERROR_FAIL[coincidencias2]){
			coincidencias2++;
			if(coincidencias2 == 16){
				//statusAT++;
				PrioridadRed1 = ~PrioridadRed1;
				coincidencias = 0;
				coincidencias2 = 0;
				readyToSend = 1;
				SSIDNuno = 0;
				SSIDMicros = 0;
			}
		}
		//test[iii++] = espRx.buf[espRx.iR];
//		if(espRx.iR == espRx.iW && ((!SSIDNuno) || (!SSIDMicros))){
//					timeout2 = 14000;
//				}
		espRx.iR++;

	break;
	case 3:
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
	case 4:
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
						REENVIARMENSAJE = 0;
						readyToSend = 1;
					}
				}
			break;
		}

		espRx.iR++;
	break;
	case 5:
		if(SSIDNuno){
			if(espRx.buf[espRx.iR]==ANS_CIPSTART2[coincidencias]){
				coincidencias++;
				if(coincidencias == 61){
					statusAT++;
					statusESP++;
					coincidencias = 0;
					coincidencias2 = 0;
					readyToSend = 1;
					REENVIARMENSAJE = 0;
				}
			}
			if(espRx.buf[espRx.iR]==ANS_CIPSTART2_ERROR[coincidencias2]){
				coincidencias2++;
				if(coincidencias2 == 72){
					statusAT++;
					statusESP++;
					coincidencias = 0;
					coincidencias2 = 0;
					readyToSend = 1;
					REENVIARMENSAJE = 0;
				}
			}
		}
		else{
			if(SSIDMicros){
				if(espRx.buf[espRx.iR]==ANS_CIPSTART[coincidencias]){
					coincidencias++;
					if(coincidencias == 59){
						statusAT++;
						statusESP++;
						coincidencias = 0;
						readyToSend = 1;
						REENVIARMENSAJE = 0;
					}
				}
				if(espRx.buf[espRx.iR]==ANS_CIPSTART_ERROR[coincidencias2]){
					coincidencias2++;
					if(coincidencias2 == 70){
						statusAT++;
						statusESP++;
						coincidencias = 0;
						coincidencias2 = 0;
						readyToSend = 1;
						REENVIARMENSAJE = 0;
					}
				}
			}
		}
		espRx.iR++;
	break;
	case 6:
		switch(statusDecoCIPSEND){
			case 0:
				if(espRx.buf[espRx.iR]==CIPSEND_1[coincidencias]){
					coincidencias++;
					if(coincidencias == 11){
						//statusAT++;
						statusDecoCIPSEND++;
						//readyToSend = 1;
						//parte1 = 0;
						//statusESP++;
						coincidencias = 0;
						//readyToSend = 1;
					}
				}
				espRx.iR++;
			break;
			case 1:
				if(espRx.buf[espRx.iR]==(bytesToSend+'0') && (bytesToSend<10)){ //veo si lo que envie es menora 10 bytes si no hago cuentas abajo
					statusDecoCIPSEND = 3;
//					if(coincidencias == 25){
//						//statusAT++;
//						statusDecoCIPSEND=0;
//						statusESP = 0;
//						statusAT = 0;
//						SSIDNuno = 0;
//						SSIDMicros= 0;
//						lIp=0;
//						timeoutESP = 10;
//						//DESCONECTADO = 0;
//						parte1 = 1;
//						readyToSend = 1;
//						//statusESP++;
//						coincidencias = 0;
//						//readyToSend = 1;
//					}
				}
				else{
					if(espRx.buf[espRx.iR]==(bytesToSend/10+'0')){
						statusDecoCIPSEND++;
						bytesToSend_aux=bytesToSend%10;
					}
				}
				espRx.iR++;
			break;
			case 2:
				if(espRx.buf[espRx.iR]==(bytesToSend_aux+'0')){
					statusDecoCIPSEND++;
				}
				espRx.iR++;
			break;
			case 3:
				if(espRx.buf[espRx.iR]==CIPSEND_2[coincidencias]){
					coincidencias++;
					if(coincidencias == 9){
						//statusAT++;
						statusDecoCIPSEND++;
						//readyToSend = 1;
						parte1 = 0;
						//statusESP++;
						coincidencias = 0;
						readyToSend = 1;
					}
				}
				espRx.iR++;
			break;
			case 4:
				if(espRx.buf[espRx.iR]==CIPSEND_3[coincidencias]){
					coincidencias++;
					if(coincidencias == 4){
						//statusAT++;
						statusDecoCIPSEND++;
						//readyToSend = 1;
						//parte1 = 0;
						//statusESP++;
						coincidencias = 0;
						//readyToSend = 1;
					}
				}
				espRx.iR++;
			break;
			case 5:
				if(espRx.buf[espRx.iR]==(bytesToSend+'0') && (bytesToSend<10)){
					statusDecoCIPSEND = 7;
				}
				else{
					if(espRx.buf[espRx.iR]==(bytesToSend/10+'0')){
						statusDecoCIPSEND++;
						//bytesToSend_aux=bytesToSend%10;
					}
				}
				espRx.iR++;
			break;
			case 6:
				if(espRx.buf[espRx.iR]==(bytesToSend_aux+'0')){
					statusDecoCIPSEND++;
				}
				espRx.iR++;
			break;
			case 7:
				if(espRx.buf[espRx.iR]==CIPSEND_4[coincidencias]){
					coincidencias++;
					if(coincidencias == 19){
						//statusAT++;
						//statusDecoCIPSEND++;
						readyToSend = 1;
						parte1 = 1;
						statusDecoCIPSEND = 0;
						//timeout2 = 50;
						if(!PrimerMensaje){
							PrimerMensaje = 1;
							timeoutPrueba = 3000;
							statusESP++;
						}
						coincidencias = 0;
						statusAT++;
						//readyToSend = 1;
					}
				}
				espRx.iR++;
			break;
		}
		test[iii++] = espRx.buf[espRx.iR];
	break;
	case 7:
		switch(espRx.header){
		case 0:
			if (espRx.buf[espRx.iR] == IPD[coincidencias]){
				coincidencias++;
				if (coincidencias == 5){
					coincidencias = 0;
					espRx.header++;
				}
			}
			else{
				if(coincidencias > 0){
					coincidencias =0;
					espRx.header = 0;
					espRx.iR = espRx.iW;
					espRx.iR--;
				}
			}
			break;

		case 1:
			if (espRx.buf[espRx.iR] == ':'){
				espRx.header++;
			}
			break;
		case 2:
			if (espRx.buf[espRx.iR] == 'U')
				espRx.header++;
			else{
				espRx.header = 0;
				espRx.iR = espRx.iW;
				espRx.iR--;
			}
			break;
		case 3:
			if (espRx.buf[espRx.iR] == 'N')
				espRx.header++;
			else{
				espRx.header = 0;
				espRx.iR = espRx.iW;
				espRx.iR--;
			}
			break;
		case 4:
			if (espRx.buf[espRx.iR] == 'E')
				espRx.header++;
			else{
				espRx.header = 0;
				espRx.iR = espRx.iW;
				espRx.iR--;
			}
			break;
		case 5:
			if (espRx.buf[espRx.iR] == 'R')
				espRx.header++;
			else{
				espRx.header = 0;
				espRx.iR = espRx.iW;
				espRx.iR--;
			}
			break;
		case 6:
			espRx.nBytes = espRx.buf[espRx.iR];
			espRx.header++;
			break;
		case 7:
			if (espRx.buf[espRx.iR] == 0x00)
				espRx.header++;
			else{
				espRx.header = 0;
				espRx.iR = espRx.iW - 1;
			}
			break;
		case 8:
			if (espRx.buf[espRx.iR] == ':')
			{
				espRx.cks= 'U'^'N'^'E'^'R'^espRx.nBytes^0x00^':';
				espRx.header++;
				espRx.iData = espRx.iR+1;
			}
			else{
				espRx.header = 0;
				espRx.iR = espRx.iW;
				espRx.iR--;
			}
			break;

		case 9:
			if(espRx.nBytes>1){
				espRx.cks^=espRx.buf[espRx.iR];
			}
			espRx.nBytes--;
			if(espRx.nBytes==0){
				espRx.header=0;
				if(espRx.cks==espRx.buf[espRx.iR]){
					//RecibirDatos(ringRx.iData);
					RecibirComandoESP(espRx.buf[espRx.iData]);
				}
			}
			break;
		default:
			espRx.header = 0;
			break;
		}

		test[iii++] = espRx.buf[espRx.iR];
		espRx.iR++;
	break;
	}

}


void RecibirComandoESP(uint8_t comando){
	switch (comando){
		case 0xF0:
			ALIVESENTESP = 1;
			statusAT = 6;
			readyToSend = 1;
			parte1 = 1;
			CommandUdp(comando);
			//algo
		break;
		case 0xF1:
			MOTORSENTESP = 1;
			statusAT = 6;
			readyToSend = 1;
			parte1 = 1;
			CommandUdp(comando);
//			PWM1.u8[0] = ringRx.buf[head++];
//			PWM1.u8[1] = ringRx.buf[head++];
//			PWM1.u8[2] = ringRx.buf[head++];
//			PWM1.u8[3] = ringRx.buf[head++];
//			PWM2.u8[0] = ringRx.buf[head++];
//			PWM2.u8[1] = ringRx.buf[head++];
//			PWM2.u8[2] = ringRx.buf[head++];
//			PWM2.u8[3] = ringRx.buf[head++];
//			MOTORSSENT = 1; //CONFIRMACION QUE RECIBI LOS DATOS
		break;
		case MOTORSONOCMD:
			MOTORSSENT = 1;
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
  if(!timeoutRead)
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
  if(timeoutESP){
  	  timeoutESP--;
  }

  if(timeoutPrueba){
	  timeoutPrueba--;
    }

  if(timeoutRead){
    	  timeoutRead--;
    }

  if(timeout2){
	  timeout2--;
  }
  if(timeout3){
	  timeout3--;
  }

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}
