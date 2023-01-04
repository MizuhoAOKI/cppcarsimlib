/* Functions to load a VS API DLL using GetProcAddress. The functions declared
   in vs_api.h are global, so this only works for a single loaded DLL.

   Log:
   Aug 31, 16. Changes for CarSim 2017, including Linux support.
   Nov 13, 15. Changes for CarSim 2016
   Sep 24, 14. Added new function for CarSim 9.0.
   May 17, 10. M. Sayers. Complete re-write with vs_get_api, better error handling.
   May 18, 09. M. Sayers. Include vs_get_api_install_external for CarSim 8.0.
   Jun 13, 08. M. Sayers. Created for the release of CarSim 7.1.
   */

// Standard C headers.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "vs_deftypes.h" // VS types and definitions
#include "vs_api.h"      // VS API definitions as prototypes
#include "vs_api_road.h"

#ifdef _WIN64
#define BITNESS_SUFFIX "_64"
#elif _WIN32
#define BITNESS_SUFFIX "_32"
#else
#define BITNESS_SUFFIX ""
#endif

#if (defined(_WIN32) || defined(_WIN64))
#define LIBRARYTYPESTR "DLL"
#else
#define LIBRARYTYPESTR "SharedLibrary"
#include <dlfcn.h>
#endif

#if defined(_MSC_VER)
  // Defining _CRT_SECURE_NO_WARNINGS here doesn't seem to work, so we disable this warning directly:
  #pragma warning(disable: 4996)
#endif

// declare global variables (they are "extern" listed in vs_api.h)
// run a simulation with one call (chapter 2)
int (ENC *vs_run)(const char *simfile);

// more control over a run (chapter 2)
int     (ENC *vs_bar_graph_update)(int *);
vs_bool (ENC *vs_error_occurred)(void);
void    (ENC *vs_initialize) (vs_real t, void (*ext_calc) (vs_real, vs_ext_loc),
          void (*ext_echo) (vs_ext_loc));
int     (ENC *vs_integrate)(vs_real *t,
                        void (*ext_eq_in) (vs_real, vs_ext_loc));
void    (ENC *vs_set_macro)(const char *macro_name, const char* macro_value);
vs_real (ENC *vs_setdef_and_read)(const char *simfile, void (*ext_setdef) (void),
            int (*ext_scan) (char *, char *));
int   (ENC *vs_statement)(const char *key, const char *buffer, int stopError);
int   (ENC *vs_stop_run)(void);
void  (ENC *vs_terminate_run) (vs_real t);

// simulating with import/export arrays (chapter 4)
void    (ENC *vs_copy_export_vars) (vs_real *exports);
vs_real (ENC *vs_get_tstep)(void);
int     (ENC *vs_integrate_io)(vs_real t, vs_real *imports, vs_real *exports);
void    (ENC *vs_read_configuration) (const char *simfile, int *n_import,
           int *n_export, vs_real *tstart, vs_real *tstop, vs_real *tstep);

// utility functions: conditions (chapter 5)
int     (ENC *vs_during_event)(void);
vs_real (ENC *vs_get_time)(void);
vs_bool (ENC *vs_opt_pause)(void);

// utility functions: messages (chapter 5)
void  (ENC *vs_clear_error_message) (void);
void  (ENC *vs_clear_output_message) (void);
char *(ENC *vs_get_error_message)(void);
char *(ENC *vs_get_output_message)(void);
char *(ENC *vs_get_version_model)(void);
char *(ENC *vs_get_version_product)(void);
char *(ENC *vs_get_version_vs)(void);
void  (ENC *vs_printf)(const char *format, ...);
void  (ENC *vs_printf_error)(const char *format, ...);
void  (ENC *vs_printf_message)(const char* format, ...);

// utility functions: file names and directories (chapter 5)
char *(ENC *vs_get_database_dir)(void);
char *(ENC *vs_get_echofile_name)(void);
char *(ENC *vs_get_endfile_name)(void);
char *(ENC *vs_get_erdfile_name)(void);
char *(ENC *vs_get_filebase_name)(void);
char *(ENC *vs_get_infile_name)(void);
char *(ENC *vs_get_logfile_name)(void);
char *(ENC *vs_get_simfile_name)(void);

// utility functions: writing documentation files (chapter 5)
void  (ENC *vs_write_doc_files) (const char *simfile, const char *root, int model,
         int import, int output);

// installation of callback functions (chapter 6)
void (ENC *vs_install_calc_function) (void (*calc) (vs_real time, vs_ext_loc where));
void (ENC *vs_install_echo_function) (void (*echo) (vs_ext_loc where));
void (ENC *vs_install_setdef_function) (void (*setdef) (void));
void (ENC *vs_install_scan_function) (vs_bool (*scan)(char *, char *));
void (ENC *vs_install_free_function) (void (*free) (void));
void (ENC *vs_install_status_function) (status_func_t extStatusFunc, void* userData);
void (ENC *vs_install_calc_function2) (void (*calc) (vs_real time, vs_ext_loc where,
        void* userData), void* userData);
void (ENC *vs_install_echo_function2)
        (void (*echo) (vs_ext_loc where, void* userData), void* userData);
void (ENC *vs_install_setdef_function2)
        (void (*setdef) (void* userData), void* userData);
void (ENC *vs_install_scan_function2) (void (*scan)
        (char *, char *, void* userData), void* userData);
void (ENC *vs_install_free_function2) (void (*func) (void* userData), void* userData);

// functions for interacting with the VS math model (chapter 7)
vs_bool (ENC *vs_check_import)(const char *key, int stopError);
int (ENC *vs_define_imp)(char *keyword, vs_real *real, char *units, int native,
                              char *component, char *desc);
int (ENC *vs_define_ipar_1d)(char *key, int n, char *index);
int (ENC *vs_define_ipar_2d)(char *key, int n1, char *index1, int n2, char *index2);
int (ENC *vs_define_ipar_3d)(char *key, int n1, char *index1, int n2, char *index2,
       int n3, char *index3);
int (ENC *vs_define_ipar_4d)(char *key, int n1, char *index1, int n2, char *index2,
       int n3, char *index3, int n4, char *index4);
int (ENC *vs_define_ipar_5d)(char *key, int n1, char *index1, int n2, char *index2,
       int n3, char *index3, int n4, char *index4, int n5, char *index5);
int (ENC *vs_define_out)(char *shortname, char *longname, vs_real *real,
                              char *units, char *genname, char *bodyname, char *comp);
int (ENC *vs_define_out_with_shortname)(char* extname, char* erdname, char* longname,
       vs_real* real, char* units, char* genname, char* bodyname, char* component);
int (ENC *vs_define_par)(char *keyword, void *par, vs_real value, char *units,
                              int visible, int init, char *desc);
int (ENC *vs_define_sypar)(char *keyword, void *par, vs_real value, char *units,
       int visible,  char *desc);
int (ENC *vs_define_sv)(char *keyword, vs_real *real, vs_real value, char *units,
       char *desc);
int (ENC* vs_define_sv_ode)(char*, vs_real*, vs_real, char*, char*, vs_real*);
void  (ENC *vs_define_units) (char *desc, vs_real gain);

