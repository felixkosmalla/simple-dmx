package dmx;
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

class dmxConnect{
    public native int open(int tBits, int rRate); //rate in MilliSeconds; 0 < TBits < 513
    public native int close(int h);
    
    public native boolean isSimulation(int h);
    public native int stopFade(int h);
    
    public native int setStartCode(int h, short sc);

    public native int setProfile(int h, int index, short profile[]);
	public native int setProfiles(int h, short profiles[][]);
	public native int clearProfile(int h, int i);
	public native int clearProfiles(int h);
	
	public native int setProportionalOutput(int h, int i, short level);
	public native int setProportionalOutputs(int h, short levels[]);
	public native int clearProportionalOutput(int h, int i);
	public native int clearProportionalOutputs(int h);
	
	public native int setGrandMaster(int h, short level);

	
	public native int setSingle(int h, int index, short Data);
	public native int setSingleTimed(int h, int index, short Data, int t);
	
	public native int quickLoad(int h, short Data[]);
	public native int fullLoadSplit(int h, short Data[], int ut, int dt);
	public native int fullLoad(int h, short Data[], int t);
	public native int partialLoadSplit(int h, short Data[], int change[], int ut, int dt);
	public native int partialLoad(int h, short Data[], int change[], int t);
	public native int complexLoadSplit(int h, short Data[], int change[], int ut[], int dt[]);
	public native int complexLoad(int h, short Data[], int change[], int t[]);

	public native int getCurrentFade(int h, short Data[]);
	public native int getCurrentOutput(int h, short Data[]);

	public native int getVersion(char version[]);
	public native int lookUpReturn(int error_code, char error[]);

    static {
        System.loadLibrary("dmxConnect");
    }
}
