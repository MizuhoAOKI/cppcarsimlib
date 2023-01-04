// This is needed for SDK examples that load a VS Solver and use the API functions.
// It has prototypes for the VS API functions in any VS DLL, listed more or less as
// they appear in the VS API reference manual.

#ifndef _vs_api_h_INCLUDED
#define _vs_api_h_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define PRODUCT_VERSION "2022.1"

#include "vs_deftypes.h"

// ENC = External Naming Convention
#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#define ENC __cdecl
#else
#define HMODULE void*
#define ENC
#endif

// load a VS Solver link and link to the API functions (chapter 2). These are public
// functions used in the wrapper; they are not in the VS Solver library.
HMODULE vs_load_library(char* pathDLL);
void vs_free_library (HMODULE dll);
int vs_get_dll_path(const char *simfile, char *pathDLL);
int vs_get_api(HMODULE dll, const char *dll_fname);
void vs_set_error_message_function(int(*errorMsgFunc)(const char *format, ...));

// run a simulation with one call (chapter 2)
extern int (ENC *vs_run)(const char *simfile);

// more control over a run (chapter 2)
extern int     (ENC *vs_bar_graph_update)(int *);
extern vs_bool (ENC *vs_error_occurred)(void);
extern void    (ENC *vs_initialize) (vs_real t, void (*ext_calc) (vs_real, vs_ext_loc),
          void (*ext_echo) (vs_ext_loc));
extern int     (ENC *vs_integrate)(vs_real *t,
                        void (*ext_eq_in) (vs_real, vs_ext_loc));
extern void    (ENC *vs_set_macro)(const char *macro_name, const char* macro_value);
extern vs_real (ENC *vs_setdef_and_read)(const char *simfile, void (*ext_setdef) (void),
            int (*ext_scan) (char *, char *));
extern int   (ENC *vs_statement)(const char *key, const char *buffer, int stopError);
extern int   (ENC *vs_stop_run)(void);
extern void  (ENC *vs_terminate_run) (vs_real t);

// simulating with import/export arrays (chapter 4)
extern void    (ENC *vs_copy_export_vars) (vs_real *exports);
extern vs_real (ENC *vs_get_tstep)(void);
extern int     (ENC *vs_integrate_io)(vs_real t, vs_real *imports, vs_real *exports);
extern void    (ENC *vs_read_configuration) (const char *simfile, int *n_import,
           int *n_export, vs_real *tstart, vs_real *tstop, vs_real *tstep);

// utility functions: conditions (chapter 5)
extern int     (ENC *vs_during_event)(void);
extern vs_real (ENC *vs_get_time)(void);
extern vs_bool (ENC *vs_opt_pause)(void);

// utility functions: messages (chapter 5)
extern void  (ENC *vs_clear_error_message) (void);
extern void  (ENC *vs_clear_output_message) (void);
extern char *(ENC *vs_get_error_message)(void);
extern char *(ENC *vs_get_output_message)(void);
extern char *(ENC *vs_get_version_model)(void);
extern char *(ENC *vs_get_version_product)(void);
extern char *(ENC *vs_get_version_vs)(void);
extern void  (ENC *vs_printf)(const char *format, ...);
extern void  (ENC *vs_printf_error)(const char *format, ...);
extern void  (ENC *vs_printf_message)(const char* format, ...);

// utility functions: file names and directories (chapter 5)
extern char *(ENC *vs_get_database_dir)(void);
extern char *(ENC *vs_get_echofile_name)(void);
extern char *(ENC *vs_get_endfile_name)(void);
extern char *(ENC *vs_get_erdfile_name)(void);
extern char *(ENC *vs_get_filebase_name)(void);
extern char *(ENC *vs_get_infile_name)(void);
extern char *(ENC *vs_get_logfile_name)(void);
extern char *(ENC *vs_get_simfile_name)(void);

// utility functions: writing documentation files (chapter 5)
extern void  (ENC *vs_write_doc_files) (const char *simfile, const char *root, int model,
         int import, int output);

