/************************************************************************************
 *  Copyright 2006 Carleton Coffrin
 * 
 *  This file is part of Simple DMX.
 *  
 *  Simple DMX is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  Simple DMX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with Simple DMX; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *  
 *************************************************************************************/
 
#include "timedDMX_internal.h"
#include <stdio.h>

//Internal function used to close the USB -> DMX connection
EXPORT DMXINT closeDevice(DRIVER_HANDLE *fth) {
    FT_W32_CloseHandle(*fth);
    return (DMXINT)1;
}

EXPORT  bool isSimulation(dmx_handle h){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return false;
	}
	return pInstance->simulation;
}

//Internal function used to open the USB -> DMX connection
//Configures the serial output with the settings specific to the DMX512 standard
EXPORT DMXINT openDevice(dmx_instance * i) {
	FT_STATUS ftStatus;
    char Buf[64]; //does this need to not go out of scope? (apparently not)

    ftStatus = FT_ListDevices(0,Buf,FT_LIST_BY_INDEX|FT_OPEN_BY_DESCRIPTION);

	//Try to open a port
    i->dHandle = FT_W32_CreateFile(Buf,GENERIC_READ|GENERIC_WRITE,0,0,
        OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL | FT_OPEN_BY_DESCRIPTION,0);

	//****************************************Upgrade this to support multiple devices
    // connect to first device
    if (i->dHandle == INVALID_HANDLE_VALUE) {
        //("No device","Error",MB_OK);
        i->simulation = true;
        return (DMXINT)4;
    }

	i->simulation = false;

    FTDCB ftDCB;
    if (FT_W32_GetCommState(i->dHandle,&ftDCB)) {
        // FT_W32_GetCommState ok, device state is in ftDCB
        ftDCB.BaudRate = 250000;
        ftDCB.Parity = FT_PARITY_NONE;
        ftDCB.StopBits = FT_STOP_BITS_2;
        ftDCB.ByteSize = FT_BITS_8;
        ftDCB.fOutX = false;
        ftDCB.fInX = false;
        ftDCB.fErrorChar = false;
        ftDCB.fBinary = true;
        ftDCB.fRtsControl = false;
        ftDCB.fAbortOnError = false;

        if (!FT_W32_SetCommState(i->dHandle,&ftDCB)) {
            //("Set baud rate error","Error",MB_OK);
            return (DMXINT)0;
        }
    }

    FT_W32_PurgeComm(i->dHandle,FT_PURGE_TX | FT_PURGE_RX);

	//?????????????????????????????????? Maybe need this?
    //Sleep(1000L);
	
	return (DMXINT)1;
}

//Internal funtion that outputs data to the DMX port
//Intened to be spawed as another thread and run infinatly
//Precondition: Connected DMX_Instance
//Postcondition: data of the DMX_Instance is continusly transmited
EXPORT DWORD WINAPI sender(void *param){
	dmx_instance *i = (dmx_instance*)param;
	DMXINT x;
	double temp;
	INT32 sendDelay = i->sendDelay;
	clock_t currentTime = clock();
	clock_t *startTime = i->st;
	clock_t *endTime = i->et;
	
	DMXDATA *data = i->ov;
	DMXDATA *fadedata = i->cv;
	DMXDATA *startValue = i->sv;
	DMXDATA *endValue = i->ev;
	//DMXDATA *profile = i->profile;
	DMXDATA tempValue;

	ULONG bytesWritten;

     // set RS485 for sendin
    if(!(i->simulation))
    	FT_W32_EscapeCommFunction((i->dHandle),CLRRTS);

	
    while(i->send) {
		WaitForSingleObject(i->hSemaphore, INFINITE);

		//calculate the next what the DMX data should be at the current time
		currentTime = clock(); //CLK_TCK = number of clock ticks in 1 second
		for(x = 0; x < i->totalBits; x++){
			if(currentTime >= endTime[x]){
				((dmx_instance*)param)->cv[x] = endValue[x];
				//((dmx_instance*)param)->ov[x] = i->profile[x][fadedata[x]];
			}
			else{
				temp = (currentTime-startTime[x])/(double)(endTime[x]-startTime[x]);
				if(temp <= 1)
					((dmx_instance*)param)->cv[x]=(DMXDATA)(int)((endValue[x]-startValue[x])*temp+startValue[x]);
				
				
			}
			tempValue = i->profile[x][fadedata[x]]; //apply profile transformation
			tempValue = i->pov[x][tempValue]; //apply proportional output transformation
			((dmx_instance*)param)->ov[x] = i->grandMaster[tempValue]; //apply grand master transformation
		}
		
		if(!(i->simulation)){
			FT_W32_SetCommBreak(i->dHandle);
	        FT_W32_ClearCommBreak(i->dHandle);
			
			//Transmit the Start Code and DMX Data
	        FT_W32_WriteFile(i->dHandle, &i->startCode, 1, &bytesWritten, NULL);
	        FT_W32_WriteFile(i->dHandle, data, 512, &bytesWritten, NULL);
		
			(i->framesSent)++;
		}
		ReleaseSemaphore(i->hSemaphore,1,NULL);
        Sleep(sendDelay);
    }

	return(0);
}
//**********************************************************************
//**********************************************************************
//**********************************************************************


