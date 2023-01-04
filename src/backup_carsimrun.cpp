// sample program to run a carsim solver from a cpp program.
#include <signal.h>
#include "CarsimLib.hpp"

// set simfile.sim location on your environment.
#define  SIMFILE_PATH  "C:\\Users\\Public\\Documents\\CarSim2022.1_Data\\simfile.sim"

// flag for normal termination with Ctrl+C
bool interrupt = false;
void throwBreak(int aStatus){signal(SIGINT, NULL); interrupt = true;}
inline void setSigBreak(){signal(SIGINT, throwBreak);}

// main process
int main(int argc, char** argv) {
    setSigBreak(); // for normal termination with Ctrl+C

    // launch carsim
    CarsimLib* carsim;
    carsim = new CarsimLib(SIMFILE_PATH);

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