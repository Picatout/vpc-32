/*********************************************************************
 *
 *                  NVMem
 *                  DIOS Forth
 *
 * v.1.0 - 10.12.2012, Prog flash memory, L.Pekny
 ********************************************************************/

#include <plib.h>
#include "..\GenericTypeDefs.h"
#include "../../../hardware/HardwareProfile.h"
//#include "..\HardwareProfile.h"
#include "NVMem.h"


UINT FBuf, FBufCnt=0;
char *pFBuf=(char*)&FBuf;
UINT *vFHere, *vFHead;


void NVMopen(UINT *pFHere)							// Start addr for write
{
	FBufCnt=0; vFHere=pFHere;
}


char NVMclose(void)
{
	UINT i;
	if (!NVMflushBuf()) {return 0;}					// Copy Buf to flash

	for (i=EETAB; i<EETAB+FLASH_PAGE_SIZE; i+=4) {if (*(int *)i==-1) {break;}}
	if (i>=EETAB+FLASH_PAGE_SIZE) {i=EETAB;}		// Empty place or erase page
	return NVMwrite((UINT*)i,(UINT)vFHere); 		// Write vFHere to EETAB
}


char NVMerase(UINT *pAddr)
{
	UINT i, Ad=(UINT)pAddr&~0xFFF;

	if (Ad>FLASH_END) {return 0;}

	NVMErasePage((void *)Ad);

	for(i=0; i<FLASH_PAGE_SIZE; i+=4) {if (*(int *)(Ad+i)!=-1) {return 0;}}
	return -1;
}


char NVMwrite(UINT *pAddr, UINT Data)
{
	UINT Addr=(UINT)pAddr;

	if (Addr>FLASH_END) {return 0;}
	if (!(Addr&0xFFF)&&(pAddr>=vFHere)) {if (!NVMerase(pAddr)) {return 0;}}  // Erase new page
	NVMWriteWord(pAddr, Data); if (*pAddr!=Data) {return 0;}  // Write and compare
	return -1;
}


char NVMflushBuf(void)
{
	int r;
	if (FBufCnt)
	{	
	  FBufCnt=0;
	  r=NVMwrite(vFHere,FBuf); vFHere++;			// 0-err, -1-NVM
	  return r;
	} else return 1;								// 1-Buf
}


char NVMwriteBuf(UINT Data)							// Write UINT to NVM
{
	int r;
	NVMflushBuf();									// Align 32b
	r=NVMwrite(vFHere,Data); vFHere++;				// 0-err, -1-NVM
	return r;
}


char NVMwriteBufc(char Data)						// Delayed write to NVM
{
	int r;
	*(char*)(pFBuf+FBufCnt)=Data; FBufCnt++;		// Write byte to FBuf

	if (FBufCnt>3)									// Full? Write to Flash
	{
	  FBufCnt=0; 
	  r=NVMwrite(vFHere,FBuf); vFHere++;			// 0-err, -1-NVM
	  return r;
	} else return 1;								// 1-Buf
}


char NVMwriteBufw(WORD Data)						// Delayed write to NVM
{
	int r;
	if (FBufCnt==1) {FBufCnt++;}					// Align 16b
	if (FBufCnt==3) {NVMflushBuf();}				// Align 32b
	*(WORD*)(pFBuf+FBufCnt)=Data; FBufCnt+=2;		// Write word to FBuf

	if (FBufCnt>3)									// Full? Write to Flash
	{
	  FBufCnt=0; 
	  r=NVMwrite(vFHere,FBuf); vFHere++;			// 0-err, -1-NVM
	  return r;
	} else return 1;								// 1-Buf
}