vs_real (ENC *vs_get_numeric_value)(char *buffer);
int     (ENC *vs_get_int_value)(char* buffer);
int     (ENC *vs_get_sym_attribute)(int id, vs_sym_attr_type type, void **att);
int     (ENC *vs_get_var_id)(char *keyword, vs_sym_attr_type *type);
vs_real *(ENC *vs_get_var_ptr)(char *keyword);
vs_real *(ENC *vs_get_var_ptr2)(const char *keyword, vs_keyword_type *outopt_keywordType, vs_bool allowErrors);
int     *(ENC *vs_get_var_ptr_int)(const char *keyword);
int     *(ENC *vs_get_var_ptr_int2)(const char *keyword, vs_keyword_type* outopt_keywordType, vs_bool allowErrors);
vs_real (ENC *vs_get_var_output_gain)(const char* keyword);
vs_bool (ENC *vs_have_keyword_in_database)(char *keyword);

int   (ENC *vs_get_import_count)();
int   (ENC *vs_get_import_active_index)(int id);
const char *(ENC *vs_get_import_keyword)(int id);
const char *(ENC *vs_get_import_units)(int id);
vs_real (ENC *vs_get_import_value)(int id);
vs_real (ENC *vs_import_result)(int id, vs_real native);
int   (ENC *vs_import_type)(int id);

int   (ENC  *vs_get_active_import_count)();
int   (ENC  *vs_get_active_import_id)(int index);
const char *(ENC *vs_get_active_import_keyword)(int index);
const char *(ENC *vs_get_active_import_units)(int index);
vs_import_option  (ENC *vs_get_active_import_mode)(int index);
vs_real (ENC *vs_get_active_import_value)(int index);
vs_real (ENC *vs_get_active_import_result)(int index, vs_real native);

void  (ENC *vs_install_symbolic_func) (char *name, void *func, int n_args);
int   (ENC *vs_install_keyword_alias)(char *existing, char *alias, int isDeprecated);
void  (ENC *vs_put_on_free_list) (void *thing);
void  (ENC* vs_post_proc_function) (const char *Message, int (*post_proc) (void* userData), void *userData);
void  (ENC *vs_pre_proc_function) (const char *Message, int (*pre_proc) (void* userData), void* userData);
void  (ENC *vs_set_stop_run) (vs_real stop_gt_0, const char *format, ...);
int   (ENC *vs_set_sym_attribute)(int id, vs_sym_attr_type type, const void *att);
int   (ENC *vs_set_sym_int)(int id, vs_sym_attr_type dataType, int value);
char *(ENC *vs_string_copy_internal)(char **target, char *source);
void  (ENC *vs_set_opt_error_dialog)(int value);

// working with simulation files (chapter 7)
int  (ENC *vs_add_new_par_group)(char *text);
int  (ENC *vs_insert_echo_blank_line)(void);
int  (ENC *vs_insert_echo_comment)(char *desc);
int  (ENC *vs_get_active_par_group)(void);
void (ENC *vs_set_active_par_group) (int i);

void (ENC *vs_read_next_line) (char *buffer, int n);
void (ENC *vs_write_f_to_echo_file) (char *key, vs_real , char *doc);
void (ENC *vs_write_header_to_echo_file) (char *buffer);
void (ENC *vs_write_i_to_echo_file) (char *key, int , char *doc);
void (ENC *vs_write_to_echo_file) (const char *format, ...);
void (ENC *vs_write_to_logfile) (int level, const char *format, ...);
void (ENC *vs_write_header_file)(const char* simfile);

// configurable table functions (chapter 7)
int     (ENC *vs_define_table)(char *root, int ntab, int ninst);
vs_real (ENC *vs_table_calc)(int index, vs_real xcol, vs_real x, int itab, int inst);
int     (ENC *vs_table_index)(char *name);
int     (ENC *vs_table_ninst)(int index);
int     (ENC *vs_table_ntab)(int index);
void    (ENC *vs_set_units_table)(const char* var_keyword, const char* units_keyword, const int which);

// moving objects and sensors (chapter 7)
int     (ENC *vs_define_moving_objects)(int n);
int     (ENC *vs_define_sensors)(int n);
void    (ENC *vs_free_sensors_and_objects)(void);

// saving and restoring the model state (chapter 8)
void    (ENC *vs_free_saved_states) (void);
int     (ENC *vs_get_request_to_restore)(void);
int     (ENC *vs_get_request_to_save)(void);
vs_real (ENC *vs_get_saved_state_time)(vs_real t);
vs_real (ENC *vs_restore_state)(void);
void    (ENC *vs_save_state) (void);
void    (ENC *vs_set_request_to_restore) (vs_real t);
void    (ENC *vs_start_save_timer) (vs_real t);
void    (ENC *vs_stop_save_timer) (void);

// managing arrays to support restarts (chapter 8)
void (ENC *vs_copy_all_state_vars_from_array) (vs_real *array);
void (ENC *vs_copy_all_state_vars_to_array) (vs_real *array);
void (ENC *vs_copy_differential_state_vars_from_array) (vs_real *array);
void (ENC *vs_copy_differential_state_vars_to_array) (vs_real *array);
void (ENC *vs_copy_extra_state_vars_from_array) (vs_real *array);
void (ENC *vs_copy_extra_state_vars_to_array) (vs_real *array);
int  (ENC *vs_get_export_names)(char **expNames);
int  (ENC *vs_get_import_names)(char **impNames);
void (ENC *vs_get_state_q) (vs_real *array);
int  (ENC* vs_n_active_derivatives)(void);
int  (ENC *vs_n_derivatives)(void);
int  (ENC *vs_n_extra_state_variables)(void);

// Licensing functions
void (ENC *vs_set_hpc_licensing_address) (const char *address);
void (ENC *vs_set_wait_for_parallel_licenses) (vs_bool value);

// undocumented functions to support legacy Sensor Ports in Simulink Gen-1
int (ENC *vs_get_n_export_sensor)(int *max_connections);
int (ENC *vs_get_sensor_connections)(vs_real *connect);

// Linearization
int (ENC* vs_linear_control)(char *buffer);
int (ENC* vs_linear_output)(char *buffer);
int (ENC* vs_linear_sv)(char *buffer);
void (ENC* vs_copy_output_vars_linearization)(vs_real *out);
void (ENC* vs_get_control_var_ptr_linearization)(vs_real **control, vs_real *prtb);

int  (ENC *vs_get_lat_pos_of_edge)(int edge, vs_real s, int opt_road, vs_real *l);
void (ENC *vs_scale_export_vars) (void);

// Alternative legacy functions (Appendix)
void (ENC *vs_add_echo_header) (const char *text);
void (ENC *vs_copy_import_vars) (vs_real *imports);
void (ENC *vs_copy_io) (vs_real *imports, vs_real *exports);
int  (ENC *vs_define_import)(char *keyword, char *desc, vs_real *real, char *);
int  (ENC *vs_define_indexed_parameter_array)(char *keyword);
int  (ENC *vs_define_output)(char *shortname, char *longname, vs_real *real, char *);
int  (ENC *vs_define_parameter)(char *keyword, char *desc, vs_real *, char *);
int  (ENC *vs_define_parameter_int)(char *keyword, char *desc, int *);
int   (ENC *vs_define_variable)(char *keyword, char *desc, vs_real *);
void (ENC *vs_free_all) (void);
vs_bool (ENC *vs_integrate_io_2)(vs_real t, vs_real *imports, vs_real *exports,
           void (*ext_calc) (vs_real, vs_ext_loc));
int   (ENC *vs_integrate_IO)(vs_real t, vs_real *imports, vs_real *exports);
void  (ENC *vs_opt_all_write) (int arg); // Jan 20, 2015
void  (ENC *vs_scale_import_vars) (void);
int   (ENC *vs_set_sym_real)(int id, vs_sym_attr_type dataType, vs_real value);
void  (ENC *vs_set_units) (char *var_keyword, char *units_keyword);
void  (ENC *vs_terminate) (vs_real t, void (*ext_echo) (vs_ext_loc));

