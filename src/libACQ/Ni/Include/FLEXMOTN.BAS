Attribute VB_Name = "FLEXMOTN"
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' FLEXMOTN.BAS File
' Include File for Visual Basic
' Copyright © National Instruments 1996-1998.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'A few notes about the structure of commands
'Commands have many arguments in common, for instance:
'
' boardID: While calling the FlexMotion Dynamic link library (FLEX16.DLL/FlexMotion32.DLL),
' this will be a number from 1 to 32. In 16 bit environment this will
' tell the FlexMotion DLL which record in the <nimotion.cfg> file to search, for
' the address of the board, as well as any the board type.
' In 32 bit environment this information is obtained from the registry (stored during
' Hardware installation).
'
' Resource/axis/encoder/vectorSpace: This tells the board exactly which resource (axis, encoder...) you are
' trying to control or read. The ResourceIDs are listed in the manual.
'
' inputVector: The Input Vector argument tells the board where you want the
' input data values for the command to come from.  For instance, the load_target_pos()
' function could have an inpvect of 0xFF (if data is being sent from the host computer)
' or 0x03 (if data is to be read from general purpose variable 3).
' See the FlexMotion user manual for more information on input
' vectors and general purpose variables.
'
' returnVector: The return vector tells the board where you would like the results of
' the operation to be sent, (ie. returned to the host or sent to a general
' purpose variable. This is only used on read commands and the flex_wait()
' function.
'
' All other arguments are specific to each command, and should be referred
' to in the function reference manual.
'
' The read commands are given in two different styles: with or without
' the _rtn suffix. The functions without the _rtn suffix do not return
' any value to the application that calls them, in fact they don't return data
' to the host application at all unless given the retvect of 0xFF. The value
' must be read explicitly by a separate execution of the flex_read_rdb command.
' The read commands that have the _rtn suffix are expecting a valid pointer
' to where you would like the data returned. These routines do in fact wait
' for the board to respond with data before returning to the calling
' application.
'
' Errors: There are two forms of errors returned:
'  1) Non Modal:   These are returned by the DLL in the return value of
'                  each function. These are usually serious errors like
'                  communication errors.
'  2) Modal:   These errors are returned by the board after the it gets the command.
'              These errors can be read using the flex_read_err_msg_rtn command. It
'              returns the command ID which caused the error, the Resource  and the error
'              code.
'  For Error Codes and Command IDs refer to the online Function Reference

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Data structures used by FlexMotion Commands
' The registry information structure for objects stored on the board
Type registryRecord
    device As Integer
    type As Integer
    pStart As Long
    size As Long
End Type

'The data structure for loading PID values to an axis (servo axis)
Type PidVals
    kp As Integer
    ki As Integer
    ilim As Integer
    kd As Integer
    td As Integer
    kv As Integer
    aff As Integer
    vff As Integer
End Type

''''' The Initialization function
Declare Function flex_initialize_controller Lib "Flex32.dll" (ByVal boardID%, settingsName%) As Integer

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''' Axis & Resource Configuration Functions
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

'''''Configure Axis Resources
Declare Function flex_config_axis Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal primaryFeedback%, ByVal secondaryFeedback%, ByVal primaryOutput%, ByVal secondaryOutput%) As Long

'''''Configure Move Complete Status Criteria
Declare Function flex_config_mc_criteria Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal criteria%, ByVal deadband%, ByVal delay%, ByVal minPulse%) As Long

'''''Configure Step Mode & Polarity
Declare Function flex_config_step_mode_pol Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrStepperOutput%, ByVal modeAndPolarityMap%) As Long

'''''Configure Vector Space
Declare Function flex_config_vect_spc Lib "FlexMotion32.dll" (ByVal boardID%, ByVal vectorSpace%, ByVal XAxis%, ByVal YAxis%, ByVal zAxis%) As Long

'''''Enable Axes
Declare Function flex_enable_axes Lib "FlexMotion32.dll" (ByVal boardID%, ByVal reserved%, ByVal PIDRate%, ByVal axisMap%) As Long

'''''Load Counts/Steps per Revolution
Declare Function flex_load_counts_steps_rev Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal unitType%, ByVal countsOrSteps&) As Long

'''''Load All PID Parameters
Declare Function flex_load_pid_parameters Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, pidvalues As PidVals, ByVal inputVector%) As Long

''''' Load Single PID Parameter
Declare Function flex_load_single_pid_parameter Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal parameterType%, ByVal PIDValue%, ByVal inputVector%) As Long

