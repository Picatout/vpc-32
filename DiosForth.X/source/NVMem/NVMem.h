/*********************************************************************
 *
 *                  NVMem.h
 *                  DIOS Forth
 *
 * v.1.0 - 10.12.2012, L.Pekny
 ********************************************************************/

#ifndef __NVMEM_H__
#define __NVMEM_H__


#include "..\GenericTypeDefs.h"


#define FLASH_PAGE_SIZE	 		0x1000
#define EETAB			 		FLASH_END-FLASH_PAGE_SIZE+1

extern UINT FBuf, FBufCnt;
extern char *pFBuf, *pHereBase;
extern UINT *vFHere, *vFHead;

void NVMopen(UINT *pFHere);
char NVMclose(void);
char NVMerase(UINT *pAddr);
char NVMwrite(UINT *pAddr, UINT Data);
char NVMwriteBuf(UINT Data);
char NVMwriteBufc(char Data);
char NVMwriteBufw(WORD Data);
char NVMflushBuf(void);

#endif		

