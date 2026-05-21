/* clang-format off */

bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_rx_fcs_remove_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_preamble_bcp1_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_mac_tx_playout_margin_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_tx_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_sreset_rx_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_len_ctrl_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_cnt_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_stomp_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fcs_swap_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ptp_1step_chksum_zero_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_check_lt_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_flush_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_am_comp_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_ifg_dic_mode_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_max_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_pkt_len_min_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_paf_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_tx_fifo_pae_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_rx_fifo_ff_disable_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_len_ctrl_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_check_lt_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_max_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_sys_mac_rx_pkt_len_min_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_static_pause_rx_mac_addr_addr_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_pfc_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_pause_rx_ctrl_port_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_cp_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_cp_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_cp_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_dp_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_dp_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_txfifo_depth_dp_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxrunt_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_dest_chk_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_vlan_chk_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxpfc_fabric_hdr_length_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxsopeop_max_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_rxfilt_rxjabpkt_len_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_en_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_txfilt_txjabpkt_len_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata0_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_wdata1_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_req_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_write_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_ctrl_addr_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata0_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rdata1_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rerr_data_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rerr_data_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_statmem_csr_rerr_data_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_stat_clear_req_chan_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_stat_clear_req_chan_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_stat_clear_req_chan_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cfg_mac_lpbk_rx2tx_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count3_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count3_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count3_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count2_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count2_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count2_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count1_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count1_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count1_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count0_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count0_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_stat_count0_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t *data_p, uint32_t val);
    
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_out_cts_set(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint64_t *data_p, uint64_t val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_out_cts_get(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint64_t *data_p, uint64_t *val, bool hw);
bf_status_t bf_ll_tof3_eth400g_sys_rspec_cts_fifo_out_cts_rmw(bf_dev_id_t dev_id, bf_subdev_id_t subdev_id, uint32_t tmac, uint32_t ch, uint64_t *data_p, uint64_t val);
    
