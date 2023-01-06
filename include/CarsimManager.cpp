#include <iostream>
#include <string>
#include "CarsimManager.hpp"

// constructor
CarsimManager::CarsimManager(std::string simfile_path)
{
    std::cout << "[INFO] Constructor of CarsimManager" << std::endl;
    std::cout << "       simfile_path = " << simfile_path << std::endl;

    // instantiate CarsimCoreAPI
    api_ = new CarsimCoreAPI(simfile_path);

    // save simfile path as a class member
    simfile_path_ = simfile_path;

    // launch carsim solver
    LaunchCarsimSolver();
}

// destructor
CarsimManager::~CarsimManager()
{
    std::cout << "[INFO] Destructor of CarsimManager" << std::endl;

    CloseCarsimSolver();
}

// launch carsim
int CarsimManager::LaunchCarsimSolver()
{
    std::cout << "[INFO] LaunchCarsimSolver" << std::endl;

    // open carsim dll
    if(OpenDLL()){
        std::cout << "[ERROR] OpenDLL failed." << std::endl;
        return 1;
    }else{
        std::cout << "[INFO] OpenDLL successful." << std::endl;
    }

    // set output_message pointer. refer carsim msg by "sPringMsg()"
    sMsg_ = api_->vs_get_output_message();

    return 0;
}

// open carsim dll
int CarsimManager::OpenDLL()
{
    std::cout << "[INFO] OpenDLL" << std::endl;

    // buffer of dll path
	char _pathDLL[FILENAME_MAX];

	// get simfile from argument list and load DLL
	if (api_->vs_get_dll_path(simfile_path_.c_str(), _pathDLL)) return 1;

	// CarsimBasic
	std::cout << "       pathDLL = " << _pathDLL << std::endl;
	vsDLL_ = api_->vs_load_library(_pathDLL);
    if (vsDLL_ == NULL)
    {
    printf("[ERROR] vs_load_library failed. \n");
    return 1;
    }else{
        printf("       vs_load_library successful. \n");
    }

    // get API functions
	if (api_->vs_get_api(vsDLL_, _pathDLL)){
		printf("[ERROR] vs_get_api failed. \n");
		return 1;
	}else{
		printf("       vs_get_api successful. \n");
        return 0;
	}
}

// close carsim
void CarsimManager::CloseCarsimSolver()
{
    std::cout << "[INFO] CloseCarsimSolver" << std::endl;
    api_->vs_terminate (carsim_time_, NULL);
    ReleaseDLL();
}

// release carsim dll
void CarsimManager::ReleaseDLL()
{
    std::cout << "[INFO] ReleaseDLL" << std::endl;
    api_->vs_free_library(vsDLL_);
}

// define control inputs
void CarsimManager::DefineCarsimControlInput()
{
    std::cout << "[INFO] DefineCarsimControlInput" << std::endl;
}

// define carsim state outputs
void CarsimManager::DefineCarsimStateOutput(){
    std::cout << "[INFO] DefineCarsimStateOutput" << std::endl;
}

// set next control action to carsim
void CarsimManager::SetCarsimControlInput()
{
    std::cout << "[INFO] SetCarsimControlInput" << std::endl;
}

// get carsim state
void CarsimManager::GetCarsimStateOutput()
{
    std::cout << "[INFO] GetCarsimStateOutput" << std::endl;
}

// Reset carsim time
int CarsimManager::Reset()
{
    std::cout << "[INFO] Reset" << std::endl;

    // initialize carsim solver
    carsim_time_ = api_->vs_setdef_and_read(simfile_path_.c_str(), NULL, NULL);
    if (api_->vs_error_occurred()) {
        std::cout << "[ERROR] Error occurred reading simfile " << simfile_path_ << std::endl;
        return 1;
    }

    api_->vs_initialize(carsim_time_, NULL, NULL);
    sPrintMsg();

    return 0;
}

// run all simulation steps at once
void CarsimManager::RunAll()
{
    std::cout << "[INFO] RunAll" << std::endl;

    // run. Each loop advances time one step
    while (!api_->vs_stop_run()) {
        api_->vs_integrate (&carsim_time_, NULL);
        api_->vs_bar_graph_update (&ibarg_); // update bar graph?
    }

    std::cout << std::endl;
}

// update carsim state for a certain time
int CarsimManager::RunStep(double delta_time_milliseconds)
{
    std::cout << "[INFO] RunStep" << std::endl;
    // return execution status
}

// is carsim running now?
int CarsimManager::IsRunning()
{
    std::cout << "[INFO] IsRunning : " << !api_->vs_stop_run() << std::endl;
    return !api_->vs_stop_run();
}

// echo time and carsim inputs/states
void CarsimManager::EchoInfo(){
    std::cout << "[INFO] ### t = ..." << std::endl;
}

// echo carsim message
void CarsimManager::sPrintMsg () {
    std::cout << "[INFO] start sPrintMsg" << std::endl;

    // exit if no valid message is given.
    if (!*sMsg_){return;} 

    // print msg
    std::cout << "[INFO] [CarsimMsg]" << std::endl << std::endl << sMsg_ << std::endl;
    // printf ("%s",sMsg_);
    *sMsg_ = 0;
    std::cout << "[INFO] end sPrintMsg" << std::endl;
}