'''''Load Velocity Filter Time Constant & Run/Stop Threshold
Declare Function flex_load_vel_tc_rs Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal filterTime%, ByVal runStopThreshold%, ByVal inputVector%) As Long

'''''Set Stepper Loop Mode
Declare Function flex_set_stepper_loop_mode Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal loopMode%) As Long


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''' Trajectory Functions
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

''''' Load Acceleraton
Declare Function flex_load_acceleration Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal accelerationType%, ByVal Acceleration&, ByVal inputVector%) As Long

'''''Load Following Error
Declare Function flex_load_follow_err Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal FollowingError%, ByVal inputVector%) As Long

'''''Load Velocity in RPM
Declare Function flex_load_rpm Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal rpm#, ByVal inputVector%) As Long

''''' Load Acceleration in RPS/sec
Declare Function flex_load_rpsps Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal accelerationType%, ByVal RPSPS#, ByVal inputVector%) As Long

'''''Load Target Position
Declare Function flex_load_target_pos Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal TargetPosition&, ByVal inputVector%) As Long

'''''Load Velocity
Declare Function flex_load_velocity Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal velocity&, ByVal inputVector%) As Long

'''''Load Vector Space Position
Declare Function flex_load_vs_pos Lib "FlexMotion32.dll" (ByVal boardID%, ByVal vectorSpace%, ByVal xPosition&, ByVal yPosition&, ByVal zPosition&, ByVal inputVector%) As Long

'''''Read per Axis Status
Declare Function flex_read_axis_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal returnVector%) As Long
'''''Read per Axis Status
Declare Function flex_read_axis_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, axisStatus%) As Long

'''''Read Blend Status
Declare Function flex_read_blend_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal returnVector%) As Long
'''''Read Blend Status
Declare Function flex_read_blend_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, blendStatus%) As Long

'''''Read Following Error
Declare Function flex_read_follow_err Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal returnVector%) As Long
'''''Read Following Error
Declare Function flex_read_follow_err_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, FollowingError%) As Long

'''''Read Motion Complete Status
Declare Function flex_read_mcs_rtn Lib "FlexMotion32.dll" (ByVal boardID%, motionCompleteStatus%) As Long

'''''Read Position
Declare Function flex_read_pos Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal returnVector%) As Long
'''''Read Position
Declare Function flex_read_pos_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, position&) As Long

'''''Read Velocity in RPM
'''''The data is returned in "IEEE f64" format and has to be converted appropriately.
Declare Function flex_read_rpm Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal returnVector%) As Long
'''''Read Velocity in RPM
'''''The data is returned in "IEEE f64" format and is converted in the DLL.
Declare Function flex_read_rpm_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, rpm#) As Long

''''' Read Trajectory Status
Declare Function flex_read_trajectory_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal statusType%, ByVal returnVector%) As Long
''''' Read Trajectory Status
Declare Function flex_read_trajectory_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal statusType%, status%) As Long

'''''Read Velocity
Declare Function flex_read_velocity Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal returnVector%) As Long
'''''Read Velocity
Declare Function flex_read_velocity_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, velocity&) As Long

'''''Read Vector Space Position
Declare Function flex_read_vs_pos Lib "FlexMotion32.dll" (ByVal boardID%, ByVal vectorSpace%, ByVal returnVector%) As Long
'''''Read Vector Space Position
Declare Function flex_read_vs_pos_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal vectorSpace%, xPosition&, yPosition&, zPosition&) As Long

'''''Reset Position
Declare Function flex_reset_pos Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal position1&, ByVal position2&, ByVal inputVector%) As Long

'''''Set Operation Mode
Declare Function flex_set_op_mode Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal OperationMode%) As Long

''''Check Move Complete Status
Declare Function flex_check_move_complete_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal axisOrVSMap%, MoveComplete%) As Long

''''Check Blend Complete Status
Declare Function flex_check_blend_complete_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal axisOrVSMap%, BlendComplete%) As Long

''''Wait for Move Complete
Declare Function flex_wait_for_move_complete Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal axisOrVSMap%, ByVal timeout&, ByVal pollInterval&, MoveComplete%) As Long