// Legacy functions from get_api.c (Appendix)
int vs_get_api_basic(HMODULE dll, const char *dll_fname);
int vs_get_api_extend(HMODULE dll, const char *dll_fname);
int vs_get_api_install_external(HMODULE dll, const char *dll_fname);
int vs_get_api_road(HMODULE dll, const char *dll_fname);

// Deprecated functions (Appendix)
void (ENC *vs_install_calc_func) (char *name, void *func);
void (ENC *vs_setdef) (void(*ext_setdef) (void));
vs_real (ENC *vs_read)(const char *simfile,
           vs_bool (*ext_scan) (char *key, char *buffer));


/// Default error message handler function.
static int sDefaultPrintf(const char *format, ...) {
  int   numberOfCharactersPrinted;
  char  tmpstr[FILENAME_MAX + 100];

  va_list args;
  va_start (args, format);
  numberOfCharactersPrinted = vsprintf(tmpstr, format, args);
  va_end (args);

#if (defined(_WIN32) || defined(_WIN64))
  MessageBoxA (NULL, tmpstr, "Sorry", MB_ICONERROR);
#else
  printf(tmpstr, args);
  printf("\n");
#endif

  return numberOfCharactersPrinted;
}


/** The API-error print function pointer. Initially, it points to our default
  error message handler, sDefaultPrintf(), but this can be overridden
  by using vs_set_error_message_function().
*/
static int( *sPrintfError ) (const char *format, ...) = sDefaultPrintf;

void vs_set_error_message_function (int(*errorMsgFunc) (const char *format, ...)) {
  sPrintfError = errorMsgFunc;
}

#if (defined(_WIN32) || defined(_WIN64))
HMODULE vs_load_library(char* pathDLL) {
  return LoadLibraryA(pathDLL);
}
void vs_free_library(HMODULE dll) {
  if (dll != NULL) FreeLibrary(dll);
}
#else
void* vs_load_library(char* pathDLL) {
  char tempPath[FILENAME_MAX] = "";
  char *errorString;

  // RTLD_DEEPBIND - Forces the solver to use its own symbols first, even if
  //   we have samely named symbols in the program that loaded it.
  //   This fixes crashes when the calling program is linked with "-rdynamic".
  int loadFlags = RTLD_NOW | RTLD_DEEPBIND;
  
  // Clear error string.
  dlerror();
  errorString = NULL;

  //printf("Loading library: %s\n", pathDLL);
  void* libPtr = dlopen(pathDLL, loadFlags);
  errorString = dlerror();

  if (libPtr == NULL || (errorString != NULL)) {
    // Also try local folder.
    sprintf(tempPath, "./%s", pathDLL);

    // Clear error string.
    dlerror();
    errorString = NULL;

    libPtr = dlopen(tempPath, loadFlags);
    errorString = dlerror();
  }

  if (libPtr == NULL || (errorString != NULL))
    printf("Error loading library: %s\n", errorString);

  return libPtr;
}

void vs_free_library(HMODULE dll) {
  //printf("Releasing library.\n");
  if (dll != NULL) dlclose(dll);
}
#endif

// Get solver path and program directory from simfile
int vs_get_dll_path(const char* simfile, char* pathDLL)
{
  FILE* fp;
  char *key, *rest, tmpstr[FILENAME_MAX], tempSimfilePath[FILENAME_MAX],
       progDir[FILENAME_MAX], dllDir[FILENAME_MAX],
       vehicleCode[80], libraryName[128], productName[64], productVersion[64];

#if !defined(_WIN32) && !defined(_WIN64)
  char *tempSimfilePathPtr = NULL;
#endif

  strcpy(tempSimfilePath, simfile);

  // JMM.2018.09.12. This may look a step backwards and maybe it is. The goal of vs_get_api.c is it
  // used and copied to many projects. Some of those project make use of PathHelper.c/h. In those projects
  // there is already defined "convert_path_delimiters" and when linking we will get a multiple definition link
  // error. As such we don't really need a local function that is only used once.
  // The below code snippet converts "\" to "/" when running on "not windows". 
  // Ideally we should be using the PathHelper.h and its "convert_path_delimiters" that does exactly that. 
  // However, under time constraints I am just removing the function from file. I am assuming it was not
  // a coincidence that we had two function doing basically the same thing with the same name and the original
  // author had a reason to copy the function instead of including PathHelpers.h.
#if !defined(_WIN32) && !defined(_WIN64)
  tempSimfilePathPtr = &tempSimfilePath[0];
  while (*tempSimfilePathPtr != '\0')
  {
    if (*tempSimfilePathPtr == '\\')
      *tempSimfilePathPtr = '/';
    tempSimfilePathPtr++;
  }
#endif

  if ((fp = fopen(tempSimfilePath, "r")) == NULL)
  {
    sPrintfError("This program needs a simfile to obtain other file names. The "
                 "file \"%s\" either does not exist or could not be opened.\n",
                 tempSimfilePath);
    return -1;
  }

  pathDLL[0] = progDir[0] = dllDir[0] = vehicleCode[0] = libraryName[0] = productName[0] = productVersion[0] = 0;

  // scan simfile one line at a time
  while (fgets(tmpstr, FILENAME_MAX, fp))
  {
    key = (char *)strtok(tmpstr, " \t\r\n");
    rest = (char *)strtok(NULL, "\r\n\t");

    if (!key) // skip empty lines
      continue;

    // get DLL if specified directly
#if (defined(_WIN32) || defined(_WIN64))
    if (!strcmp(key, "DLLFILE") && rest && rest[0])
#elif defined(_VS_LINUX)
    if ((!strcmp(key, "DLLFILE") || !strcmp(key, "SOFILE")) && rest && rest[0])
#endif
      strcpy(pathDLL, rest);
    if (!strcmp(key, "VEHICLE_CODE")) // get vehicle code
      strcpy(vehicleCode, rest);
    else if (!strcmp(key, "PROGDIR")) // get program directory
      strcpy(progDir, rest);
    else if (!strcmp(key, "PRODUCT_ID")) // get product
      strcpy(productName, rest);
    else if (!strcmp(key, "PRODUCT_VER")) // get version
      strcpy(productVersion, rest);
    else if (!strcmp(key, "END"))
      break;
  }
  fclose(fp);

  if (strstr(vehicleCode, "tire") != NULL)
    sprintf(libraryName, "%s%s", vehicleCode, BITNESS_SUFFIX);
  else if (strcmp(productName, "CarSim") == 0)
    sprintf(libraryName, "carsim%s", BITNESS_SUFFIX);
  else if (strcmp(productName, "TruckSim") == 0)
    sprintf(libraryName, "trucksim%s", BITNESS_SUFFIX);
  else if (strcmp(productName, "BikeSim") == 0)
    sprintf(libraryName, "bikesim%s", BITNESS_SUFFIX);
  else
    sprintf(libraryName, "%s%s", vehicleCode, BITNESS_SUFFIX);

  // Quit if DLL was not specified and neither was vehicle code
  if ((pathDLL[0] == 0) && (libraryName[0] == 0))
  {
    sPrintfError("Unable to determine Solver type from SIMFILE.\n");
    return -1;
  }

  // if DLLFILE was not used, resolve the standard location
#if (defined(_WIN32) || defined(_WIN64))
  if (pathDLL[0] == 0 && progDir[0] != 0 && libraryName[0] != 0)
  {
    sprintf(pathDLL, "%s\\Programs\\solvers\\%s.dll", progDir, libraryName);
  }
#elif defined(_VS_LINUX)
  if (pathDLL[0] == 0 && progDir[0] != 0 && libraryName[0] != 0 && productVersion[0] != 0)
  {
    sprintf(pathDLL, "%s/lib%s.so.%s", progDir, libraryName, productVersion);
  }
  else if (pathDLL[0] == 0 && progDir[0] == 0 && libraryName[0] != 0 && productVersion[0] != 0)
  {
    sprintf(progDir, "/opt/%s_%s/bin", libraryName, productVersion);
    sprintf(pathDLL, "%s/lib%s.so.%s", progDir, libraryName, productVersion);
}
#endif

  return 0;
}

