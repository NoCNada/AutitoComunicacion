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
		'"',',','"','1','9','2','.','1','6','8','.','1','.','3','"',',','3','0','0','1','5',',','3','0','1','5','\r','\n'};
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
		'"',',','"','1','9','2','.','1','6','8','.','1','.','3','"',',','3','0','0','1','5',',','3','0','1','5','\r',
		'\n','C','O','N','N','E','C','T','\r','\n','\r','\n','O','K','\r','\n'};//59

const char ANS_CIPSTART_ERROR[]={'A','T','+','C','I','P','S','T','A','R','T','=','"','U','D','P',
		'"',',','"','1','9','2','.','1','6','8','.','1','.','3','"',',','3','0','0','1','5',',','3','0','1','5','\r',
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
const int POS_SENSORES[]={-5,-4,-3,-2,-1,1,2,3,4,5};

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
#define ALIVESENTESP flag2.bit.b2
#define MOTORSENTESP flag2.bit.b3
#define MOTORSOFF flag2.bit.b4
#define TIMECONFIG flag2.bit.b5
#define ALIVE30S flag2.bit.b6
#define ADCSENT flag2.bit.b7

#define PrimerMensaje flag3.bit.b1
#define ReceiveMessage flag3.bit.b2
#define EnviarADCS flag3.bit.b3
//#define CalibrarADCS flag3.bit.b4
#define STARTLINE flag3.bit.b5
#define timeact	flag3.bit.b6
#define lineant flag3.bit.b7

#define negativo1 flag4.bit.b0
#define negativo2 flag4.bit.b1



#define INITESPCMD 0xC0 //Inicializar ESP
#define MOTORSCMD 0xD0 //comando motor
#define MOTORSONOCMD 0xD2 // COMANDO ON/OFF MOTOR
#define ALIVECMD 0xF0 // comando alive




// Variables Globales
volatile _sFlag flag1, flag2, flag3, flag4;
_sWork PWM1, PWM2,bufADC[9],bufADCCAL[9],PWM1BASE,PWM2BASE,AUXTOSEND[9],errorcito,kp,kd,ki;
uint8_t statusAT = 0, readyToSend = 1, lIp = 0,statusESP,parte1 = 1,timeoutESP = 100, timeoutRead = 100, timeout3 = 0, timeout4;
uint8_t timeoutADC = 50, timeoutADCError = 3, timeoutPID = 1, timeoutStop = 30;
uint16_t timeout2 = 0, timeoutPrueba = 2, timeoutmotor = 0, timemotor = 150;
char espIP[20],CIPSEND_NBYTES[30];
uint8_t coincidencias = 0, statusCIFSR = 0, statusDecoCIPSEND = 0,coincidencias2 = 0,statusCWQAP = 0;
uint8_t bytesToSend = 0, bytesToSend_aux = 0;
uint32_t g_systickCounter;
uint8_t test[256];
uint8_t NADADEPRUEBAS = 1;
uint8_t iii = 0, CommandoPepe = 0, iiiii = 0;
uint8_t iwBufADC;
volatile uint8_t statusanalog = 0;
float  Constante_Relacion[8],integral = 0,proporcional = 0,derivativo = 0,lastError = 0,Error = 0, cuentapid=0;
float NUMERADOR, DENOMINADOR, cuenta, fx2_x1,fx2_x3,x2_x1,x2_x3,x2_x1_Cuadrado,x2_x3_Cuadrado;
uint8_t CalibrarADCS=0, YACALIBRADO=0,sensors = 0,posSensor,posSensorDerecha,posSensorIzquierda;
int valorsensormin = 0;
float valorsensormax = 0;


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
void ADC_Calibracion();
void EncontrarLinea(_sWork *BUFADC);
void CalibrarValores(void);
void CalcularPID(uint32_t PWM1base, uint32_t PWM2base);
void StopLinea(void);



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

   // ADC16_Deinit(ADC0);


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
//    FTM0_PERIPHERAL->CONTROLS[2].CnV = 3100;
//    FTM0_PERIPHERAL->CONTROLS[6].CnV = 3999;
//    FTM_StartTimer(FTM0_PERIPHERAL, kFTM_SystemClock);
//
//    Delay_ms(10000);
//
//    FTM_StopTimer(FTM0_PERIPHERAL);
//    FTM0_PERIPHERAL->CONTROLS[2].CnV = 0;
//    FTM0_PERIPHERAL->CONTROLS[6].CnV = 0;
//    FTM_StartTimer(FTM0_PERIPHERAL, kFTM_SystemClock);
//    Delay_ms(10000);

            /* Init output LED GPIO. */
          //  GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);
         //   GPIO_PinInit(BOARD_RST_ESP_GPIO, BOARD_RST_ESP_PIN, config)
    /* Force the counter to be placed into memory. */
    PrioridadRed1 = 1;
    EnviarADCS = 0;
    kp.f = 100;
	kd.f = 0;
	ki.f = 0;
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
    		REENVIARMENSAJE = 0;
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
    		CommandUdp(0xD0);
    	}
    	if(TIMECONFIG){
    		CommandUdp(0xD2);
    	}
    	if(ADCSENT){
    		CommandUdp(0xA1);
    	}
    	if(!timeoutADC){
    		if((!ALIVESENTESP || !MOTORSENTESP || !TIMECONFIG) &&(EnviarADCS)){
    			ADCSENT = 1;
    			statusAT = 6;
    			readyToSend = 1;
    			parte1 = 1;
    			CommandUdp(0xA1);
    			espRx.header=0;
    		}
    		timeoutADC = 50;
    	}
    	if(CalibrarADCS && !YACALIBRADO){
    		ADC_Calibracion();
    		YACALIBRADO = 1;
    	}

    	if(!timeoutADCError){
    		if(CalibrarADCS){
    			CalibrarValores();
    			EncontrarLinea(bufADCCAL);
    		}
    		else{
    			EncontrarLinea(bufADC);
    		}
    		timeoutADCError = 3;
    	}

    	if(!timeoutPID && STARTLINE){
    		CalcularPID(PWM1.u32,PWM2.u32);
    		timeoutPID = 1;
    	}
    	if(STARTLINE && !timeoutPrueba){
    		StopLinea();
    		timeoutPrueba = 2;
    	}
    	if((!timeoutmotor && MOTORSOFF) && !STARTLINE){
    		MOTORSOFF = 0;
    	    FTM_StopTimer(FTM0_PERIPHERAL);
    	    FTM0_PERIPHERAL->CONTROLS[3].CnV = 0;
    	    FTM0_PERIPHERAL->CONTROLS[1].CnV = 0;
    	    FTM_StartTimer(FTM0_PERIPHERAL, kFTM_SystemClock);
    	}
