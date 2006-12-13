//=============================================================================================
// FlexComp.h  -  Compatability Function definitions.
//
// The functions in this header file are for compatability only. The right is reserved
// to remove them in the future.  It is recommended that only functions defined in
// Flexmotn.h be used.
//
// Copyright 2000
// National Instruments Corporation.
// All rights reserved.
//=============================================================================================

#ifndef ___FlexComp_h___
#define ___FlexComp_h___


#ifndef ___FlexMotn_h___
   #include "flexmotn.h"
#endif

#ifdef __cplusplus
	extern "C"{
#endif

//ENB_ENC_CMD, Enable the encoders
FLEXFUNC flex_enable_encs(BOARD, u8 device, u16 encmap);

//Load Circular Arc
FLEXFUNC flex_load_2d_arc(BOARD, u8 device, u32 radius, i32 startangle,
							i32 travelangle, u8 inpvect);

//Load Spherical Arc
FLEXFUNC flex_load_3d_arc(BOARD, u8 device, u32 radius, u32 planepitch,
				i32 planeyaw, i32 startangle, i32 travelangle, u8 inpvect);

//Load Helical Arc
FLEXFUNC flex_load_helix(BOARD, u8 device, u32 radius, i32 startangle,
					i32 travelangle, i32 lineartravel, u8 inpvect);

//ENB_GEAR_CMD, Enable Gearing
FLEXFUNC flex_enable_gear(BOARD, u8 device, u16 axismap);

//ACQ_SAMPLES_CMD, Acquire Samples Command
FLEXFUNC flex_acq_traj_data(BOARD, u8 device, u16 axismap, u16 numsamps, u16 timeperiod);

//READ_SAMPLE_CMD, Read Sample From Acquire Samples to RDB
FLEXFUNC flex_read_traj_data(BOARD, u8 device, u8 retvect);

//READ_SAMPLE_CMD, Read Sample From Acquire Samples
FLEXFUNC flex_read_traj_data_rtn(BOARD, u8 device, long FAR *retdata);

//OBSOLETE
//READ_RUNST_CMD, Read Run/Stop Status
FLEXFUNC flex_read_rs_status(BOARD, u8 device, u8 retvect);

//OBSOLETE
//READ_RUNST_CMD, Read Run/Stop Status
FLEXFUNC flex_read_rs_status_rtn(BOARD, u8 device, u16 FAR *status);

//READ_MOFF_CMD, Read Motor Off Status
FLEXFUNC flex_read_motoff_status(BOARD, u8 device, u8 retvect);

//READ_MOFF_CMD, Read Motor Off Status
FLEXFUNC flex_read_motoff_status_rtn(BOARD, u8 device, u16 FAR *status);

//OBSOLETE 
//READ_DACLIM_CMD, Read DAC Limit Status
FLEXFUNC flex_read_dac_status(BOARD, u8 device, u8 retvect);

//OBSOLETE
//READ_DACLIM_CMD, Read DAC Limit Status
FLEXFUNC flex_read_dac_status_rtn(BOARD, u8 device, u16 FAR *status);


//Load all Control Loop Parameters
FLEXFUNC flex_load_loop_params(BOARD, u8 axis, PID FAR *PIDValues, u8 inputVector);

//Load Proportional Gain
FLEXFUNC flex_load_kp(BOARD, u8 axis, u16 kp, u8 inpvect);

//Load Integral Gain
FLEXFUNC flex_load_ki(BOARD, u8 axis, u16 ki, u8 inpvect);

//Load Integration Limit
FLEXFUNC flex_load_ilim(BOARD, u8 axis, u16 ilim, u8 inpvect);

//Load Derivative Gain
FLEXFUNC flex_load_kd(BOARD, u8 axis, u16 kd, u8 inpvect);

//Load Derivative Sample Period Multiplier
FLEXFUNC flex_load_td(BOARD, u8 axis, u16 td, u8 inpvect);

//Load Velocity Feedback Gain
FLEXFUNC flex_load_kv(BOARD, u8 axis, u16 kv, u8 inpvect);

//Load Acceleration Feedforward Gain
FLEXFUNC flex_load_aff(BOARD, u8 axis, u16 aff, u8 inpvect);

//Load Velocity Feedforward Gain
FLEXFUNC flex_load_vff(BOARD, u8 axis, u16 vff, u8 inpvect);

//Load Acceleration
FLEXFUNC flex_load_accel(BOARD, u8 axisorvectorspace, u32 accel, u8 inpvect);

//Load Deceleration
FLEXFUNC flex_load_decel(BOARD, u8 axisorvectorspace, u32 decel, u8 inpvect);

//Load Acceleration in RPS/sec
FLEXFUNC flex_load_accel_rpsps(BOARD, u8 axisorvectorspace, f64 rpsps, u8 inpvect);

//Load Deceleration in RPS/sec
FLEXFUNC flex_load_decel_rpsps(BOARD, u8 axisorvectorspace, f64 rpsps, u8 inpvect);

//Load Velocity Override
FLEXFUNC flex_load_vel_override(BOARD, u8 axisorvectorspace, u16 overridescale, u8 inpvect);

//Stop Motion
FLEXFUNC flex_stop(BOARD, u8 axisorvectorspace, u16 axismap);

//Halt Motion
FLEXFUNC flex_halt(BOARD, u8 axisorvectorspace, u16 axismap);

//Kill Motion
FLEXFUNC flex_kill(BOARD, u8 axisorvectorspace, u16 axismap);

//Enable Limit Inputs
FLEXFUNC flex_enable_lims(BOARD, u8 device, u16 limmap);

//Enable Software Limits
FLEXFUNC flex_enable_sw_lims(BOARD, u8 device, u16 swlimmap);

//Read Limit Input Status
FLEXFUNC flex_read_lim_status(BOARD, u8 device, u8 retvect);
//Read Limit Input Status
FLEXFUNC flex_read_lim_status_rtn(BOARD, u8 device, u16 FAR *limstatus);

//Read Software Limit Status
FLEXFUNC flex_read_sw_lim_status(BOARD, u8 device, u8 retvect);
//Read Software Limit Status
FLEXFUNC flex_read_sw_lim_status_rtn(BOARD, u8 device, u16 FAR *swlimstatus);

//Read Position Breakpoint Status
FLEXFUNC flex_read_pos_bp_status(BOARD, u8 device, u8 retvect);
//Read Position Breakpoint Status
FLEXFUNC flex_read_pos_bp_status_rtn(BOARD, u8 device, u16 FAR *bpstatus);

//Read Anticipation Time Breakpoint Status
FLEXFUNC flex_read_at_bp_status(BOARD, u8 device, u8 retvect);
//Read Anticipation Time Breakpoint Status
FLEXFUNC flex_read_at_bp_status_rtn(BOARD, u8 device, u16 FAR *status);

//Save Memory Object
FLEXFUNC flex_save_object(BOARD, u8 device);

//Delete Memory Object
FLEXFUNC flex_delete_object(BOARD, u8 device);

//Free Memory Object
FLEXFUNC flex_free_object(BOARD, u8 device);

//Wait on Condition
FLEXFUNC flex_wait(BOARD, u8 device, u16 cc, u8 muston, u8 mustoff, u16 timeout, u8 retvect);

//Wait on Condition
FLEXFUNC flex_wait_or(BOARD, u8 device, u16 cc, u8 muston, u8 mustoff, u16 timeout, u8 retvect);

//Jump to Label on Condition
FLEXFUNC flex_jump_label(BOARD, u8 resource, u16 condition, u8 mustOn, u8 mustOff, u16 labelNumber);

//Read Boot Version Number
FLEXFUNC flex_read_boot_ver_rtn(BOARD, u32 FAR *bootver);

//Read Firmware Version Number
FLEXFUNC flex_read_firm_ver_rtn(BOARD, u32 FAR *firmver);

//Read DSP Version Number
FLEXFUNC flex_read_dsp_ver_rtn(BOARD, u32 FAR *dspver);

//Read FPGA Version Number
FLEXFUNC flex_read_fpga_ver_rtn(BOARD, u32 FAR *fpgaver);

//Set Home Input Polarity
FLEXFUNC flex_set_home_pol(BOARD, u8 device, u16 homepol);

//Enable Home Inputs
FLEXFUNC flex_enable_homes(BOARD, u8 device, u16 homemap);

//Set Limit Input Polarity
FLEXFUNC flex_set_lim_pol(BOARD, u8 device, u16 polarity);

//Set Inhibit MOMO
FLEXFUNC flex_set_inhib_momo(BOARD, u8 device, u8 muston, u8 mustoff);

//Configure Inhibit Outputs
FLEXFUNC flex_config_inhibs(BOARD, u8 device, u8 axismap, u8 polmap);

//Read Home Input Status
FLEXFUNC flex_read_home_status(BOARD, u8 device, u8 retvect);
//Read Home Input Status
FLEXFUNC flex_read_home_status_rtn(BOARD, u8 device, u16 FAR *homestatus);

//Read Velocity Threshold Status
FLEXFUNC flex_read_vel_status(BOARD, u8 device, u8 retvect);
//Read Velocity Threshold Status
FLEXFUNC flex_read_vel_status_rtn(BOARD, u8 device, u16 FAR *status);

//Insert Label
FLEXFUNC flex_insert_label(BOARD, u8 device, u16 labelnum);

// Load Velocity
FLEXFUNC flex_load_vel(BOARD, u8 axisOrVectorSpace, i32 velocity, u8 inputVector);

//Read Velocity
FLEXFUNC flex_read_vel(BOARD, u8 axisOrVectorSpace, u8 returnVector);
//Read Velocity
FLEXFUNC flex_read_vel_rtn(BOARD, u8 axisOrVectorSpace, i32 FAR *velocity);

//Load Velocity Threshold
FLEXFUNC flex_load_vel_thresh(BOARD, u8 axis, u32 threshold, u8 inputVector); 

// Set I/O Port Direction
FLEXFUNC flex_set_port_dir (BOARD, u8 port, u16 direction);

//Load Counts per Revolution
FLEXFUNC flex_load_counts_rev(BOARD, u8 axis, u16 counts);

//Load Steps per Revolution
FLEXFUNC flex_load_steps_rev(BOARD, u8 axis, u16 steps);

//Set Breakpoint Output MOMO (replaced by flex_set_breakpoint_momo)
FLEXFUNC flex_set_bp_momo(BOARD, u8 axisOrEncoder, u8 mustOn, u8 mustOff);

//Set High-Speed Capture Polarity (replaced by flex_configure_hs_capture)
FLEXFUNC flex_set_hs_cap_pol(BOARD, u8 axisOrEncoder, u16 highSpeedCapturePolarity);

//Enable High-Speed Position Capture (replaced by flex_enable_hs_capture)
FLEXFUNC flex_enable_hs_caps(BOARD, u8 axisOrEncoder, u16 captureMap);

//Enable Breakpoint
FLEXFUNC flex_enable_bp(BOARD, u8 axisOrEncoder, u8 enableMode, u8 actionOnBreakpoint);

//Set Port MOMO (replaced by flex_set_port)
FLEXFUNC flex_set_port_momo(BOARD, u8 port, u8 mustOn, u8 mustOff);

//Read Error Message
FLEXFUNC flex_read_err_msg_rtn(BOARD, u16 FAR *commandID, u16 FAR *resourceID, u16 FAR *errorCode);

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////  THESE FUNCTIONS ARE COMPATABILITY, BUT NOT REPLACED BY A NEW FUNCTION \\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////

//Read Unfiltered Velocity
FLEXFUNC flex_read_raw_vel(BOARD, u8 axisorvectorspace, u8 retvect);
//Read Unfiltered Velocity
FLEXFUNC flex_read_raw_vel_rtn(BOARD, u8 axisorvectorspace, i32 FAR *rawvel);

//Set Axis Mode
FLEXFUNC flex_set_axis_mode(BOARD, u8 axis, u16 axismode);

#ifdef __cplusplus
	}
#endif


#endif
