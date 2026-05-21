#ifndef _PIPE_MGR_ALPM_HA_H_
#define _PIPE_MGR_ALPM_HA_H_

pipe_status_t pipe_mgr_alpm_hlp_restore_state(bf_dev_id_t dev_id,
                                              pipe_mat_tbl_hdl_t tbl_hdl);

pipe_status_t pipe_mgr_alpm_hlp_entry_restore(bf_dev_id_t dev_id,
                                              pipe_mat_tbl_hdl_t ll_tbl_hdl,
                                              pipe_mgr_move_list_t *move_node);

pipe_status_t pipe_mgr_alpm_hlp_compute_delta_changes(
    bf_dev_id_t dev_id,
    pipe_mat_tbl_hdl_t mat_tbl_hdl,
    pipe_mgr_move_list_t **move_head_p);

void pipe_mgr_alpm_cleanup_hlp_ha_state(bf_dev_id_t device_id,
                                        pipe_mat_tbl_hdl_t mat_tbl_hdl);

pipe_status_t pipe_mgr_alpm_get_ha_reconc_report(
    dev_target_t dev_tgt,
    pipe_mat_tbl_hdl_t mat_tbl_hdl,
    pipe_tbl_ha_reconc_report_t *ha_report);
#endif