//API funtions **********************************************************************

//Opens a USB -> DMX device and starts outputing data
//rRate - refresh rate in MilliSeconds: Range 22 to 1000 MS between packets
//tBits - Number of DMX bytes to be sent per packet: Range 1 to 513 
EXPORT dmx_handle open(DMXINT tBits, DMXINT rRate) {
	dmx_instance *info = new dmx_instance;

	if(openDevice(info)==0){	
		return (dmx_handle)5;
	}

	if(tBits > 512)
		return (dmx_handle)6;
	//cout << "This is it ** " << info->dHandle << "\n";
	
	//FT_W32_EscapeCommFunction((info->dHandle),CLRRTS);

	clock_t tempTime = clock();

	for(int x = 0; x < 512; x++){
		for(int y = 0; y < 256; y++){
			info->profile[x][y] = (DMXDATA)y;
			info->pov[x][y] = (DMXDATA)y;
		}
	
		info->ov[x] = (DMXDATA)0;
		info->cv[x] = (DMXDATA)0;
		info->sv[x] = (DMXDATA)0; 
		info->ev[x] = (DMXDATA)0; 
		info->st[x] = tempTime;
		info->et[x] = tempTime;
	}
	
	for(int y = 0; y < 256; y++){
		info->grandMaster[y] = (DMXDATA)y;
	}
	
	info->startCode = (DMXDATA)0;
	info->totalBits = tBits;
	info->framesSent = 0;
    info->sendDelay = rRate; //should range from between 22 to 1000 MS between packets
	info->send = true;


	HANDLE dmxSender = CreateThread(NULL, NULL, sender, info, NULL, &(info->dwSenderThread));
	if(dmxSender == NULL){
		printf("CreateThread(Sender) failed\n");
	}

	return (dmx_handle)info;
}


//Closes a USB -> DMX device and stops output
//h - a valid dmx_instance with an open connection
EXPORT dmx_status close(dmx_handle h) {
	DWORD status = 0;
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;

	if (pInstance == NULL){
		return (dmx_status)1;
	}

	
	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	pInstance->send = false;
	TerminateThread(&(pInstance->dwSenderThread), status);
	if(!(pInstance->simulation)){
		if(closeDevice(&(pInstance->dHandle)) == 1){
			ReleaseSemaphore(pInstance->hSemaphore,1,NULL);
			return (dmx_status)0;
		}
		else{
			ReleaseSemaphore(pInstance->hSemaphore,1,NULL);
			return (dmx_status)1;
		}	
	}
	return (dmx_status)0;
}

EXPORT dmx_status setStartCode(dmx_handle h, DMXDATA startCode){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL) {
		return (dmx_status)1;
	}
	
	pInstance->startCode = startCode;
	
	return (dmx_status)0;
}

EXPORT dmx_status setGrandMaster(dmx_handle h, DMXDATA level){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL) {
		return (dmx_status)1;
	}
	
	for(int x=0; x<256; x++){
		pInstance->grandMaster[x] = (x*level)/255;
	}
	
	return (dmx_status)0;
}

EXPORT dmx_status stopFade(dmx_handle h) {
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL) {
		return (dmx_status)1;
	}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);
	//clock_t endUpTime = currTime + ((CLK_TCK*ut)/1000);
	clock_t currTime = clock();
	
	for(DMXINT x = 0; x < pInstance->totalBits; x++) {
		pInstance->et[x] = currTime;
		pInstance->ev[x] = pInstance->cv[x];
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}

EXPORT dmx_status setProportionalOutput(dmx_handle h, DMXINT index, DMXDATA level){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}
	
	if(index > pInstance->totalBits){
		return (dmx_status)3;
	}
	
	for(int y = 0; y <256; y++){
		pInstance->pov[index][y] = (y*level)/255;
	}

	return (dmx_status)0;
}