''''Wait for Blend Complete
Declare Function flex_wait_for_blend_complete Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal axisOrVSMap%, ByVal timeout&, ByVal pollInterval&, BlendComplete%) As Long


'''''''' Arcs  Functions

'''''Load Circular Arc
Declare Function flex_load_circular_arc Lib "FlexMotion32.dll" (ByVal boardID%, ByVal vectorSpace%, ByVal Radius&, ByVal StartAngle#, ByVal travelAngle#, ByVal inputVector%) As Long

'''''Load Helical Arc
Declare Function flex_load_helical_arc Lib "FlexMotion32.dll" (ByVal boardID%, ByVal vectorSpace%, ByVal Radius&, ByVal StartAngle#, ByVal travelAngle#, ByVal linearTravel&, ByVal inputVector%) As Long

'''''Load Spherical Arc
Declare Function flex_load_spherical_arc Lib "FlexMotion32.dll" (ByVal boardID%, ByVal vectorSpace%, ByVal Radius&, ByVal planePitch#, ByVal planeYaw#, ByVal StartAngle#, ByVal travelAngle#, ByVal inputVector%) As Long


'''''''' Gearing Functions

'''''Configure Gear Master
Declare Function flex_config_gear_master Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal masterAxisOrEncoderOrADC%) As Long

''''Enable Gearing Single Axis
Declare Function flex_enable_gearing_single_axis Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal enable%) As Long

'''''Enable Gearing
Declare Function flex_enable_gearing Lib "FlexMotion32.dll" (ByVal boardID%, ByVal gearMap%) As Long

'''''Load Gear Ratio
Declare Function flex_load_gear_ratio Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal absoluteOrRelative%, ByVal ratioNumerator%, ByVal ratioDenominator%, ByVal inputVector%) As Long



'''''''' Advanced Trajectory Functions

'''''Acquire Trajectory Data
Declare Function flex_acquire_trajectory_data Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisMap%, ByVal numberOfSamples%, ByVal timePeriod%) As Long

'''''Load Base Velocity
Declare Function flex_load_base_vel Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal baseVelocity%, ByVal inputVector%) As Long

'''''Load Blend Factor
Declare Function flex_load_blend_fact Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal BlendFactor%, ByVal inputVector%) As Long

'''''Load Position Modulus
Declare Function flex_load_pos_modulus Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal positionModulus&, ByVal inputVector%) As Long

''''Load Velocity Threshold in RPM
Declare Function flex_load_rpm_thresh Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal threshold#, ByVal inputVector%) As Long

'''''Load S-Curve Time
Declare Function flex_load_scurve_time Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal sCurveTime%, ByVal inputVector%) As Long

'''''Load Torque Limit
Declare Function flex_load_torque_lim Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal primaryPositiveLimit%, ByVal primaryNegativeLimit%, ByVal secondaryPositiveLimit%, ByVal secondaryNegativeLimit%, ByVal inputVector%) As Long

'''''Load Torque Offset
Declare Function flex_load_torque_offset Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal primaryOffset%, ByVal secondaryOffset%, ByVal inputVector%) As Long

'''''Load Velocity Override
Declare Function flex_load_velocity_override Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal overridePercentage!, ByVal inputVector%) As Long

''''Load Velocity Threshold
Declare Function flex_load_velocity_threshold Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal velthresh&, ByVal inputVector%) As Long

'''''Read DAC
Declare Function flex_read_dac Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrDAC%, ByVal returnVector%) As Long
'''''Read DAC
Declare Function flex_read_dac_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrDAC%, DACValue%) As Long

'''''Read DAC Limit Status
Declare Function flex_read_dac_limit_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal returnVector%) As Long
'''''Read DAC Limit Status
Declare Function flex_read_dac_limit_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, positiveStatus%, negativeStatus%) As Long

'''''Read Steps Generated
Declare Function flex_read_steps_gen Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrStepperOutput%, ByVal returnVector%) As Long
'''''Read Steps Generated
Declare Function flex_read_steps_gen_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrStepperOutput%, steps&) As Long

'''''Read Target Position
Declare Function flex_read_target_pos Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal returnVector%) As Long
'''''Read Target Position
Declare Function flex_read_target_pos_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, TargetPosition&) As Long

'''''Read Trajectory Data
Declare Function flex_read_trajectory_data Lib "FlexMotion32.dll" (ByVal boardID%, ByVal returnVector%) As Long
'''''Read Trajectory Data
'''''returnData points to an array of size <= 12 depending upon the
'''''number of axes data is being acquired for. For 6 axes returnData
'''''points to an array of size 12.
Declare Function flex_read_trajectory_data_rtn Lib "FlexMotion32.dll" (ByVal boardID%, returnData&) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Buffered Functions
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''Configure Buffer
Declare Function flex_configure_buffer Lib "FlexMotion32.dll" (ByVal boardID%, ByVal buffer%, ByVal resource%, ByVal bufferType%, ByVal bufferSize&, ByVal totalPoints&, ByVal oldDataStop%, ByVal requestedInterval#, ByRef actualInterval#) As Long

''''Write Buffer
Declare Function flex_write_buffer Lib "FlexMotion32.dll" (ByVal boardID%, ByVal buffer%, ByVal numberOfPoints&, ByVal regenerationMode%, data&, ByVal inputVector%) As Long

