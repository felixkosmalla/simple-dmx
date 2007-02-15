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

import javax.swing.JComponent;
import javax.swing.JFrame;

import dmx.SimpleDMX;
import dmx.DMX;



public class TestPro2 implements Runnable{
	

    public static void main(String[] args) {
    	new TestPro2().testIt();	
    }
	
	public TestPro2()
	{}
	DMX dc;
	public void testIt()
    {
		int x;
        //Schedule a job for the event-dispatching thread:
        //creating and showing this application's GUI.
    	
		short cue[] = new short[512];
		short temp[] = new short[512];
		short BO[] = new short[512];
		int Patch[] = new int[512];
		int changed[] = new int[512];
		int upTime[] = new int[512];
		int downTime[] = new int[512];
		int waitTime[] = new int[512];
		
		short profiles[][] = new short[512][256];
		short profileNonDim[] = new short[256];
		short profileInvert[] = new short[256];
		
		for(x = 0; x<256; x++)
		{
			profileNonDim[x] = 255;
			profileInvert[x] = (short)(255 - x);
		}
		profileNonDim[0] = 0; 
		
		
		for(x = 0; x < 512; x++)
		{
			profiles[x] = profileInvert;
			cue[x] = -127;
			BO[x] = 0;
			temp[x] = 0;
			changed[x] = 0;
			upTime[x] = 0;
			downTime[x] = 0;
			waitTime[x] = 0;
			Patch[x] = x;
		}
    	
		
		
		dc = new SimpleDMX();
		
    	if(!dc.open(512, 20))
		{
			System.out.println("Could not open port!");
			return;
		}
		System.out.println("USB -> DMX Opened" );
		dc.close();
		System.out.println("USB -> DMX Closed" );
    	
		if(!dc.open(512, 20))
		{
			System.out.println("Could not open port!");
			return;
		}
		System.out.println("USB -> DMX Opened" );
		
		//dc.close();
		//dc.profile(0, profileNonDim);
		//dc.profile(0, profileInvert);
		dc.profile(profiles);
		
		
	    waitLocal(5000);
		
        javax.swing.SwingUtilities.invokeLater(this);
        System.out.println("Start GUI" );
        
        waitLocal(5000);
        
        
        
        
        
		System.out.println("Sectional Cross Fade Test: ON");
		for(x = 0; x < 6; x++)
		{
			temp[x] =  255;
			changed[x] = 1;
			upTime[x] = 2000*x;
			downTime[x] = 2000*x;
		}
		dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
		waitLocal(12000);
		dc.quickLoad(BO);
		waitLocal(1000);
		
		System.out.println("Sectional Cross Fade Test: ON GM @ 50");
		dc.setGrandMaster((short)128);
		for(x = 0; x < 6; x++)
		{
			temp[x] =  255;
			changed[x] = 1;
			upTime[x] = 2000*x;
			downTime[x] = 2000*x;
		}
		dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
		waitLocal(12000);
		dc.quickLoad(BO);
		waitLocal(1000);
		
		System.out.println("Sectional Cross Fade Test: ON PO @ 50");
		dc.setGrandMaster((short)255);
		for(x = 0; x < 6; x++)
		{
			dc.setProportionalOutput(x,(short)128);
			temp[x] =  255;
			changed[x] = 1;
			upTime[x] = 2000*x;
			downTime[x] = 2000*x;
		}
		dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
		waitLocal(12000);
		dc.quickLoad(BO);
		waitLocal(1000);
		
		System.out.println("Sectional Cross Fade Test: ON GM & PO @ 50");
		dc.setGrandMaster((short)128);
		for(x = 0; x < 6; x++)
		{
			//dc.setProportionalOutput(x,(short)128);
			temp[x] =  255;
			changed[x] = 1;
			upTime[x] = 2000*x;
			downTime[x] = 2000*x;
		}
		dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
		waitLocal(12000);
		dc.quickLoad(BO);
		waitLocal(1000);
		
		System.out.println("Sectional Cross Fade Test: ON & Stop");
		dc.setGrandMaster((short)255);
		dc.clearProportionalOutputs();
		for(x = 0; x < 6; x++)
		{
			temp[x] =  255;
			changed[x] = 1;
			upTime[x] = 4000*x;
			downTime[x] = 4000*x;
		}
		dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
		waitLocal(6000);
		dc.stopFade();
		waitLocal(4000);
		dc.quickLoad(BO);
		
		waitLocal(2000);
        System.out.println("Set Single Test:BUMB ON");
		dc.setSingle(0, (short)255);
		System.out.println("Channel: " + "0");
		waitLocal(3000);
		dc.setSingle(0, (short)0);
		waitLocal(3000);
		System.out.println("Set Single Test:FADE ON");
		dc.setSingle(0, (short)255, 5000);
		System.out.println("Channel: " + "0");
		waitLocal(7000);
		System.out.println("Set Single Test:FADE OFF");
		dc.setSingle(0, (short)0, 5000);
		System.out.println("Channel: " + "0");
		waitLocal(7000);
		
        System.out.println("Overlaping Delayed Bumb Test: ON");
		for(x = 0; x < 6; x++)
		{
			temp[x] =  255;
			changed[x] = 1;
			//quickLoad(usb, startCode, temp);
			dc.partialLoad(temp, changed, 0, 0, (7000 - x*1000), (7000 - x*1000));
			System.out.println("Channel: " + x);
			waitLocal(1000);
			temp[x] = 0;
			changed[x] = 0;

		}
		waitLocal(3000);
	
        System.out.println("Overlaping Delayed Bumb Test: OFF");
		for(x = 0; x < 6; x++)
		{
			temp[x] =  0;
			changed[x] = 1;
			//quickLoad(usb, startCode, temp);
			dc.partialLoad(temp, changed, 0, 0, (7000 - x*1000), (7000 - x*1000));
			System.out.println("Channel: " + x);
			waitLocal(1000);
			temp[x] = 0;
			changed[x] = 0;

		}
		waitLocal(2000);
		
		//dc.quickLoad(BO);
		waitLocal(1000);
        
		System.out.println("Delayed Simple Cross Fade Test"  );
		for(x = 0; x < 6; x++)
		{
			temp[x] = 255;
			dc.fullLoad(temp, 0, 0, 3000);
			System.out.println("Channel: " + x);
			waitLocal(1000);
			temp[x] = 0;

		}
		//waitLocal(5000);
		dc.quickLoad(BO);
		waitLocal(5000);
        
        
    	System.out.println("Overlaping Cross Fade Level Feedback Test");
		for(x = 0; x < 6; x++)
		{
			temp[x] = 255;
			changed[x] = 1;
			//quickLoad(usb, startCode, temp);
			dc.partialLoad(temp, changed, 7000, 7000, 0, 0);
			System.out.println("Channel: " + x);
			waitLocal(3000);
			printDMX(dc);
			temp[x] = 0;
			changed[x] = 0;

		}

		System.out.println("Simple Bumb Test");
		for(x = 0; x < 6; x++)
		{
			temp[x] =  255;
			dc.quickLoad(temp);
			System.out.println("Channel: " + x);
			waitLocal(1000);
			temp[x] =  0;

		}
		dc.quickLoad(BO);
		waitLocal(1000);
		

		System.out.println("Sectional Cross Fade Test: ON");
		for(x = 0; x < 6; x++)
		{
			temp[x] =  255;
			changed[x] = 1;
			upTime[x] = 4000*x;
			downTime[x] = 4000*x;
		}
		dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
		waitLocal(24000);


		System.out.println("Sectional Cross Fade Test: OFF");
		for(x = 0; x < 6; x++)
		{
			temp[x] =  0;
			changed[x] = 1;
			upTime[x] = 4000*x;
			downTime[x] = 4000*x;
		}
		dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
		waitLocal(24000);
		dc.quickLoad(BO);
		waitLocal(1000);

		

		System.out.println("Overlaping Delayed Bumb Test: ON");
		for(x = 0; x < 6; x++)
		{
			temp[x] =  255;
			changed[x] = 1;
			waitTime[x] = (7000 - x*1000);
			//quickLoad(usb, startCode, temp);
			dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
			System.out.println("Channel: " + x);
			waitLocal(1000);
			temp[x] = 0;
			changed[x] = 0;

		}
		waitLocal(2000);
		dc.quickLoad(BO);
		waitLocal(1000);


		System.out.println("Sectional Cross Fade Test: BUMB ON");
		for(x = 0; x < 6; x++)
		{
			temp[x] = 255;
			changed[x] = 1;
			upTime[x] = 0;
			downTime[x] = 0;
		}
		dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
		waitLocal(1000);


		System.out.println("Sectional Cross Fade Test: BUMB OFF");
		for(x = 0; x < 6; x++)
		{
			temp[x] = 0;
			changed[x] = 1;
			upTime[x] = 0;
			downTime[x] = 0;
		}
		dc.complexLoad(temp, changed, upTime, downTime, 0, 0);
		waitLocal(1000);
		dc.quickLoad(BO);
		waitLocal(1000);



		System.out.println("Overlaping Cross Fade Test: ON");
		for(x = 0; x < 6; x++)
		{
			temp[x] = 255;
			changed[x] = 1;
			//quickLoad(usb, startCode, temp);
			dc.partialLoad(temp, changed, 7000, 7000, 0, 0);
			System.out.println("Channel: " + x);
			waitLocal(3000);
			temp[x] = 0;
			changed[x] = 0;

		}
//				quickLoad(usb, startCode, BO);
		waitLocal(1000);

		System.out.println("Overlaping Cross Fade Test: OFF");
		for(x = 0; x < 6; x++)
		{
			temp[x] = 0;
			changed[x] = 1;
			dc.partialLoad(temp, changed, 7000, 7000, 0, 0);
			System.out.println("Channel: " + x);
			waitLocal(3000);
			temp[x] = 0;
			changed[x] = 0;

		}
		dc.quickLoad(BO);
		waitLocal(1000);


		System.out.println("Overlaping Cross Fade Test: Bumb On");
		for(x = 0; x < 6; x++)
		{
			temp[x] = 255;
			changed[x] = 1;
			dc.partialLoad(temp, changed, 0, 0, 0, 0);
			System.out.println("Channel: " + x);
			waitLocal(1000);
			temp[x] = 0;
			changed[x] = 0;

		}

		System.out.println("Overlaping Cross Fade Test: Bumb OFF");
		for(x = 0; x < 6; x++)
		{
			temp[x] = 0;
			changed[x] = 1;
			dc.partialLoad(temp, changed, 0, 0, 0, 0);
			System.out.println("Channel: " + x);
			waitLocal(1000);
			temp[x] = 0;
			changed[x] = 0;

		}
		dc.quickLoad(BO);
		waitLocal(1000);


		System.out.println("Simple Cross Fade Test"  );
		for(x = 0; x < 6; x++)
		{
			temp[x] = 255;
			dc.fullLoad(temp, 5000, 2000, 0);
			System.out.println("Channel: " + x);
			waitLocal(5000);
			temp[x] = 0;

		}
		dc.quickLoad(BO);
		waitLocal(1000);
		
        
		System.out.println("Simple Cross Fade Bumb Test");
		for(x = 0; x < 6; x++)
		{
			temp[x] = 255;
			dc.fullLoad(temp, 5000, 5000, 0);
			System.out.println("Channel: " + x);
			waitLocal(7000);
			temp[x] = 0;
			
		}
		dc.quickLoad(BO);
		waitLocal(1000);
		
        
		System.out.println("Simple Bumb Test"  );
		for(x = 0; x < 6; x++)
		{
			temp[x] = 255;
			dc.quickLoad(temp);
			System.out.println("Channel: " + x);
			waitLocal(1000);
			temp[x] = 0;

		}
		dc.quickLoad(BO);
		waitLocal(1000);
	
		
		dc.close();
		System.out.println("Connection Closed");
//			System.out.println("after close. \n");

    }
	
