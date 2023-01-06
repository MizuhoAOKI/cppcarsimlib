// sample program to run a carsim solver from a cpp program.
#include <signal.h>
#include <iostream>
#include <filesystem>
#include <cmath>
#include "CarsimManager.hpp"

// [CHANGE HERE IF NECESSARY] set simfile.sim location on your environment.
#define  SIMFILE_PATH    "C:\\Users\\Public\\Documents\\CarSim2022.1_Data\\simfile.sim"
#define  CARSIM_DB_PATH  "C:\\Users\\Public\\Documents\\CarSim2022.1_Data"

// prepare flag for normal termination when Ctrl+C are pressed.
bool interrupt = false;
void throwBreak(int aStatus){signal(SIGINT, NULL); interrupt = true;}
inline void setSigBreak(){signal(SIGINT, throwBreak);}

// main process
int main(int argc, char** argv) {

    // set an event-listener for normal termination with Ctrl+C
    setSigBreak();

    // get carsim manager with solver initialization process
    CarsimManager* cm = new CarsimManager(std::string(SIMFILE_PATH));

    // reset carsim-time of the target solver instance
    cm->Reset();

    // define and initialize input/output variables
    //// inputs
    std::map<std::string, vs_real> carsim_input{
        {"IMP_STEER_SW",0.0}, // [rad]
        {"IMP_FBK_PDL",0.0}, // [-]
        {"IMP_THROTTLE_ENGINE",0.0} // [-]
    };

    //// outputs
    std::map<std::string, vs_real> carsim_output;
    std::vector<std::string> carsim_output_keys = {
        "XCG_TM", // [m]
        "YCG_TM", // [m]
        "YAW", // [rad]
        "VX",  // [m/s]
        "VY",  // [m/s]
        "VZ",  // [m/s]
        "AVZ", // [rad/s]
        "AX",  // [m/s^2]
        "AY"   // [m/s^2]
    };

    cm->DefineCarsimControlInput(carsim_input);
    cm->DefineCarsimStateOutput(carsim_output_keys);

    // prepare simulation params
    double current_sim_time = 0.0; // [sec]
    double total_sim_time = 10.0; // [sec]
    double sim_step_delta_sec = 0.01; // [sec]

    // simulation loop
    while( (cm->IsRunning()) && (!interrupt) && (current_sim_time < total_sim_time) )
    {
        // activate if necessary. variables to show are 1. simulation time, 2. latest carsim input, 3. latest carsim output.
        // cm->EchoInfo();
        /* Format : 
        [INFO] ### t = 0.01 [sec] ###
               Control Inputs : 
                 IMP_STEER_SW = 0.0
                 IMP_FBK_PDL = 0.0
                 IMP_THROTTLE_ENGINE = 0.0
               Observations : 
                 XCG_TM = 0.0
                 YCG_TM = 0.0
                 YAW = 0.0
                 ...
        */

        // run carsim solver for a while, and update simulation time.
        cm->RunStep(sim_step_delta_sec * 1000);
        current_sim_time += sim_step_delta_sec;

        // get updated observation outputs
        carsim_output = cm->GetCarsimStateOutput();
        std::cout << "### Carsim states at t = " << current_sim_time << " [sec] ###" << std::endl
                  << "(X, Y, Yaw) = ( "
                  << carsim_output["XCG_TM"] << ", "
                  << carsim_output["YCG_TM"] << ", "
                  << carsim_output["YAW"] << ") " << std::endl;

        // set next control inputs
        carsim_input["IMP_STEER_SW"] = 1.0 * std::sin(current_sim_time); // [rad]

        if(current_sim_time < total_sim_time/2){
            // first half : press the throttle pedal.
            carsim_input["IMP_THROTTLE_ENGINE"] = 10; // [-]
            carsim_input["IMP_FBK_PDL"] = 0.0; //[-]
        }else{
            // second half : press the brake pedal.
            carsim_input["IMP_THROTTLE_ENGINE"] = 0; // [-]
            carsim_input["IMP_FBK_PDL"] = 500; //[-]
        }
        cm->SetCarsimControlInput(carsim_input);
    }

    // close carsim solver normally
    cm->CloseCarsimSolver();

    // [Optional] copy latest result to carsim db to visualize results with "Video + Plot" button on the carsim GUI.
    if(CARSIM_DB_PATH)
    {
        std::cout << "[INFO] Copy results to carsim database location" << std::endl;
        std::filesystem::recursive_directory_iterator e = std::filesystem::recursive_directory_iterator(".\\Results");
        for (auto f : e) {
            if(std::filesystem::is_directory(std::filesystem::status(f))){continue;}
            std::filesystem::path _carsim_db = CARSIM_DB_PATH;
            auto _source_path = f.path();
            auto _target_path = _carsim_db.concat(f.path().string().replace(0,1, "")); // remove "."
            std::cout << "       Copy " << f.path() << " to " << _target_path << std::endl;
            std::filesystem::remove(_target_path);
            std::filesystem::copy(std::filesystem::absolute(f), _target_path, std::filesystem::copy_options::overwrite_existing);
        }
    }

    // return normal termination flag
    std::cout << "[INFO] Normal termination. " << std::endl;
    return 0;
}
