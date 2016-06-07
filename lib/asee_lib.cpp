#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <limits.h>

extern int gs; //global speed to be shared by all async methods
extern int gd; //global distance away from the wall to use when wall following
extern int gm; //global mode variable to be used by any method
extern int gc; //global clock variable, may be time or iterations
extern int async_state = -1;

//ASYNC
  float dt;

  void async_reset()
  {
    switch(async_state)
    {
      case LINEF:
        pidlf.set_pid(.9, 0, 32);
        w = 0; wsum = 0; adj = 0; pos = CENTEROFLINE;
        break;
      case LFSET:
        pidlf.set_pid(.5,0,200);
        async_state = LINEF;
      case DRIVED:
        pida.set_pid(500, 0, 50);
        dd = 0;motion[0] = 0; adj = 0;dd_flag = 1;
        break;
      case WALLF:
        pidwf.set_pid(.4, 0, 6.8);
        break;
    }
    em = 0;
  }

  void async()
  {
  	dt = get_emili();

    update(dt);
    digitalWrite(13, LOW);

    switch(async_state)
    {
      case LINEF:

      	lf_async(dt);

        break;
      /////////////////////////////////////////////////////////////////////
      case DRIVED:
      	dr_async();
        break;
      ////////////////////////////////////////////////////////////////////
      case WALLF:
      	wf_async(dt);
        break;
      }
  }
