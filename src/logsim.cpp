// Simple program to run carsim with time-series control input written in csv.

#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h> // Windows-specific header
#endif

#include <signal.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "CarsimLib.hpp"
#include "svg_visualizer.hpp"

// Define several paths
#define  LOG_CSV_PATH  "../csv/chrono_carsim_input.csv"
#define  SIMFILE_PATH  "/workspace/CarSim_2020.1/Database_StaticRun/simfile.sim"
#define  SAVESHELL_PATH "../shscripts/get_carsim_output_files.sh" 
// One step length of simulation in carsim.
#define  ONE_STEP_LEN 0.01 // 0.00050000 * 20 [s]
// Column numbers in the csv logfile
#define  TIME 0
#define  THROTTLE 1
#define  BRAKE 2
#define  STEER 3


// Flag for normal termination with Ctrl+C
bool interrupt = false;
void throwBreak(int aStatus){signal(SIGINT, NULL); interrupt = true;}
inline void setSigBreak(){signal(SIGINT, throwBreak);}

int main(int argc, char** argv) {
    setSigBreak(); // Func. for for normal termination with Ctrl+C

    // Load csv log file
    int ignore_rownum = 1 ; // ignore title row.
    printf("Loading %s \n", LOG_CSV_PATH);
    printf("#  col_0 : time  #  col_1 : IMP_THROTTLE_ENGINE  #  col_2 : IMP_FBK_PDL  #  col_3 : IMP_STEER_SW  # \n");
    std::ifstream ifs_csv(LOG_CSV_PATH);
    std::vector<std::vector<std::string>> input_ary;
    int csvrownum = 0;

    for (std::string value; std::getline(ifs_csv, value);) {
        input_ary.push_back(std::vector<std::string>());
        for (std::stringstream ss(value); std::getline(ss, value, ',');) {
        input_ary[input_ary.size()-1].push_back(value);
        csvrownum++;
        }
    }

    // Write csv contents :
    for (auto& v : input_ary) {
        for (auto& e : v) std::cout << " " << e ;
        std::cout << "\n";
    }

    // Launch carsim
    CarsimLib* carsim;
    carsim = new CarsimLib(SIMFILE_PATH);
    carsim->run_init_step();

    // Class to get info from carsim
    InfoFromCarsim* output = new InfoFromCarsim();
    // Class to set control input to carsim
    InfoToCarsim* operate = new InfoToCarsim();

    // Initialize log counter and simulation time.
    int log_counter = ignore_rownum;
    int log_counter_max = csvrownum - ignore_rownum - 1 - 480;
    double latest_logtime = 0.0;
    double carsim_time = 0.0;
    bool endflag = false;

    while (carsim->is_continuing() && !interrupt && !endflag) {
        // Set log_counter to the latest value
        while(true){
            latest_logtime = stod(input_ary[log_counter][TIME]);
            if(log_counter > log_counter_max){
                endflag = true;
                break;
            }else if(carsim_time <= latest_logtime){
                break;
            }else{
                log_counter++;
            }
        }

        if(endflag){
            break;
        }

        operate->IMP_THROTTLE_ENGINE = stod(input_ary[log_counter][THROTTLE]); 
        operate->IMP_FBK_PDL = stod(input_ary[log_counter][BRAKE]); 
        operate->IMP_STEER_SW = stod(input_ary[log_counter][STEER]);
        carsim->SetState(operate);
        carsim->run_integrate();
        carsim_time = carsim->carsim_t;
        output = carsim->GetState();
        // Echo log
        printf("Vehicle States : \n");
        printf("(X, Y, Yaw) = (%f, %f, %f) \n", output->XCG_TM, output->YCG_TM, output->YAW);
        printf("(Vx, Vy, Yaw_rate, Ax, Ay) = (%f, %f, %f, %f, %f) \n", output->VX, output->VY, output->AVZ);
        printf("Control Inputs : \n");
        printf("(throttle, brake, steer) = (%f, %f, %f)", operate->IMP_THROTTLE_ENGINE, operate->IMP_FBK_PDL, operate->IMP_STEER_SW);
    }

    printf("\nClosing Carsim...\n");
    carsim->ReleaseCarsim();

    // Get output files
    char command[256];
    char *shellpath_ptr = SAVESHELL_PATH;
    char *simfiliepath_ptr = SIMFILE_PATH;
    sprintf(command, "%s %s", shellpath_ptr, simfiliepath_ptr);
    system(command);

    // Get dirname including latest result files.
    std::string result_dirname;
    std::ifstream shell_log("../Results/latest_result_path.txt");
    getline(shell_log, result_dirname);
    std::cout << "result_dirname=" << result_dirname << std::endl;

    // Output svg animation for visualization
    SVGOutput::OutputSVGfromCSV(result_dirname + "/LastRun.csv", "../Results/trajectory.svg", 10);
    // OutputSVGfromCSV( input_path, output_path, animation_speed (n-times) )

    return 0;
}