// installation of callback functions (chapter 6)
extern void (ENC *vs_install_calc_function) (void (*calc) (vs_real time, vs_ext_loc where));
extern void (ENC *vs_install_echo_function) (void (*echo) (vs_ext_loc where));
extern void (ENC *vs_install_setdef_function) (void (*setdef) (void));
extern void (ENC *vs_install_scan_function) (vs_bool (*scan)(char *, char *));
extern void (ENC *vs_install_free_function) (void (*free) (void));
extern void (ENC *vs_install_status_function) (status_func_t extStatusFunc, void* userData);
extern void (ENC *vs_install_calc_function2) (void (*calc) (vs_real time, vs_ext_loc where,
        void* userData), void* userData);
extern void (ENC *vs_install_echo_function2)
        (void (*echo) (vs_ext_loc where, void* userData), void* userData);
extern void (ENC *vs_install_setdef_function2)
        (void (*setdef) (void* userData), void* userData);
extern void (ENC *vs_install_scan_function2) (void (*scan)
        (char *, char *, void* userData), void* userData);
extern void (ENC *vs_install_free_function2) (void (*func) (void* userData), void* userData);

// functions for interacting with the VS math model (chapter 7)
extern vs_bool (ENC *vs_check_import)(const char *key, int stopError);
extern int (ENC *vs_define_imp)(char *keyword, vs_real *real, char *units, int native,
                              char *component, char *desc);
extern int (ENC *vs_define_ipar_1d)(char *key, int n, char *index);
extern int (ENC *vs_define_ipar_2d)(char *key, int n1, char *index1, int n2, char *index2);
extern int (ENC *vs_define_ipar_3d)(char *key, int n1, char *index1, int n2, char *index2,
       int n3, char *index3);
extern int (ENC *vs_define_ipar_4d)(char *key, int n1, char *index1, int n2, char *index2,
       int n3, char *index3, int n4, char *index4);
extern int (ENC *vs_define_ipar_5d)(char *key, int n1, char *index1, int n2, char *index2,
       int n3, char *index3, int n4, char *index4, int n5, char *index5);
extern int (ENC *vs_define_out)(char *shortname, char *longname, vs_real *real,
                              char *units, char *genname, char *bodyname, char *comp);
extern int (ENC *vs_define_out_with_shortname)(char* extname, char* erdname, char* longname,
       vs_real* real, char* units, char* genname, char* bodyname, char* component);
extern int (ENC *vs_define_par)(char *keyword, void *par, vs_real value, char *units,
                              int visible, int init, char *desc);
extern int (ENC *vs_define_sypar)(char *keyword, void *par, vs_real value, char *units,
       int visible,  char *desc);
extern int (ENC *vs_define_sv)(char *keyword, vs_real *real, vs_real value, char *units,
       char *desc);
extern int (ENC* vs_define_sv_ode)(char*, vs_real*, vs_real, char*, char*, vs_real*);
extern void  (ENC *vs_define_units) (char *desc, vs_real gain);

extern vs_real (ENC *vs_get_numeric_value)(char *buffer);
extern int     (ENC *vs_get_int_value)(char* buffer);
extern int     (ENC *vs_get_sym_attribute)(int id, vs_sym_attr_type type, void **att);
extern int     (ENC *vs_get_var_id)(char *keyword, vs_sym_attr_type *type);
extern vs_real *(ENC *vs_get_var_ptr)(char *keyword);
extern vs_real *(ENC *vs_get_var_ptr2)(const char *keyword, vs_keyword_type *outopt_keywordType, vs_bool allowErrors);
extern int     *(ENC *vs_get_var_ptr_int)(const char *keyword);
extern int     *(ENC *vs_get_var_ptr_int2)(const char *keyword, vs_keyword_type* outopt_keywordType, vs_bool allowErrors);
extern vs_real (ENC *vs_get_var_output_gain)(const char* keyword);
extern vs_bool (ENC *vs_have_keyword_in_database)(char *keyword);

extern int   (ENC *vs_get_import_count)();
extern int   (ENC *vs_get_import_active_index)(int id);
extern const char *(ENC *vs_get_import_keyword)(int id);
extern const char *(ENC *vs_get_import_units)(int id);
extern vs_real (ENC *vs_get_import_value)(int id);
extern vs_real (ENC *vs_import_result)(int id, vs_real native);
extern int   (ENC *vs_import_type)(int id);