''''Read Buffer
Declare Function flex_read_buffer Lib "FlexMotion32.dll" (ByVal boardID%, ByVal buffer%, ByVal numberOfPoints&, ByVal returnVector%) As Long
Declare Function flex_read_buffer_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal buffer%, ByVal numberOfPoints&, data&) As Long

''''Check Buffer
Declare Function flex_check_buffer Lib "FlexMotion32.dll" (ByVal boardID%, ByVal buffer%, ByVal returnVector%) As Long
Declare Function flex_check_buffer_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal buffer%, backlog&, bufferState%, pointsDone&) As Long

''''Clear Buffer
Declare Function flex_clear_buffer Lib "FlexMotion32.dll" (ByVal boardID%, ByVal buffer%) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Start & Stop Motion
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''Blend Motion
Declare Function flex_blend Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal axisMap%) As Long

''''Start Motion
Declare Function flex_start Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal axisMap%) As Long

''''Stop Motion
Declare Function flex_stop_motion Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal stopType%, ByVal axisOrVSMap%) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Motion IO Functions
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''Configure Inhibit Outputs
Declare Function flex_configure_inhibits Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisMap%, ByVal polarityMap%) As Long

''''Enable Home Inputs
Declare Function flex_enable_home_inputs Lib "FlexMotion32.dll" (ByVal boardID%, ByVal homeMap%) As Long

''''Enable Limit
Declare Function flex_enable_limits Lib "FlexMotion32.dll" (ByVal boardID%, ByVal limitType%, ByVal forwardLimitMap%, ByVal reverseLimitMap%) As Long

''''Load Software Limit Positions
Declare Function flex_load_sw_lim_pos Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal forwardLimit&, ByVal reverseLimit&, ByVal inputVector%) As Long

''''Read Home Input Status
Declare Function flex_read_home_input_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal returnVector%) As Long
''''Read Home Input Status
Declare Function flex_read_home_input_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, homeStatus%) As Long

''''Read Limit Status
Declare Function flex_read_limit_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal limitType%, ByVal returnVector%) As Long
''''Read Limit Status
Declare Function flex_read_limit_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal limitType%, forwardLimitStatus%, reverseLimitStatus%) As Long

''''Set Home Input Polarity
Declare Function flex_set_home_polarity Lib "FlexMotion32.dll" (ByVal boardID%, ByVal homePolarityMap%) As Long

''''Set Inhibit MOMO
Declare Function flex_set_inhibit_momo Lib "FlexMotion32.dll" (ByVal boardID%, ByVal mustOn%, ByVal mustOff%) As Long

''''Set Limit Input Polarity
Declare Function flex_set_limit_polarity Lib "FlexMotion32.dll" (ByVal boardID%, ByVal forwardPolarityMap%, ByVal reversePolarityMap%) As Long


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'''' Breakpoint  Functions
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

''''Configure Breakpoint
Declare Function flex_configure_breakpoint Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal enableMode%, ByVal actionOnBreakpoint%, ByVal reserved%) As Long

''''Enable Breakpoint
Declare Function flex_enable_breakpoint Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal enable%) As Long

''''Load Breakpoint Modulus
Declare Function flex_load_bp_modulus Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal breakpointModulus&, ByVal inputVector%) As Long

''''Load Breakpoint Position
Declare Function flex_load_pos_bp Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal breakpointPosition&, ByVal breakpointPosition%) As Long

''''Read Breakpoint Status
Declare Function flex_read_breakpoint_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal breakpointType%, ByVal returnVector%) As Long
''''Read Breakpoint Status
Declare Function flex_read_breakpoint_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal breakpointType%, breakpointStatus%) As Long

''Set Breakpoint Output MOMO
Declare Function flex_set_bp_momo Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal mustOn%, ByVal mustOff%) As Long

'''''''' High-speed Capture Functions

