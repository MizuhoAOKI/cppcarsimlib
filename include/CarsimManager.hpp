#include <map>
#include <string>
#include <vector>
#include "CarsimCoreAPI.hpp"
#include "vs_deftypes.h" // import vs_real definition

// ENC = External Naming Convention
#if (defined(_WIN32) || defined(_WIN64))
// windows
#else
// linux
#define HMODULE void*
#endif

class CarsimManager
{
/*
Class to launch, run, update states, and close carsim solver.
*/
	private:
		// carsim api instance
		CarsimCoreAPI *api_;
		HMODULE vsDLL_;
		std::string simfile_path_;
		std::map<std::string, vs_real*> input_variable_map_;
		std::map<std::string, vs_real*> output_variable_map_;

		int ibarg_ = 0;
		char *sMsg_ = 0; // pointer to support local sPringMsg function
		vs_real carsim_time_ = 0.0f; // [sec]
		vs_real carsim_tstep_ = 0.0f; // [sec]

	public:

		// for debug
		double* debug_steer_input;

		// constructor
		CarsimManager(std::string fn);
		// destructor
		~CarsimManager();

		// launch carsim
		int LaunchCarsimSolver();
		// open carsim dll
		int OpenDLL();
		// close carsim
		void CloseCarsimSolver();
		// release carsim dll
		void ReleaseDLL();

		// define control inputs
		void DefineCarsimControlInput(std::map<std::string, vs_real> carsim_input);
		// define carsim state outputs
		void DefineCarsimStateOutput(std::vector<std::string> carsim_output_keys);

		// reset
		int Reset();
		// run all simulation steps at once
		void RunAll();
		// update carsim state for a certain time
		int RunStep(double delta_time_milliseconds);
		// set next control action to carsim
		void SetCarsimControlInput();
		// get carsim state
		std::map<std::string, vs_real> GetCarsimStateOutput();
		// is carsim running now?
		int IsRunning();

		// echo time and carsim inputs/states
		void EchoInfo();

		void sPrintMsg();

};