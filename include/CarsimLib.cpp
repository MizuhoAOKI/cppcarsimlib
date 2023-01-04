#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
#include <cmath>
#include "CarsimLib.hpp"

CarsimLib* CarsimLib::instance = NULL;
std::map<std::string, vs_real*> CarsimLib::input_variable_map;
std::map<std::string, vs_real*> CarsimLib::output_variable_map;

CarsimLib::CarsimLib(std::string fn) {
	// Open Library
	std::cout << "Carsim Basic Constructed. \n" << std::endl;
	std::cout << "Loading " << fn << std::endl;
	// set instance
	CarsimLib::instance = this;
	open_dll(fn);

	// for debug
	printf("simfile : %s \n", fn.c_str());
	carsim_t = vs_setdef_and_read(fn.c_str(), NULL, NULL);
	if (vs_error_occurred()) {
		printf("\n\nError occurred reading simfile \"%s\"", fn.c_str());
	}

	vs_initialize (carsim_t, NULL, NULL);

	// Run. Each loop advances time one step
	while (!vs_stop_run()) {
		vs_integrate (&carsim_t, NULL);
		// vs_bar_graph_update (&ibarg); // update bar graph?
	}

	// Terminate
	vs_terminate (carsim_t, NULL);

}

void CarsimLib::open_dll(std::string simfile_path)
{
	std::cout << "simfile = " << simfile_path << std::endl;

	char pathDLL[FILENAME_MAX];

	// get simfile from argument list and load DLL
	if (vs_get_dll_path(simfile_path.c_str(), pathDLL)) return;

	// CarsimBasic
	dll_path = pathDLL;
	std::cout << "dll_path = " << pathDLL << std::endl;

	vsDLL = vs_load_library(pathDLL);

    // get API functions
	if (vs_get_api(vsDLL, pathDLL)){
		printf("vs_get_api unsuccessful\n");
		return; // fundamental functions
	}else{
		printf("vs_get_api successful\n");
	}
	// if (vs_get_api_basic(vsDLL, pathDLL)) return; // basic functions
	// if (vs_get_api_install_external(vsDLL, pathDLL)) return; // install functions
	// if (vs_get_api_extend(vsDLL, pathDLL)) return; // model extension functions
	// if (vs_get_api_road(vsDLL, pathDLL)) return; // road-related functions
}

CarsimLib::~CarsimLib() {
	ReleaseCarsim();
}

void CarsimLib::ReleaseCarsim() {
	std::cout << "Carsim Basic Destructed. \n" << std::endl;
	run_terminate();
	free_dll();
}

void CarsimLib::free_dll(void)
{
	// Release Library
	// Wait for a keypress if the parameter opt_pause was specified.
	if (vs_opt_pause()) {
		printf(
			"\n\nThe run ended normally. OPT_PAUSE was set to keep this display visible."
			"\nPress the Return key to exit this solver program. ");
		fgetc(stdin);
	}
	vs_free_library(vsDLL);
}

// ########## Change here if you'd like to change info to carsim #######
void CarsimLib::SetState(InfoToCarsim* info2sim) {
	*imp_throttle_engine = info2sim->IMP_THROTTLE_ENGINE;
	*imp_fbk_pdl = info2sim->IMP_FBK_PDL;
	*imp_steer_sw = info2sim->IMP_STEER_SW;
}

// ########## Change here if you'd like to change info from carsim #######
InfoFromCarsim* CarsimLib::GetState() {
	InfoFromCarsim* infofromsim = new InfoFromCarsim();

	infofromsim->XCG_TM = *xcg_tm;
	infofromsim->YCG_TM = *ycg_tm;
	infofromsim->YAW = *yaw;

	infofromsim->VX = *vx;
	infofromsim->VY = *vy;
	infofromsim->AVZ = *avz;

	infofromsim->AX = *ax;
	infofromsim->AY = *ay;

	infofromsim->TSTART = *tstart;
	infofromsim->PBK_CON = *pbk_con;
	// infofromsim->MFUEL = *mfuel;
	// infofromsim->AV_ENG = *av_eng;

	return infofromsim;
}