''''Enable High-Speed Capture
Declare Function flex_enable_hs_capture Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal enable%) As Long

''''Read Captured Position
Declare Function flex_read_cap_pos Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal returnVector%) As Long
''''Read Captured Position
Declare Function flex_read_cap_pos_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncodere%, capturedPosition&) As Long

''''Read High-Speed Capture Status
Declare Function flex_read_hs_cap_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, highSpeedCaptureStatus%) As Long
''''Read High-Speed Capture Status
Declare Function flex_read_hs_cap_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal returnVector%) As Long

''''Configure High-Speed Capture (replaces flex_set_hs_cap_pol)
Declare Function flex_configure_hs_capture Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal captureMode%, ByVal reserved%) As Long

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'Find Home & Index Functions
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''Find Home
Declare Function flex_find_home Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal directionMap%) As Long

''''Find Index
Declare Function flex_find_index Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal Direction%, ByVal offset%) As Long

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Analog & Digital IO Functions
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''Configure PWM Output
Declare Function flex_configure_pwm_output Lib "FlexMotion32.dll" (ByVal boardID%, ByVal PWMOutput%, ByVal enable%, ByVal clock%) As Long

''''Enable ADCs
Declare Function flex_enable_adcs Lib "FlexMotion32.dll" (ByVal boardID%, ByVal reserved%, ByVal ADCMap%) As Long

''''Enable Encoders
Declare Function flex_enable_encoders Lib "FlexMotion32.dll" (ByVal boardID%, ByVal encoderMap%) As Long

''''Load DAC
Declare Function flex_load_dac Lib "FlexMotion32.dll" (ByVal boardID%, ByVal DAC%, ByVal outputValuel%, ByVal inputVector%) As Long

''''Load PWM Duty Cycle
Declare Function flex_load_pwm_duty Lib "FlexMotion32.dll" (ByVal boardID%, ByVal PWMOutput%, ByVal dutycycle%, ByVal inpvect%) As Long

''''Read ADC
Declare Function flex_read_adc Lib "FlexMotion32.dll" (ByVal boardID%, ByVal ADCe%, ByVal returnVector%) As Long
''''Read ADC
Declare Function flex_read_adc_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal ADC%, ADCValue%) As Long

'''''Read Encoder Position
Declare Function flex_read_encoder Lib "FlexMotion32.dll" (ByVal boardID%, ByVal encoder%, ByVal returnVector%) As Long
'''''Read Encoder Position
Declare Function flex_read_encoder_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal encoder%, encoderCounts&) As Long

''''Read I/O Port
Declare Function flex_read_port Lib "FlexMotion32.dll" (ByVal boardID%, ByVal port%, ByVal returnVectort%) As Long
''''Read I/O Port
Declare Function flex_read_port_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal port%, portData%) As Long

'''''Reset Encoder Position
Declare Function flex_reset_encoder Lib "FlexMotion32.dll" (ByVal boardID%, ByVal encoder%, ByVal position&, ByVal inputVector%) As Long

''''Select Signal (RTSI)
Declare Function flex_select_signal Lib "FlexMotion32.dll" (ByVal boardID%, ByVal destination%, ByVal source%) As Long

'''' Set ADC Range
Declare Function flex_set_adc_range Lib "FlexMotion32.dll" (ByVal boardID%, ByVal ADC%, ByVal range%) As Long

'''' Configure Encoder Filter
Declare Function flex_configure_encoder_filter Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal frequency%) As Long

''''Set I/O Port Direction
Declare Function flex_set_port_direction Lib "FlexMotion32.dll" (ByVal boardID%, ByVal port%, ByVal Direction%) As Long

''''Set I/O Port MOMO
Declare Function flex_set_port_momo Lib "FlexMotion32.dll" (ByVal boardID%, ByVal port%, ByVal mustOn%, ByVal mustOff%) As Long

''''Set I/O Port Polarity
Declare Function flex_set_port_pol Lib "FlexMotion32.dll" (ByVal boardID%, ByVal port%, ByVal portPolarityMap%) As Long

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Error & Utility Functions
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''Get Error Description
Declare Function flex_get_error_description Lib "FlexMotion32.dll" (ByVal descriptionType%, ByVal errorCode&, ByVal commandID%, ByVal resourceID%, ByVal charArray As String, sizeOfArray&) As Long

''''Get Motion Board Information
Declare Function flex_get_motion_board_info Lib "FlexMotion32.dll" (ByVal boardID%, ByVal informationType&, informationValue&) As Long

''''Get Motion Board Name
Declare Function flex_get_motion_board_name Lib "FlexMotion32.dll" (ByVal boardID%, ByVal charArray As String, sizeOfArray&) As Long

