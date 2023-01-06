#include <iostream>
#include <string>
#include "CarsimManager.hpp"

// constructor
CarsimManager::CarsimManager(std::string simfile_path)
{
    std::cout << "[INFO] Constructor of CarsimManager" << std::endl;
    std::cout << "[INFO] simfile_path = " << simfile_path << std::endl;

    // instantiate CarsimCoreAPI
    api_ = new CarsimCoreAPI(simfile_path);
    simfile_path_ = simfile_path;

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
        std::cout << "[INFO] OpenDLL unsuccessful." << std::endl;
        return 1;
    }else{
        std::cout << "[INFO] OpenDLL successful." << std::endl;
    }

    std::cout << "[INFO] start RunAll for debug" << std::endl;
    // initialize carsim solver
    vs_real t = api_->vs_setdef_and_read(simfile_path_.c_str(), NULL, NULL);
    api_->vs_initialize(t, NULL, NULL);

    int _ibarg = 0;

    // Run. Each loop advances time one step
    while (!api_->vs_stop_run()) {
        api_->vs_integrate (&t, NULL);
        api_->vs_bar_graph_update (&_ibarg); // update bar graph?
    }

    // Terminate
    api_->vs_terminate (t, NULL);

    std::cout << "[INFO] end RunAll for debug" << std::endl;

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

    // get API functions
	if (api_->vs_get_api(vsDLL_, _pathDLL)){
		printf("       vs_get_api unsuccessful\n");
		return 1;
	}else{
		printf("       vs_get_api successful\n");
        return 0;
	}
}

// close carsim
void CarsimManager::CloseCarsimSolver()
{
    std::cout << "[INFO] CloseCarsimSolver" << std::endl;
    ReleaseDLL();
}

// release carsim dll
void CarsimManager::ReleaseDLL()
{
    std::cout << "[INFO] ReleaseDLL" << std::endl;
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
    // return execution status
}

// run all simulation steps at once
int CarsimManager::RunAll()
{
    std::cout << "[INFO] RunAll" << std::endl;
    // return execution status
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
    std::cout << "[INFO] IsRunning" << std::endl;
    // return execution status
    return 1; // for debug
}

// echo time and carsim inputs/states
void CarsimManager::EchoInfo(){
    std::cout << "[INFO] ### t = ..." << std::endl;
}
