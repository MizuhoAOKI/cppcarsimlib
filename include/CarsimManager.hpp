#include <map>
#include <string>
// #include "CarsimCoreAPI.hpp"
#include "vs_deftypes.h" // import vs_real definition

class CarsimManager
{
/*
Class to launch, run, update states, and close carsim solver.
*/
	private:
		// carsim api instance
		// CarsimCoreAPI *api_;
		std::map<std::string, vs_real*> input_variable_map_;
		std::map<std::string, vs_real*> output_variable_map_;

	public:
		// constructor
		CarsimManager(std::string fn);
		// destructor
		~CarsimManager();

		// launch carsim
		void LaunchCarsimSolver();
		// open carsim dll
		int OpenDLL();
		// close carsim
		void CloseCarsimSolver();
		// release carsim dll
		void ReleaseDLL();

		// define control inputs
		void DefineCarsimControlInput();
		// define carsim state outputs
		void DefineCarsimStateOutput();

		// reset
		int Reset();
		// run all simulation steps at once
		int RunAll();
		// update carsim state for a certain time
		int RunStep(double delta_time_milliseconds);
		// set next control action to carsim
		void SetCarsimControlInput();
		// get carsim state
		void GetCarsimStateOutput();
		// is carsim running now?
		int IsRunning();

		// echo time and carsim inputs/states
		void EchoInfo();
};