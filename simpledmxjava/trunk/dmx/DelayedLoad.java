package dmx;

abstract class DelayedLoad extends Thread{
	private DMX _controler;
	protected dmxConnect _connection;
	protected int _handel;
	
	protected short _data[];
	protected int _delay;

	protected DelayedLoad(DMX caller, dmxConnect connection, int handel, short[] data, int delay){
		_controler = caller;
		_connection = connection;
		_handel = handel;
		_data = data;
		_delay = delay;
	}
	
	public void run(){
		try{
			sleep(_delay);
		}
    	catch (InterruptedException e)
    		{System.out.println("waitLocaling Interrupted Exception: " + e.toString());}
    	if(_controler.isOpen())
    		load();
	}
	
	protected abstract void load();
	
}
