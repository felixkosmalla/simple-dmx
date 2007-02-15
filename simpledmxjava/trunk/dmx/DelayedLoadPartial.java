package dmx;

class DelayedLoadPartial extends DelayedLoad {
	private int _change[];
	private int _time;
	
	DelayedLoadPartial(DMX caller, dmxConnect connection, int handel, short[] data, int delay, int[] change, int time){
		super(caller, connection, handel, data, delay);
		_change = change;
		_time = time;
	}
	
	@Override
	protected void load() {
		_connection.partialLoad(_handel, _data, _change, _time);
	}

}
