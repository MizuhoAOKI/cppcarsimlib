/* Wrapper program that can be launched anywhere in Windows, and will then load a
   VehicleSim solver DLL. This example shows how a model can be extended with
   external C code using three callbacks.

   Last revision: March 2021.
*/

#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h> // Windows-specific header
#endif

#include <stdio.h>
#include <string.h>

#include "vs_deftypes.h" // VS types and definitions
#include "vs_api.h"  // VS API functions
#include "external.h" // user-supplied custom C code

/* ---------------------------------------------------------------------------------
   Main program to run DLL with VS API.
--------------------------------------------------------------------------------- */
int main(int argc, char **argv) {
  HMODULE vsDLL = NULL; // DLL with VS API
  char   pathDLL[FILENAME_MAX], simfile[FILENAME_MAX]={"simfile.sim"};

  // get simfile from argument list and load DLL
  if (argc > 1) strcpy (simfile, &argv[1][0]);
  if (vs_get_dll_path(simfile, pathDLL)) return 1;
#if (defined(_WIN32) || defined(_WIN64))
  vsDLL = LoadLibrary(pathDLL);
#else
  vsDLL = vs_load_library(pathDLL);
#endif
  // get API functions
  if (vs_get_api(vsDLL, pathDLL)) return 1;

  // install external functions from custom code
  vs_install_setdef_function (external_setdef);
  vs_install_calc_function (external_calc);
  vs_install_echo_function (external_echo);

  // Make the run; vs_run returns 0 if OK
#if (defined(_WIN32) || defined(_WIN64))
  if (vs_run (simfile))
    MessageBox (NULL, vs_get_error_message(), NULL, MB_ICONERROR);
#else
  vs_run(simfile);
#endif

  // Wait for a keypress if the parameter opt_pause was specified.
  if (vs_opt_pause()) {
    printf (
      "\n\nThe run ended normally. OPT_PAUSE was set to keep this display visible."
      "\nPress the Return key to exit this solver program. ");
    fgetc (stdin);
  }
  vs_free_library (vsDLL);
  return 0;
}