//    	if(timeouterror){
//    		EncontrarLinea();
//    	}
    	i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}

void StopLinea(void){
	volatile uint8_t i = 0;
	for (i = 0; i < 8; i++) {
		if(bufADCCAL[i].f < valorsensormax){
			timeact = 0;
			lineant = 0;
			timeoutStop = 0;
			break;
		}
		else{
			lineant = 1;
		}
	}
	if(!timeact && lineant){
		timeoutStop = 30;
		timeact = 1;
		lineant = 1;
	}
	if(!timeoutStop && lineant){
		//stop
		MOTORSOFF = 1;
		timeoutStop = 0;
		timeact = 0;
		lineant = 0;
		STARTLINE = 0;
	}
}



void CalcularPID(uint32_t PWM1base, uint32_t PWM2base){
	int32_t pwm1b,pwm2b;

	integral += errorcito.f;

	if(integral > 1000){
		integral = 0;
	}

	derivativo = errorcito.f - lastError;
	cuentapid = (kp.u32 * errorcito.f) + (kd.u32 * derivativo) + (ki.u32 * integral);
	pwm1b = PWM1base - cuentapid;
	pwm2b = PWM2base + cuentapid;

//	 FTM0_PERIPHERAL->CONTROLS[2].CnV = 3100;
//	    FTM0_PERIPHERAL->CONTROLS[6].CnV = 3999;

	FTM_StopTimer(FTM0_PERIPHERAL);

	if((pwm1b < 1000) && (pwm1b > -1000)){
		FTM0_PERIPHERAL->CONTROLS[3].CnV = 0;
		FTM0_PERIPHERAL->CONTROLS[2].CnV = 0;
	}

	else if(pwm1b > 3999){
		FTM0_PERIPHERAL->CONTROLS[2].CnV = 0;
		FTM0_PERIPHERAL->CONTROLS[3].CnV = 3999;
	}
	else if (pwm1b <= -1000) {
		FTM0_PERIPHERAL->CONTROLS[3].CnV = 0;
		FTM0_PERIPHERAL->CONTROLS[2].CnV = PWM1base;
	}
	else{
		FTM0_PERIPHERAL->CONTROLS[2].CnV = 0;
		FTM0_PERIPHERAL->CONTROLS[3].CnV = pwm1b;
	}



	if((pwm2b < 1000) && (pwm2b > -1000)){
		FTM0_PERIPHERAL->CONTROLS[1].CnV = 0;
		FTM0_PERIPHERAL->CONTROLS[6].CnV = 0;
	}
	else if(pwm2b > 3999){
		FTM0_PERIPHERAL->CONTROLS[6].CnV = 0;
		FTM0_PERIPHERAL->CONTROLS[1].CnV = 3999;
	}
	else if (pwm2b <= -1000) {
		FTM0_PERIPHERAL->CONTROLS[1].CnV = 0;
		FTM0_PERIPHERAL->CONTROLS[6].CnV = PWM2base;
	}
	else{
		FTM0_PERIPHERAL->CONTROLS[6].CnV = 0;
		FTM0_PERIPHERAL->CONTROLS[1].CnV = pwm2b;
	}

	FTM_StartTimer(FTM0_PERIPHERAL, kFTM_SystemClock);

//	FTM_StopTimer(FTM0_PERIPHERAL);
//	if(negativo1){
//		FTM0_PERIPHERAL->CONTROLS[3].CnV = 0;
//		FTM0_PERIPHERAL->CONTROLS[2].CnV = PWM1base;
//		negativo1=0;
//	}
//	else{
//		FTM0_PERIPHERAL->CONTROLS[2].CnV = 0;
//		FTM0_PERIPHERAL->CONTROLS[3].CnV = PWM1BASE.u16[0]; //ES MAS LENTO
//	}
//	if(negativo2){
//		FTM0_PERIPHERAL->CONTROLS[1].CnV = 0;
//		FTM0_PERIPHERAL->CONTROLS[6].CnV = PWM2base;
//		negativo2 = 0;
//	}
//	else{
//		FTM0_PERIPHERAL->CONTROLS[6].CnV = 0;
//		FTM0_PERIPHERAL->CONTROLS[1].CnV = PWM2BASE.u16[0];
//	}
//
//	//FTM0_PERIPHERAL->CONTROLS[3].CnV = PWM1.u16[1]; //ES MAS LENTO
//
//	//FTM0_PERIPHERAL->CONTROLS[1].CnV = PWM2.u16[1];
//	FTM_StartTimer(FTM0_PERIPHERAL, kFTM_SystemClock);

	lastError = errorcito.f;

}

