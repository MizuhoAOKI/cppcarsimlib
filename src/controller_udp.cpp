#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fstream>
#include <sstream>
#include <vector>

#include <iostream>
#include <thread>
#include <chrono>
#include "udp_lib_linux.hpp"
#include "json.hpp"
using namespace nlohmann; // for use json.hpp
// Define communication settings
#define INDENT_SPACE 2
// Define several paths
#define  LOG_CSV_PATH  "../csv/chrono_carsim_input.csv"
// Column numbers in the csv logfile
#define  TIME 0
#define  THROTTLE 1
#define  BRAKE 2
#define  STEER 3

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

	// Set up sockets for udp communication
	udp_lib::Sender<std::string> sender("127.0.0.1", 60000); // Port: 60000, communicate control_input
	udp_lib::Receiver<std::string> receiver(60001);			 // Port: 60001, communicate vehicle_state

	std::string input;  // control_input msg
	std::string output; // vehicle_state msg

	json output_json; // json object of vehicle_state

	// Initialize control_input
	double steer_angle = 0.0;
	double throttle    = 0.0;
	double brake 	   = 0.0;

    // Initialize log counter and simulation time.
    int log_counter = ignore_rownum;
    int log_counter_max = csvrownum - ignore_rownum - 1 - 480;
    double latest_logtime = 0.0;
    bool endflag = false;
	double time = 0.0;  // controller's time

	while (!interrupt && !endflag)
	{
		////////// Receive vehicle_state //////////
		if (receiver.udp_receive(&output))
		{
			std::cout << "\nReceived vehicle_state" << std::endl;
			output_json = json::parse(output);
			std::cout << output << std::endl;
			// access parsed json object :
			std::cout << "simulator_time = " << output_json["time"] 							  << std::endl;
			std::cout << "x = "              << output_json["vehicle_state"]["pos"]["x"]          << std::endl;
			std::cout << "y = "              << output_json["vehicle_state"]["pos"]["y"]          << std::endl;
			std::cout << "z = "              << output_json["vehicle_state"]["pos"]["z"]          << std::endl;
			std::cout << "pitch = "          << output_json["vehicle_state"]["pos"]["pitch"]      << std::endl;
			std::cout << "roll = "           << output_json["vehicle_state"]["pos"]["roll"]       << std::endl;
			std::cout << "yaw = "            << output_json["vehicle_state"]["pos"]["yaw"]        << std::endl;
			std::cout << "vx = "             << output_json["vehicle_state"]["vel"]["vx"]         << std::endl;
			std::cout << "vy = "             << output_json["vehicle_state"]["vel"]["vy"]         << std::endl;
			std::cout << "vz = "             << output_json["vehicle_state"]["vel"]["vz"]         << std::endl;
			std::cout << "pitch_rate = "     << output_json["vehicle_state"]["vel"]["pitch_rate"] << std::endl;
			std::cout << "roll_rate = "      << output_json["vehicle_state"]["vel"]["roll_rate"]  << std::endl;
			std::cout << "yaw_rate = "       << output_json["vehicle_state"]["vel"]["yaw_rate"]   << std::endl;
			std::cout << "ax = "             << output_json["vehicle_state"]["acc"]["ax"]         << std::endl;
			std::cout << "ay = "             << output_json["vehicle_state"]["acc"]["ay"]         << std::endl;
			std::cout << "az = "             << output_json["vehicle_state"]["acc"]["az"]         << std::endl;
		}
		else
		{
			std::cout << "Not receive new msg yet." << std::endl;
		}

		////////// Calculate control input //////////
		// Set log_counter to the latest value
		if(output_json.is_null()){
			time = 0.0;
		}else{
			// Adjust time to simulator.
			time = output_json["time"];
		}

        while(!interrupt){
            latest_logtime = stod(input_ary[log_counter][TIME]);
            if(log_counter > log_counter_max){
                endflag = true;
                break;
            }else if(time <= latest_logtime){
                break;
            }else{
                log_counter++;
            }
        }

        if(endflag){
            break;
        }
		
		// Determine control input
		steer_angle = stod(input_ary[log_counter][STEER]); 
		throttle    = stod(input_ary[log_counter][THROTTLE]); 
		brake       = stod(input_ary[log_counter][BRAKE]); 


		////////// Send control_input //////////
		json input_json = 
		{
			{"time", time},
			{"control_input", {
				{"lateral",{
					{"steer_angle", steer_angle}
				  }
				},
				{"longitudinal",{
					{"throttle", throttle},
					{"brake", brake}
				  }
				}
			  }
			}
		};

		input = input_json.dump(INDENT_SPACE);
		sender.udp_send(input);

		std::cout << "\nSent control_input" << std::endl;
		std::cout << input << std::endl;
	}

	return 0;
}