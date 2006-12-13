/********************************************************************/
/*      	                  FLEXMOTN.H                                */
/*	            Include File for FlexMotion32.dll                     */
/*																	                  */
/*	 Written by the Motion Control Development Team				         */
/*  COPYRIGHT 1997-2000 National Instruments All Rights Reserved.    */
/********************************************************************/
// A few notes about the structure of commands
// Commands have many arguments in common, for instance:

// boardID: In DOS, this is simply the board address.  In a DLL, 
// 	this will be a number from 1 to 32. In 16 bit environment this will 
//	   tell the DLL which record in the <nimotion.cfg> file to search for 
// 	the hex address of the board, as well as any other parameters needed.
// 	In 32 bit environment this information is obtained from the registry.

// resource\axis\vectorSpace\Encoder: The device number tells the board exactly 
// 	which resource\axis\vectorSpace\Encoder ... you are trying to control or read. 
//    The resource numbers are listed in the function reference manual.

// inputVector: The Input Vector argument tells the board where you want the
//	    input data values to the command to come from.  For instance, the load_target_pos()
//	   function could have an inpvect of 0xFF (if the data is being sent from the host computer)
//    or 0x03 (if the data is to be read from the general purpose variable 3).  
//    See the FlexMotion user manual for more information on input
//	   vectors and general purpose variables.

// returnVector: The returnVector tells the board where you would like the results of
//	   the operation to be sent, (ie. returned to the host or sent to a general
//	   purpose variable. This is only used on read commands and the flex_wait()
//	   function.

// All other arguments are specific to each command, and should be referred
// 	to in the manual.

// The read commands are given in two different styles: with or without
//	the _rtn suffix. The functions without the _rtn suffix do not return
//	any value to the application that calls them, in fact they don't return data
//	to the host application at all unless given the returnVector of 0xFF. The value
//	must be read explicitly by a separate execution of the flex_communicate command.
//	The read commands that have the _rtn suffix are expecting a valid pointer
//	to where you would like the data returned. These routines do in fact wait
//	for the board to respond with data before returning to the calling
//	application.

// Errors: There are two forms of errors returned:
//	1) Non Modal:	These are returned by the DLL in the return value of
//					each function. These are usually serious errors like
//					communication errors.
//	2) Modal:These errors are returned by the board after the it gets the command.
//				These errors can be read using the flex_read_error_msg_rtn command. It
//				returns the command ID which caused the error, the device ID  and the error
//				code.
//	For Error Codes and Command IDs refer to errors.doc and commands.doc respectively.


#ifndef ___FlexMotn_h___
#define ___FlexMotn_h___


#ifdef WIN32
	#ifndef _WINDOWS
		#define _WINDOWS
	#endif
#endif

#ifdef _Windows
	#define _WINDOWS	//For Borland C++ Ver5.0 Support
#endif

#ifdef _WINDOWS
	#include <windows.h>
#endif


