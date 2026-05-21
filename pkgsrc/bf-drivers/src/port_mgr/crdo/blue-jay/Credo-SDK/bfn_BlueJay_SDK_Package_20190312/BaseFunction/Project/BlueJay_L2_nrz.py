import BaseFunction.IP.IP_BlueJay_L2_nrz as BlueJay_l2
from regfunc import inject_func
import credo

def load_basefunction_nrz(chip):
    inject_func(chip.NRZ25, BlueJay_l2.tx_prbs_en_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.tx_prbs_mode_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.tx_taps)
    inject_func(chip.PAM50, BlueJay_l2.tx_taps)
    inject_func(chip.NRZ25, BlueJay_l2.tx_pol_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.tx_pol_flip_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.tx_test_patt_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.dac_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.ready_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.rx_checker_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.rx_prbs_mode_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.lane_reset)
    inject_func(chip.NRZ25, BlueJay_l2.lr_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.sm_cont_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.rx_pol_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.rx_pol_flip_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.bp1)
    inject_func(chip.NRZ25, BlueJay_l2.bp2)
    inject_func(chip.NRZ25, BlueJay_l2.ctle_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.eye_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.ctle_map_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.dc_gain)
    inject_func(chip.PAM50, BlueJay_l2.dc_gain)
    inject_func(chip.NRZ25, BlueJay_l2.delta_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.state_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.dfe_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.get_err_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.prbs_rst_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.ber_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.bb_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.edge)
    inject_func(chip.PAM50, BlueJay_l2.edge)
    inject_func(chip.NRZ25, BlueJay_l2.skef)
    inject_func(chip.PAM50, BlueJay_l2.skef)
    inject_func(chip.NRZ25, BlueJay_l2.agcgain)
    inject_func(chip.PAM50, BlueJay_l2.agcgain)
    inject_func(chip.NRZ25, BlueJay_l2.set_tx_lane)
    inject_func(chip.NRZ25, BlueJay_l2.set_rx_lane)
    inject_func(chip.NRZ25, BlueJay_l2.dfe_nrz)
    inject_func(chip.PAM50, BlueJay_l2.gc)
    inject_func(chip.PAM50, BlueJay_l2.pc)
    inject_func(chip.PAM50, BlueJay_l2.msblsb)
    inject_func(chip.LANE, BlueJay_l2.get_lane_pll)
    #inject_func(chip.PAM50, BlueJay_l2.get_lane_pll)
    inject_func(chip.NRZ25, BlueJay_l2.err_inject_nrz)
    inject_func(chip.NRZ25, BlueJay_l2.tx_sj_ampl)
    inject_func(chip.NRZ25, BlueJay_l2.tx_sj_freq)
    inject_func(chip.NRZ25, BlueJay_l2.tx_sj_en)
    inject_func(chip.NRZ25, BlueJay_l2.tx_sj)
    inject_func(chip.NRZ25, BlueJay_l2.tx_sj_range)
    inject_func(chip.PAM50, BlueJay_l2.tx_sj_ampl)
    inject_func(chip.PAM50, BlueJay_l2.tx_sj_freq)
    inject_func(chip.PAM50, BlueJay_l2.tx_sj_en)
    inject_func(chip.PAM50, BlueJay_l2.tx_sj)
    inject_func(chip.PAM50, BlueJay_l2.tx_sj_range)


def reload_L2():
    reload(BlueJay_l2)








