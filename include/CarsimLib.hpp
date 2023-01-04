#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include "vs_deftypes.h" // VS types and definitions
#include "vs_api.h"      // VS API definitions as prototypes

// infomation from carsim
class InfoFromCarsim
{
public:
	double XCG_TM;	// X in Glocal
	double YCG_TM;	// Y in Flobal
	double YAW;		// Yaw angle

	double VX;		// x speed
	double VY;		// y speed
	double AVZ;		// yaw rate

	double AX;		// x accel
	double AY;		// y accel

	double TSTART;	// Carsim Start Time : 0
	double PBK_CON;	// brake pressure
	// double MFUEL;	// consumed fuel mass
	// double AV_ENG;	// engine rotation

	double GEARAUTO;// gear number in automatic shift control
};

// information to carsim
class InfoToCarsim
{
public:
	double IMP_STEER_SW;		// steering input
	double IMP_THROTTLE_ENGINE;	// throttele input
	double IMP_FBK_PDL;			// brake input
};

class CarsimLib
{
protected:
	// void* vsDLL; // for Linux
	HMODULE vsDLL; // for windows

public:
	// open dll and load function pointers
	void open_dll(std::string simfile_path);

	// free dlls
	void free_dll();

	// run simulation all W/O stop
	virtual int run_all();
	virtual int run_init_step();
	virtual int run_integrate();
	virtual int run_terminate();
	virtual bool is_continuing();

	std::string dll_path;
	std::string simfile;

	// simulation time in carsim
	double carsim_t;

	// number of integrate in one control frame , default = 0.01s <=> 20 
	// length of 1 step is set by simfile.sim parameter.
	int integrate_count = 20; // betagaki yametai

	// control input/output variable map
	static std::map<std::string, vs_real*> input_variable_map;
	static std::map<std::string, vs_real*> output_variable_map;

	// map pointers to link input variables
	/// input variables
	double* imp_steer_sw;
	double* imp_throttle_engine;
	double* imp_fbk_pdl;

	/// output variables
	double* xcg_tm;
	double* ycg_tm;
	double* yaw;
	double* vx;
	double* vy;
	double* avz;
	double* ax;
	double* ay;
	double* tstart;
	double* pbk_con;
	// double* mfuel;
	// double* av_eng;
	//double* gearauto;

	void set_def();
	static void external_set_def();
	virtual void register_callbacks();

    static CarsimLib* instance;
	CarsimLib(std::string fn);
	~CarsimLib();

	void SetState(InfoToCarsim* info2sim);
	InfoFromCarsim* GetState();

	void ReleaseCarsim();
};

