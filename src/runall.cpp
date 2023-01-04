/* Wrapper program that can be launched anywhere in Windows, and will then load
   a VehicleSim solver DLL. This example does not use vs_run, but instead gives
   access to the integration loop. It support the OPT_PAUSE parameter.

   Copyright 1996 - 2021. Mechanical Simulation Corporation.
*/

#include <string.h>
#include <stdio.h>

// #include "vs_deftypes.h" // VS types and definitions
#include "vs_api.h"  // VS API functions

static char *sMsg = NULL; // pointer to support local sPringMsg function

// Print text and reset (set first character to 0)
static void sPrintMsg () {
  if (!*sMsg) return;
  printf ("%s",sMsg);
  *sMsg = 0;
}

/* ---------------------------------------------------------------------------------
   Main program to run DLL with VS API. 
--------------------------------------------------------------------------------- */
int main(int argc, char **argv) {
  HMODULE vsDLL = NULL; // DLL with VS API
  double t; // simulation time
  char   pathDLL[FILENAME_MAX], simfile[FILENAME_MAX]={"C:\\Users\\Public\\Documents\\CarSim2022.1_Data\\simfile.sim"};
  int    ibarg = 0; // counter for text bar graph

  // get simfile from argument list and identify DLL path
  if (argc > 1) strcpy(simfile, &argv[1][0]);
  if (vs_get_dll_path(simfile, pathDLL)) return 1;
  printf("[INFO] pathDLL = %s \n", pathDLL);

  // load dll
  vsDLL = vs_load_library(pathDLL);
  if (vsDLL == NULL)
  {
  printf("[ERROR] vs_load_library failed. \n");
  return 1;
  }else{
    printf("[INFO] vs_load_library finished successfully. \n");
  }

  // get API functions
  if (vs_get_api(vsDLL, pathDLL)){
    printf("[ERROR] vs_get_api failed. \n");
    return 1;
  }else{
    printf("[INFO] vs_get_api finished successfully. \n");
  }

  printf("koko 0 \n");
  if (*(void**)(&vs_get_output_message))
    printf("koko 1 \n");

  printf("(main) vs_error_occurred pointer : %p \n", &vs_error_occurred);
//   printf("debug function : %d \n", vs_error_occurred());

  printf("koko0 \n");
  // vs_api_get_output_message test = (vs_api_get_output_message)GetProcAddress(vsDLL, "vs_get_output_message");
  // sMsg = test();
  printf("%d \n", vs_error_occurred());
  sMsg = vs_get_output_message(); // pointer to text from DLL for sPrintMsg()
  printf("koko1 \n");

  // read inputs and initialize
  t = vs_setdef_and_read(simfile, NULL, NULL);
  if (vs_error_occurred()) {
    printf("\n\nError occurred reading simfile \"%s\"", simfile);
    return 1;
  }

  vs_initialize (t, NULL, NULL);
  sPrintMsg ();

  // Run. Each loop advances time one step
  while (!vs_stop_run()) {
    vs_integrate (&t, NULL);
    vs_bar_graph_update (&ibarg); // update bar graph?
  }

  // Terminate
  vs_terminate (t, NULL);
  sPrintMsg ();
  
  // pause?
  if (vs_opt_pause()) {
    printf ("\nOPT_PAUSE was set to keep this display visible."
            "\nPress the Return key to exit. ");
    fgetc (stdin);
  }
  
  vs_free_library(vsDLL);
  
  return 0;
}
