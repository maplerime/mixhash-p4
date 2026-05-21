/*
** ? 2020 Alphawave IP Inc.
*/

#ifndef AW_ALPHACORE_CUSTOM_H
#define AW_ALPHACORE_CUSTOM_H

#include <stdint.h>

#include "aw_driver_sim.h"
#include "aw_alphacore_csr_defines.h"
#include "aw_alphacore_ip_defines.h"

typedef struct vga_opt_s {
    /**
     *        'en': VGA cap adaptation enable  
     *              0 - VGA cap adapt Disabled
     *              1 - VGA cap adapt Enabled  
     */
    uint32_t en; 
    /**
     *        'vga_cap': VGA cap code ranges from 0-3  
     */
    uint32_t vga_cap;
    /**
     *        'use_custom_takeover_ratio': 0 - Use the custom takeover ratio, as specified by 'takeover ratio'
     *                                     1 - Use takeover ratio hardcoded in FW   
     */
    uint32_t use_custom_takeover_ratio;
    /**
     *        'custom_takeover_ratio': Custom takeover ratio converted to a format that FW accepts                                   
     */
    uint32_t custom_takeover_ratio;
    /**
     *        'custom_nyq_mask': Unsigned 8-bit integer whose bits [7:0] represent whether the nyquist energies for [1p00..p125] will be used  
     */
    uint32_t custom_nyq_mask;

} vga_opt_t;

/**Readback vga cap adapt settings. 
*  
* Args:  
*     `mss`: Object containing driver methods.  
*     `lane`: lane number  
*     `opts`:   
*             'en':  
*                0 - vga_cap adapt disabled  
*                1 - vga_cap adapt enabled  
*             'vga_cap': Forced vga_cap code when 'en' is 0. Ranges from 0-3.  
*             'use_custom_takeover_ratio':  
*                0 - use the custom takeover ratio, as specified by 'takeover_ratio'  
*                1 - use takeover ratio hardcoded in firmware  
*              'custom_takeover_ratio': custom takeover ratio converted to format that FW accepts  
*              'custom_nyq_mask': an unsigned 8-bit integer whose bits [7:0] represent whether the nyquist energies for [1p00 .. p125] respectively will be used  
*            
*    
* Return:  
*     Null
*
* Constraints:
*     Null
*
* Typical Application Usage:
*     Used to readback VGA cap adapt settings used by link training. 
**/
int aw_pmd_rx_vga_cap_adapt_get(mss_access_t *mss, vga_opt_t *opts);

/**Sets values used for vga_cap adaptation. If vga_cap adapt is disabled, vga_cap code will default to opts['vga_cap']. 
*    If vga_cap adapt is enabled, initial vga_cap value will default to 0. 
*  
* Args:  
*     `mss`: Object containing driver methods.  
*     `lane`: lane number  
*     `opts`:   
*             'en':  
*                0 - vga_cap adapt disabled  
*                1 - vga_cap adapt enabled  
*             'vga_cap': Forced vga_cap code when 'en' is 0. Ranges from 0-3.  
*             'use_custom_takeover_ratio':  
*                0 - use the custom takeover ratio, as specified by 'takeover_ratio'  
*                1 - use takeover ratio hardcoded in firmware  
*             'custom_takeover_ratio': custom takeover ratio converted to format that FW accepts  
*             'custom_nyq_mask': an unsigned 8-bit integer whose bits [7:0] represent whether the nyquist energies for [1p00 .. p125] respectively will be used  
*            
*    
* Return:  
*     Null
*
* Constraints:
*     Power State: Don't care.
*     Rate State: Don't care.  
*     Link Training State: Before EqEval.   
*     Effect: Takes effect immediately. 
*
* Typical Application Usage:
*     vga_cap adaptation is usually enabled in link training.  
**/
int aw_pmd_rx_vga_cap_adapt_set(mss_access_t *mss, vga_opt_t opts);

/**Digital signal detector threshold controls
 *       
 * Args:
 *    `mss`: Object containing driver methods.  
 *    'adc_valid_thres_nt': Digital signal detect valid threshold
 *                 Default 3200 , programable range 0 - 65535 
 *    'adc_invalid_thres_nt': Digital signal detect invalid threshold<
 *                 Default 2800 , programable range 0 - 65535 
 *
 * Return: 
 *     Null  
 *
 * Constraints:  
 *     Power State:  Before or during P0.  
 *     Rate State:   After desired rate change.  
 *     EqEval State: Before eqeval.  
 *     Effect:       Takes effect immediately.  
 *
 * Typical Application Usage:
 *     Changes the threshold setting of the digital signal detection block (Lock protection) 
**/
int aw_pmd_rx_dig_pwr_det_threshold_set(mss_access_t *mss, uint32_t adc_valid_thresh_nt, uint32_t adc_invalid_thresh_nt);

/**Readback forced signal detect value.
 *       
 * Args:
 *    `mss`: Object containing driver methods.  
 *    'adc_valid_thres_nt': Digital signal detect valid threshold
 *                 Default 3200 , programable range 0 - 65535 
 *     'adc_invalid_thres_nt': Digital signal detect invalid threshold<
 *                 Default 2800 , programable range 0 - 65535 
 * Return: 
 *     Null  
 *
 * Constraints:  
 *    Null 
 *
 *  Typical Application Usage:
 *    Used to readback signal detect status.
**/
int aw_pmd_rx_dig_pwr_det_threshold_get(mss_access_t *mss, uint32_t *adc_valid_thresh_nt, uint32_t *adc_invalid_thresh_nt);