// try to get the address of an API function from a DLL
static int sGetApi (void *api_func, HMODULE dll, const char *func, const char *dll_name,
                    const char *get_func) {
#if !defined(_WIN32) && !defined(_WIN64)
  char *errorString;
#endif

  if (dll == NULL)
    return -1;

#if (defined(_WIN32) || defined(_WIN64))
  (*(void**)api_func) = (void *)GetProcAddress(dll, func); // cast to work

  if (*(void**)api_func)
    return 0;
#else

  dlerror();  // Clear any existing error, so we can check it afterwards.

  (*(void**)api_func) = dlsym(dll, func);

  if ((errorString = dlerror()) != NULL)
    sPrintfError( "dlerror(): %s", errorString);
  else
    return 0;
#endif

  sPrintfError( "The function %s could not get the requested VS API function \"%s\" "
                "from the " LIBRARYTYPESTR ": \"%s\".\n", get_func, func, dll_name);
  return -2;
}

// utility to handle error if a DLL isn't there.
static int sPrintErrorNoDll (const char *where, const char *dll_name) {
  sPrintfError("The function %s was not given a valid " LIBRARYTYPESTR ". "
                              "\"%s\" did not load.\n", where, dll_name);
  return -1;
}

// Helper function to get road API from a particular source function.
static int sGetApiRoad(HMODULE dll, const char *dname, char *me) {
  if (dll == NULL) return sPrintErrorNoDll(me, dname);

  // 2D reference path properties (chapter 7)
  if (sGetApi(&vs_get_road_start_stop, dll, "vs_get_road_start_stop", dname, me)) return -2;
  if (sGetApi(&vs_path_curv_i, dll, "vs_path_curv_i", dname, me)) return -2;
  if (sGetApi(&vs_road_curv_i, dll, "vs_road_curv_i", dname, me)) return -2;
  if (sGetApi(&vs_path_l_i, dll, "vs_path_l_i", dname, me)) return -2;
  if (sGetApi(&vs_road_l, dll, "vs_road_l", dname, me)) return -2;
  if (sGetApi(&vs_road_l_i, dll, "vs_road_l_i", dname, me)) return -2;
  if (sGetApi(&vs_path_s_i, dll, "vs_path_s_i", dname, me)) return -2;
  if (sGetApi(&vs_road_s, dll, "vs_road_s", dname, me)) return -2;
  if (sGetApi(&vs_road_s_i, dll, "vs_road_s_i", dname, me)) return -2;
  if (sGetApi(&vs_path_x_i, dll, "vs_path_x_i", dname, me)) return -2;
  if (sGetApi(&vs_road_x, dll, "vs_road_x", dname, me)) return -2;
  if (sGetApi(&vs_road_x_i, dll, "vs_road_x_i", dname, me)) return -2;
  if (sGetApi(&vs_path_x_sl_i, dll, "vs_path_x_sl_i", dname, me)) return -2;
  if (sGetApi(&vs_road_x_sl_i, dll, "vs_road_x_sl_i", dname, me)) return -2;
  if (sGetApi(&vs_path_y_i, dll, "vs_path_y_i", dname, me)) return -2;
  if (sGetApi(&vs_road_y, dll, "vs_road_y", dname, me)) return -2;
  if (sGetApi(&vs_road_y_i, dll, "vs_road_y_i", dname, me)) return -2;
  if (sGetApi(&vs_path_y_sl_i, dll, "vs_path_y_sl_i", dname, me)) return -2;
  if (sGetApi(&vs_road_y_sl_i, dll, "vs_road_y_sl_i", dname, me)) return -2;
  if (sGetApi(&vs_path_yaw_i, dll, "vs_path_yaw_i", dname, me)) return -2;
  if (sGetApi(&vs_road_yaw, dll, "vs_road_yaw", dname, me)) return -2;
  if (sGetApi(&vs_road_yaw_i, dll, "vs_road_yaw_i", dname, me)) return -2;
  if (sGetApi(&vs_s_loop, dll, "vs_s_loop", dname, me)) return -2;
  if (sGetApi(&vs_s_loop_i, dll, "vs_s_loop_i", dname, me)) return -2;
  if (sGetApi(&vs_s_path_loop, dll, "vs_s_path_loop", dname, me)) return -2;
  if (sGetApi(&vs_target_heading, dll, "vs_target_heading", dname, me)) return -2;
  if (sGetApi(&vs_target_l, dll, "vs_target_l", dname, me)) return -2;

  if (sGetApi(&vs_get_path_xy, dll, "vs_get_path_xy", dname, me)) return -2;
  if (sGetApi(&vs_get_road_xy, dll, "vs_get_road_xy", dname, me)) return -2;

  // 3D properties of current road (chapter 7)
  if (sGetApi(&vs_get_dzds_dzdl, dll, "vs_get_dzds_dzdl", dname, me)) return -2;
  if (sGetApi(&vs_get_dzds_dzdl_i, dll, "vs_get_dzds_dzdl_i", dname, me)) return -2;
  if (sGetApi(&vs_get_road_contact, dll, "vs_get_road_contact", dname, me)) return -2;
  if (sGetApi(&vs_get_road_contact_sl, dll, "vs_get_road_contact_sl", dname, me)) return -2;
  if (sGetApi(&vs_get_road_xyz, dll, "vs_get_road_xyz", dname, me)) return -2;
  if (sGetApi(&vs_road_pitch_sl_i, dll, "vs_road_pitch_sl_i", dname, me)) return -2;
  if (sGetApi(&vs_road_roll_sl_i, dll, "vs_road_roll_sl_i", dname, me)) return -2;
  if (sGetApi(&vs_road_z, dll, "vs_road_z", dname, me)) return -2;
  if (sGetApi(&vs_road_z_2, dll, "vs_road_z_2", dname, me)) return -2;
  if (sGetApi(&vs_road_z_i, dll, "vs_road_z_i", dname, me)) return -2;
  if (sGetApi(&vs_road_z_sl_i, dll, "vs_road_z_sl_i", dname, me)) return -2;

  // Functions the make use of user ID for paths (chapter 7)
  if (sGetApi(&vs_path_sstart_id, dll, "vs_path_sstart_id", dname, me)) return -2;
  if (sGetApi(&vs_path_sstop_id, dll, "vs_path_sstop_id", dname, me)) return -2;
  if (sGetApi(&vs_path_length_id, dll, "vs_path_length_id", dname, me)) return -2;
  if (sGetApi(&vs_path_is_looped_id, dll, "vs_path_is_looped_id", dname, me)) return -2;
  if (sGetApi(&vs_path_s_id, dll, "vs_path_s_id", dname, me)) return -2;
  if (sGetApi(&vs_path_l_id, dll, "vs_path_l_id", dname, me)) return -2;
  if (sGetApi(&vs_path_x_id, dll, "vs_path_x_id", dname, me)) return -2;
  if (sGetApi(&vs_path_y_id, dll, "vs_path_y_id", dname, me)) return -2;
  if (sGetApi(&vs_path_dxds_id, dll, "vs_path_dxds_id", dname, me)) return -2;
  if (sGetApi(&vs_path_dyds_id, dll, "vs_path_dyds_id", dname, me)) return -2;
  if (sGetApi(&vs_path_dxdl_id, dll, "vs_path_dxdl_id", dname, me)) return -2;
  if (sGetApi(&vs_path_dydl_id, dll, "vs_path_dydl_id", dname, me)) return -2;
  if (sGetApi(&vs_path_curv_id, dll, "vs_path_curv_id", dname, me)) return -2;
  if (sGetApi(&vs_path_yaw_id, dll, "vs_path_yaw_id", dname, me)) return -2;

  // Functions that make use of user ID for roads (chapter 7)
  if (sGetApi(&vs_road_sstart_id, dll, "vs_road_sstart_id", dname, me)) return -2;
  if (sGetApi(&vs_road_sstop_id, dll, "vs_road_sstop_id", dname, me)) return -2;
  if (sGetApi(&vs_road_length_id, dll, "vs_road_length_id", dname, me)) return -2;
  if (sGetApi(&vs_road_is_looped_id, dll, "vs_road_is_looped_id", dname, me)) return -2;
  if (sGetApi(&vs_road_s_id, dll, "vs_road_s_id", dname, me)) return -2;
  if (sGetApi(&vs_road_l_id, dll, "vs_road_l_id", dname, me)) return -2;
  if (sGetApi(&vs_road_x_id, dll, "vs_road_x_id", dname, me)) return -2;
  if (sGetApi(&vs_road_y_id, dll, "vs_road_y_id", dname, me)) return -2;
  if (sGetApi(&vs_road_dxds_id, dll, "vs_road_dxds_id", dname, me)) return -2;
  if (sGetApi(&vs_road_dyds_id, dll, "vs_road_dyds_id", dname, me)) return -2;
  if (sGetApi(&vs_road_dxdl_id, dll, "vs_road_dxdl_id", dname, me)) return -2;
  if (sGetApi(&vs_road_dydl_id, dll, "vs_road_dydl_id", dname, me)) return -2;
  if (sGetApi(&vs_road_curv_id, dll, "vs_road_curv_id", dname, me)) return -2;
  if (sGetApi(&vs_road_z_id, dll, "vs_road_z_id", dname, me)) return -2;
  if (sGetApi(&vs_road_dzds_id, dll, "vs_road_dzds_id", dname, me)) return -2;
  if (sGetApi(&vs_road_dzdl_id, dll, "vs_road_dzdl_id", dname, me)) return -2;

  if (sGetApi(&vs_road_read, dll, "vs_road_read", dname, me)) return -2;
  if (sGetApi(&vs_road_generate_path_shapes, dll, "vs_road_generate_path_shapes", dname, me)) return -2;
  if (sGetApi(&vs_road_generate_road_shapes, dll, "vs_road_generate_road_shapes", dname, me)) return -2;
  if (sGetApi(&vs_road_write_animator_par, dll, "vs_road_write_animator_par", dname, me)) return -2;

  if (sGetApi(&vs_road_install_terrain_function, dll, "vs_road_install_terrain_function", dname, me)) return -2;
  
  if (sGetApi(&vs_copy_inst_from_array, dll, "vs_copy_inst_from_array", dname, me)) return -2;
  if (sGetApi(&vs_copy_inst_to_array, dll, "vs_copy_inst_to_array", dname, me)) return -2;

  return 0;
}

