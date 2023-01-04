// VS API road and path functions in a VS DLL that uses VS Paths

#ifndef _vs_api_road_h_INCLUDED
#define _vs_api_road_h_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#include "vs_deftypes.h"

// ENC = External Naming Convention
#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#define ENC __cdecl
#else
#define HMODULE void*
#define ENC
#endif

// 2D reference path properties
void (ENC *vs_get_road_start_stop) (vs_real *start, vs_real *stop);
vs_real (ENC *vs_path_curv_i)(vs_real s, vs_real inst);
vs_real (ENC *vs_road_curv_i)(vs_real s, vs_real inst);
vs_real (ENC *vs_path_l_i)(vs_real x, vs_real y, vs_real inst);
vs_real (ENC *vs_road_l)(vs_real x, vs_real y);
vs_real (ENC *vs_road_l_i)(vs_real x, vs_real y, vs_real inst);
vs_real (ENC *vs_path_s_i)(vs_real x, vs_real y, vs_real inst);
vs_real (ENC *vs_road_s)(vs_real x, vs_real y);
vs_real (ENC *vs_road_s_i)(vs_real x, vs_real y, vs_real inst);
vs_real (ENC *vs_path_x_i)(vs_real sy, vs_real inst);
vs_real (ENC *vs_road_x)(vs_real s);
vs_real (ENC *vs_road_x_i)(vs_real sy, vs_real inst);
vs_real (ENC *vs_path_x_sl_i)(vs_real s, vs_real l, vs_real inst);
vs_real (ENC *vs_road_x_sl_i)(vs_real s, vs_real l, vs_real inst);
vs_real (ENC *vs_path_y_i)(vs_real s, vs_real inst);
vs_real (ENC *vs_road_y)(vs_real s);
vs_real (ENC *vs_road_y_i)(vs_real sy, vs_real inst);
vs_real (ENC *vs_path_y_sl_i)(vs_real s, vs_real l, vs_real inst);
vs_real (ENC *vs_road_y_sl_i)(vs_real s, vs_real l, vs_real inst);
vs_real (ENC *vs_path_yaw_i)(vs_real sta, vs_real directiony, vs_real inst);
vs_real (ENC *vs_road_yaw)(vs_real sta, vs_real direction);
vs_real (ENC *vs_road_yaw_i)(vs_real sta, vs_real directiony, vs_real inst);
vs_real (ENC *vs_s_loop)(vs_real s);
vs_real (ENC *vs_s_loop_i)(vs_real s, int i);
vs_real (ENC *vs_s_path_loop)(vs_real s);
vs_real (ENC *vs_target_heading)(vs_real s);
vs_real (ENC *vs_target_l)(vs_real s);

void (ENC *vs_get_path_xy)(vs_real s, vs_real l, vs_real * x, vs_real * y, int inst);
void (ENC *vs_get_road_xy)(vs_real s, vs_real l, vs_real * x, vs_real * y, int inst);


// 3D properties of current road
void (ENC *vs_get_dzds_dzdl) (vs_real s, vs_real l, vs_real *dzds, vs_real *dzdl);
void (ENC *vs_get_dzds_dzdl_i) (vs_real s, vs_real l, vs_real *dzds,
        vs_real *dzdl, vs_real inst);
void (ENC *vs_get_road_contact) (vs_real y, vs_real x, int inst, vs_real *z,
        vs_real *dzdy, vs_real *dzdx, vs_real *mu);
void (ENC *vs_get_road_contact_sl) (vs_real s, vs_real l, int inst, vs_real *z,
        vs_real *dzds, vs_real *dzdl, vs_real *mu);
void (ENC *vs_get_road_xyz) (vs_real s, vs_real l, vs_real *x, vs_real *y, vs_real *z);
vs_real (ENC *vs_road_pitch_sl_i)(vs_real s, vs_real l, vs_real yaw, vs_real inst);
vs_real (ENC *vs_road_roll_sl_i)(vs_real s, vs_real l, vs_real yaw, vs_real inst);
vs_real (ENC *vs_road_z)(vs_real x, vs_real y);
vs_real (ENC *vs_road_z_2)(vs_real x, vs_real y, vs_real z_in);
vs_real (ENC *vs_road_z_i)(vs_real x, vs_real yy, vs_real inst);
vs_real (ENC *vs_road_z_sl_i)(vs_real s, vs_real l, vs_real inst);

// Functions the make use of user ID for paths
vs_real (ENC *vs_path_sstart_id)(vs_real user_id);
vs_real (ENC *vs_path_sstop_id)(vs_real user_id);
vs_real (ENC *vs_path_length_id)(vs_real user_id);
vs_real (ENC *vs_path_is_looped_id)(vs_real user_id);
vs_real (ENC *vs_path_s_id)(vs_real x, vs_real y, vs_real user_id, vs_real inst);
vs_real (ENC *vs_path_l_id)(vs_real x, vs_real y, vs_real user_id, vs_real inst);
vs_real (ENC *vs_path_x_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_path_y_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_path_dxds_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_path_dyds_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_path_dxdl_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_path_dydl_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_path_curv_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_path_yaw_id)(vs_real s, vs_real path_id, vs_real ltarg_id, vs_real inst);

// Functions that make use of user ID for roads
vs_real (ENC *vs_road_sstart_id)(vs_real user_id);
vs_real (ENC *vs_road_sstop_id)(vs_real user_id);
vs_real (ENC *vs_road_length_id)(vs_real user_id);
vs_real (ENC *vs_road_is_looped_id)(vs_real user_id);
vs_real (ENC *vs_road_s_id)(vs_real x, vs_real y, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_l_id)(vs_real x, vs_real y, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_x_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_y_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_dxds_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_dyds_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_dxdl_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_dydl_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_curv_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_z_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_dzds_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);
vs_real (ENC *vs_road_dzdl_id)(vs_real s, vs_real l, vs_real user_id, vs_real inst);

vs_real (ENC *vs_road_read)(const char* runSimfile);
vs_real (ENC *vs_road_generate_path_shapes)(int targetPathIndex);
vs_real (ENC *vs_road_generate_road_shapes)(int targetPathIndex);
vs_real (ENC *vs_road_write_animator_par)();
void    (ENC* vs_copy_inst_from_array)(void *road, void *driver);
void    (ENC* vs_copy_inst_to_array)(void **road, void **driver);

// User defined road terrain
void    (ENC *vs_road_install_terrain_function)(terrain_func_t userFunction, void *userData);

#ifdef __cplusplus
}
#endif


#endif  // _vs_api_road_h_INCLUDED

