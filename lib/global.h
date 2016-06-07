#ifdef motion
  #define motion	
	int gs = 0; //global speed to be shared by all async methods
	int gd = 0; //global distance away from the wall to use when wall following
	int gm = 0; //global mode variable to be used by any method
	int gc = 0; //global clock variable, may be time or iterations
	int async_state = -1;
#endif