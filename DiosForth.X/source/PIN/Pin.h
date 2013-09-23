/*********************************************************************
 *
 *                  Pin.h
 *                  DUINOMITE - DIOS Forth
 *
 * v.1.0 - 20.12.2012, L.Pekny
 ********************************************************************/

#ifndef __PIN_H__
#define __PIN_H__


#include "..\GenericTypeDefs.h"


void ADCopen(void);
void ADCclose(void);
WORD ADCread(void);
void PINopen(void);
void PINclose(void);
WORD PINread(UINT portpin);
void PINwrite(UINT portpin, char pinstate);
void PINdir(UINT portpin, char pindir);

const void* DevTabADC[];
const void* DevTabPIN[];

#define pOUT	0
#define pIN		1


//Set value for init ADC
#define PARAM0	(ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN0)  // +AN0 -AVss
#define PARAM1	(ADC_MODULE_ON | ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON)  // Auto
#define PARAM2	(ADC_VREF_AVDD_AVSS | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_16)  // 16 smp
#define PARAM3	(ADC_CONV_CLK_PB | ADC_SAMPLE_TIME_31)  //80Mhz/512/32/12=407 smp/s
#define PARAM4	ENABLE_AN0_ANA|ENABLE_AN1_ANA
#define PARAM5	SKIP_SCAN_ALL


// Pin name				position
#define pOSC2			BIT_15 | (IOPORT_C<<16)	//C i
#define pOSC1			BIT_12 | (IOPORT_C<<16)	//C i

#define pSOSC0			BIT_14 | (IOPORT_C<<16)	//C i
#define pSOSCI			BIT_13 | (IOPORT_C<<16)	//C i

#define pA3				BIT_7  | (IOPORT_B<<16)	//B 1
#define pA2				BIT_6  | (IOPORT_B<<16)	//B 1
#define pRB5			BIT_5  | (IOPORT_B<<16)	//B i
#define pA1				BIT_4  | (IOPORT_B<<16)	//B 1
#define pA0				BIT_3  | (IOPORT_B<<16)	//B 1
#define pRB2			BIT_2  | (IOPORT_B<<16)	//B i
#define pADC1			BIT_1  | (IOPORT_B<<16)	//B AN1
#define pADC0			BIT_0  | (IOPORT_B<<16)	//B AN0

#define pU5RX			BIT_8  | (IOPORT_B<<16)	//B i
#define pA4				BIT_9  | (IOPORT_B<<16)	//B 1
#define pA5				BIT_10 | (IOPORT_B<<16)	//B 1
#define pCS				BIT_11 | (IOPORT_B<<16)	//B 1
#define pLED2			BIT_12 | (IOPORT_B<<16)	//B 0
#define pVGA_VS			BIT_12 | (IOPORT_B<<16)	//B 0
#define pRB13			BIT_13 | (IOPORT_B<<16)	//B i
#define pU5TX			BIT_14 | (IOPORT_B<<16)	//B i
#define pLED1			BIT_15 | (IOPORT_B<<16)	//B 0
#define pSTATUS_LED             BIT_3  | (IOPORT_B<<16)
#define pOC1			BIT_0  | (IOPORT_D<<16)	//D 0
#define pSD_CL			BIT_1  | (IOPORT_D<<16)	//D 1
#define pSD_I			BIT_2  | (IOPORT_D<<16)	//D i
#define pSD_O			BIT_3  | (IOPORT_D<<16)	//D 1
#define pVGA_HS			BIT_4  | (IOPORT_D<<16)	//D 1
#define pSD_CS			BIT_5  | (IOPORT_D<<16)	//D 1
#define pKBD_C			BIT_6  | (IOPORT_D<<16)	//D i
#define pKBD_D			BIT_7  | (IOPORT_D<<16)	//D i

#define pBTN_FW			BIT_8  | (IOPORT_D<<16)	//D i
#define pSDA1			BIT_9  | (IOPORT_D<<16)	//D i
#define pSCL1			BIT_10 | (IOPORT_D<<16)	//D 1
#define pSS				BIT_11 | (IOPORT_D<<16)	//D 1

#define pRF0			BIT_0  | (IOPORT_F<<16)	//F i
#define pRF1			BIT_1  | (IOPORT_F<<16)	//F i
#define pRF3			BIT_3  | (IOPORT_F<<16)	//F i
#define pU2RX			BIT_4  | (IOPORT_F<<16)	//F i
#define pU2TX			BIT_5  | (IOPORT_F<<16)	//F 1

#define pRG6			BIT_6  | (IOPORT_G<<16)	//G i
#define pRG7			BIT_7  | (IOPORT_G<<16)	//G i
#define pVIDEO			BIT_8  | (IOPORT_G<<16)	//G 1
#define pSS_HS			BIT_9  | (IOPORT_G<<16)	//G 1
#define pRG2			BIT_2  | (IOPORT_G<<16)	//G i
#define pRG3			BIT_3  | (IOPORT_G<<16)	//G i

#define pD0				BIT_0  | (IOPORT_E<<16)	//E i
#define pD1				BIT_1  | (IOPORT_E<<16)	//E i
#define pD2				BIT_2  | (IOPORT_E<<16)	//E i
#define pD3				BIT_3  | (IOPORT_E<<16)	//E i
#define pD4				BIT_4  | (IOPORT_E<<16)	//E i
#define pD5				BIT_5  | (IOPORT_E<<16)	//E i
#define pD6				BIT_6  | (IOPORT_E<<16)	//E i
#define pD7				BIT_7  | (IOPORT_E<<16)	//E i


//Set out value and direction, SETx=1.., INx=i.. 
#define cSETA	(0xFFFF)&0xFFFF
#define cINA	(0xFFFF)&0xFFFF

#define cSETB	(pA3|pA2|pA1|pA0|pA4|pA5|pCS)&0xFFFF
#define cINB	(pRB5|pRB2|pADC0|pADC1|pU5RX|pRB13|pU5TX)&0xFFFF

#define cSETC	(0)&0xFFFF
#define cINC	(pOSC2|pOSC1|pSOSC0|pSOSCI)&0xFFFF

#define cSETD	(pSD_CL|pSD_O|pVGA_HS|pSD_CS|pSCL1|pSS)&0xFFFF
#define cIND	(pSD_I|pKBD_C|pKBD_D|pBTN_FW|pSDA1)&0xFFFF

#define cSETE	(0)&0xFFFF
#define cINE	(pD0|pD1|pD2|pD3|pD4|pD5|pD6|pD7)&0xFFFF

#define cSETF	(pU2TX)&0xFFFF
#define cINF	(pRF0|pRF1|pRF3|pU2RX)&0xFFFF

#define cSETG	(pVIDEO|pSS_HS)&0xFFFF
#define cING	(pRG6|pRG7|pRG2|pRG3)&0xFFFF

#define cADC	(pADC0|pADC1)&0xFFFF


#endif		