/* ----------------------------------------------------------------------------
   Get all VS API functions declared in "vs_api.h"
---------------------------------------------------------------------------- */
int vs_get_api (HMODULE dll, const char *dname) {
  char *me = "vs_get_api";
  if (dll == NULL) return sPrintErrorNoDll(me, dname);

  // basic run functions (chapter 2)
  if (sGetApi(&vs_run, dll, "vs_run", dname, me)) return -2;
  if (sGetApi(&vs_bar_graph_update, dll, "vs_bar_graph_update", dname, me)) return -2;
  if (sGetApi(&vs_error_occurred, dll, "vs_error_occurred", dname, me)) return -2;
  if (sGetApi(&vs_initialize, dll, "vs_initialize", dname, me)) return -2;
  if (sGetApi(&vs_integrate, dll, "vs_integrate", dname, me)) return -2;
  if (sGetApi(&vs_set_macro, dll, "vs_set_macro", dname, me)) return -2;
  if (sGetApi(&vs_setdef_and_read, dll, "vs_setdef_and_read", dname, me)) return -2;
  if (sGetApi(&vs_statement, dll, "vs_statement", dname, me)) return -2;
  if (sGetApi(&vs_stop_run, dll, "vs_stop_run", dname, me)) return -2;
  if (sGetApi(&vs_terminate_run, dll, "vs_terminate_run", dname, me)) return -2;

  // managing import/export arrays (chapter 4)
  if (sGetApi(&vs_copy_export_vars, dll, "vs_copy_export_vars", dname, me)) return -2;
  if (sGetApi(&vs_integrate_io, dll, "vs_integrate_io", dname, me)) return -2;
  if (sGetApi(&vs_get_tstep, dll, "vs_get_tstep", dname, me)) return -2;
  if (sGetApi(&vs_read_configuration, dll, "vs_read_configuration", dname, me)) return -2;

  // utility functions: conditons (chapter 5)
  if (sGetApi(&vs_during_event, dll, "vs_during_event", dname, me)) return -2;
  if (sGetApi(&vs_get_time, dll, "vs_get_time", dname, me)) return -2;
  if (sGetApi(&vs_opt_pause, dll, "vs_opt_pause", dname, me)) return -2;

  // utility functions: messages (chapter 5)
  if (sGetApi(&vs_clear_output_message, dll, "vs_clear_output_message", dname, me)) return -2;
  if (sGetApi(&vs_clear_error_message, dll, "vs_clear_error_message", dname, me)) return -2;
  if (sGetApi(&vs_get_error_message, dll, "vs_get_error_message", dname, me)) return -2;
  if (sGetApi(&vs_get_output_message, dll, "vs_get_output_message", dname, me)) return -2;
  if (sGetApi(&vs_get_version_model, dll, "vs_get_version_model", dname, me)) return -2;
  if (sGetApi(&vs_get_version_product, dll, "vs_get_version_product", dname, me)) return -2;
  if (sGetApi(&vs_get_version_vs, dll, "vs_get_version_vs", dname, me)) return -2;
  if (sGetApi(&vs_printf, dll, "vs_printf", dname, me)) return -2;
  if (sGetApi(&vs_printf_error, dll, "vs_printf_error", dname, me)) return -2;
  if (sGetApi(&vs_printf_message, dll, "vs_printf_message", dname, me)) return -2;

  // utility functions: file names and directories (chapter 5)
  if (sGetApi(&vs_get_database_dir, dll, "vs_get_database_dir", dname, me)) return -2;
  if (sGetApi(&vs_get_echofile_name, dll, "vs_get_echofile_name", dname, me)) return -2;
  if (sGetApi(&vs_get_endfile_name, dll, "vs_get_endfile_name", dname, me)) return -2;
  if (sGetApi(&vs_get_erdfile_name, dll, "vs_get_erdfile_name", dname, me)) return -2;
  if (sGetApi(&vs_get_filebase_name, dll, "vs_get_filebase_name", dname, me)) return -2;
  if (sGetApi(&vs_get_infile_name, dll, "vs_get_infile_name", dname, me)) return -2;
  if (sGetApi(&vs_get_logfile_name, dll, "vs_get_logfile_name", dname, me)) return -2;
  if (sGetApi(&vs_get_simfile_name, dll, "vs_get_simfile_name", dname, me)) return -2;
  if (sGetApi(&vs_write_doc_files, dll, "vs_write_doc_files", dname, me)) return -2;

   // install external functions (chapter 6)
  if (sGetApi(&vs_install_calc_function, dll, "vs_install_calc_function", dname, me)) return -2;
  if (sGetApi(&vs_install_echo_function, dll, "vs_install_echo_function", dname, me)) return -2;
  if (sGetApi(&vs_install_setdef_function, dll, "vs_install_setdef_function", dname, me)) return -2;
  if (sGetApi(&vs_install_scan_function, dll, "vs_install_scan_function", dname, me)) return -2;
  if (sGetApi(&vs_install_free_function, dll, "vs_install_free_function", dname, me)) return -2;
  if (sGetApi(&vs_install_status_function, dll, "vs_install_status_function", dname, me)) return -2;
  if (sGetApi(&vs_install_calc_function2, dll, "vs_install_calc_function2", dname, me)) return -2;
  if (sGetApi(&vs_install_echo_function2, dll, "vs_install_echo_function2", dname, me)) return -2;
  if (sGetApi(&vs_install_setdef_function2, dll, "vs_install_setdef_function2", dname, me)) return -2;
  if (sGetApi(&vs_install_scan_function2, dll, "vs_install_scan_function2", dname, me)) return -2;
  if (sGetApi(&vs_install_free_function2, dll, "vs_install_free_function2", dname, me)) return -2;

  // functions for interacting with the VS math model (chapter 7)
  if (sGetApi(&vs_define_imp, dll, "vs_define_imp", dname, me)) return -2;
  if (sGetApi(&vs_define_ipar_1d, dll, "vs_define_ipar_1d", dname, me)) return -2;
  if (sGetApi(&vs_define_ipar_2d, dll, "vs_define_ipar_2d", dname, me)) return -2;
  if (sGetApi(&vs_define_ipar_3d, dll, "vs_define_ipar_3d", dname, me)) return -2;
  if (sGetApi(&vs_define_ipar_4d, dll, "vs_define_ipar_4d", dname, me)) return -2;
  if (sGetApi(&vs_define_ipar_5d, dll, "vs_define_ipar_5d", dname, me)) return -2;
  if (sGetApi(&vs_define_out, dll, "vs_define_out", dname, me)) return -2;
  if (sGetApi(&vs_define_out_with_shortname, dll, "vs_define_out_with_shortname", dname, me)) return -2;
  if (sGetApi(&vs_define_par, dll, "vs_define_par", dname, me)) return -2;
  if (sGetApi(&vs_define_sypar, dll, "vs_define_sypar", dname, me)) return -2;
  if (sGetApi(&vs_define_sv, dll, "vs_define_sv", dname, me)) return -2;
  if (sGetApi(&vs_define_sv_ode, dll, "vs_define_sv_ode", dname, me)) return -2;
  if (sGetApi(&vs_define_units, dll, "vs_define_units", dname, me)) return -2;
  if (sGetApi(&vs_get_numeric_value, dll, "vs_get_numeric_value", dname, me)) return -2;
  if (sGetApi(&vs_get_int_value, dll, "vs_get_int_value", dname, me)) return -2;
  if (sGetApi(&vs_get_sym_attribute, dll, "vs_get_sym_attribute", dname, me)) return -2;
  if (sGetApi(&vs_get_var_id, dll, "vs_get_var_id", dname, me)) return -2;
  if (sGetApi(&vs_get_var_ptr, dll, "vs_get_var_ptr", dname, me)) return -2;
  if (sGetApi(&vs_get_var_ptr2, dll, "vs_get_var_ptr2", dname, me)) return -2;
  if (sGetApi(&vs_get_var_ptr_int, dll, "vs_get_var_ptr_int", dname, me)) return -2;
  if (sGetApi(&vs_get_var_ptr_int2, dll, "vs_get_var_ptr_int2", dname, me)) return -2;
  if (sGetApi(&vs_get_var_output_gain, dll, "vs_get_var_output_gain", dname, me)) return -2;
  if (sGetApi(&vs_have_keyword_in_database, dll, "vs_have_keyword_in_database", dname, me)) return -2;
  if (sGetApi(&vs_check_import, dll, "vs_check_import", dname, me)) return -2;
  if (sGetApi(&vs_import_result, dll, "vs_import_result", dname, me)) return -2;
  if (sGetApi(&vs_import_type, dll, "vs_import_type", dname, me)) return -2;
  if (sGetApi(&vs_install_symbolic_func, dll, "vs_install_symbolic_func", dname, me)) return -2;
  if (sGetApi(&vs_install_keyword_alias, dll, "vs_install_keyword_alias", dname, me)) return -2;
  if (sGetApi(&vs_put_on_free_list, dll, "vs_put_on_free_list", dname, me)) return -2;
  if (sGetApi(&vs_post_proc_function, dll, "vs_post_proc_function", dname, me)) return -2;
  if (sGetApi(&vs_pre_proc_function, dll, "vs_pre_proc_function", dname, me)) return -2;

  if (sGetApi(&vs_set_stop_run, dll, "vs_set_stop_run", dname, me)) return -2;
  if (sGetApi(&vs_set_sym_attribute, dll, "vs_set_sym_attribute", dname, me)) return -2;
  if (sGetApi(&vs_set_sym_int, dll, "vs_set_sym_int", dname, me)) return -2;
  if (sGetApi(&vs_string_copy_internal, dll, "vs_string_copy_internal", dname, me)) return -2;
  if (sGetApi(&vs_set_opt_error_dialog, dll, "vs_set_opt_error_dialog", dname, me)) return -2;

  // working with simulation files (chapter 7)
  if (sGetApi(&vs_add_new_par_group, dll, "vs_add_new_par_group", dname, me)) return -2;
  if (sGetApi(&vs_insert_echo_blank_line, dll, "vs_insert_echo_blank_line", dname, me)) return -2;
  if (sGetApi(&vs_insert_echo_comment, dll, "vs_insert_echo_comment", dname, me)) return -2;
  if (sGetApi(&vs_get_active_par_group, dll, "vs_get_active_par_group", dname, me)) return -2;
  if (sGetApi(&vs_set_active_par_group, dll, "vs_set_active_par_group", dname, me)) return -2;

  if (sGetApi(&vs_read_next_line, dll, "vs_read_next_line", dname, me)) return -2;
  if (sGetApi(&vs_write_f_to_echo_file, dll, "vs_write_f_to_echo_file", dname, me)) return -2;
  if (sGetApi(&vs_write_header_to_echo_file, dll, "vs_write_header_to_echo_file", dname, me)) return -2;
  if (sGetApi(&vs_write_i_to_echo_file, dll, "vs_write_i_to_echo_file", dname, me)) return -2;
  if (sGetApi(&vs_write_to_echo_file, dll, "vs_write_to_echo_file", dname, me)) return -2;
  if (sGetApi(&vs_write_to_logfile, dll, "vs_write_to_logfile", dname, me)) return -2;
  if (sGetApi(&vs_write_header_file, dll, "vs_write_header_file", dname, me)) return -2;

  // configurable table functions (chapter 7)
  if (sGetApi(&vs_define_table, dll, "vs_define_table", dname, me)) return -2;
  if (sGetApi(&vs_table_calc, dll, "vs_table_calc", dname, me)) return -2;
  if (sGetApi(&vs_table_index, dll, "vs_table_index", dname, me)) return -2;
  if (sGetApi(&vs_table_ninst, dll, "vs_table_ninst", dname, me)) return -2;
  if (sGetApi(&vs_table_ntab, dll, "vs_table_ntab", dname, me)) return -2;
  //if (sGetApi(&vs_install_keyword_tab_group, dll, "vs_install_keyword_tab_group", dname, me)) return -2;
  if (sGetApi(&vs_set_units_table, dll, "vs_set_units_table", dname, me)) return -2;
  //if (sGetApi(&vs_copy_table_data, dll, "vs_copy_table_data", dname, me)) return -2;
  //if (sGetApi(&vs_malloc_table_data, dll, "vs_malloc_table_data", dname, me)) return -2;

  // get entire road api (chapter 7)
  if (sGetApiRoad(dll, dname, me)) return -2;

  // moving objects and sensors (chapter 7)
  if (sGetApi(&vs_define_moving_objects, dll, "vs_define_moving_objects", dname, me)) return -2;
  if (sGetApi(&vs_define_sensors, dll, "vs_define_sensors", dname, me)) return -2;
  if (sGetApi(&vs_free_sensors_and_objects, dll, "vs_free_sensors_and_objects", dname, me)) return -2;

  // saving and restoring the model state (chapter 8)
  if (sGetApi(&vs_free_saved_states, dll, "vs_free_saved_states", dname, me)) return -2;
  if (sGetApi(&vs_get_request_to_restore, dll, "vs_get_request_to_restore", dname, me)) return -2;
  if (sGetApi(&vs_get_request_to_save, dll, "vs_get_request_to_save", dname, me)) return -2;
  if (sGetApi(&vs_get_saved_state_time, dll, "vs_get_saved_state_time", dname, me)) return -2;
  if (sGetApi(&vs_restore_state, dll, "vs_restore_state", dname, me)) return -2;
  if (sGetApi(&vs_save_state, dll, "vs_save_state", dname, me)) return -2;
  if (sGetApi(&vs_set_request_to_restore, dll, "vs_set_request_to_restore", dname, me)) return -2;
  if (sGetApi(&vs_start_save_timer, dll, "vs_start_save_timer", dname, me)) return -2;
  if (sGetApi(&vs_stop_save_timer, dll, "vs_stop_save_timer", dname, me)) return -2;

  // managing arrays to support restarts (chapter 8)
  if (sGetApi(&vs_copy_all_state_vars_from_array, dll, "vs_copy_all_state_vars_from_array", dname, me)) return -2;
  if (sGetApi(&vs_copy_all_state_vars_to_array, dll, "vs_copy_all_state_vars_to_array", dname, me)) return -2;
  if (sGetApi(&vs_copy_differential_state_vars_from_array, dll, "vs_copy_differential_state_vars_from_array", dname, me)) return -2;
  if (sGetApi(&vs_copy_differential_state_vars_to_array, dll, "vs_copy_differential_state_vars_to_array", dname, me)) return -2; 
  if (sGetApi(&vs_copy_extra_state_vars_from_array, dll, "vs_target_l", dname, me)) return -2;
  if (sGetApi(&vs_copy_extra_state_vars_to_array, dll, "vs_copy_extra_state_vars_to_array", dname, me)) return -2;
  if (sGetApi(&vs_get_export_names, dll, "vs_get_export_names", dname, me)) return -2;
  if (sGetApi(&vs_get_import_names, dll, "vs_get_import_names", dname, me)) return -2;
  if (sGetApi(&vs_get_state_q, dll, "vs_get_state_q", dname, me)) return -2;
  if (sGetApi(&vs_n_active_derivatives, dll, "vs_n_active_derivatives", dname, me)) return -2;
  if (sGetApi(&vs_n_derivatives, dll, "vs_n_derivatives", dname, me)) return -2;
  if (sGetApi(&vs_n_extra_state_variables, dll, "vs_n_extra_state_variables", dname, me)) return -2;

  // undocumented functions to support legacy Sensor Ports in Simulink Gen-1
  if (sGetApi(&vs_get_n_export_sensor, dll, "vs_get_n_export_sensor", dname, me)) return -2;
  if (sGetApi(&vs_get_sensor_connections, dll, "vs_get_sensor_connections", dname, me)) return -2;

  // Licensing functions
  if (sGetApi(&vs_set_hpc_licensing_address, dll, "vs_set_hpc_licensing_address", dname, me)) return -2;
  if (sGetApi(&vs_set_wait_for_parallel_licenses, dll, "vs_set_wait_for_parallel_licenses", dname, me)) return -2;

  // Linearization
  if (sGetApi(&vs_linear_control, dll, "vs_linear_control", dname, me)) return -2;
  if (sGetApi(&vs_linear_output, dll, "vs_linear_output", dname, me)) return -2;
  if (sGetApi(&vs_linear_sv, dll, "vs_linear_sv", dname, me)) return -2;
  if (sGetApi(&vs_copy_output_vars_linearization, dll, "vs_copy_output_vars_linearization", dname, me)) return -2;
  if (sGetApi(&vs_get_control_var_ptr_linearization, dll, "vs_get_control_var_ptr_linearization", dname, me)) return -2;

  // undocumented
  if (sGetApi(&vs_get_lat_pos_of_edge, dll, "vs_get_lat_pos_of_edge", dname, me)) return -2;
  if (sGetApi(&vs_scale_export_vars, dll, "vs_scale_export_vars", dname, me)) return -2;

 // legacy stuff (Appendix)
  if (sGetApi(&vs_add_echo_header, dll, "vs_add_echo_header", dname, me)) return -2;
  if (sGetApi(&vs_copy_import_vars, dll, "vs_copy_import_vars", dname, me)) return -2;
  if (sGetApi(&vs_copy_io, dll, "vs_copy_io", dname, me)) return -2;
  if (sGetApi(&vs_define_import, dll, "vs_define_import", dname, me)) return -2;
  if (sGetApi(&vs_define_indexed_parameter_array, dll, "vs_define_indexed_parameter_array", dname, me)) return -2;
  if (sGetApi(&vs_define_output, dll, "vs_define_output", dname, me)) return -2;
  if (sGetApi(&vs_define_parameter, dll, "vs_define_parameter", dname, me)) return -2;
  if (sGetApi(&vs_define_parameter_int, dll, "vs_define_parameter_int", dname, me)) return -2;
  if (sGetApi(&vs_define_variable, dll, "vs_define_variable", dname, me)) return -2;
  if (sGetApi(&vs_free_all, dll, "vs_free_all", dname, me)) return -2;
  if (sGetApi(&vs_integrate_IO, dll, "vs_integrate_IO", dname, me)) return -2;
  if (sGetApi(&vs_integrate_io_2, dll, "vs_integrate_io_2", dname, me)) return -2;
  if (sGetApi(&vs_opt_all_write, dll, "vs_opt_all_write", dname, me)) return -2;
  if (sGetApi(&vs_scale_import_vars, dll, "vs_scale_import_vars", dname, me)) return -2;
  if (sGetApi(&vs_set_sym_real, dll, "vs_set_sym_real", dname, me)) return -2;
  if (sGetApi(&vs_set_units, dll, "vs_set_units", dname, me)) return -2;
  if (sGetApi(&vs_terminate, dll, "vs_terminate", dname, me)) return -2;

  if (sGetApi(&vs_setdef, dll, "vs_setdef", dname, me)) return -2;
  if (sGetApi(&vs_read, dll, "vs_read", dname, me)) return -2;
  if (sGetApi(&vs_install_calc_func, dll, "vs_install_calc_func", dname, me)) return -2;
  return 0;
}