''''Read Error Message
Declare Function flex_read_error_msg_rtn Lib "FlexMotion32.dll" (ByVal boardID%, commandID%, resourceID%, errorCode&) As Long

''''Get Last Error
Declare Function flex_get_last_error Lib "FlexMotion32.dll" (ByVal boardID%, commandID%, resourceID%, errorCode&) As Long

''''Get u32
Declare Function flex_getu32 Lib "FlexMotion32.dll" (ByVal boardID%, ByVal resource%, ByVal attrib%, value&) As Long

''''Set u32
Declare Function flex_setu32 Lib "FlexMotion32.dll" (ByVal boardID%, ByVal resource%, ByVal attrib%, ByVal value&) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' On-board Programming Functions
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''Begin Program Storage
Declare Function flex_begin_store Lib "FlexMotion32.dll" (ByVal boardID%, ByVal program%) As Long

''''End Program Storage
Declare Function flex_end_store Lib "FlexMotion32.dll" (ByVal boardID%, ByVal program%) As Long

''''Insert Label
Declare Function flex_insert_program_label Lib "FlexMotion32.dll" (ByVal boardID%, ByVal labelNumber%) As Long

''''Jump to Label on Condition
Declare Function flex_jump_label_on_condition Lib "FlexMotion32.dll" (ByVal boardID%, ByVal resource%, ByVal condition%, ByVal mustOn%, ByVal mustOff%, ByVal matchType%, ByVal labelNumber%) As Long

''''Load Program Delay
Declare Function flex_load_delay Lib "FlexMotion32.dll" (ByVal boardID%, ByVal delayTime&) As Long

''''Pause/Resume Program
Declare Function flex_pause_prog Lib "FlexMotion32.dll" (ByVal boardID%, ByVal program%) As Long

''''Read Program Status
Declare Function flex_read_program_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal program%, ByVal returnVector%) As Long
''''Read Program Status
Declare Function flex_read_program_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal program%, programStatus%) As Long

''''Run Program
Declare Function flex_run_prog Lib "FlexMotion32.dll" (ByVal boardID%, ByVal program%) As Long

''''Set User Status MOMO
Declare Function flex_set_status_momo Lib "FlexMotion32.dll" (ByVal boardID%, ByVal mustOn%, ByVal mustOff%) As Long

''''Stop Program
Declare Function flex_stop_prog Lib "FlexMotion32.dll" (ByVal boardID%, ByVal program%) As Long

''''Wait on Condition
Declare Function flex_wait_on_condition Lib "FlexMotion32.dll" (ByVal boardID%, ByVal resource%, ByVal waitType%, ByVal condition%, ByVal mustOn%, ByVal mustOff%, ByVal matchType%, ByVal timeout%, ByVal returnVector%) As Long

''''''Object Management Functions

''''Load Memory Object Description
''''Maximum size of description that can be entered is 32 chars
Declare Function flex_load_description Lib "FlexMotion32.dll" (ByVal boardID%, ByVal object%, ByVal description As String) As Long
''''Object Memory Management
Declare Function flex_object_mem_manage Lib "FlexMotion32.dll" (ByVal boardID%, ByVal object%, ByVal operation%) As Long

''''Read Memory Object Description
''''description points to an array of size 32
Declare Function flex_read_description_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal object%, ByVal description As String) As Long

''''Read Object Registry
Declare Function flex_read_registry_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal Index%, rec As registryRecord) As Long

''''''Data Operations Functions

''''Add Variables
Declare Function flex_add_vars Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal variable2%, ByVal returnVector%) As Long

''''And Variables
Declare Function flex_and_vars Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal variable2%, ByVal returnVector%) As Long

''''Divide Variables
Declare Function flex_div_vars Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal variable2%, ByVal returnVector%) As Long

''''Load Constant to Variable
Declare Function flex_load_var Lib "FlexMotion32.dll" (ByVal boardID%, ByVal value&, ByVal variable1%) As Long

''''Logical Shift Variable
Declare Function flex_lshift_var Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal logicalShift%, ByVal returnVector%) As Long

''''Multiply Variables
Declare Function flex_mult_vars Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal variable2%, ByVal returnVector%) As Long

''''Invert Variable
Declare Function flex_not_var Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal returnVector%) As Long

''''OR Variables
Declare Function flex_or_vars Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal variable2%, ByVal returnVector%) As Long

''''Read Variable
Declare Function flex_read_var Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal returnVector%) As Long

''''Read Variable
Declare Function flex_read_var_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, value&) As Long

''''Subtract Variables
Declare Function flex_sub_vars Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal variable2%, ByVal returnVector%) As Long