void CalibrarValores(void){
	bufADCCAL[0].f = bufADC[0].f;
	for(uint8_t i = 1; i<8; i++){
		bufADCCAL[i].f= (bufADC[i].f)*(Constante_Relacion[i]);
	}
}

void EncontrarLinea(_sWork *BUFADC){
	float aux[10];

	valorsensormin = BUFADC[0].i32;
	posSensor = 1;
	//valorsensormin = bufADC[0].i32;
	while(sensors < 8){
		if(valorsensormin > BUFADC[sensors].i32){
			valorsensormin = BUFADC[sensors].i32;
			posSensor = sensors+1;
		}
		aux[sensors+1]=BUFADC[sensors].i32;
		sensors++;
	}

	sensors = 0;
	aux[0]=aux[2];
	aux[9]=aux[7];

	posSensorDerecha = posSensor+1;
	posSensorIzquierda = posSensor-1;

	fx2_x1 = aux[posSensor]-aux[posSensorIzquierda];
	fx2_x3 = aux[posSensor]-aux[posSensorDerecha];
	x2_x1 = POS_SENSORES[posSensor]-POS_SENSORES[posSensorIzquierda];
	x2_x3 = POS_SENSORES[posSensor]-POS_SENSORES[posSensorDerecha];
	x2_x1_Cuadrado = (x2_x1)*(x2_x1);
	x2_x3_Cuadrado = (x2_x3)*(x2_x3);
	NUMERADOR = (x2_x1_Cuadrado*fx2_x3)-(x2_x3_Cuadrado*fx2_x1);
	DENOMINADOR = 2*((x2_x1*fx2_x3)-(x2_x3*fx2_x1));
	cuenta = NUMERADOR/DENOMINADOR;
	if(DENOMINADOR != 0){
		BUFADC[8].f = -(POS_SENSORES[posSensor] - cuenta);
		errorcito.f=BUFADC[8].f;
	}


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
	if(readyToSend && (!timeout4)){
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
					//timeout4 = 2;
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
					//timeout4 = 1;
					if(ALIVESENTESP)
						ALIVESENTESP = 0;
					ReceiveMessage = 0;
				//	timeout2 = 15;
				}

			break;
			case 0xD0:
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
					espTx.buf[espTx.iW++] = '1'; //CAMBIAR ESTO
					espTx.buf[espTx.iW++] = '1'; //CAMBIAR ESTO
					espTx.buf[espTx.iW++] = '\r';
					espTx.buf[espTx.iW++] = '\n';
					bytesToSend = 11; //CAMBIAR ESTO
					//timeout4 = 2;
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
					espTx.buf[espTx.iW++] = 0x04;//CAMBIAR ESTO
					espTx.buf[espTx.iW++] = 0x00;
					espTx.buf[espTx.iW++] = ':';
					espTx.buf[espTx.iW++] = 0xD0;
					espTx.buf[espTx.iW++] = 0x0D;
					espTx.buf[espTx.iW++] = 0xDC;
					espTx.cks = 'U'^'N'^'E'^'R'^0x04^0x00^':'^0xD0^0x0D^0xDC;
					espTx.buf[espTx.iW++] = espTx.cks;
					readyToSend = 0;
					parte1=0;
					//timeout4 = 1;
					if(MOTORSENTESP)
						MOTORSENTESP = 0;
					ReceiveMessage = 0;
					//	timeout2 = 15;
				}

				break;
			case 0xD2:
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
					//espTx.buf[espTx.iW++] = '0';
					espTx.buf[espTx.iW++] = '\r';
					espTx.buf[espTx.iW++] = '\n';
					bytesToSend = 9;
					//timeout4 = 2;
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
					espTx.buf[espTx.iW++] = 0xD2;
					espTx.cks = 'U'^'N'^'E'^'R'^0x02^0x00^':'^0xD2;
					espTx.buf[espTx.iW++] = espTx.cks;
					readyToSend = 0;
					parte1=0;
					if(TIMECONFIG)
						TIMECONFIG = 0;
					//timeout4 = 2;
					//	timeout2 = 15;
				}
			break;
			case 0xA1:
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
					espTx.buf[espTx.iW++] = '4';
					espTx.buf[espTx.iW++] = '5';
					//espTx.buf[espTx.iW++] = '0';
					espTx.buf[espTx.iW++] = '\r';
					espTx.buf[espTx.iW++] = '\n';
					bytesToSend = 45;
					timeout4 = 1;
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
					//bufADC[8].f = 0;
					espTx.buf[espTx.iW++] = 'U';
					espTx.buf[espTx.iW++] = 'N';
					espTx.buf[espTx.iW++] = 'E';
					espTx.buf[espTx.iW++] = 'R';
					espTx.buf[espTx.iW++] = 38; //A1 + 1 + 36
					espTx.buf[espTx.iW++] = 0x00;
					espTx.buf[espTx.iW++] = ':';
					espTx.buf[espTx.iW++] = 0xA1; //7 + 9*4 =  ------ 36 + 1 = 37
					//36
					//ADC_Calibracion();