	public void run() {createAndShowGUI();}
	
    private void createAndShowGUI() {
        //Make sure we have nice window decorations.
        JFrame.setDefaultLookAndFeelDecorated(true);

        //Create and set up the window.
        JFrame frame = new JFrame("Simple DMX Test Output");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        //Create and set up the content pane.
        JComponent newContentPane = new TTest(dc);
        newContentPane.setOpaque(true); //content panes must be opaque
        frame.setContentPane(newContentPane);

        //Display the window.
        frame.pack();
        frame.setVisible(true);
        
        //Make sure we have nice window decorations.
        JFrame.setDefaultLookAndFeelDecorated(true);

        //Create and set up the window.
        JFrame frame2 = new JFrame("Simple DMX Test Fade");
        frame2.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        //Create and set up the content pane.
        JComponent newContentPane2 = new TTestF(dc);
        newContentPane.setOpaque(true); //content panes must be opaque
        frame2.setContentPane(newContentPane2);

        //Display the window.
        frame2.pack();
        frame2.setVisible(true);
    }    
	
	    private void waitLocal(int t)
	    {
	    	try
	    	{
	    		//this.wait((long) t);
	    		Thread.sleep(t);
	    		//Thread.sleep(0);
	    	}
	    	catch (InterruptedException e)
	    	{
	    		System.out.println("waitLocaling Interrupted Exception: " + e.toString());
	    	}
	    }
	    
	    private void printDMX(DMX conn)
	    {
	    	short dd[] = new short[512];
	    	dd = conn.getCurrentOutput();
	    	for(int y = 0; y<512; y++)
	    		if (dd[y] > 0)
	    			System.out.println("DIM " + y + ": " + (int)dd[y]);
	    }

}
