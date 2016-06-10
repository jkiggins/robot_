//GENERAL PID
	class PID
	{
		float lt, ct;

		public:
			float pidd[4]; //err, last error, running integral, adjust
			float w[3];
			void set_pid(float p, float i, float d);
			float slice(float err, float dt);
	};

typedef	struct 
	{
		float low, high;
	}bounds;

class TUNEPID
{
private:
	PID pidt;
	bounds p_b, i_b, d_b, t_b; //last one is test boundry
	float perr, ierr, derr, precision, tp;
	int state, test_count, cb, test_index;
	int max_err[2];

	void tune_p();
	void tune_i();
	void tune_d();
	void reset_max();

public:
	TUNEPID(PID * arg_pidt);
	void tune_data();
	void save_tune();
	void load_tune(int addr);
	void test_boundry();
};

TUNEPID::TUNEPID(PID * arg_pidt, float arg_perr, float arg_ierr, float arg_derr)
{
	pidt = arg_pidt;
	p_b.low = plow;
	p_b.high = phigh;
	i_s = i_scale;
	d_s = d_scale;
	precision = arg_pre;
	state = 0;
}

void TUNEPID::tune_data()
{
	if(abs(pidt.pidd[0]) > max[])
		max_err[test_index] = pidt.pidd[0];

	switch(state)
	{
		case 0:
			tune_p();break;
		case 1:
			tune_i();break;
		case 2:
			tune_d();break;
	}
}

void TUNEPID::tune_p()
{
	//based on test_count determine test_index
	test_index = test_count / 500;
}