//			    	if(CalibrarADCS && !YACALIBRADO){
//			    		ADC_Calibracion();
//			    		YACALIBRADO = 1;
//			    	}
					if(CalibrarADCS){
						for(uint8_t i = 0; i<9; i++){
							AUXTOSEND[i].f= bufADCCAL[i].f;
						}
					}
					else{
						for(uint8_t i = 0; i<9; i++){
							AUXTOSEND[i].f= bufADC[i].f;
						}
					}
//					for (uint8_t i = 1; i < 8; i++) {
//							Constante_Relacion[i] = (bufADC[0].f)/(bufADC[i].f);
//							bufADC[i].f = (bufADC[i].f)*(Constante_Relacion[i]);
//						}
//						bufADCCAL[0].i32=bufADC[0].i32;
					for(uint8_t i = 0; i<9; i++){
						for (uint8_t j = 0; j < 4; j++) {
							espTx.buf[espTx.iW++] = AUXTOSEND[i].i8[j];
						}
					}
					espTx.cks=0;
					for(uint8_t i=espTx.iR; i<espTx.iW; i++) {
						espTx.cks^=espTx.buf[i];
						//pc.printf("%d - %x - %d   v: %d \n",i,cks,cks,tx[i]);
					}
					if(espTx.cks>0)
						espTx.buf[espTx.iW++]=espTx.cks;


					//espTx.cks = 'U'^'N'^'E'^'R'^0x02^0x00^':'^0xD2;
					//espTx.buf[espTx.iW++] = espTx.cks;
					readyToSend = 0;
					parte1=0;
					timeout4 = 1;
					//	timeout2 = 15;
				}
				break;
		}
		readyToSend = 0;
	}

}

