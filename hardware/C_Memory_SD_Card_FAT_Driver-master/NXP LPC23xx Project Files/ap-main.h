/*
IBEX UK LTD http://www.ibexuk.com
Electronic Product Design Specialists
RELEASED SOFTWARE

The MIT License (MIT)

Copyright (c) 2013, IBEX UK Ltd, http://ibexuk.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
//Project Name:		FAT FILING SYSTEM FAT16 & FAT 32 DRIVER
//NXP LPC2365 SAMPLE PROJECT C CODE HEADER FILE




//*****************************
//*****************************
//********** DEFINES **********
//*****************************
//*****************************
#ifndef MAIN_C_INIT				//(Include this section only once for each source file that includes this header file)
#define	MAIN_C_INIT

//------------------------
//----- USER DEFINES -----
//------------------------


//----------------------
//----- IO DEFINES -----
//----------------------
#define	LED1(state)			(state ? (FIO2CLR = 0x00000004) : (FIO2SET = 0x00000004))
#define	LED2(state)			(state ? (FIO2CLR = 0x00000008) : (FIO2SET = 0x00000008))

//--------------------------
//----- SWITCH DEFINES -----
//--------------------------
#define	SWITCH_1_PRESSED			switches_debounced & 0x01

#define	SWITCH_1_NEW_PRESS			switches_new & 0x01




typedef enum _TEST_APPLICATION_STATE
{
    TA_PROCESS_WAIT_FOR_CARD,
    TA_PROCESS_CARD_INSERTED,
    TA_PROCESS_DELETE_AND_CREATE_FILES,
    TA_PROCESS_CARD_OPERTATION_DONE,
    TA_PROCESS_ERROR
} TEST_APPLICATION_STATE;



#endif






//*******************************
//*******************************
//********** FUNCTIONS **********
//*******************************
//*******************************
#ifdef MAIN_C
//-----------------------------------
//----- INTERNAL ONLY FUNCTIONS -----
//-----------------------------------
void initialise (void);
void read_switches (void);
void process_mode (void);
void timer0_irq_handler (void);


//-----------------------------------------
//----- INTERNAL & EXTERNAL FUNCTIONS -----
//-----------------------------------------
//(Also defined below as extern)


#else
//------------------------------
//----- EXTERNAL FUNCTIONS -----
//------------------------------


#endif




//****************************
//****************************
//********** MEMORY **********
//****************************
//****************************
#ifdef MAIN_C
//--------------------------------------------
//----- INTERNAL ONLY MEMORY DEFINITIONS -----
//--------------------------------------------
BYTE read_switches_flag;
WORD user_mode_10ms_timer;
BYTE test_application_state = TA_PROCESS_WAIT_FOR_CARD;



//--------------------------------------------------
//----- INTERNAL & EXTERNAL MEMORY DEFINITIONS -----
//--------------------------------------------------
//(Also defined below as extern)
BYTE switches_debounced = 0;
BYTE switches_new = 0;
WORD general_use_10ms_timer;
WORD general_use_100ms_timer;


#else
//---------------------------------------
//----- EXTERNAL MEMORY DEFINITIONS -----
//---------------------------------------
extern BYTE switches_debounced;
extern BYTE switches_new;
extern WORD general_use_10ms_timer;
extern WORD general_use_100ms_timer;


#endif






