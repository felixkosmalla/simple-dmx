package dmx;

class DelayedLoadComplex extends DelayedLoad {
	private int _change[];
	private int _time[];
	
	DelayedLoadComplex(DMX caller, dmxConnect connection, int handel, short[] data, int delay, int[] change, int[] time){
		super(caller, connection, handel, data, delay);
		_change = change;
		_time = time;
	}
	
	@Override
	protected void load() {
		_connection.complexLoad(_handel, _data, _change, _time);
	}

}