int CarsimLib::run_all() {
	// return run_all();
	printf("run_all \n");
	run_init_step();
	while (is_continuing())
	{
		run_integrate();
	}
	run_terminate();
	return 0;
}

int CarsimLib::run_init_step() {
	// carsim initialization process
	printf("run_init_step \n");
	char* printMsg = vs_get_output_message(); // pointer to text from DLL
	// initialize VS model

	// Read inputs and initialize
	carsim_t = vs_setdef_and_read(simfile.c_str(), NULL, NULL);
	if (vs_error_occurred()) {
		printf("\n\nError occurred reading simfile \"%s\"", simfile.c_str());
		return 1;
	}
	vs_initialize (carsim_t, NULL, NULL);

	// set input parameters
	imp_steer_sw = vs_get_var_ptr(const_cast<char*>("IMP_STEER_SW"));
	imp_throttle_engine = vs_get_var_ptr(const_cast<char*>("IMP_THROTTLE_ENGINE"));
	imp_fbk_pdl = vs_get_var_ptr(const_cast<char*>("IMP_FBK_PDL"));

	// set output parameters
	xcg_tm = vs_get_var_ptr(const_cast<char*>("XO"));
	ycg_tm = vs_get_var_ptr(const_cast<char*>("YO"));
	yaw = vs_get_var_ptr(const_cast<char*>("YAW"));
	vx = vs_get_var_ptr(const_cast<char*>("VX"));
	vy = vs_get_var_ptr(const_cast<char*>("VY"));
	avz = vs_get_var_ptr(const_cast<char*>("AVZ"));
	ax = vs_get_var_ptr(const_cast<char*>("AX"));
	ay = vs_get_var_ptr(const_cast<char*>("AY"));
	tstart = vs_get_var_ptr(const_cast<char*>("TSTART"));
	pbk_con = vs_get_var_ptr(const_cast<char*>("PBK_CON"));
	// mfuel = vs_get_var_ptr(const_cast<char*>("MFUEL"));
	// av_eng = vs_get_var_ptr(const_cast<char*>("AV_ENG"));

	if (vs_error_occurred()) return 1;
	printf("run_init_step finished\n");
	return 0;
}

int CarsimLib::run_integrate() {
	std::cout << "\n\n###############################    carsim_t =  " << carsim_t << "    ###############################\n" <<std::endl;
	for (int i = 0; i < integrate_count; ++i)
	{
		vs_integrate(&carsim_t, NULL);
		if (vs_error_occurred()) {
			return 1;
		}
	}
	// vs_bar_graph_update (&ibarg); // activate and update bar graph
	return 0;
}

int CarsimLib::run_terminate() {
	// Terminate
	vs_terminate(carsim_t, NULL);
	//vss_print_message (printMsg);
	vs_free_all();
	return 0;
}

bool CarsimLib::is_continuing() {
	return !vs_stop_run();
}

void CarsimLib::set_def()
{
	//input variable mapping
	for (auto itr = input_variable_map.begin();
		itr != input_variable_map.end();
		itr++)
	{
		itr->second = vs_get_var_ptr(const_cast<char*>(itr->first.c_str()));
	}
	//output variable mapping
	for (auto itr = output_variable_map.begin();
		itr != output_variable_map.end();
		itr++)
	{
		itr->second = vs_get_var_ptr(const_cast<char*>(itr->first.c_str()));
	}
}

void CarsimLib::external_set_def() {
	CarsimLib* trg = CarsimLib::instance;
	// call member function
	trg->set_def();
}

void CarsimLib::register_callbacks() {
	vs_install_setdef_function(CarsimLib::external_set_def);
	printf("register_callbacks finished \n");
}