package dmx;

class DelayedLoadFull extends DelayedLoad {
	private int _upTime;
	private int _downTime;
	
	DelayedLoadFull(DMX caller, dmxConnect connection, int handel, short[] data, int delay, int upTime, int downTime){
		super(caller, connection, handel, data, delay);
		_upTime = upTime;
		_downTime = downTime;
	}
	
	@Override
	protected void load() {
		_connection.fullLoadSplit(_handel, _data, _upTime, _downTime);
	}

}