/* ----------------------------------------------------------------------------
   Legacy get functions for subsets of the API functions.
---------------------------------------------------------------------------- */
int vs_get_api_basic (HMODULE dll, const char *dname) {
  char *me = "vs_get_api_basic";
  if (dll == NULL) return sPrintErrorNoDll(me, dname);

  if (sGetApi(&vs_bar_graph_update, dll, "vs_bar_graph_update", dname, me)) return -2;
  if (sGetApi(&vs_copy_io, dll, "vs_copy_io", dname, me)) return -2;
  if (sGetApi(&vs_error_occurred, dll, "vs_error_occurred", dname, me)) return -2;
  if (sGetApi(&vs_free_all, dll, "vs_free_all", dname, me)) return -2;
  if (sGetApi(&vs_get_error_message, dll, "vs_get_error_message", dname, me)) return -2;
  if (sGetApi(&vs_get_output_message, dll, "vs_get_output_message", dname, me)) return -2;
  if (sGetApi(&vs_get_tstep, dll, "vs_get_tstep", dname, me)) return -2;
  if (sGetApi(&vs_get_version_product, dll, "vs_get_version_product", dname, me)) return -2;
  if (sGetApi(&vs_get_version_vs, dll, "vs_get_version_vs", dname, me)) return -2;
  if (sGetApi(&vs_initialize, dll, "vs_initialize", dname, me)) return -2;
  if (sGetApi(&vs_integrate, dll, "vs_integrate", dname, me)) return -2;
  if (sGetApi(&vs_integrate_io, dll, "vs_integrate_io", dname, me)) return -2;
  if (sGetApi(&vs_opt_pause, dll, "vs_opt_pause", dname, me)) return -2;
  if (sGetApi(&vs_read_configuration, dll, "vs_read_configuration", dname, me)) return -2;
  if (sGetApi(&vs_set_macro, dll, "vs_set_macro", dname, me)) return -2;
  if (sGetApi(&vs_setdef_and_read, dll, "vs_setdef_and_read", dname, me)) return -2;
  if (sGetApi(&vs_stop_run, dll, "vs_stop_run", dname, me)) return -2;
  if (sGetApi(&vs_terminate, dll, "vs_terminate", dname, me)) return -2;
  if (sGetApi(&vs_write_doc_files, dll, "vs_write_doc_files", dname, me)) return -2;
  if (sGetApi(&vs_set_opt_error_dialog, dll, "vs_set_opt_error_dialog", dname, me)) return -2;

  return 0;
}


