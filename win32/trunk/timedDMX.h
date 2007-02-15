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

#ifdef BUILD_DLL
   // the dll exports
   #define EXPORT __declspec(dllexport)
   #else
   // the exe imports
   #define EXPORT __declspec(dllimport)
#endif

#define DMXINT		unsigned int
#define DMXDATA		unsigned char //This value should range from [0..255]
#define dmx_handle	DMXINT
#define dmx_status	DMXINT
//#define NULL		(void *)0


EXPORT bool	   isSimulation(dmx_handle h);

EXPORT dmx_handle open(DMXINT tBits, DMXINT rRate); //rate in MilliSeconds; 0 < TBits < 513
EXPORT dmx_status close(dmx_handle h);

EXPORT dmx_status setStartCode(dmx_handle h, DMXDATA startCode);

EXPORT dmx_status setProfile(dmx_handle h, DMXINT index, DMXDATA profile[256]);
EXPORT dmx_status setProfiles(dmx_handle h, DMXDATA profiles[512][256]);
EXPORT dmx_status clearProfile(dmx_handle h, DMXINT index);
EXPORT dmx_status clearProfiles(dmx_handle h);

EXPORT dmx_status setProportionalOutput(dmx_handle h, DMXINT index, DMXDATA level);
EXPORT dmx_status setProportionalOutputs(dmx_handle h, DMXDATA levels[512]);
EXPORT dmx_status clearProportionalOutput(dmx_handle h, DMXINT index);
EXPORT dmx_status clearProportionalOutputs(dmx_handle h);

EXPORT dmx_status setGrandMaster(dmx_handle h, DMXDATA level);

EXPORT dmx_status stopFade(dmx_handle h);

EXPORT dmx_status setSingle(dmx_handle h, DMXINT index, DMXDATA Data);
EXPORT dmx_status setSingleTimed(dmx_handle h, DMXINT index, DMXDATA Data, DMXINT t);

EXPORT dmx_status quickLoad(dmx_handle h, DMXDATA Data[512]);
EXPORT dmx_status fullLoadSplit(dmx_handle h, DMXDATA Data[512], DMXINT ut, DMXINT dt);
EXPORT dmx_status fullLoad(dmx_handle h, DMXDATA Data[512], DMXINT t);
EXPORT dmx_status partialLoadSplit(dmx_handle h, DMXDATA Data[512], DMXINT change[512], DMXINT ut, DMXINT dt);
EXPORT dmx_status partialLoad(dmx_handle h, DMXDATA Data[512], DMXINT change[512], DMXINT t);
EXPORT dmx_status complexLoadSplit(dmx_handle h, DMXDATA Data[512], DMXINT change[512], DMXINT ut[512], DMXINT dt[512]);
EXPORT dmx_status complexLoad(dmx_handle h, DMXDATA Data[512], DMXINT change[512], DMXINT t[512]);

EXPORT dmx_status getCurrentFade(dmx_handle h, DMXDATA Data[512]);
EXPORT dmx_status getCurrentOutput(dmx_handle h, DMXDATA Data[512]);

EXPORT dmx_status getVersion(char* version);
EXPORT dmx_status lookUpReturn(dmx_status error_code, char* error);


