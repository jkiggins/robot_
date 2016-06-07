#ifndef asee_h
  #define asee_h

  //MISC
  #define TPM 600
  #define LAST_DIMENSION 1 


  //Async flags
    #define LINEF 0
    #define DRIVED 1
    #define LFSET 2
    #define WALLF 3 
  
  //INIT
    void init_a();
    void blink_led();

  //ASYNC
    void async_reset();
    void async();
    void no_state();


#endif