#ifdef __cplusplus
	extern "C"{
#endif

#include "motncnst.h"
#include "MotnErr.h"

#define   MAXBOARDS 0x20	//32 boards allowed in this version of the DLL


//Function Prototypes For flexmotion32.dll
#ifdef WIN32
	#define FLEXFUNC		 __declspec(dllexport) APIENTRY
	#define PCFUNC			 __declspec(dllimport) APIENTRY
#else
	#define FLEXFUNC		i32
	#define FAR
#endif

#define BOARD 			u8 boardID
#define BOARDID			boardID



// PID is the structure passed to flex_load_loop_params() function
//	(PID and PIVFF parameters)
typedef struct{
	u16 kp;
	u16 ki;
	u16 ilim;
	u16 kd;
	u16 td;
	u16 kv;
	u16 aff;
	u16 vff;
} PID;

// REGISTRY is the structure passed to flex_read_registry() function in order to
// return info about the object registry entry.
typedef struct{
	u16 device;
	u16 type;
	u32 pstart;
	u32 size;
} REGSTRY, REGISTRY;


// The Initialization function
FLEXFUNC flex_initialize_controller (BOARD, i8 FAR *settingsName);

///////////////////////////////////////////////////////////////////////////////////////
// Axis & Resource Configuration Functions
///////////////////////////////////////////////////////////////////////////////////////

//Configure Axis Resources
FLEXFUNC flex_config_axis(BOARD, u8 axis, u8 primaryFeedback, u8 secondaryFeedback,
                          u8 primaryOutput, u8 secondaryOutput);

//Configure Move Complete Status Criteria
FLEXFUNC flex_config_mc_criteria(BOARD, u8 axis, u16 criteria, u16 deadband, u8 delay,
											u8 minPulse);

//Configure Step Mode & Polarity 
FLEXFUNC flex_config_step_mode_pol(BOARD, u8 axisOrStepperOutput, u16 modeAndPolarityMap);

//Configure Vector Space
FLEXFUNC flex_config_vect_spc(BOARD, u8 vectorSpace, u8 xAxis, u8 yAxis, u8 zAxis);

//Enable Axes
FLEXFUNC flex_enable_axes(BOARD, u8 reserved, u8 PIDRate, u8 axisMap);

// Load Counts/Steps per Revolution
FLEXFUNC flex_load_counts_steps_rev(BOARD, u8 axisOrEncoder, u16 unitType, u32 countsOrSteps);

//Load All PID Parameters
FLEXFUNC flex_load_pid_parameters(BOARD, u8 axis, PID FAR *PIDValues, u8 inputVector);

// Load Single PID Parameter
FLEXFUNC flex_load_single_pid_parameter(BOARD, u8 axis, u16 parameterType, u16 PIDValue, u8 inputVector);

//Load Velocity Filter Time Constant & Run/Stop Threshold
FLEXFUNC flex_load_vel_tc_rs(BOARD, u8 axis, u16 filterTime, u16 runStopThreshold, u8 inputVector);

//Set Stepper Loop Mode
FLEXFUNC flex_set_stepper_loop_mode(BOARD, u8 axis, u16 loopMode);


///////////////////////////////////////////////////////////////////////////////////////
// Trajectory Functions
///////////////////////////////////////////////////////////////////////////////////////

// Load Acceleraton
FLEXFUNC flex_load_acceleration(BOARD, u8 axisOrVectorSpace, u16 accelerationType, u32 acceleration, u8 inputVector);

//Load Following Error
FLEXFUNC flex_load_follow_err(BOARD, u8 axis, u16 followingError, u8 inputVector);

//Load Velocity in RPM
FLEXFUNC flex_load_rpm(BOARD, u8 axisOrVectorSpace, f64 RPM, u8 inputVector);

// Load Acceleration in RPS/sec
FLEXFUNC flex_load_rpsps(BOARD, u8 axisOrVectorSpace, u16 accelerationType, f64 RPSPS, u8 inputVector);

//Load Target Position
FLEXFUNC flex_load_target_pos(BOARD, u8 axis, i32 targetPosition, u8 inputVector);

//Load Velocity
FLEXFUNC flex_load_velocity(BOARD, u8 axisOrVectorSpace, i32 velocity, u8 inputVector);

//Load Vector Space Position
FLEXFUNC flex_load_vs_pos (BOARD, u8 vectorSpace, i32 xPosition, i32 yPosition, i32 zPosition, u8 inputVector);

//Read per Axis Status
FLEXFUNC flex_read_axis_status(BOARD, u8 axis, u8 returnVector);
//Read per Axis Status
FLEXFUNC flex_read_axis_status_rtn(BOARD, u8 axis, u16 FAR *axisStatus);

//Read Blend Status
FLEXFUNC flex_read_blend_status(BOARD, u8 axisOrVectorSpace, u8 returnVector);
//Read Blend Status
FLEXFUNC flex_read_blend_status_rtn(BOARD, u8 axisOrVectorSpace, u16 FAR *blendStatus);

//Read Following Error
FLEXFUNC flex_read_follow_err(BOARD, u8 axisOrVectorSpace, u8 returnVector);
//Read Following Error
FLEXFUNC flex_read_follow_err_rtn(BOARD, u8 axisOrVectorSpace, i16 FAR *followingError);

//Read Motion Complete Status
FLEXFUNC flex_read_mcs_rtn(BOARD, u16 FAR *moveCompleteStatus);

//Read Position
FLEXFUNC flex_read_pos(BOARD, u8 axis, u8 returnVector);
//Read Position
FLEXFUNC flex_read_pos_rtn(BOARD, u8 axis, i32 FAR *position);

//Read Velocity in RPM
//The data is returned in "IEEE f64" format and has to be converted appropriately.
FLEXFUNC flex_read_rpm(BOARD, u8 axisOrVectorSpace, u8 returnVector);
//Read Velocity in RPM
//The data is returned in "IEEE f64" format and is converted in the DLL.
FLEXFUNC flex_read_rpm_rtn(BOARD, u8 axisOrVectorSpace, f64 FAR *RPM);

// Read Trajectory Status
FLEXFUNC flex_read_trajectory_status(BOARD, u8 axisOrVectorSpace, u16 statusType, u8 returnVector);
// Read Trajectory Status
FLEXFUNC flex_read_trajectory_status_rtn(BOARD, u8 axisOrVectorSpace, u16 statusType, u16 FAR *status);

//Read Velocity
FLEXFUNC flex_read_velocity(BOARD, u8 axisOrVectorSpace, u8 returnVector);
//Read Velocity
FLEXFUNC flex_read_velocity_rtn(BOARD, u8 axisOrVectorSpace, i32 FAR *velocity);

//Read Vector Space Position
FLEXFUNC flex_read_vs_pos (BOARD, u8 vectorSpace, u8 returnVector);
//Read Vector Space Position
FLEXFUNC flex_read_vs_pos_rtn (BOARD, u8 vectorSpace, i32 FAR *xPosition, i32 FAR *yPosition, i32 FAR *zPosition);

//Reset Position
FLEXFUNC flex_reset_pos (BOARD, u8 axis, i32 position1, i32 position2, u8 inputVector);

//Set Operation Mode
FLEXFUNC flex_set_op_mode (BOARD, u8 axisOrVectorSpace, u16 operationMode);

// Check Move Complete Status
FLEXFUNC flex_check_move_complete_status (BOARD, u8 axisOrVectorSpace, 
														u16 axisOrVSMap, u16* moveComplete);

// Check Blend Complete Status
FLEXFUNC flex_check_blend_complete_status (BOARD, u8 axisOrVectorSpace, 
														 u16 axisOrVSMap, u16* blendComplete);

// Wait for Move Complete
FLEXFUNC flex_wait_for_move_complete (BOARD, u8 axisOrVectorSpace, u16 axisOrVSMap, 
												  u32 timeout, i32 pollInterval, u16* moveComplete);

// Wait for Blend Complete
FLEXFUNC flex_wait_for_blend_complete (BOARD, u8 axisOrVectorSpace, u16 axisOrVSMap, 
													u32 timeout, i32 pollInterval, u16* blendComplete);

//////// Arcs  Functions

//Load Circular Arc
FLEXFUNC flex_load_circular_arc (BOARD, u8 vectorSpace, u32 radius, f64 startAngle,
                     f64 travelAngle, u8 inputVector);

//Load Helical Arc
FLEXFUNC flex_load_helical_arc (BOARD, u8 vectorSpace, u32 radius, f64 startAngle,
               f64 travelAngle, i32 linearTravel, u8 inputVector);

//Load Spherical Arc
FLEXFUNC flex_load_spherical_arc (BOARD, u8 vectorSpace, u32 radius, f64 planePitch,
            f64 planeYaw, f64 startAngle, f64 travelAngle, u8 inputVector);


//////// Gearing Functions

//Configure Gear Master
FLEXFUNC flex_config_gear_master(BOARD, u8 axis, u8 masterAxisOrEncoderOrADC);

//Enable Gearing
FLEXFUNC flex_enable_gearing(BOARD, u16 gearMap);

//Enable Gearing Single Axis
FLEXFUNC flex_enable_gearing_single_axis (BOARD, u8 axis, u16 enable);

//Load Gear Ratio
FLEXFUNC flex_load_gear_ratio(BOARD, u8 axis, u16 absoluteOrRelative,
									i16 ratioNumerator, u16 ratioDenominator, u8 inputVector);


//////// Advanced Trajectory Functions

//Acquire Trajectory Data
FLEXFUNC flex_acquire_trajectory_data(BOARD, u16 axisMap, u16 numberOfSamples, u16 timePeriod);

//Load Base Velocity
FLEXFUNC flex_load_base_vel(BOARD, u8 axis, u16 baseVelocity, u8 inputVector);

//Load Blend Factor
FLEXFUNC flex_load_blend_fact(BOARD, u8 axisOrVectorSpace, i16 blendFactor, u8 inputVector);

//Load Position Modulus
FLEXFUNC flex_load_pos_modulus(BOARD, u8 axis, u32 positionModulus, u8 inputVector);

//Load Velocity Threshold in RPM
FLEXFUNC flex_load_rpm_thresh(BOARD, u8 axis, f64 threshold, u8 inputVector);

//Load S-Curve Time
FLEXFUNC flex_load_scurve_time(BOARD, u8 axisOrVectorSpace, u16 sCurveTime, u8 inputVector);

//Load Torque Limit
FLEXFUNC flex_load_torque_lim(BOARD, u8 axis, i16 primaryPositiveLimit, i16 primaryNegativeLimit,
												 i16 secondaryPositiveLimit, i16 secondaryNegativeLimit, u8 inputVector);

//Load Torque Offset
FLEXFUNC flex_load_torque_offset(BOARD, u8 axis, i16 primaryOffset, i16 secondaryOffset, u8 inputVector);

//Load Velocity Override
FLEXFUNC flex_load_velocity_override(BOARD, u8 axisOrVectorSpace, f32 overridePercentage, u8 inputVector);

//Load Velocity Threshold
FLEXFUNC flex_load_velocity_threshold(BOARD, u8 axis, u32 threshold, u8 inputVector); 

//Read DAC
FLEXFUNC flex_read_dac(BOARD, u8 axisOrDAC, u8 returnVector);
//Read DAC
FLEXFUNC flex_read_dac_rtn(BOARD, u8 axisOrDAC, i16 FAR *DACValue);

//Read DAC Limit Status
FLEXFUNC flex_read_dac_limit_status(BOARD, u8 returnVector);
//Read DAC Limit Status
FLEXFUNC flex_read_dac_limit_status_rtn(BOARD, u8 FAR *positiveStatus, u8 FAR *negativeStatus);

//Read Steps Generated
FLEXFUNC flex_read_steps_gen(BOARD, u8 axisOrStepperOutput, u8 returnVector);
//Read Steps Generated
FLEXFUNC flex_read_steps_gen_rtn(BOARD, u8 axisOrStepperOutput, i32 FAR *steps);

//Read Target Position
FLEXFUNC flex_read_target_pos(BOARD, u8 axis, u8 returnVector);
//Read Target Position
FLEXFUNC flex_read_target_pos_rtn(BOARD, u8 axis, i32 FAR *targetPosition);

//Read Trajectory Data
FLEXFUNC flex_read_trajectory_data(BOARD, u8 returnVector);
//Read Trajectory Data
//returnData points to an array of size <= 12 depending upon the
//number of axes data is being acquired for. For 6 axes returnData
//points to an array of size 12.
FLEXFUNC flex_read_trajectory_data_rtn(BOARD, i32 FAR *returnData);


///////////////////////////////////////////////////////////////////////////////////////
// Buffered Functions
///////////////////////////////////////////////////////////////////////////////////////

// Configure Buffer
FLEXFUNC flex_configure_buffer (BOARD, u8 buffer, u8 resource, u16 bufferType, i32 bufferSize,
				u32 totalPoints, u16 oldDataStop, f64 requestedInterval, f64 FAR *actualInterval);

// Write Buffer
FLEXFUNC flex_write_buffer (BOARD, u8 buffer, u32 numberOfPoints, 
				u16 regenerationMode, i32 *data, u8 inputVector);

// Read Buffer
FLEXFUNC flex_read_buffer (BOARD, u8 buffer, u32 numberOfPoints, u8 returnVector);
FLEXFUNC flex_read_buffer_rtn (BOARD, u8 buffer, u32 numberOfPoints, i32* data);

// Check Buffer
FLEXFUNC flex_check_buffer (BOARD, u8 buffer, u8 returnVector);
FLEXFUNC flex_check_buffer_rtn (BOARD, u8 buffer, u32 *backlog, u16 *bufferState,
				u32 *pointsDone);

// Clear Buffer
FLEXFUNC flex_clear_buffer (BOARD, u8 buffer);

///////////////////////////////////////////////////////////////////////////////////////
// Start & Stop Motion Functions
///////////////////////////////////////////////////////////////////////////////////////

//Blend Motion
FLEXFUNC flex_blend(BOARD, u8 axisOrVectorSpace, u16 axisOrVSMap);

//Start Motion
FLEXFUNC flex_start(BOARD, u8 axisOrVectorSpace, u16 axisOrVSMap);

//Stop Motion
FLEXFUNC flex_stop_motion(BOARD, u8 axisOrVectorSpace, u16 stopType, u16 axisOrVSMap);


///////////////////////////////////////////////////////////////////////////////////////
// Motion IO Functions
///////////////////////////////////////////////////////////////////////////////////////

//Configure Inhibit Outputs
FLEXFUNC flex_configure_inhibits(BOARD, u8 axisMap, u8 polarityMap);

//Enable Home Inputs
FLEXFUNC flex_enable_home_inputs(BOARD, u16 homeMap);

//Enable Limits
FLEXFUNC flex_enable_limits(BOARD, u16 limitType, u8 forwardLimitMap, u8 reverseLimitMap);

//Load Software Limit Positions
FLEXFUNC flex_load_sw_lim_pos(BOARD, u8 axis, i32 forwardLimit, i32 reverseLimit, u8 inputVector);

//Read Home Input Status
FLEXFUNC flex_read_home_input_status(BOARD, u8 returnVector);
//Read Home Input Status
FLEXFUNC flex_read_home_input_status_rtn(BOARD, u16 FAR *homeStatus);

//Read Limit Status
FLEXFUNC flex_read_limit_status(BOARD, u16 limitType, u8 returnVector);
//Read Limit Status
FLEXFUNC flex_read_limit_status_rtn(BOARD, u16 limitType, u8 FAR *forwardLimitStatus, u8 FAR *reverseLimitStatus);

//Set Home Input Polarity
FLEXFUNC flex_set_home_polarity(BOARD, u16 homePolarityMap);

//Set Inhibit MOMO
FLEXFUNC flex_set_inhibit_momo(BOARD, u8 mustOn, u8 mustOff);

//Set Limit Input Polarity
FLEXFUNC flex_set_limit_polarity(BOARD, u8 forwardPolarityMap, u8 reversePolarityMap);


//////// Breakpoint  Functions

//Configure Breakpoint
FLEXFUNC flex_configure_breakpoint(BOARD, u8 axisOrEncoder, u16 enableMode, u16 actionOnBreakpoint, u16 reserved);

//Enable Breakpoint
FLEXFUNC flex_enable_breakpoint(BOARD, u8 axisOrEncoder, u16 enable);

//Load Breakpoint Modulus
FLEXFUNC flex_load_bp_modulus(BOARD, u8 axisOrEncoder, u32 breakpointModulus, u8 inputVector);

//Load Breakpoint Position
FLEXFUNC flex_load_pos_bp(BOARD, u8 axisOrEncoder, i32 breakpointPosition, u8 inputVector);

//Read Breakpoint Status
FLEXFUNC flex_read_breakpoint_status(BOARD, u8 axisOrEncoder, u16 breakpointType, u8 returnVector);
//Read Breakpoint Status
FLEXFUNC flex_read_breakpoint_status_rtn(BOARD, u8 axisOrEncoder, u16 breakpointType, u16 FAR *breakpointStatus);

//Set Breakpoint Output MOMO
FLEXFUNC flex_set_breakpoint_momo(BOARD, u8 axisOrEncoder, u8 mustOn, u8 mustOff, u8 inputVector);

//Enable High-Speed Capture
FLEXFUNC flex_enable_hs_capture(BOARD, u8 axisOrEncoder, u16 enable);

//Read Captured Position
FLEXFUNC flex_read_cap_pos(BOARD, u8 axisOrEncoder, u8 returnVector);
//Read Captured Position
FLEXFUNC flex_read_cap_pos_rtn(BOARD, u8 axisOrEncoder, i32 FAR *capturedPosition);

//Read High-Speed Capture Status
FLEXFUNC flex_read_hs_cap_status(BOARD, u8 axisOrEncoder, u8 returnVector);
//Read High-Speed Capture Status
FLEXFUNC flex_read_hs_cap_status_rtn(BOARD, u8 axisOrEncoder, u16 FAR *highSpeedCaptureStatus);

//Configure High-Speed Capture (replaces flex_set_hs_cap_pol)
FLEXFUNC flex_configure_hs_capture(BOARD, u8 axisOrEncoder, u16 captureMode, u16 reserved);

///////////////////////////////////////////////////////////////////////////////////////
// Find Home & Index Functions
///////////////////////////////////////////////////////////////////////////////////////

//Find Home
FLEXFUNC flex_find_home(BOARD, u8 axis, u16 directionMap);

//Find Index
FLEXFUNC flex_find_index(BOARD, u8 axis, u16 direction, i16 offset);


///////////////////////////////////////////////////////////////////////////////////////
// Analog & Digital IO Functions
///////////////////////////////////////////////////////////////////////////////////////

// Configure PWM Output
FLEXFUNC flex_configure_pwm_output (BOARD, u8 PWMOutput, u16 enable, u16 clock);

// Enable ADCs
FLEXFUNC flex_enable_adcs (BOARD, u8 reserved, u16 ADCMap);

// Enable Encoders
FLEXFUNC flex_enable_encoders (BOARD, u16 encoderMap);

// Load DAC
FLEXFUNC flex_load_dac (BOARD, u8 DAC, i16 outputValue, u8 inputVector);

// Load PWM Duty Cycle
FLEXFUNC flex_load_pwm_duty (BOARD, u8 PWMOutput, u16 dutycycle, u8 inputVector);

// Read ADC
FLEXFUNC flex_read_adc (BOARD, u8 ADC, u8 returnVector);
// Read ADC
FLEXFUNC flex_read_adc_rtn (BOARD, u8 ADC, i16 FAR *ADCValue);

// Read Encoder Position
FLEXFUNC flex_read_encoder (BOARD, u8 axisOrEncoder, u8 returnVector);
// Read Encoder Position
FLEXFUNC flex_read_encoder_rtn (BOARD, u8 axisOrEncoder, i32 FAR *encoderCounts);

// Read I/O Port
FLEXFUNC flex_read_port (BOARD, u8 port, u8 returnVector);
// Read I/O Port
FLEXFUNC flex_read_port_rtn (BOARD, u8 port, u16 FAR *portData);

// Reset Encoder Position 
FLEXFUNC flex_reset_encoder (BOARD, u8 encoder, i32 position, u8 inputVector);

// Select Signal (RTSI)
FLEXFUNC flex_select_signal (BOARD, u16 destination, u16 source);

// Set ADC Range
FLEXFUNC flex_set_adc_range (BOARD, u8 ADC, u16 range);

// Configure Encoder Filter
FLEXFUNC flex_configure_encoder_filter (BOARD, u8 axisOrEncoder, u16 frequency);

// Set I/O Port Direction
FLEXFUNC flex_set_port_direction (BOARD, u8 port, u16 directionMap);

// Set I/O Port MOMO
FLEXFUNC flex_set_port(BOARD, u8 port, u8 mustOn, u8 mustOff, u8 inputVector);

// Set I/O Port Polarity
FLEXFUNC flex_set_port_pol (BOARD, u8 port, u16 portPolarityMap);

///////////////////////////////////////////////////////////////////////////////////////
// Error & Utility Functions
///////////////////////////////////////////////////////////////////////////////////////

// Get Error Description
FLEXFUNC flex_get_error_description(u16 descriptionType, i32 errorCode, u16 commandID, u16 resourceID, 
												i8 FAR *charArray, u32 FAR *sizeOfArray);

// Get Motion Board Information
FLEXFUNC flex_get_motion_board_info (BOARD, u32 informationType, u32 FAR *informationValue);

// Get Motion Board Name
FLEXFUNC flex_get_motion_board_name (BOARD, i8 FAR *charArray, u32 FAR *sizeOfArray);

//Read Error Message
FLEXFUNC flex_read_error_msg_rtn(BOARD, u16 FAR *commandID, u16 FAR *resourceID, i32 FAR *errorCode);

// Get Last Error
FLEXFUNC flex_get_last_error(BOARD, u16 *commandID, u16 *resourceID, i32 *errorCode);

// Get u32
FLEXFUNC flex_getu32(BOARD, u8 resource, u16 attribute, u32 *value);

// Set u32
FLEXFUNC flex_setu32(BOARD, u8 resource, u16 attribute, u32 value);

///////////////////////////////////////////////////////////////////////////////////////
// On-board Programming Functions
///////////////////////////////////////////////////////////////////////////////////////

//Begin Program Storage
FLEXFUNC flex_begin_store(BOARD, u8 program);

//End Program Storage
FLEXFUNC flex_end_store(BOARD, u8 program);

//Insert Label
FLEXFUNC flex_insert_program_label(BOARD, u16 labelNumber);

//Jump to Label on Condition
FLEXFUNC flex_jump_label_on_condition (BOARD, u8 resource, u16 condition, u8 mustOn, 
                                       u8 mustOff, u16 matchType, u16 labelNumber);

//Load Program Delay
FLEXFUNC flex_load_delay(BOARD, u32 delayTime);

//Pause/Resume Program
FLEXFUNC flex_pause_prog(BOARD, u8 program);

//Read Program Status
FLEXFUNC flex_read_program_status (BOARD, u8 program, u8 returnVector);
//Read Program Status
FLEXFUNC flex_read_program_status_rtn (BOARD, u8 program, u16 FAR *programStatus);

//Run Program
FLEXFUNC flex_run_prog(BOARD, u8 program);

//Set User Status MOMO
FLEXFUNC flex_set_status_momo(BOARD, u8 mustOn, u8 mustOff);

//Stop Program
FLEXFUNC flex_stop_prog(BOARD, u8 program);

//Wait on Condition
FLEXFUNC flex_wait_on_condition(BOARD, u8 resource, u16 waitType, u16 condition, u8 mustOn, 
                                u8 mustOff, u16 matchType, u16 timeOut, u8 returnVector);


//////// Object Management Functions

//Load Memory Object Description
//Maximum size of description that can be entered is 32 chars
FLEXFUNC flex_load_description(BOARD, u8 object, i8 FAR *description);

// Object Memory Management
FLEXFUNC flex_object_mem_manage(BOARD, u8 object, u16 operation);

//Read Memory Object Description
//description points to an array of size 32
FLEXFUNC flex_read_description_rtn(BOARD, u8 object, i8 FAR * description);

//Read Object Registry
FLEXFUNC flex_read_registry_rtn(BOARD, u8 index, REGISTRY FAR *registryRecord);


//////// Data Operations Functions

//Add Variables
FLEXFUNC flex_add_vars(BOARD, u8 variable1, u8 variable2, u8 returnVector);

//AND Variables
FLEXFUNC flex_and_vars(BOARD, u8 variable1, u8 variable2, u8 returnVector);

//Divide Variables
FLEXFUNC flex_div_vars(BOARD, u8 variable1, u8 variable2, u8 returnVector);

//Load Constant to Variable
FLEXFUNC flex_load_var(BOARD, i32 value, u8 variable1);

//Logical Shift Variable
FLEXFUNC flex_lshift_var(BOARD, u8 variable1, i8 logicalShift, u8 returnVector);

//Multiply Variables
FLEXFUNC flex_mult_vars(BOARD, u8 variable1, u8 variable2, u8 returnVector);

//Invert Variable
FLEXFUNC flex_not_var(BOARD, u8 variable1, u8 returnVector);

//OR Variables
FLEXFUNC flex_or_vars(BOARD, u8 variable1, u8 variable2, u8 returnVector);

//Read Variable
FLEXFUNC flex_read_var(BOARD, u8 variable1, u8 returnVector);
//Read Variable
FLEXFUNC flex_read_var_rtn(BOARD, u8 variable1, i32 FAR *value);

//Subtract Variables
FLEXFUNC flex_sub_vars(BOARD, u8 variable1, u8 variable2, u8 returnVector);

//Exclusive OR Variables
FLEXFUNC flex_xor_vars(BOARD, u8 variable1, u8 variable2, u8 returnVector);


///////////////////////////////////////////////////////////////////////////////////////
// Advanced Functions
///////////////////////////////////////////////////////////////////////////////////////

//Clear Power Up Status
FLEXFUNC flex_clear_pu_status(BOARD);

//Communicate
FLEXFUNC flex_communicate(BOARD, u8 mode, u8 wordCount, u8 FAR *resource,
				  u16 FAR *command, u16 FAR *data, u8 vector);

//Flush Return Data Buffer
FLEXFUNC flex_flush_rdb(BOARD);

//Enable Auto Start
FLEXFUNC flex_enable_auto_start(BOARD, u8 enableOrDisable, u8 programToExecute);

//Enable Shut Down
FLEXFUNC flex_enable_shutdown(BOARD);

//Enable 1394 Watchdog
FLEXFUNC flex_enable_1394_watchdog(BOARD, u16 enableOrDisable);

//Read Communication Status
FLEXFUNC flex_read_csr_rtn(BOARD, u16 FAR *csr);

//Reset Default Parameters
FLEXFUNC flex_reset_defaults (BOARD);

//Save Default Parameters
FLEXFUNC flex_save_defaults (BOARD);

//Set Interrupt Event Mask
FLEXFUNC flex_set_irq_mask(BOARD, u16 mask);


///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// This includes functions that have been retired.  They may be completely removed at 
// any time, please do not use them in new development.
///////////////////////////////////////////////////////////////////////////////////////

#include "FlexComp.h"
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif

