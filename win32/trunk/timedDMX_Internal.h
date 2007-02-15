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
 
#include "timedDMX.h"

#include <time.h>
#include <windows.h>

#include "FTD2XX.h"
#define DRIVER_HANDLE FT_HANDLE

typedef struct {
	bool simulation;
	DWORD dwSenderThread;
	HANDLE hSemaphore;

	DMXINT totalBits;
	DRIVER_HANDLE dHandle;
	
	DMXDATA profile[512][256];

    DMXINT framesSent;
    INT32 sendDelay; //MS
	INT32 send;
	
	//NOTE: pov and grandMaster are setup as lookup tables to avoide multiplication and numerical inacuracy in the output send code
	
	DMXDATA startCode;
	DMXDATA grandMaster[256];
	DMXDATA pov[512][256]; //Proportional Output Value
	DMXDATA ov[512];  //Output Value (after profile)
	DMXDATA cv[512];  //Current Fade Value
	DMXDATA sv[512];  //Start Fade Value
	DMXDATA ev[512];  //End Fade Value
	clock_t st[512];  //Fade Start Time
	clock_t et[512];  //Fade End Time
		
	
}dmx_instance;

