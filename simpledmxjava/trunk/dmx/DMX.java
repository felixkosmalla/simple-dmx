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

public interface DMX {

	public boolean setStartCode(short s);

	public boolean open(int b, int r);
	public boolean close();
	public boolean isOpen();

	public boolean isSimulation();
    public boolean stopFade();
    
	public boolean profile(int i, short profile[]);
	public boolean profile(short profile[][]);

	public boolean clearProfile(int i);
	public boolean clearProfile();
	
	public boolean setProportionalOutput(int index, short level);
	public boolean setProportionalOutputs(short levels[]);
	public boolean clearProportionalOutput(int index);
	public boolean clearProportionalOutputs();
	
	public boolean setGrandMaster(short level);

	public void setSingle(int i, short Data);
	public void setSingle(int i, short Data, int t);

	public void quickLoad(short Data[]);
	public void fullLoad(short Data[], int ut, int dt, int d);
	public void partialLoad(short Data[], int ut,int dt, int udt, int ddt);
	public void partialLoad(short Data[], int change[], int ut,int dt, int udt, int ddt);
	public void complexLoad(short Data[], int change[], int ut[],int dt[], int udt, int ddt);

	public short[] getCurrentFade();
	public short[] getCurrentOutput();
	
	public String getVersion();
}