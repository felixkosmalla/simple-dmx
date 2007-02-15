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

//import java.awt.*;
//import java.awt.event.*;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;
import dmx.DMX;
/*
 * ProgressBarDemo.java is a 1.4 application that requires these files:
 *   LongTask.java
 *   SwingWorker.java
 */
public class TTestF extends JPanel  implements ActionListener{
    public final static int ONE_SECOND = 1000;

    	private DMX conn;
    	private static final long serialVersionUID = 1L;
    	private String[] columnNames = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"};
    	private JTable table;
    	private Integer data[][] = new Integer[32][16];
    	private Timer timer;
    	private short dd[] = new short[512];

    	public TTestF(DMX d)
    	{
    		super(new BorderLayout());
    		conn = d;

    		for(int y = 0; y < 512; y++)
    			dd[y] = 0;
    		
    		for(int x = 0; x < 32; x++)
    			for(int y = 0; y < 16; y++)
    				data[x][y] = new Integer(0);
    		
    		table = new JTable(data, columnNames);
    		for(int x = 0; x < 16; x++)
    		{
    			table.getColumnModel().getColumn(x).setMaxWidth(30);
    		}
    		add(table);	
    		//timer = new Timer(100, new TableUpdater(d, table));
    		timer = new Timer(50, this);
    		timer.start();
    		/*
    		Thread t = new Thread(new TableUpdater(conn, table));
    		//Thread t = new Thread(g);
    		t.start();
    		*/
    	}
    	
    	public void actionPerformed(ActionEvent e) 
    	{
    			dd = conn.getCurrentFade();
    			
    			if(dd != null)
    			{
    				for(int x = 0; x < 32; x++)
    					for(int y = 0; y < 16; y++)
    						table.setValueAt(new Integer((int)dd[x*16+y]),x,y);
    				
    				table.updateUI();
    			}
    	}

    /**
     * Create the GUI and show it.  For thread safety,
     * this method should be invoked from the
     * event-dispatching thread.
     */


    
  
    

}