void ADC_Calibracion(){

	for (uint8_t i = 1; i < 8; i++) {
		Constante_Relacion[i] = (bufADC[0].f)/(bufADC[i].f);
		//bufADCCAL[i].f = (bufADC[i].f)*(Constante_Relacion[i]);
	}

	valorsensormax = bufADC[0].f;
	for (uint8_t i = 1; i < 8; i++) {
		if(valorsensormax < (bufADC[i].f * Constante_Relacion[i])){
			valorsensormax = (bufADC[i].f * Constante_Relacion[i]);
		}
	}
	valorsensormax = valorsensormax * 0.75;
		//valorsensormin = bufADC[0].i32;


	//bufADCCAL[0].i32=bufADC[0].i32;
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
				test[iii++] = espRx.buf[espRx.iR];
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
							//timeoutPrueba = 3000;
							statusESP++;
						}
						if(ADCSENT){
							ADCSENT = 0;
							timeout4 = 1;
							timeoutADC = 50;
						}
						coincidencias = 0;
						statusAT++;
						//readyToSend = 1;
					}
				}
				espRx.iR++;
			break;
		}

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
				CommandoPepe = espRx.iR+1;
				espRx.iData = espRx.iR+2;
				//espRx.iData + 1;
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
				espRx.header = 0;
				if(espRx.cks==espRx.buf[espRx.iR]){
					//RecibirDatos(ringRx.iData);
					ReceiveMessage = 1;
					RecibirComandoESP(espRx.buf[CommandoPepe]);
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
	//iiiii = 0;
	switch (comando){
		case 0xF0:
			ALIVESENTESP = 1;
			statusAT = 6;
			readyToSend = 1;
			parte1 = 1;
			CommandUdp(comando);
			espRx.header=0;
			//TODOOOK = 0;
			//algo
		break;
		case 0xD0:
			PWM1.u8[0]=espRx.buf[espRx.iR-8];
			PWM1.u8[1]=espRx.buf[espRx.iR-7];
			PWM1.u8[2]=espRx.buf[espRx.iR-6];
			PWM1.u8[3]=espRx.buf[espRx.iR-5];
			PWM2.u8[0]=espRx.buf[espRx.iR-4];
			PWM2.u8[1]=espRx.buf[espRx.iR-3];
			PWM2.u8[2]=espRx.buf[espRx.iR-2];
			PWM2.u8[3]=espRx.buf[espRx.iR-1];
			//test[iiiii] = espRx.buf[CommandoPepe];
			//if(iiiii > 7){
			//FTM_StopTimer(FTM0_PERIPHERAL);
			//FTM0_PERIPHERAL->CONTROLS[3].CnV = PWM1.u16[0]; //ES MAS LENTO
			//	FTM0_PERIPHERAL->CONTROLS[3].CnV = PWM1.u16[1]; //ES MAS LENTO
			//FTM0_PERIPHERAL->CONTROLS[1].CnV = PWM2.u16[0];
			//	FTM0_PERIPHERAL->CONTROLS[1].CnV = PWM2.u16[1];
			//FTM_StartTimer(FTM0_PERIPHERAL, kFTM_SystemClock);
			MOTORSENTESP = 1;
			statusAT = 6;
			readyToSend = 1;
			parte1 = 1;
			//timeoutmotor = timemotor;
			//MOTORSOFF = 1;
			iiiii = 0;
			CommandUdp(comando);
			espRx.header = 0;
		break;
		case 0xD2:
			TIMECONFIG = 1;
			statusAT = 6;
			readyToSend = 1;
			parte1 = 1;
			timemotor = espRx.buf[espRx.iR-1]*100;
			CommandUdp(comando);
			espRx.header=0;
			//TODOOOK = 0;
			//algo
			break;
//		case MOTORSONOCMD:
//			MOTORSSENT = 1;
//		break;
		case 0xD5:
			STARTLINE = !STARTLINE;
			if(STARTLINE){
				FTM_StopTimer(FTM0_PERIPHERAL);
				FTM0_PERIPHERAL->CONTROLS[3].CnV = 2500; //ES MAS LENTO
				//FTM0_PERIPHERAL->CONTROLS[3].CnV = PWM1.u16[1]; //ES MAS LENTO
				FTM0_PERIPHERAL->CONTROLS[1].CnV = 2500;
				//FTM0_PERIPHERAL->CONTROLS[1].CnV = PWM2.u16[1];
				FTM_StartTimer(FTM0_PERIPHERAL, kFTM_SystemClock);
			}
			else{
				MOTORSOFF = 1;
			}

		break;
		case 0xA5:
			CalibrarADCS = !CalibrarADCS;
			YACALIBRADO = 0;
		break;
		case 0xB0:
			EnviarADCS = !EnviarADCS;
		break;
		case 0xB3:
			if(STARTLINE){
				STARTLINE = 0;
			}
			timeoutmotor = timemotor;
			MOTORSOFF = 1;
		break;
		case 0xC7:
			kp.u8[0]=espRx.buf[espRx.iR-12];
			kp.u8[1]=espRx.buf[espRx.iR-11];
			kp.u8[2]=espRx.buf[espRx.iR-10];
			kp.u8[3]=espRx.buf[espRx.iR-9];
			kd.u8[0]=espRx.buf[espRx.iR-8];
			kd.u8[1]=espRx.buf[espRx.iR-7];
			kd.u8[2]=espRx.buf[espRx.iR-6];
			kd.u8[3]=espRx.buf[espRx.iR-5];
			ki.u8[0]=espRx.buf[espRx.iR-4];
			ki.u8[1]=espRx.buf[espRx.iR-3];
			ki.u8[2]=espRx.buf[espRx.iR-2];
			ki.u8[3]=espRx.buf[espRx.iR-1];
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


/* ADC0_IRQn interrupt handler */
void ADC0_IRQHANDLER(void) {
  /* Array of result values*/
//  uint32_t result_values[2] = {0};
  /* Get flags for each group */
//  for ( int i=0; i<2; i++){
//  uint32_t status = ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, i);
//  	if ( status == kADC16_ChannelConversionDoneFlag){
//  		result_values[i] = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, i);
//  	}
//  }

  /* Place your code here */
	bufADC[iwBufADC++].i32 = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP);
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/* ADC1_IRQn interrupt handler */
void ADC1_IRQHANDLER(void) {
//  /* Array of result values*/
//  uint32_t result_values[2] = {0};
//  /* Get flags for each group */
//  for ( int i=0; i<2; i++){
//  uint32_t status = ADC16_GetChannelStatusFlags(ADC1_PERIPHERAL, i);
//  	if ( status == kADC16_ChannelConversionDoneFlag){
//  		result_values[i] = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, i);
//  	}
//  }

  /* Place your code here */


	bufADC[iwBufADC++].i32 = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, ADC1_CH0_CONTROL_GROUP);


  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}



/* PIT1_IRQn interrupt handler */
void PIT_CHANNEL_1_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_1);
  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_1, intStatus);

  /* Place your code here */
  switch(statusanalog){
  case 0:
	  ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, &ADC0_channelsConfig[0]);
	  statusanalog++;
	  break;
  case 1:
	  ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, &ADC0_channelsConfig[1]);
	  statusanalog++;
	  break;
  case 2:
	  ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, &ADC0_channelsConfig[2]);
	  statusanalog++;
	  break;
  case 3:
	  ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, &ADC0_channelsConfig[3]);
	  statusanalog++;
	  break;
  case 4:
	  ADC16_SetChannelConfig(ADC1_PERIPHERAL, ADC1_CH1_CONTROL_GROUP, &ADC1_channelsConfig[0]);
	  //ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, &ADC0_channelsConfig[0]);
	  statusanalog++;
	  break;
  case 5:
	  ADC16_SetChannelConfig(ADC1_PERIPHERAL, ADC1_CH1_CONTROL_GROUP, &ADC1_channelsConfig[1]);
	  statusanalog++;
	  break;
  case 6:
	  ADC16_SetChannelConfig(ADC1_PERIPHERAL, ADC1_CH1_CONTROL_GROUP, &ADC1_channelsConfig[2]);
	  statusanalog++;
	  break;
  case 7:
	  ADC16_SetChannelConfig(ADC1_PERIPHERAL, ADC1_CH1_CONTROL_GROUP, &ADC1_channelsConfig[3]);
	  statusanalog++;
	  break;
  default:
	  statusanalog = 0;
	  break;
  }


  if(iwBufADC >= 8){
	  iwBufADC = 0;
  }
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
  if(timeoutmotor){
	  timeoutmotor--;
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
  if(timeout4){
	  timeout4--;
  }


  if(timeoutADC){
	  timeoutADC--;
  }

  if(timeoutADCError){
	  timeoutADCError--;
  }
  if(timeoutPID){
	  timeoutPID--;
  }
  if(timeoutStop){
	  timeoutStop--;
  }


  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


