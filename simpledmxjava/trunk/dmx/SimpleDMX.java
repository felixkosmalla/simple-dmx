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

public class SimpleDMX implements DMX {
	private static int MAX_DMX_CONNECTIONS = 1;
	private static int NO_HANDEL = -1;
	private dmxConnect _connection;
	private int _handel;
	
	public SimpleDMX(){
		_connection = new dmxConnect();
		_handel = NO_HANDEL;
	}

	/********************
	 * setup            *
	 ********************/
	//refesh rate in MilliSeconds; 0 < TBits < 513
    public boolean open(int b, int r) {
    	_handel = _connection.open(b, r);
    	return isOpen();
    }
    
    public boolean close(){
    	int temp = _connection.close(_handel);
    	if (temp > 0)
    		return false;
    		
    	_handel = NO_HANDEL;
    	return true;
    }

	public boolean setStartCode(short s){
		return _connection.setStartCode(_handel, s) > 0 ? false : true;
	}
	
    public boolean profile(int i, short profile[]){
    	return _connection.setProfile(_handel, i, profile) > 0 ? false : true;
    }
    
    public boolean profile(short profile[][]) {
    	return _connection.setProfiles(_handel, profile) > 0 ? false : true;
    }
    
    public boolean clearProfile(int i){
    	return _connection.clearProfile(_handel, i) > 0 ? false : true;
    }
    
    public boolean clearProfile(){
    	return _connection.clearProfiles(_handel) > 0 ? false : true;
    }
  
    public boolean clearProportionalOutput(int index) {
    	return _connection.clearProportionalOutput(_handel, index) > 0 ? false : true;
	}

	public boolean clearProportionalOutputs() {
		return _connection.clearProportionalOutputs(_handel) > 0 ? false : true;
	}

	public boolean setGrandMaster(short level) {
		return _connection.setGrandMaster(_handel, level) > 0 ? false : true;
	}

	public boolean setProportionalOutput(int index, short level) {
		return _connection.setProportionalOutput(_handel, index, level) > 0 ? false : true;
	}

	public boolean setProportionalOutputs(short[] levels) {
		return _connection.setProportionalOutputs(_handel, levels) > 0 ? false : true;
	}
    
	/********************
	 * setup inspection *
	 ********************/
	public boolean isOpen(){return(_handel != NO_HANDEL);}
	public boolean isSimulation() {return _connection.isSimulation(_handel);}

	
	
	/*********************
	 * output inspection *
	 *********************/
	public short[] getCurrentFade(){
    	short data[] = new short[512];
    	return _connection.getCurrentFade(_handel, data) > 0 ? null : data;
	}
	
	public short[] getCurrentOutput(){
    	short data[] = new short[512];
    	return _connection.getCurrentOutput(_handel, data) > 0 ? null : data;
	}

	public String getVersion(){
		char ver[] = new char[512];
		return _connection.getVersion(ver) > 0 ? null : new String(ver);
	}
	
	
	
	/*********************
	 * fade control      *
	 *********************/
	
	public boolean stopFade() {return _connection.stopFade(_handel) > 0 ? false : true;}
	
	public void setSingle(int i, short Data){
		_connection.setSingle(_handel, i, Data);
	}
	
	public void setSingle(int i, short Data, int t){
		_connection.setSingleTimed(_handel, i, Data, t);
	}
	
	public void quickLoad(short Data[]){
    	_connection.quickLoad(_handel, Data);
	}
	
	public void fullLoad(short Data[], int ut, int dt, int d){
    	short temp[] = new short[Data.length];

    	for(int x = 0; x < Data.length; x++)
    		temp[x] = Data[x];
    	
    	new DelayedLoadFull(this, _connection, _handel, temp, d, ut, dt).start();
	}
	
	public void partialLoad(short[] Data, int ut, int dt, int udt, int ddt) {
		short current[] = getCurrentOutput();
		short up[] = new short[Data.length];
		short down[] = new short[Data.length];
		int upchanged[] = new int[Data.length];
		int downchanged[] = new int[Data.length];
		
		for(int x = 0; x < Data.length; x++){
			if(Data[x] < current[x]){
				down[x] = Data[x];
				downchanged[x] = 1;
				upchanged[x] = 0;
			}
			else{
				up[x] = Data[x];
				downchanged[x] = 0;
				upchanged[x] = 1;
			}
		}
		new DelayedLoadPartial(this, _connection, _handel, up, udt, upchanged,ut).start();
		new DelayedLoadPartial(this, _connection, _handel, down, ddt, downchanged,dt).start();
	}
	
	public void partialLoad(short Data[], int change[], int ut, int dt, int udt, int ddt){
		short current[] = getCurrentOutput();
		short up[] = new short[Data.length];
		short down[] = new short[Data.length];
		int upchanged[] = new int[Data.length];
		int downchanged[] = new int[Data.length];
		
		for(int x = 0; x < Data.length; x++){
			if(change[x] == 1){
				if(Data[x] < current[x]){
					down[x] = Data[x];
					downchanged[x] = 1;
					upchanged[x] = 0;
				}
				else{
					up[x] = Data[x];
					downchanged[x] = 0;
					upchanged[x] = 1;
				}
			}
			else{
				downchanged[x] = 0;
				upchanged[x] = 0;
			}
		}
		new DelayedLoadPartial(this, _connection, _handel, up, udt, upchanged, ut).start();
		new DelayedLoadPartial(this, _connection, _handel, down, ddt, downchanged, dt).start();
	}
	
	public void complexLoad(short Data[], int change[], int ut[], int dt[], int udt, int ddt){
		short current[] = getCurrentOutput();
		short up[] = new short[Data.length];
		short down[] = new short[Data.length];
		int upchanged[] = new int[Data.length];
		int downchanged[] = new int[Data.length];
		int uptime[] = new int[Data.length];
		int downtime[] = new int[Data.length];
		
		for(int x = 0; x < Data.length; x++){
			if(change[x] == 1){
				if(Data[x] < current[x]){
					down[x] = Data[x];
					downchanged[x] = 1;
					upchanged[x] = 0;
					downtime[x] = dt[x];
					uptime[x] = 0;
				}
				else{
					up[x] = Data[x];
					downchanged[x] = 0;
					upchanged[x] = 1;
					downtime[x] = 0;
					uptime[x] = ut[x];
				}
			}
			else{
				downchanged[x] = 0;
				upchanged[x] = 0;
			}
		}
		new DelayedLoadComplex(this, _connection, _handel, up, udt, upchanged, ut).start();
		new DelayedLoadComplex(this, _connection, _handel, down, ddt, downchanged, dt).start();
	}

}
