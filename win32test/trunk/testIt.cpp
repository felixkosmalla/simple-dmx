/************************************************************************************
 *  Copyright 2006 Carleton Coffrin
 * 
 *  This file is part of Straight DMX.
 *  
 *  Straight DMX is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  Straight DMX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with Straight DMX; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *  
 *************************************************************************************/

#include "timedDMX.h"

#include <iostream>
#include <windows.h>

void printDMX(dmx_handle d);

int main (int argc, char *argv[]) {
	DMXDATA startCode = (DMXDATA)0;
	DMXDATA cue[512];
	DMXDATA temp[512];
	DMXDATA BO[512];
	//DMXINT Patch[512];
	DMXINT changed[512];
	DMXINT upTime[512];
	DMXINT downTime[512];
	DMXINT waitTime[512];
	int x;
	for(x = 0; x < 512; x++)
	{
		cue[x] = 128;
		BO[x] = 0;
		temp[x] = 0;
		changed[x] = 0;
		upTime[x] = 0;
		downTime[x] = 0;
		waitTime[x] = 0;
	//	Patch[x] = x;
	}
	//for(x = 0; x < 512; x++)
		

	//cout << "before load. \n";
	dmx_handle usb = open((DMXINT)512, (DMXINT)20);
	if(isSimulation(usb))
	{
		std::cout << "Could not open port! Running in simulation mode\n";
		//return;
	}

	std::cout << "after open: " << usb <<" \n";

	close(usb);

	std::cout << "after Close: " << usb <<" \n";
//	Sleep(5000);

	usb = open((DMXINT)512, (DMXINT)20);
	if(isSimulation(usb))
	{
		std::cout << "Could not open port! Running in simulation mode\n";
		//return;
	}

	std::cout << "after open: " << usb <<" \n";
	setStartCode(usb, startCode);
	/*
	Patch[0] = 7;
	Patch[1] = 1;
	Patch[2] = 5;
	Patch[3] = 8;
	Patch[4] = 12;

	Patch[7] = 0;
	Patch[5] = 2;
	Patch[8] = 3;
	Patch[12] = 4;
	*/

/*	for(x = 0; x < 13; x++)
	{
		cout << "Patch[" << x << "] = " << Patch[x] << endl;
	}//*/
		

	//patch(usb, Patch);
//	Sleep(1000);
	//clearPatch(usb);

	quickLoad(usb, BO);
	Sleep(1000);

/*
	cout << "Overlaping Cross Fade Delayed Up and Down Times" << endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		changed[x] = 1;
		//quickLoad(usb, startCode, temp);
		partialLoad(usb, startCode, temp, changed, 6000, 2000, 1000, 5000);
		cout << "Channel: " << x << endl;
		Sleep(7000);
		printDMX(usb);
		temp[x] = 0;
		changed[x] = 0;

	}
	*/

	stopFade(usb);
	std::cout << "Overlaping Cross Fade Level Feedback Test" << std::endl;
	for(x = 0; x < 6; x++){
		temp[x] = (DMXDATA)255;
		changed[x] = 1; //quickLoad(usb, startCode, temp);
		partialLoadSplit(usb, temp, changed, 7000, 7000);
		std::cout << "Channel: " << x << std::endl;
		Sleep(3000);
		printDMX(usb);
		temp[x] = 0;
		changed[x] = 0;

	}
	
	quickLoad(usb, BO);
	Sleep(1000);
	
	setGrandMaster(usb, 128);
	std::cout << "Overlaping Cross Fade Level Feedback Test - GM @ 50" << std::endl;
	for(x = 0; x < 6; x++){
		temp[x] = (DMXDATA)255;
		changed[x] = 1; //quickLoad(usb, startCode, temp);
		partialLoadSplit(usb, temp, changed, 7000, 7000);
		std::cout << "Channel: " << x << std::endl;
		Sleep(3000);
		printDMX(usb);
		temp[x] = 0;
		changed[x] = 0;

	}
	
	quickLoad(usb, BO);
	Sleep(1000);
	
	setGrandMaster(usb, 255);
	setProportionalOutputs(usb, cue);
	std::cout << "Overlaping Cross Fade Level Feedback Test - PO @ 50" << std::endl;
	for(x = 0; x < 6; x++){
		temp[x] = (DMXDATA)255;
		changed[x] = 1; //quickLoad(usb, startCode, temp);
		partialLoadSplit(usb, temp, changed, 7000, 7000);
		std::cout << "Channel: " << x << std::endl;
		Sleep(3000);
		printDMX(usb);
		temp[x] = 0;
		changed[x] = 0;

	}
	
	
	quickLoad(usb, BO);
	Sleep(1000);
	
	setGrandMaster(usb, 126);
	std::cout << "Overlaping Cross Fade Level Feedback Test - PO & GM @ 50" << std::endl;
	for(x = 0; x < 6; x++){
		temp[x] = (DMXDATA)255;
		changed[x] = 1; //quickLoad(usb, startCode, temp);
		partialLoadSplit(usb, temp, changed, 7000, 7000);
		std::cout << "Channel: " << x << std::endl;
		Sleep(3000);
		printDMX(usb);
		temp[x] = 0;
		changed[x] = 0;

	}
	
	setGrandMaster(usb, 255);
	clearProportionalOutputs(usb);

	std::cout << "Simple Bumb Test" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		quickLoad(usb, temp);
		std::cout << "Channel: " << x << std::endl;
		Sleep(1000);
		temp[x] = (DMXDATA)0;

	}//*/
	quickLoad(usb, BO);
	Sleep(1000);
	

	std::cout << "Sectional Cross Fade Test: ON" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		changed[x] = 1;
		upTime[x] = 4000*x;
		downTime[x] = 4000*x;
	}//*/
	complexLoadSplit(usb, temp, changed, upTime, downTime);
	Sleep(24000);


	std::cout << "Sectional Cross Fade Test: OFF" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)0;
		changed[x] = 1;
		upTime[x] = 4000*x;
		downTime[x] = 4000*x;
	}//*/
	complexLoadSplit(usb, temp, changed, upTime, downTime);
	Sleep(24000);
	quickLoad(usb, BO);
	Sleep(1000);

	std::cout << "Overlaping Delayed Bumb Test: ON" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		changed[x] = 1;
		//quickLoad(usb, startCode, temp);
		partialLoadSplit(usb, temp, changed, 0, 0);
		std::cout << "Channel: " << x << std::endl;
		Sleep(1000);
		temp[x] = 0;
		changed[x] = 0;

	}//*/
	Sleep(2000);
	quickLoad(usb, BO);
	Sleep(1000);

	std::cout << "Overlaping Delayed Bumb Test: ON" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		changed[x] = 1;
		waitTime[x] = (7000 - x*1000);
		//quickLoad(usb, startCode, temp);
		complexLoadSplit(usb, temp, changed, upTime, downTime);
		std::cout << "Channel: " << x << std::endl;
		Sleep(1000);
		temp[x] = 0;
		changed[x] = 0;

	}//*/
	Sleep(2000);
	quickLoad(usb, BO);
	Sleep(1000);

	std::cout << "Sectional Cross Fade Test: BUMB ON" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		changed[x] = 1;
		upTime[x] = 0;
		downTime[x] = 0;
	}//*/
	complexLoadSplit(usb, temp, changed, upTime, downTime);
	Sleep(1000);


	std::cout << "Sectional Cross Fade Test: BUMB OFF" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)0;
		changed[x] = 1;
		upTime[x] = 0;
		downTime[x] = 0;
	}//*/
	complexLoadSplit(usb, temp, changed, upTime, downTime);
	Sleep(1000);
	quickLoad(usb, BO);
	Sleep(1000);



