struct TimeStamp {
  	1: i64 time
	2: i64 id
}


service lamportservice {    
	TimeStamp request(1:TimeStamp timestamp);
	void release(1:TimeStamp timestamp);
	void lock();
	void unlock();
}