EXPORT dmx_status setProportionalOutputs(dmx_handle h, DMXDATA levels[512]){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		for(int y = 0; y <256; y++){
			pInstance->pov[x][y] = (y*levels[x])/255;
		}
	}

	return (dmx_status)0;
}

EXPORT dmx_status clearProportionalOutput(dmx_handle h, DMXINT index){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}
	
	if(index > pInstance->totalBits){
		return (dmx_status)3;
	}
	
	for(int y = 0; y <256; y++){
		pInstance->pov[index][y] = y;
	}

	return (dmx_status)0;
}

EXPORT dmx_status clearProportionalOutputs(dmx_handle h){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		for(int y = 0; y <256; y++){
			pInstance->pov[x][y] = y;
		}
	}

	return (dmx_status)0;
}



	
EXPORT dmx_status setProfile(dmx_handle h, DMXINT index,DMXDATA profile[256]) {
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL) {
		return (dmx_status)1;
	}
	if(index > pInstance->totalBits)
		{return (dmx_status)3;}

	for(DMXINT y = 0; y < 256; y++) {
		pInstance->profile[index][y] = profile[y];
	}

	return (dmx_status)0;
}

EXPORT dmx_status setProfiles(dmx_handle h, DMXDATA profiles[512][256]){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		for(int y = 0; y <256; y++)
			pInstance->profile[x][y] = profiles[x][y];
	}

	return (dmx_status)0;
}


EXPORT dmx_status clearProfile(dmx_handle h, DMXINT index) {
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL)
		{return (dmx_status)1;}
	if(index > pInstance->totalBits)
		{return (dmx_status)3;}

	for(int y = 0; y <256; y++)
		pInstance->profile[index][y] = y;

	return (dmx_status)0;
}

EXPORT dmx_status clearProfiles(dmx_handle h) {
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL)
	{
		return (dmx_status)1;
	}

	for(DMXINT x = 0; x < pInstance->totalBits; x++)
	{
		for(int y = 0; y <256; y++)
			pInstance->profile[x][y] = y;
	}

	return (dmx_status)0;
}


EXPORT dmx_status setSingle(dmx_handle h, DMXINT index, DMXDATA Data) {
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL)
		{return (dmx_status)1;}
	
	if(index > pInstance->totalBits)
		{return (dmx_status)3;}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	clock_t currTime = clock();

	pInstance->et[index] = currTime;
	pInstance->ev[index] = Data;

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}

EXPORT dmx_status setSingleTimed(dmx_handle h, DMXINT index, DMXDATA Data, DMXINT t) {
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL)
		{return (dmx_status)1;}
	
	if(index > pInstance->totalBits)
		{return (dmx_status)3;}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	clock_t currTime = clock();
	clock_t endTime = currTime + ((CLK_TCK*t)/1000);

	pInstance->sv[index] = pInstance->cv[index];
	pInstance->st[index] = currTime;
	
	pInstance->ev[index] = Data;
	pInstance->et[index] = endTime;

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}


EXPORT dmx_status quickLoad(dmx_handle h, DMXDATA Data[512]) {
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL)
	{
		return (dmx_status)1;
	}


	WaitForSingleObject(pInstance->hSemaphore, INFINITE);
	//clock_t endUpTime = currTime + ((CLK_TCK*ut)/1000);
	clock_t currTime = clock();

	for(DMXINT x = 0; x < pInstance->totalBits; x++)
	{
		pInstance->et[x] = currTime;
		pInstance->ev[x] = Data[x];
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}

EXPORT dmx_status fullLoadSplit(dmx_handle h, DMXDATA Data[512], DMXINT ut, DMXINT dt){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	clock_t currTime = clock();//CLK_TCK = number of clock ticks in 1 second /1000ms
	clock_t startTime = currTime;
	clock_t endUpTime = currTime + ((CLK_TCK*ut)/1000);
	clock_t endDownTime = currTime + ((CLK_TCK*dt)/1000);

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		pInstance->sv[x] = pInstance->cv[x];
		pInstance->ev[x] = Data[x];

		pInstance->st[x] = startTime;
		if(pInstance->sv[x] < pInstance->ev[x]){
			pInstance->et[x] = endUpTime;
		}
		else {
			pInstance->et[x] = endDownTime;
		}
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);
	
	return (dmx_status)0;
}

