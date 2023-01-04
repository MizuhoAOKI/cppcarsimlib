#include <iostream>
#include <string>
#include "CarsimManager.hpp"

// constructor
CarsimManager::CarsimManager(std::string simfile_path)
{
    std::cout << "[INFO] Constructor of CarsimManager" << std::endl;
    std::cout << "[INFO] simfile_path = " << simfile_path << std::endl;

    // instantiate CarsimCoreAPI
    // this->api = CarsimCoreAPI()

    LaunchCarsimSolver();
}

// destructor
CarsimManager::~CarsimManager()
{
    std::cout << "[INFO] Destructor of CarsimManager" << std::endl;

    CloseCarsimSolver();
}

// launch carsim
void CarsimManager::LaunchCarsimSolver()
{
    std::cout << "[INFO] LaunchCarsimSolver" << std::endl;

    OpenDLL();

    DefineCarsimControlInput();
    
    DefineCarsimStateOutput();

}

// open carsim dll
int CarsimManager::OpenDLL()
{
    std::cout << "[INFO] OpenDLL" << std::endl;
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