//*/
//	quickLoad(usb, startCode, BO);
	Sleep(1000);

	std::cout << "Overlaping Cross Fade Test: OFF" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)0;
		changed[x] = 1;
		partialLoadSplit(usb, temp, changed, 7000, 7000);
		std::cout << "Channel: " << x << std::endl;
		Sleep(3000);
		temp[x] = 0;
		changed[x] = 0;

	}//*/
	quickLoad(usb, BO);
	Sleep(1000);


	std::cout << "Overlaping Cross Fade Test: Bumb On" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		changed[x] = 1;
		partialLoadSplit(usb, temp, changed, 0, 0);
		std::cout << "Channel: " << x << std::endl;
		Sleep(1000);
		temp[x] = 0;
		changed[x] = 0;

	}//*/

	std::cout << "Overlaping Cross Fade Test: Bumb OFF" << std::endl;
	for(x = 0; x < 6; x++){
		temp[x] = (DMXDATA)0;
		changed[x] = 1;
		partialLoadSplit(usb, temp, changed, 0, 0);
		std::cout << "Channel: " << x << std::endl;
		Sleep(1000);
		temp[x] = 0;
		changed[x] = 0;

	}//*/
	quickLoad(usb, BO);
	Sleep(1000);


	std::cout << "Simple Cross Fade Test" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		fullLoadSplit(usb, temp, 5000, 2000);
		std::cout << "Channel: " << x << std::endl;
		Sleep(5000);
		temp[x] = (DMXDATA)0;

	}//*/
	quickLoad(usb, BO);
	Sleep(1000);

	std::cout << "Simple Cross Fade Bumb Test" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		fullLoadSplit(usb, temp, 0, 0);
		std::cout << "Channel: " << x << std::endl;
		Sleep(5000);
		temp[x] = (DMXDATA)0;

	}//*/
	quickLoad(usb, BO);
	Sleep(1000);

	std::cout << "Simple Bumb Test" << std::endl;
	for(x = 0; x < 6; x++)
	{
		temp[x] = (DMXDATA)255;
		quickLoad(usb, temp);
		std::cout << "Channel: " << x << std::endl;
		Sleep(1000);
		temp[x] = (DMXDATA)0;

	}//*/
	quickLoad(usb, BO);
	Sleep(1000);
	
	close(usb);
//	cout << "after close. \n";
}

void printDMX(dmx_handle d)
{
	DMXDATA dd[512];
	for(int x = 0; x<512; x++)
		dd[x] = 0;

	getCurrentOutput(d, dd);
	for(int y = 0; y<512; y++)
		if (dd[y] > 0)
			std::cout << "DIM "<< y << ": " << (unsigned int)dd[y] << "\n";
}