''''Exclusive OR Variables
Declare Function flex_xor_vars Lib "FlexMotion32.dll" (ByVal boardID%, ByVal variable1%, ByVal variable2%, ByVal returnVector%) As Long

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Advanced Functions
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''Clear Power Up Status
Declare Function flex_clear_pu_status Lib "FlexMotion32.dll" (ByVal boardID%) As Long

''''Communicate
Declare Function flex_communicate Lib "FlexMotion32.dll" (ByVal boardID%, ByVal Mode%, ByVal wordcount%, resource%, command%, data%, ByVal vector%) As Long

''''Enable Auto Start
Declare Function flex_enable_auto_start Lib "FlexMotion32.dll" (ByVal boardID%, ByVal enableOrDisable%, ByVal programToExecute%) As Long

''''Enable Shut Down
Declare Function flex_enable_shutdown Lib "FlexMotion32.dll" (ByVal boardID%) As Long

''''Enable 1394 Watchdog
Declare Function flex_enable_1394_watchdog Lib "FlexMotion32.dll" (ByVal boardID%, ByVal enableWatchdog%) As Long

''''Flush Return Data Buffer
Declare Function flex_flush_rdb Lib "FlexMotion32.dll" (ByVal boardID%) As Long

''''Read Communication Status
Declare Function flex_read_csr_rtn Lib "FlexMotion32.dll" (ByVal boardID%, csr%) As Long

''''Set Interrupt Event Mask
Declare Function flex_set_irq_mask Lib "FlexMotion32.dll" (ByVal boardID%, ByVal mask%) As Long

''''Reset Default Parameters
Declare Function flex_reset_defaults Lib "FlexMotion32.dll" (ByVal boardID%) As Long

''''Save Default Parameters
Declare Function flex_save_defaults Lib "FlexMotion32.dll" (ByVal boardID%) As Long


