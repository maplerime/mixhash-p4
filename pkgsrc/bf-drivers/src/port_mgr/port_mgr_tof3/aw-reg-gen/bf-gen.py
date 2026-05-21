#!/usr/bin/python

"""
bf-gen.py: Parse aw_vector_types.h and generate BF_ APIs

typedef int (*aw_pmd_cmn_r2l1_lsref_sel_set)(mss_access_t *mss, uint32_t sel);
typedef int (*aw_pmd_cmn_l2r_hsref_sel_set)(mss_access_t *mss, uint32_t sel);

"""

import sys
import textwrap
import re

global parts


if __name__ == "__main__":

  if len(sys.argv) < 2:
    print("python ./bf-gen.py <path-to-vector-file> [vfld]\n")
    exit()

  csr_file = str(sys.argv[1])
  if (len(sys.argv) == 3):
    guard_def = "BF_AW_VFLD_PMD_INCLUDED"
  else:
    guard_def = "BF_AW_PMD_INCLUDED"

  with open('bf_api.c',"w") as f_out_c:
    with open('bf_api.h',"w") as f_out_h:
      with open(csr_file) as f_in:
        f_out_h.write("#ifndef " + guard_def + "\n")
        f_out_h.write("#define " + guard_def + "\n")
        f_out_h.write("\n")
        f_out_h.write("#include <stdint.h>\n")
        f_out_h.write("#include <bf_types/bf_types.h>\n")
        f_out_h.write("\n")
        f_out_h.write("/* Allow the use in C++ code.  */\n")
        f_out_h.write("#ifdef __cplusplusa\n")
        f_out_h.write("extern \"C\" {\n")
        f_out_h.write("#endif\n")

        f_out_c.write("\n")
        f_out_c.write("#include <stdint.h>\n")
        f_out_c.write("#include <bf_types/bf_types.h>\n")
        f_out_c.write("#include \"aw_if.h\"\n")
        f_out_c.write("\n")

        end_of_types = False
        for line in f_in:
          ln = line.rstrip()
          # skip some lines
          if end_of_types:
              continue
          elif "end-of-types" in ln:
              end_of_types = True
              continue
          elif "typedef" not in ln:
              continue
          elif "//" in ln:
              continue
          l2 = ln.replace("typedef int (*aw_","")
          part = l2.split(")")
          api = part[0]
          args = part[1].replace("(","")

          extra_args = args.replace("mss_access_t *mss, ","")
          extra_args = extra_args.replace("mss_access_t *mss","")
       
          if len(extra_args) < 2:
            bf_args = "bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln"
          else:
            bf_args = "bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, " + extra_args

          if api == "pmd_uc_ucode_load":
            bf_args = "bf_dev_id_t dev_id, bf_dev_port_t dev_port, uint32_t ln, aw_ucode_t *ucode_arr, uint32_t size"
            c_args  = "&mss, ucode_arr, size"
          else:
            c_args  = args.replace("mss_access_t *","&")
          c_args  = c_args.replace("uint32_t *","")
          c_args  = c_args.replace("uint32_t*","")
          c_args  = c_args.replace("uint32_t ","")
          c_args  = c_args.replace("uint8_t *","")
          c_args  = c_args.replace("uint8_t ","")
          c_args  = c_args.replace("int32_t *","")
          c_args  = c_args.replace("int32_t ","")
          c_args  = c_args.replace("int ","")
          c_args  = c_args.replace("bool ","")
          c_args  = c_args.replace("char **","")
          c_args  = c_args.replace("char *","")
          c_args  = c_args.replace("char ","")
          c_args  = c_args.replace("float *","")
          c_args  = c_args.replace("float ","")
          c_args  = c_args.replace("double *","")
          c_args  = c_args.replace("double*","")
          c_args  = c_args.replace("double ","")

          # handle special case arguments
          c_args  = c_args.replace("aw_reg_defs_t **","")
          c_args  = c_args.replace("aw_reg_defs_t *","")
          c_args  = c_args.replace("aw_fld_defs_t **","")
          c_args  = c_args.replace("aw_refclk_term_mode_t *","")
          c_args  = c_args.replace("aw_refclk_term_mode_t ","")
          c_args  = c_args.replace("aw_acc_term_mode_t *","")
          c_args  = c_args.replace("aw_acc_term_mode_t ","")
          c_args  = c_args.replace("aw_force_sigdet_mode_t *","")
          c_args  = c_args.replace("aw_force_sigdet_mode_t ","")
          c_args  = c_args.replace("aw_txfir_config_t *","")
          c_args  = c_args.replace("aw_txfir_config_t ","")
          c_args  = c_args.replace("aw_dcdiq_data_t *","")
          c_args  = c_args.replace("aw_dcdiq_data_t ","")
          c_args  = c_args.replace("tx_hbridge_t *","")
          c_args  = c_args.replace("tx_hbridge_t ","")
          c_args  = c_args.replace("vga_opt_t *","")
          c_args  = c_args.replace("vga_opt_t ","")
          c_args  = c_args.replace("aw_adc_temp_data_t *","")
          c_args  = c_args.replace("aw_adc_temp_data_t ","")
          c_args  = c_args.replace("aw_adc_temp_calibration_t *","")
          c_args  = c_args.replace("aw_adc_temp_calibration_t ","")
          c_args  = c_args.replace("aw_adc_temp_method_t *","")
          c_args  = c_args.replace("aw_adc_temp_method_t ","")
          c_args  = c_args.replace("aw_version_t *","")
          c_args  = c_args.replace("aw_version_t ","")

          c_args  = c_args.replace("aw_an_spec_t *","")
          c_args  = c_args.replace("aw_an_spec_t ","")
          c_args  = c_args.replace("aw_analog_loopback_txfir_config_t *","")
          c_args  = c_args.replace("aw_analog_loopback_txfir_config_t ","")
          c_args  = c_args.replace("aw_rx_ffe_tap_count_t*","")
          c_args  = c_args.replace("aw_rx_ffe_tap_count_t *","")
          c_args  = c_args.replace("aw_rx_ffe_tap_count_t ","")
          c_args  = c_args.replace("aw_rx_roaming_mode_t *","")
          c_args  = c_args.replace("aw_rx_roaming_mode_t ","")
          c_args  = c_args.replace("aw_eq_type_t *","")
          c_args  = c_args.replace("aw_eq_type_t ","")
          c_args  = c_args.replace("aw_pmd_tracebuffer_mode_t *","")
          c_args  = c_args.replace("aw_pmd_tracebuffer_mode_t ","")

          c_args  = c_args.replace("aw_afe_data_t *","")
          c_args  = c_args.replace("uint64_t user_defined_pattern *","user_defined_pattern")
          c_args  = c_args.replace("uint64_t user_defined_pattern ","user_defined_pattern")
          c_args  = c_args.replace("aw_bist_pattern_t *","")
          c_args  = c_args.replace("aw_bist_pattern_t ","")
          c_args  = c_args.replace("aw_bist_mode_t *","")
          c_args  = c_args.replace("aw_bist_mode_t ","")
          c_args  = c_args.replace("aw_uc_diag_regs_t *","")
          c_args  = c_args.replace("int branch","branch")
          c_args  = c_args.replace("int tolerance","tolerance")
          c_args  = c_args.replace("uint64_t *","")
          c_args  = c_args.replace("uint64_t ","")
          c_args  = c_args.replace("double *","")
          c_args  = c_args.replace("double ","")
          c_args  = c_args.replace("[]","")
          c_args  = c_args.replace(", mss_access_t *mss","")
          c_args  = c_args.replace("int32_t ","")
          c_args  = c_args.replace("aw_cmn_pstate_t ","")
          c_args  = c_args.replace("aw_pstate_t ","")

          trace_args = c_args.replace("&mss,","")
          arg_list   = trace_args.split(",")
          arg_str    = ""
          get_arg_str= ""
          arg_cnt    = 0
          for arg in arg_list:
            if arg == " txfir_cfg":
              if "_set" in api:
                arg_str = arg_str + ",\n              \"cm3\", txfir_cfg.CM3,\n              \"cm2\", txfir_cfg.CM2,\n              \"cm1\", txfir_cfg.CM1,\n             \"c0\", txfir_cfg.C0,\n              \"c1\", txfir_cfg.C1,\n              \"main_or_max\", txfir_cfg.main_or_max"
              else:
                arg_str = arg_str + ",\n              \"cm3\", txfir_cfg->CM3,\n              \"cm2\", txfir_cfg->CM2,\n              \"cm1\", txfir_cfg->CM1,\n             \"c0\", txfir_cfg->C0,\n              \"c1\", txfir_cfg->C1,\n              \"main_or_max\", txfir_cfg->main_or_max"
                get_arg_str = arg_str
              arg_cnt += 6
            elif arg == " nes_txfir_cfg":
              if "_set" in api:
                arg_str = arg_str + ",\n              \"nes_post1\", nes_txfir_cfg.nes_post1,\n              \"nes_c0\", nes_txfir_cfg.nes_c0"
              else:
                arg_str = arg_str + ",\n              \"nes_post1\", nes_txfir_cfg->nes_post1,\n              \"nes_c0\", nes_txfir_cfg->nes_c0"
                get_arg_str = arg_str
              arg_cnt += 2
            else:
              if "[" in arg: # array
                arg = re.sub("\[.*\]", "", arg)
                c_args = re.sub("\[.*\]", "", c_args)

              arg_str = arg_str + ",\n              \"" + str(arg) + "\", (uint32_t)((intptr_t)" + arg + " & 0xffffffff)"
              if "_get" in api:
                stripped_arg = arg.replace(" ","")
                #if "tx_ppm" in arg:
                #  print("arg=" + arg + " extra_args=" + extra_args + " search=" + ("*" + stripped_arg) + " or " + ("* " + stripped_arg))

                if ("*" + stripped_arg) in extra_args or ("* " + stripped_arg) in extra_args:
                  get_arg_str = get_arg_str + ",\n              \"" + str(arg) + "\", *(uint32_t*)(intptr_t)((intptr_t)(" + arg + ") & UINTMAX_MAX)"
                else:
                  get_arg_str = arg_str
              arg_cnt +=1

          if "tx" in api:
            section = "MSS_SECTION_TX"
          elif "rx" in api:
            section = "MSS_SECTION_RX"
          elif "cmn" in api:
            section = "MSS_SECTION_CMN"
          else:
            section = "MSS_SECTION_RX"

          f_out_c.write("/***********************************************************************\n")
          f_out_c.write("*           bf_aw_" + api + "\n")
          f_out_c.write("***********************************************************************/\n")          
          f_out_h.write("bf_status_t bf_aw_" + api + "(" + bf_args + ");\n")
          f_out_c.write("bf_status_t bf_aw_" + api + "(" + bf_args + ") {\n")
          f_out_c.write("  mss_access_t mss;\n")
          f_out_c.write("  int rc;\n")
          f_out_c.write("  bf_tf3_sd_t *tf3_sd = bf_tof3_serdes_addr_set(dev_id, dev_port, ln, &mss, " + section + ");\n")
          f_out_c.write("\n")

          if "_set" in api:
            f_out_c.write("  bf_aw_trace(\"" + str(api) + "\", dev_id, dev_port, ln, " + str(arg_cnt)  + arg_str + ");\n")

          f_out_c.write("\n")
          f_out_c.write("  // Call thru API vector\n")
          f_out_c.write("  rc = tf3_sd->api->" + api + "(" + c_args + ");\n")
          f_out_c.write("\n")

          if "_get" in api:
            f_out_c.write("  bf_aw_trace(\"" + str(api) + "\", dev_id, dev_port, ln, " + str(arg_cnt)  + get_arg_str + ");\n")
            f_out_c.write("\n")

          f_out_c.write("  // map AW error codes to bf_types_t error codes\n")
          f_out_c.write("  return map_aw_err_to_bf_err(rc);\n")

          f_out_c.write("}\n\n")

        f_out_h.write("#ifdef __cplusplus\n")
        f_out_h.write("}\n")
        f_out_h.write("#endif /* C++ */\n")

        f_out_h.write("#endif // " + guard_def + "\n")