int vs_get_api_extend (HMODULE dll, const char *dname) {
  char *me = "vs_get_api_extend";
  if (dll == NULL) return sPrintErrorNoDll(me, dname);

  if (sGetApi(&vs_define_import, dll, "vs_define_import", dname, me)) return -2;
  if (sGetApi(&vs_define_output, dll, "vs_define_output", dname, me)) return -2;
  if (sGetApi(&vs_define_parameter, dll, "vs_define_parameter", dname, me)) return -2;
  if (sGetApi(&vs_define_units, dll, "vs_define_units", dname, me)) return -2;
  if (sGetApi(&vs_define_variable, dll, "vs_define_variable", dname, me)) return -2;
  if (sGetApi(&vs_get_var_ptr, dll, "vs_get_var_ptr", dname, me)) return -2;
  if (sGetApi(&vs_get_var_ptr2, dll, "vs_get_var_ptr2", dname, me)) return -2;
  if (sGetApi(&vs_get_var_ptr_int, dll, "vs_get_var_ptr_int", dname, me)) return -2;
  if (sGetApi(&vs_get_var_ptr_int2, dll, "vs_get_var_ptr_int2", dname, me)) return -2;
  if (sGetApi(&vs_set_units, dll, "vs_set_units", dname, me)) return -2;
  if (sGetApi(&vs_install_calc_func, dll, "vs_install_calc_func", dname, me)) return -2;
  if (sGetApi(&vs_printf, dll, "vs_printf", dname, me)) return -2;
  if (sGetApi(&vs_printf_error, dll, "vs_printf_error", dname, me)) return -2;
  if (sGetApi(&vs_set_sym_int, dll, "vs_set_sym_int", dname, me)) return -2;
  if (sGetApi(&vs_set_sym_real, dll, "vs_set_sym_real", dname, me)) return -2;
  if (sGetApi(&vs_set_sym_attribute, dll, "vs_set_sym_attribute", dname, me)) return -2;
  if (sGetApi(&vs_read_next_line, dll, "vs_read_next_line", dname, me)) return -2;
  if (sGetApi(&vs_write_to_echo_file, dll, "vs_write_to_echo_file", dname, me)) return -2;
  if (sGetApi(&vs_write_header_to_echo_file, dll, "vs_write_header_to_echo_file", dname, me)) return -2;
  if (sGetApi(&vs_write_f_to_echo_file, dll, "vs_write_f_to_echo_file", dname, me)) return -2;
  if (sGetApi(&vs_write_i_to_echo_file, dll, "vs_write_i_to_echo_file", dname, me)) return -2;
  if (sGetApi(&vs_get_sym_attribute, dll, "vs_get_sym_attribute", dname, me)) return -2;
  if (sGetApi(&vs_define_parameter_int, dll, "vs_define_parameter_int", dname, me)) return -2;
  if (sGetApi(&vs_set_opt_error_dialog, dll, "vs_set_opt_error_dialog", dname, me)) return -2;
  if (sGetApi(&vs_write_doc_files, dll, "vs_write_doc_files", dname, me)) return -2;
  if (sGetApi(&vs_set_hpc_licensing_address, dll, "vs_set_hpc_licensing_address", dname, me)) return -2;
  if (sGetApi(&vs_set_wait_for_parallel_licenses, dll, "vs_set_wait_for_parallel_licenses", dname, me)) return -2;

  return 0;
}