'End
'------------------------------------------------------------------------------------------------------------------------------------------'

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' FlexMotion Compatibility functions (Not Recommended for use) - for compatibility only
' Refer to the function reference manual and use the newer functions that have
' replaced these
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Declare Function flex_enable_encs Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal enabledencs%) As Long
Declare Function flex_load_2d_arc Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal Radius&, ByVal StartAngle&, ByVal travelAngle&, ByVal inpvect%) As Long
Declare Function flex_load_3d_arc Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal Radius&, ByVal planePitch&, ByVal planeYaw&, ByVal StartAngle&, ByVal travelAngle&, ByVal inpvect%) As Long
Declare Function flex_load_helix Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal Radius&, ByVal StartAngle&, ByVal travelAngle&, ByVal linearTravel&, ByVal inpvect%) As Long
Declare Function flex_enable_gear Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal gearMap%) As Long
Declare Function flex_acq_traj_data Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal axisMap%, ByVal numsamps%, ByVal timePeriod%) As Long
Declare Function flex_read_traj_data Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
'Read Trajectory Data
'  Input parameters:
'                  boardID ( 1 - 32)
'                  device
'                  an array of longs (of size 12)
'  Return Value :  0 - Success ( retdata points to an array of 12 longs )
'                 >0 - Error Code (refer to Errors.doc)
Declare Function flex_read_traj_data_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, rettraj&) As Long
Declare Function flex_read_rs_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_rs_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, runstatus%) As Long
Declare Function flex_read_motoff_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_motoff_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, offstatus%) As Long
Declare Function flex_read_dac_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_dac_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, status%) As Long
Declare Function flex_load_loop_params Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, pidvalues As PidVals, ByVal inpvect%) As Long
Declare Function flex_load_kp Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal kp%, ByVal inpvect%) As Long
Declare Function flex_load_ki Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal ki%, ByVal inpvect%) As Long
Declare Function flex_load_ilim Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal ilim%, ByVal inpvect%) As Long
Declare Function flex_load_kd Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal kd%, ByVal inpvect%) As Long
Declare Function flex_load_td Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal td%, ByVal inpvect%) As Long
Declare Function flex_load_kv Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal kv%, ByVal inpvect%) As Long
Declare Function flex_load_aff Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal aff%, ByVal inpvect%) As Long
Declare Function flex_load_vff Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal vff%, ByVal inpvect%) As Long
Declare Function flex_load_accel Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal accel&, ByVal inpvect%) As Long
Declare Function flex_load_decel Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal decel&, ByVal inpvect%) As Long
Declare Function flex_load_accel_rpsps Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal RPSPS#, ByVal inpvect%) As Long
Declare Function flex_load_decel_rpsps Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal RPSPS#, ByVal inpvect%) As Long
Declare Function flex_load_vel_override Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal overridescale%, ByVal inpvect%) As Long
Declare Function flex_stop Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal axisMap%) As Long
Declare Function flex_halt Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal axisMap%) As Long
Declare Function flex_kill Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal axisMap%) As Long
Declare Function flex_enable_lims Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal limmap%) As Long
Declare Function flex_enable_sw_lims Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal swlimmap%) As Long
Declare Function flex_read_lim_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_lim_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, limstatus%) As Long
Declare Function flex_read_sw_lim_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_sw_lim_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, swlimstatus%) As Long
Declare Function flex_read_pos_bp_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_pos_bp_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, bpstatus%) As Long
Declare Function flex_read_at_bp_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_at_bp_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, status%) As Long
Declare Function flex_save_object Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%) As Long
Declare Function flex_delete_object Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%) As Long
Declare Function flex_free_object Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%) As Long
Declare Function flex_wait Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal cc%, ByVal mustOn%, ByVal mustOff%, ByVal timeout%, ByVal retvect%) As Long
Declare Function flex_wait_or Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal cc%, ByVal mustOn%, ByVal mustOff%, ByVal timeout%, ByVal retvect%) As Long
Declare Function flex_set_home_pol Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal homepol%) As Long
Declare Function flex_enable_homes Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal homeMap%) As Long
Declare Function flex_set_lim_pol Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal polarity%) As Long
Declare Function flex_set_inhib_momo Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal mustOn%, ByVal mustOff%) As Long
Declare Function flex_config_inhibs Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal axisMap%, ByVal polmap%) As Long
Declare Function flex_read_home_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_home_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, homeStatus%) As Long
Declare Function flex_read_vel_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_vel_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, status%) As Long
Declare Function flex_insert_label Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal labelnum%) As Long
Declare Function flex_jump_label Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal cc%, ByVal mustOn%, ByVal mustOff%, ByVal labelnum%) As Long
Declare Function flex_read_boot_ver_rtn Lib "FlexMotion32.dll" (ByVal boardID%, bootver&) As Long
Declare Function flex_read_firm_ver_rtn Lib "FlexMotion32.dll" (ByVal boardID%, firmver&) As Long
Declare Function flex_read_dsp_ver_rtn Lib "FlexMotion32.dll" (ByVal boardID%, dspver&) As Long
Declare Function flex_read_fpga_ver_rtn Lib "FlexMotion32.dll" (ByVal boardID%, fpgaver&) As Long
Declare Function flex_load_vel_thresh Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal velthresh&, ByVal inputVector%) As Long
Declare Function flex_load_vel Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal velocity&, ByVal inputVector%) As Long
Declare Function flex_read_vel Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, ByVal returnVector%) As Long
Declare Function flex_read_vel_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrVectorSpace%, velocity&) As Long
Declare Function flex_read_vel_thresh_status Lib "FlexMotion32.dll" (ByVal boardID%, ByVal returnVector%) As Long
Declare Function flex_read_vel_thresh_status_rtn Lib "FlexMotion32.dll" (ByVal boardID%, velocityThresholdStatus%) As Long
Declare Function flex_set_port_dir Lib "FlexMotion32.dll" (ByVal boardID%, ByVal port%, ByVal Direction%) As Long
Declare Function flex_load_counts_rev Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal counts%) As Long
Declare Function flex_load_steps_rev Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axis%, ByVal steps%) As Long

Declare Function flex_read_err_msg_rtn Lib "FlexMotion32.dll" (ByVal boardID%, commandID%, resource%, errorCode%) As Long
Declare Function flex_enable_bp Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal enableModee%, ByVal actionOnBreakpoint%) As Long
Declare Function flex_set_hs_cap_pol Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal highSpeedCapturePolarity%) As Long
Declare Function flex_enable_hs_caps Lib "FlexMotion32.dll" (ByVal boardID%, ByVal axisOrEncoder%, ByVal captureMapp%) As Long


''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
''''''''''''''  THESE FUNCTIONS ARE COMPATIBILITY, BUT NOT REPLACEd BY A NEW FUNCTION ''''''''''''''
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Declare Function flex_read_raw_vel Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal retvect%) As Long
Declare Function flex_read_raw_vel_rtn Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, rawvel&) As Long
'Use flex_set_stepper_loop_mode()
Declare Function flex_set_axis_mode Lib "FlexMotion32.dll" (ByVal boardID%, ByVal device%, ByVal axismode%) As Long