EXPORT dmx_status fullLoad(dmx_handle h, DMXDATA Data[512], DMXINT t){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	clock_t currTime = clock();//CLK_TCK = number of clock ticks in 1 second /1000ms
	clock_t startTime = currTime;
	clock_t endTime = currTime + ((CLK_TCK*t)/1000);

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		pInstance->sv[x] = pInstance->cv[x];
		pInstance->ev[x] = Data[x];
		pInstance->st[x] = startTime;
		pInstance->et[x] = endTime;
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);
	
	return (dmx_status)0;
}

EXPORT dmx_status partialLoadSplit(dmx_handle h, DMXDATA Data[512], DMXINT change[512], DMXINT ut, DMXINT dt){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	clock_t currTime = clock();//CLK_TCK = number of clock ticks in 1 second /1000ms
	clock_t startTime = currTime;

	clock_t endUpTime = currTime + ((CLK_TCK*ut)/1000);
	clock_t endDownTime = currTime + ((CLK_TCK*dt)/1000);

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		if(change[x] > 0){
			pInstance->sv[x] = pInstance->cv[x];
			pInstance->ev[x] = Data[x];
			
			pInstance->st[x] = startTime;
			if(pInstance->sv[x] < pInstance->ev[x]){
				pInstance->et[x] = endUpTime;
			}
			else{
				pInstance->et[x] = endDownTime;
			}
		}
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}

EXPORT dmx_status partialLoad(dmx_handle h, DMXDATA Data[512], DMXINT change[512], DMXINT t){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	clock_t currTime = clock();//CLK_TCK = number of clock ticks in 1 second /1000ms
	clock_t startTime = currTime;

	clock_t endTime = currTime + ((CLK_TCK*t)/1000);

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		if(change[x] > 0){
			pInstance->sv[x] = pInstance->cv[x];
			pInstance->ev[x] = Data[x];
			pInstance->st[x] = startTime;
			pInstance->et[x] = endTime;
		}
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}

EXPORT dmx_status complexLoadSplit(dmx_handle h, DMXDATA Data[512], DMXINT change[512], DMXINT ut[512], DMXINT dt[512]){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	clock_t currTime = clock();//CLK_TCK = number of clock ticks in 1 second /1000ms

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		if(change[x] > 0){
			pInstance->sv[x ] = pInstance->cv[x];
			pInstance->ev[x] = Data[x];
			
			pInstance->st[x] = currTime;

			if(pInstance->sv[x] < pInstance->ev[x]){
				pInstance->et[x] = currTime + ((CLK_TCK*ut[x])/1000);
			}
			else{
				pInstance->et[x] = currTime + ((CLK_TCK*dt[x])/1000);
			}
		}
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}

EXPORT dmx_status complexLoad(dmx_handle h, DMXDATA Data[512], DMXINT change[512], DMXINT t[512]){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	clock_t currTime = clock();//CLK_TCK = number of clock ticks in 1 second /1000ms

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		if(change[x] > 0){
			pInstance->sv[x] = pInstance->cv[x];
			pInstance->ev[x] = Data[x];
			pInstance->st[x] = currTime;
			pInstance->et[x] = currTime + ((CLK_TCK*t[x])/1000);
		}
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}

EXPORT dmx_status getCurrentFade(dmx_handle h, DMXDATA Data[512]){

	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		Data[x] = pInstance->cv[x];
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}

EXPORT dmx_status getCurrentOutput(dmx_handle h, DMXDATA Data[512]){
	dmx_instance *pInstance;
	pInstance = (dmx_instance*) h;
	if (pInstance == NULL){
		return (dmx_status)1;
	}

	WaitForSingleObject(pInstance->hSemaphore, INFINITE);

	for(DMXINT x = 0; x < pInstance->totalBits; x++){
		Data[x] = pInstance->ov[x];
	}

	ReleaseSemaphore(pInstance->hSemaphore,1,NULL);

	return (dmx_status)0;
}


EXPORT dmx_status getVersion(char* version){
	version = "0.5";
	return (dmx_status)0;
}



EXPORT dmx_status lookUpReturn(dmx_status error_code, char* error){
	switch((DMXINT)error_code){
		case 0:
			error = "Working properly.";
		break;

		case 1:
			error = "dmx_handle was NULL.";
		break;

		case 2:
			error = "dmx_handle not open.";
		break;

		case 3:
			error = "index out of range of DMX output bits.";
		break;
		
		case 4:
			error = "running in simulation mode.";
		break;
		
		case 5:
			error = "failed accessing the USB to DMX device driver.  Try reinstalling the device.";
		break;
		
		case 6:
			error = "too many data bits specified.  The DMX specification allows at most 512 bits of data.";
		break;

		default:
			error = "Unknown Error Code";
		break;
	}
	
	return (dmx_status)0;
}