extern int   (ENC  *vs_get_active_import_count)();
extern int   (ENC  *vs_get_active_import_id)(int index);
extern const char *(ENC *vs_get_active_import_keyword)(int index);
extern const char *(ENC *vs_get_active_import_units)(int index);
extern vs_import_option  (ENC *vs_get_active_import_mode)(int index);
extern vs_real (ENC *vs_get_active_import_value)(int index);
extern vs_real (ENC *vs_get_active_import_result)(int index, vs_real native);

extern void  (ENC *vs_install_symbolic_func) (char *name, void *func, int n_args);
extern int   (ENC *vs_install_keyword_alias)(char *existing, char *alias, int isDeprecated);
extern void  (ENC *vs_put_on_free_list) (void *thing);
extern void  (ENC* vs_post_proc_function) (const char *Message, int (*post_proc) (void* userData), void *userData);
extern void  (ENC *vs_pre_proc_function) (const char *Message, int (*pre_proc) (void* userData), void* userData);
extern void  (ENC *vs_set_stop_run) (vs_real stop_gt_0, const char *format, ...);
extern int   (ENC *vs_set_sym_attribute)(int id, vs_sym_attr_type type, const void *att);
extern int   (ENC *vs_set_sym_int)(int id, vs_sym_attr_type dataType, int value);
extern char *(ENC *vs_string_copy_internal)(char **target, char *source);
extern void  (ENC *vs_set_opt_error_dialog)(int value);

// working with simulation files (chapter 7)
extern int  (ENC *vs_add_new_par_group)(char *text);
extern int  (ENC *vs_insert_echo_blank_line)(void);
extern int  (ENC *vs_insert_echo_comment)(char *desc);
extern int  (ENC *vs_get_active_par_group)(void);
extern void (ENC *vs_set_active_par_group) (int i);

extern void (ENC *vs_read_next_line) (char *buffer, int n);
extern void (ENC *vs_write_f_to_echo_file) (char *key, vs_real , char *doc);
extern void (ENC *vs_write_header_to_echo_file) (char *buffer);
extern void (ENC *vs_write_i_to_echo_file) (char *key, int , char *doc);
extern void (ENC *vs_write_to_echo_file) (const char *format, ...);
extern void (ENC *vs_write_to_logfile) (int level, const char *format, ...);
extern void (ENC *vs_write_header_file)(const char* simfile);

// configurable table functions (chapter 7)
extern int     (ENC *vs_define_table)(char *root, int ntab, int ninst);
extern vs_real (ENC *vs_table_calc)(int index, vs_real xcol, vs_real x, int itab, int inst);
extern int     (ENC *vs_table_index)(char *name);
extern int     (ENC *vs_table_ninst)(int index);
extern int     (ENC *vs_table_ntab)(int index);
extern void    (ENC *vs_set_units_table)(const char* var_keyword, const char* units_keyword, const int which);

// moving objects and sensors (chapter 7)
extern int     (ENC *vs_define_moving_objects)(int n);
extern int     (ENC *vs_define_sensors)(int n);
extern void    (ENC *vs_free_sensors_and_objects)(void);

// saving and restoring the model state (chapter 8)
extern void    (ENC *vs_free_saved_states) (void);
extern int     (ENC *vs_get_request_to_restore)(void);
extern int     (ENC *vs_get_request_to_save)(void);
extern vs_real (ENC *vs_get_saved_state_time)(vs_real t);
extern vs_real (ENC *vs_restore_state)(void);
extern void    (ENC *vs_save_state) (void);
extern void    (ENC *vs_set_request_to_restore) (vs_real t);
extern void    (ENC *vs_start_save_timer) (vs_real t);
extern void    (ENC *vs_stop_save_timer) (void);

