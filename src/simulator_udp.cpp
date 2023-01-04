#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "CarsimLib.hpp"

#include "svg_visualizer.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include "udp_lib_linux.hpp"
#include "json.hpp"
using namespace nlohmann; // for use json.hpp
#define INDENT_SPACE 2
// Define several paths
#define  SIMFILE_PATH  "/workspace/CarSim_2020.1/Database_StaticRun/simfile.sim"
#define  SAVESHELL_PATH "../shscripts/get_carsim_output_files.sh" 
// One step length of simulation in carsim.
#define  SLEEP_ENABLE 0    // Flag to switch sleep activation
#define  ONE_STEP_LEN 0.01 // 0.00050000 * 20 [s]

// How to synchronize the controller time with the simulator time?
// 1. Set IS_NONBLOCKING 1 at include/udp_lib_linux.hpp
// 2. First start controller_udp, then run simulator_udp

// ### It's safe to run blocking communication in ordinary experiments.

// Flag for normal termination with Ctrl+C
bool interrupt = false;
void throwBreak(int aStatus){signal(SIGINT, NULL); interrupt = true;}
inline void setSigBreak(){signal(SIGINT, throwBreak);}


// Main process
int main()
{
	setSigBreak(); // Func. for for normal termination with Ctrl+C

    // Launch carsim
    CarsimLib* carsim;
    carsim = new CarsimLib(SIMFILE_PATH);
    carsim->run_init_step();

    // Class to get info from carsim
    InfoFromCarsim* carsimstate = new InfoFromCarsim();
    // Class to set control input to carsim
    InfoToCarsim* operate = new InfoToCarsim();
	// Set initial operation 
	operate->IMP_THROTTLE_ENGINE = 0.0;
	operate->IMP_FBK_PDL 		 = 0.0;
	operate->IMP_STEER_SW 		 = 0.0;	

	// Set up sockets for udp communication
	udp_lib::Receiver<std::string> receiver(60000); 		 // Port: 60000, communicate control_input
	udp_lib::Sender<std::string> sender("127.0.0.1", 60001); // Port: 60001, communicate vehicle_state

	std::string input;  // control_input msg
	std::string output; // vehicle_state msg

	double time = 0; // simulator's time

	json input_json; // json object of control_input
	const int sleep_time = (int)((double)ONE_STEP_LEN * 1000); // [msec]

	while (carsim->is_continuing() && !interrupt)
	{
		////////// Send vehicle_state //////////
		json output_json = 
		{
			{"time", time},
			{"vehicle_state", {
				{"pos",{
					{"x", carsimstate->XCG_TM},
					{"y", carsimstate->YCG_TM},
					{"z", 0.0},
					{"pitch", 0.0},
					{"roll", 0.0},
					{"yaw", carsimstate->YAW}
				  }
				},
				{"vel",{
					{"vx", carsimstate->VX},
					{"vy", carsimstate->VY},
					{"vz", 0.0},
					{"pitch_rate", 0.0},
					{"roll_rate", 0.0},
					{"yaw_rate", carsimstate->AVZ}
				  }
				},
				{"acc",{
					{"ax", carsimstate->AX},
					{"ay", carsimstate->AY},
					{"az", 0.0},
				  }
				}
			  }
			}
		};

		output = output_json.dump(INDENT_SPACE);
		sender.udp_send(output);

		std::cout << "\nSent vehicle_state" << std::endl;
		std::cout << output << std::endl;

		////////// Calculate vehicle state //////////
		if(!input_json.is_null()){
			operate->IMP_THROTTLE_ENGINE = input_json["control_input"]["longitudinal"]["throttle"];
			operate->IMP_FBK_PDL = input_json["control_input"]["longitudinal"]["brake"];
			operate->IMP_STEER_SW = input_json["control_input"]["lateral"]["steer_angle"];	
		}
        carsim->SetState(operate);
        carsim->run_integrate();
        // time = carsim->carsim_t;
        carsimstate = carsim->GetState();


		////////// Receive control_input //////////
		if (receiver.udp_receive(&input))
		{
			std::cout << "\nReceived control_input" << std::endl;
			input_json = json::parse(input);
			std::cout << input << std::endl;
			// access parsed json object :
			std::cout << "controller_time = " << input_json["time"] << std::endl;
			std::cout << "steer_angle = " << input_json["control_input"]["lateral"]["steer_angle"] << std::endl;
			std::cout << "throttle = " << input_json["control_input"]["longitudinal"]["throttle"] << std::endl;		
			std::cout << "brake = " << input_json["control_input"]["longitudinal"]["brake"] << std::endl;		
		}
		else
		{
			std::cout << "Not receive new msg yet." << std::endl;
		}

		////////// sleep for a while //////////
		if(SLEEP_ENABLE){
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
		}
		time += (double)sleep_time/1000; // increment controller's time
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