/**
 * Valid/Invalid Type setting.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'valid_type' : Valid entry condition type. 0 - AND, 1 - OR
 *    'invalid_type' : Invalid entry condition type. 0 - AND, 1 - OR
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *    
 *
 * Typical Application Usage:
 *   Setting AND/OR as entry/exit condition type.
 *    
 **/
int aw_pmd_rx_signal_detect_fsm_type_set(mss_access_t *mss, uint32_t valid_type, uint32_t invalid_type);

/**
 * Valid/Invalid Type Readback.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'valid_type' : Valid entry condition type. 0 - AND, 1 - OR
 *    'invalid_type' : Invalid entry condition type. 0 - AND, 1 - OR
 *
 * Return:
 *    Null
 *
 * Constraints:
 *    Null
 *    
 *
 * Typical Application Usage:
 *   Readback AND/OR as entry/exit condition type.
 *    
 **/
int aw_pmd_rx_signal_detect_fsm_type_get(mss_access_t *mss, uint32_t *valid_type, uint32_t *invalid_type);

/**
 * Write Valid entry/exit Control setting for MUX to RX detect FSM.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'valid' : Valid entry condition control enable. Each bit, when asserted, enables a control for entry into valid state
 *      Bit 0 - AFE signal detect
 *      Bit 1 - Filtered ADC data
 *      Bit 2 - Rxdisable port
 *    'invalid' : Valid exit condition control enable. Each bit, when asserted, enables a control for exit from valid state
 *      Bit 0 - AFE signal detect
 *      Bit 1 - Filtered ADC data
 *      Bit 2 - Rxdisable port
 *      Bit 3 - CDR lock (level)
 *      Bit 4 - CDR lock (negedge)
 *      Bit 5 ? Transition detector
 *    
 * Return:
 *    Null

 * Constraints:
 *    Null
 *    
 *
 * Typical Application Usage:
 *    Setting valid entry/exit condition.Each bit when asserted enables control for entry into/exit from valid state.
 *    
 **/
int aw_pmd_rx_signal_detect_fsm_ctrl_set(mss_access_t *mss, uint32_t valid, uint32_t invalid);

/**
 * Readback Valid entry/exit Control setting for MUX to RX detect FSM.
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'valid' : Valid entry condition control enable. Each bit, when asserted, enables a control for entry into valid state
 *      Bit 0 - AFE signal detect
 *      Bit 1 - Filtered ADC data
 *      Bit 2 - Rxdisable port
 *    'invalid' : Valid exit condition control enable. Each bit, when asserted, enables a control for exit from valid state
 *      Bit 0 - AFE signal detect
 *      Bit 1 - Filtered ADC data
 *      Bit 2 - Rxdisable port
 *      Bit 3 - CDR lock (level)
 *      Bit 4 - CDR lock (negedge)
 *      Bit 5 ? Transition detector
 *    
 * Return:
 *    Null

 * Constraints:
 *    Null
 *    
 *
 * Typical Application Usage:
 *    Readback valid entry/exit condition. Each bit when asserted enables control for entry into/exit from valid state.
 *    
 **/
int aw_pmd_rx_signal_detect_fsm_ctrl_get(mss_access_t *mss, uint32_t *valid, uint32_t *invalid);

/**
 * Select which signal to use for external odat_rx_signal_detect_ln[#]_a signal.
 * For more information on this signal refer to datasheet section 3.6
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'sel' : Selects which signal to use for external odat_rx_signal_detect_ln[#]_a signal.
 *             0 - AFE signal detect
 *             1 - Digital signal detect
 *             2 - Data valid
 *             3 - CDR adapt start
 *             4 ? Filtered ADC data
 *    
 * Return:
 *    Null

 * Constraints:
 *    Null
 *    
 *
 * Typical Application Usage:
 *    Select which signal to use for external odat_rx_signal_detect_ln[#]_a signal used by upper level logic.    
 **/
int aw_pmd_rx_signal_detect_valid_pcs_sel_set(mss_access_t *mss, uint32_t sel);

/**
 * Readback which signal to use for external odat_rx_signal_detect_ln[#]_a signal.
 * For more information on this signal refer to datasheet section 3.6
 *
 * Args:
 *    'mss': Object containing driver methods.
 *    'sel' : Selects which signal to use for external odat_rx_signal_detect_ln[#]_a signal.
 *             0 - AFE signal detect
 *             1 - Digital signal detect
 *             2 - Data valid
 *             3 - CDR adapt start
 *             4 ? Filtered ADC data
 *    
 * Return:
 *    Null

 * Constraints:
 *    Null
 *    
 *
 * Typical Application Usage:
 *   Readback which signal to use for external odat_rx_signal_detect_ln[#]_a signal used by upper level logic.
 *    
 **/
int aw_pmd_rx_signal_detect_valid_pcs_sel_get(mss_access_t *mss, uint32_t *sel);

#endif