// managing arrays to support restarts (chapter 8)
extern void (ENC *vs_copy_all_state_vars_from_array) (vs_real *array);
extern void (ENC *vs_copy_all_state_vars_to_array) (vs_real *array);
extern void (ENC *vs_copy_differential_state_vars_from_array) (vs_real *array);
extern void (ENC *vs_copy_differential_state_vars_to_array) (vs_real *array);
extern void (ENC *vs_copy_extra_state_vars_from_array) (vs_real *array);
extern void (ENC *vs_copy_extra_state_vars_to_array) (vs_real *array);
extern int  (ENC *vs_get_export_names)(char **expNames);
extern int  (ENC *vs_get_import_names)(char **impNames);
extern void (ENC *vs_get_state_q) (vs_real *array);
extern int  (ENC* vs_n_active_derivatives)(void);
extern int  (ENC *vs_n_derivatives)(void);
extern int  (ENC *vs_n_extra_state_variables)(void);

// Licensing functions
extern void (ENC *vs_set_hpc_licensing_address) (const char *address);
extern void (ENC *vs_set_wait_for_parallel_licenses) (vs_bool value);

// undocumented functions to support legacy Sensor Ports in Simulink Gen-1
extern int (ENC *vs_get_n_export_sensor)(int *max_connections);
extern int (ENC *vs_get_sensor_connections)(vs_real *connect);

// Linearization
extern int (ENC* vs_linear_control)(char *buffer);
extern int (ENC* vs_linear_output)(char *buffer);
extern int (ENC* vs_linear_sv)(char *buffer);
extern void (ENC* vs_copy_output_vars_linearization)(vs_real *out);
extern void (ENC* vs_get_control_var_ptr_linearization)(vs_real **control, vs_real *prtb);

extern int  (ENC *vs_get_lat_pos_of_edge)(int edge, vs_real s, int opt_road, vs_real *l);
extern void (ENC *vs_scale_export_vars) (void);

// Alternative legacy functions (Appendix)
extern void (ENC *vs_add_echo_header) (const char *text);
extern void (ENC *vs_copy_import_vars) (vs_real *imports);
extern void (ENC *vs_copy_io) (vs_real *imports, vs_real *exports);
extern int  (ENC *vs_define_import)(char *keyword, char *desc, vs_real *real, char *);
extern int  (ENC *vs_define_indexed_parameter_array)(char *keyword);
extern int  (ENC *vs_define_output)(char *shortname, char *longname, vs_real *real, char *);
extern int  (ENC *vs_define_parameter)(char *keyword, char *desc, vs_real *, char *);
extern int  (ENC *vs_define_parameter_int)(char *keyword, char *desc, int *);
extern int   (ENC *vs_define_variable)(char *keyword, char *desc, vs_real *);
extern void (ENC *vs_free_all) (void);
extern vs_bool (ENC *vs_integrate_io_2)(vs_real t, vs_real *imports, vs_real *exports,
           void (*ext_calc) (vs_real, vs_ext_loc));
extern int   (ENC *vs_integrate_IO)(vs_real t, vs_real *imports, vs_real *exports);
extern void  (ENC *vs_opt_all_write) (int arg); // Jan 20, 2015
extern void  (ENC *vs_scale_import_vars) (void);
extern int   (ENC *vs_set_sym_real)(int id, vs_sym_attr_type dataType, vs_real value);
extern void  (ENC *vs_set_units) (char *var_keyword, char *units_keyword);
extern void  (ENC *vs_terminate) (vs_real t, void (*ext_echo) (vs_ext_loc));

// Legacy functions from get_api.c (Appendix)
int vs_get_api_basic(HMODULE dll, const char *dll_fname);
int vs_get_api_extend(HMODULE dll, const char *dll_fname);
int vs_get_api_install_external(HMODULE dll, const char *dll_fname);
int vs_get_api_road(HMODULE dll, const char *dll_fname);

// Deprecated functions (Appendix)
extern void (ENC *vs_install_calc_func) (char *name, void *func);
extern void (ENC *vs_setdef) (void(*ext_setdef) (void));
extern vs_real (ENC *vs_read)(const char *simfile,
           vs_bool (*ext_scan) (char *key, char *buffer));

#ifdef __cplusplus
}
#endif

#endif  // _vs_api_h_INCLUDED