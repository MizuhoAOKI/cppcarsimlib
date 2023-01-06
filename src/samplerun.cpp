// sample program to run a carsim solver from a cpp program.
#include <signal.h>
#include <iostream>
#include "CarsimManager.hpp"

// [CHANGE HERE] set simfile.sim location on your environment.
#define  SIMFILE_PATH  "C:\\Users\\Public\\Documents\\CarSim2022.1_Data\\simfile.sim"

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
    std::map<std::string, vs_real> carsim_input;
    carsim_input["IMP_STEER_SW"] = 0.0; // [rad]
    carsim_input["IMP_FBK_PDL"] = 0.0; // [-]
    carsim_input["IMP_THROTTLE_ENGINE"] = 0.0; // [-]

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
    double total_sim_time = 1.0; // [sec]
    double sim_step_delta_sec = 0.1; // [sec]

    // activate when you want to test whole simulation at once
    // cm->RunAll();

    // simulation loop
    while( (cm->IsRunning()) && (!interrupt) && (current_sim_time < total_sim_time) )
    {
        // annouce 1. simulation time, 2. latest carsim input, 3. latest carsim output.
        cm->EchoInfo();
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

        // set next control inputs
        cm->SetCarsimControlInput();
    }

    // close carsim solver normally
    cm->CloseCarsimSolver();

    // return normal termination flag
    return 0;

    /*
    // initialize carsim solver
    carsim->run_init_step();

    // instance get observed info from carsim
    InfoFromCarsim* output = new InfoFromCarsim();
    // instance to set control input for carsim
    InfoToCarsim* operate = new InfoToCarsim();

    // initialize simulation time.
    double carsim_time = 0.0;

    // simulation loop : press Ctrl+C to interrupt.
    while (carsim->is_continuing() && !interrupt) {
        operate->IMP_THROTTLE_ENGINE = 0.2; 
        operate->IMP_FBK_PDL = 0.0; 
        operate->IMP_STEER_SW = 0.01 / 0.060039;
        carsim->SetState(operate);
        carsim->run_integrate(); // 更新時間を入れる.
        carsim_time = carsim->carsim_t;
        output = carsim->GetState();
        // printf("t = %f \n", carsim_time);
        printf("Vehicle States : \n");
        printf("(X, Y, Yaw) = (%f, %f, %f) \n", output->XCG_TM, output->YCG_TM, output->YAW);
        printf("(Vx, Vy, Yaw_rate, Ax, Ay) = (%f, %f, %f, %f, %f) \n", output->VX, output->VY, output->AVZ);
        printf("Control Inputs : \n");
        printf("(throttle, brake, steer) = (%f, %f, %f)", operate->IMP_THROTTLE_ENGINE, operate->IMP_FBK_PDL, operate->IMP_STEER_SW);
    }

    // normal termination process
    printf("\nClosing Carsim...\n");
    carsim->ReleaseCarsim();
    return 0;
    */
}