int vs_get_api_road (HMODULE dll, const char *dname) {
  return sGetApiRoad(dll, dname, "vs_get_api_road");
}

int vs_get_api_install_external (HMODULE dll, const char *dname) {
  char *me = "vs_get_api_install_external";
  if (dll == NULL) return sPrintErrorNoDll(me, dname);

  if (sGetApi(&vs_run, dll, "vs_run", dname, me)) return -2;

  if (sGetApi(&vs_install_calc_function, dll, "vs_install_calc_function", dname, me)) return -2;
  if (sGetApi(&vs_install_echo_function, dll, "vs_install_echo_function", dname, me)) return -2;
  if (sGetApi(&vs_install_setdef_function, dll, "vs_install_setdef_function", dname, me)) return -2;
  if (sGetApi(&vs_install_scan_function, dll, "vs_install_scan_function", dname, me)) return -2;
  if (sGetApi(&vs_install_free_function, dll, "vs_install_free_function", dname, me)) return -2;
  if (sGetApi(&vs_install_status_function, dll, "vs_install_status_function", dname, me)) return -2;
  if (sGetApi(&vs_install_calc_function2, dll, "vs_install_calc_function2", dname, me)) return -2;
  if (sGetApi(&vs_install_echo_function2, dll, "vs_install_echo_function2", dname, me)) return -2;
  if (sGetApi(&vs_install_setdef_function2, dll, "vs_install_setdef_function2", dname, me)) return -2;
  if (sGetApi(&vs_install_scan_function2, dll, "vs_install_scan_function2", dname, me)) return -2;
  if (sGetApi(&vs_install_free_function2, dll, "vs_install_free_function2", dname, me)) return -2;

  return 0;
}
