Attribute VB_Name = "Module2"
'*****************************************************************************************
'
'   MotnCnst.bas
'
'   This file contains standard definitions used by the Value Motion and Flex Motion
'   motion controller board families.
'*****************************************************************************************/

'********************************** Data Types *****************************************/

Global Const NIMC_FALSE = 0
Global Const NIMC_TRUE = 1

'********************************** Board Types *****************************************/
                        
Global Const PC_SERVO_2A = 7
Global Const PC_SERVO_1P = 7
Global Const PC_SERVO_4A = 3
Global Const PC_STEP_2OX = 8
Global Const PC_STEP_4OX = 4
Global Const PC_STEP_2CX = 9
Global Const PC_STEP_4CX = 5
Global Const PC_FLEXMOTION_6C = 16

Global Const PCI_SERVO_2A = 17
Global Const PCI_SERVO_4A = 11
Global Const PCI_STEP_2OX = 18
Global Const PCI_STEP_4OX = 12
Global Const PCI_STEP_2CX = 19
Global Const PCI_STEP_4CX = 13
Global Const PCI_FLEXMOTION_6C = 24
Global Const PCI_7344 = 28
Global Const PCI_7324 = 29
Global Const PCI_7314 = 30
Global Const PCI_7334 = 32

Global Const PXI_7312 = 22
Global Const PXI_7314 = 20
Global Const PXI_7322 = 23
Global Const PXI_7324 = 21
Global Const PXI_7332 = 26
Global Const PXI_7334 = 25
Global Const PXI_7344 = 27

Global Const FW_7344 = 31
Global Const ENET_7344 = 41
Global Const SER_7344 = 51

Global Const PC_CONTROL = 0
Global Const PC_STEP_OL = 1
Global Const PC_STEP_CL = 2
Global Const PC_ENCODER_4A = 6
Global Const PCI_ENCODER_4A = 14



Global Const NIMC_SERVO = 0
Global Const NIMC_OPEN_LOOP_STEPPER = 1
Global Const NIMC_CLOSED_LOOP_STEPPER = 2

'FlexMotion Board Class
Global Const NIMC_FLEX_6C = 0
Global Const NIMC_FLEX_7344 = 1

Global Const SERVO = 0
Global Const STEPPER = 1
Global Const SERVO_STEPPER = 2

Global Const NIMC_FLEX_MOTION = 0
Global Const NIMC_VALUE_MOTION = 1

Global Const NIMC_ISA_BUS = 0
Global Const NIMC_PCI_BUS = 1
Global Const NIMC_PXI_BUS = 2
Global Const NIMC_UNKNOWN_BUS = 3
Global Const NIMC_1394_BUS = 4
Global Const NIMC_ENET_BUS = 5
Global Const NIMC_SERIAL_BUS = 6

Global Const OPEN_LOOP = 0
Global Const CLOSED_LOOP = 1

Global Const TWO_AXIS = 2
Global Const THREE_AXIS = 3
Global Const FOUR_AXIS = 4
Global Const SIX_AXIS = 6

Global Const MAX_TRIGGER_NUMBER = 4
Global Const MAX_NUM_AXIS_VALUEMOTION = 4
Global Const MAX_NUM_AXIS_FLEXMOTION = 6
Global Const NIMC_MAXBOARDS = 32

Global Const NIMC_BOARD_FAMILY = 1100
Global Const NIMC_BOARD_TYPE = 1120
Global Const NIMC_BUS_TYPE = 1130
Global Const NIMC_CLOSED_LOOP_CAPABLE = 1150
Global Const NIMC_NOT_APPLICABLE = 1500
Global Const NIMC_NUM_AXES = 1510
Global Const NIMC_VALUEMOTION_BOARD_CLASS = 2020
Global Const NIMC_BOARDTYPE_TO_VALUEMOTION_BOARD_CLASS = 2150
Global Const NIMC_BOOT_VERSION = 3010
Global Const NIMC_FIRMWARE_VERSION = 3020
Global Const NIMC_DSP_VERSION = 3030
Global Const NIMC_FPGA_VERSION = 3040
Global Const NIMC_FPGA1_VERSION = 3040
Global Const NIMC_FPGA2_VERSION = 3050
Global Const NIMC_FLEXMOTION_BOARD_CLASS = 2030
Global Const NIMC_CONTROLLER_SERIAL_NUMBER = 2040

Global Const NIMC_VALUEMOTION_PROFILE_COMPLETE = &H400 'profile complete bit in 'per Axis' hardware status
Global Const NIMC_VALUEMOTION_MOTOR_OFF = 4      'motor off bit
Global Const NIMC_VALUEMOTION_MOTOR_RUNNING = &H80 'motor running bit
Global Const NIMC_VALUEMOTION_LIMIT_SWITCH = 1
Global Const NIMC_VALUEMOTION_DIRECTION = &H1000
Global Const NIMC_VALUEMOTION_FOLLOWING_ERROR = &H20
Global Const NIMC_VALUEMOTION_HOME_SWITCH = &H2
Global Const NIMC_VALUEMOTION_INDEX = &H8

Global Const NIMC_VALUEMOTION_HARDWARE_FAIL = &H40
Global Const NIMC_VALUEMOTION_FUNCTION_ERROR = &H10
Global Const NIMC_VALUEMOTION_PREVIOUS_FUNCTION_ERROR = &H80



Global Const NIMC_KP = 0
Global Const NIMC_KI = 1
Global Const NIMC_IL = 2
Global Const NIMC_KD = 3
Global Const NIMC_TD = 4
Global Const NIMC_KV = 5
Global Const NIMC_AFF = 6
Global Const NIMC_VFF = 7
Global Const NIMC_KS_POS = 8
Global Const NIMC_KS_NEG = 9

Global Const NIMC_BOTH = 0
Global Const NIMC_ACCELERATION = 1
Global Const NIMC_ACCEL = 1
Global Const NIMC_DECELERATION = 2
Global Const NIMC_DECEL = 2

Global Const NIMC_RUN_STOP_STATUS = 0
Global Const NIMC_MOTOR_OFF_STATUS = 1
Global Const NIMC_VELOCITY_THRESHOLD_STATUS = 2
Global Const NIMC_MOVE_COMPLETE_STATUS = 3

Global Const NIMC_DECEL_STOP = 0
Global Const NIMC_HALT_STOP = 1
Global Const NIMC_KILL_STOP = 2

Global Const NIMC_LIMIT_INPUTS = 0
Global Const NIMC_SOFTWARE_LIMITS = 1

Global Const NIMC_OBJECT_SAVE = 0
Global Const NIMC_OBJECT_DELETE = 1
Global Const NIMC_OBJECT_FREE = 2

Global Const NIMC_SEND_COMMAND = 0
Global Const NIMC_SEND_AND_READ = 1
Global Const NIMC_READ_RDB = 2

Global Const NIMC_WAIT = 0
Global Const NIMC_WAIT_OR = 1

Global Const NIMC_OPEN_LOOP = 0
Global Const NIMC_CLOSED_LOOP = 1

Global Const NIMC_MATCH_ALL = 0
Global Const NIMC_MATCH_ANY = 1

Global Const NIMC_PROGRAM_DONE = 0
Global Const NIMC_PROGRAM_PLAYING = 1
Global Const NIMC_PROGRAM_PAUSED = 2
Global Const NIMC_PROGRAM_STORING = 3

'********* Condition Codes for FlexMotion flex_wait_on_condition() and
'********* flex_jump_label_on_condition()
Global Const NIMC_CONDITION_LESS_THAN = 0                           '*** Less Than
Global Const NIMC_CONDITION_EQUAL = 1                           '*** Equal
Global Const NIMC_CONDITION_LESS_THAN_OR_EQUAL = 2              '*** Less Than or Equal to
Global Const NIMC_CONDITION_GREATER_THAN = 3                    '*** Greater Than
Global Const NIMC_CONDITION_NOT_EQUAL = 4                           '*** Not Equal
Global Const NIMC_CONDITION_GREATER_THAN_OR_EQUAL = 5           '*** Greater Than or Equal
Global Const NIMC_CONDITION_TRUE = 6                            '*** TRUE (default)
Global Const NIMC_CONDITION_HOME_FOUND = 7                          '*** Home Found sequence completed
Global Const NIMC_CONDITION_INDEX_FOUND = 8                     '*** Index Found on Axes
Global Const NIMC_CONDITION_HIGH_SPEED_CAPTURE = 9                  '*** High Speed Capt on Axes
Global Const NIMC_CONDITION_POSITION_BREAKPOINT = 10            '*** Position breakpoint on Axes
Global Const NIMC_CONDITION_ANTICIPATION_TIME_BREAKPOINT = 11   '*** Anticipation Time Breakpoint
Global Const NIMC_CONDITION_VELOCITY_THRESHOLD = 12                 '*** Velocity Breakpoint
Global Const NIMC_CONDITION_MOVE_COMPLETE = 13                  '*** Move Complete
Global Const NIMC_CONDITION_PROFILE_COMPLETE = 14                   '*** Profile Complete
Global Const NIMC_CONDITION_BLEND_COMPLETE = 15                 '*** Blend Complete
Global Const NIMC_CONDITION_MOTOR_OFF = 16                          '*** Motor Off
Global Const NIMC_CONDITION_HOME_INPUT_ACTIVE = 17                  '*** Home Active
Global Const NIMC_CONDITION_LIMIT_INPUT_ACTIVE = 18                 '*** Limit Active
Global Const NIMC_CONDITION_SOFTWARE_LIMIT_ACTIVE = 19          '*** Software Limit Active
Global Const NIMC_CONDITION_PROGRAM_COMPLETE = 20                   '*** Program has run to completion
Global Const NIMC_CONDITION_IO_PORT_MATCH = 21                 '*** IO port match

'Operation Modes
Global Const NIMC_ABSOLUTE_POSITION = 0
Global Const NIMC_RELATIVE_POSITION = 1
Global Const NIMC_VELOCITY = 2
Global Const NIMC_RELITIVE_TO_CAPTURE = 3
Global Const NIMC_MODULUS_POSITION = 4
Global Const NIMC_ABSOLUTE_CONTOURING = 5
Global Const NIMC_RELATIVE_CONTOURING = 6

'ValueMotion breakpoint constants
Global Const NIMC_POSITION_BREAKPOINT = 0
Global Const NIMC_ANTICIPATION_TIME_BREAKPOINT = 1

'FlexMotion breakpoint constants
Global Const NIMC_BREAKPOINT_OFF = 0
Global Const NIMC_ABSOLUTE_BREAKPOINT = 1
Global Const NIMC_RELATIVE_BREAKPOINT = 2
Global Const NIMC_MODULO_BREAKPOINT = 3

Global Const NIMC_NO_CHANGE = 0
Global Const NIMC_RESET_BREAKPOINT = 1
Global Const NIMC_SET_BREAKPOINT = 2
Global Const NIMC_TOGGLE_BREAKPOINT = 3

'These are used for getting error descriptions
Global Const NIMC_ERROR_ONLY = 0
Global Const NIMC_FUNCTION_NAME_ONLY = 1
Global Const NIMC_RESOURCE_NAME_ONLY = 2
Global Const NIMC_COMBINED_DESCRIPTION = 3

'ADC range constants
Global Const NIMC_ADC_UNIPOLAR_5 = 0
Global Const NIMC_ADC_BIPOLAR_5 = 1
Global Const NIMC_ADC_UNIPOLAR_10 = 2
Global Const NIMC_ADC_BIPOLAR_10 = 3

'RTSI constants (destination)
Global Const NIMC_RTSI0 = 0
Global Const NIMC_RTSI1 = 1
Global Const NIMC_RTSI2 = 2
Global Const NIMC_RTSI3 = 3
Global Const NIMC_RTSI4 = 4
Global Const NIMC_RTSI5 = 5
Global Const NIMC_RTSI6 = 6
Global Const NIMC_PXI_STAR_TRIGGER = 7
Global Const NIMC_HS_CAPTURE1 = 8
Global Const NIMC_HS_CAPTURE2 = 9
Global Const NIMC_HS_CAPTURE3 = 10
Global Const NIMC_HS_CAPTURE4 = 11

'RTSI constants (source)
Global Const NIMC_TRIGGER_INPUT = 8
Global Const NIMC_BREAKPOINT1 = 9
Global Const NIMC_BREAKPOINT2 = 10
Global Const NIMC_BREAKPOINT3 = 11
Global Const NIMC_BREAKPOINT4 = 12
Global Const NIMC_RTSI_SOFTWARE_PORT = 13
Global Const NIMC_DONT_DRIVE = 14
Global Const NIMC_BREAKPOINT5 = 15
Global Const NIMC_BREAKPOINT6 = 16
Global Const NIMC_PHASE_A1 = 17
Global Const NIMC_PHASE_A2 = 18
Global Const NIMC_PHASE_A3 = 19
Global Const NIMC_PHASE_A4 = 20
Global Const NIMC_PHASE_A5 = 21
Global Const NIMC_PHASE_A6 = 22
Global Const NIMC_PHASE_B1 = 23
Global Const NIMC_PHASE_B2 = 24
Global Const NIMC_PHASE_B3 = 25
Global Const NIMC_PHASE_B4 = 26
Global Const NIMC_PHASE_B5 = 27
Global Const NIMC_PHASE_B6 = 28
Global Const NIMC_INDEX1 = 29
Global Const NIMC_INDEX2 = 30
Global Const NIMC_INDEX3 = 31
Global Const NIMC_INDEX4 = 32
Global Const NIMC_INDEX5 = 33
Global Const NIMC_INDEX6 = 34

'These are the base CTRL values for resourses
Global Const NIMC_AXIS_CTRL = &H0
Global Const NIMC_VECTOR_SPACE_CTRL = &H10
Global Const NIMC_ENCODER_CTRL = &H20
Global Const NIMC_DAC_CTRL = &H30
Global Const NIMC_STEP_OUTPUT_CTRL = &H40
Global Const NIMC_ADC_CTRL = &H50
Global Const NIMC_IO_PORT_CTRL = &H0
Global Const NIMC_PWM_CTRL = &H0
Global Const NIMC_PROGRAM_CTRL = &H0

Global Const NIMC_AXIS1 = &H1
Global Const NIMC_AXIS2 = &H2
Global Const NIMC_AXIS3 = &H3
Global Const NIMC_AXIS4 = &H4
Global Const NIMC_AXIS5 = &H5
Global Const NIMC_AXIS6 = &H6

Global Const NIMC_VECTOR_SPACE1 = &H11
Global Const NIMC_VECTOR_SPACE2 = &H12
Global Const NIMC_VECTOR_SPACE3 = &H13

Global Const NIMC_ENCODER1 = &H21
Global Const NIMC_ENCODER2 = &H22
Global Const NIMC_ENCODER3 = &H23
Global Const NIMC_ENCODER4 = &H24
Global Const NIMC_ENCODER5 = &H25
Global Const NIMC_ENCODER6 = &H26

Global Const NIMC_DAC1 = &H31
Global Const NIMC_DAC2 = &H32
Global Const NIMC_DAC3 = &H33
Global Const NIMC_DAC4 = &H34
Global Const NIMC_DAC5 = &H35
Global Const NIMC_DAC6 = &H36

Global Const NIMC_STEP_OUTPUT1 = &H41
Global Const NIMC_STEP_OUTPUT2 = &H42
Global Const NIMC_STEP_OUTPUT3 = &H43
Global Const NIMC_STEP_OUTPUT4 = &H44
Global Const NIMC_STEP_OUTPUT5 = &H45
Global Const NIMC_STEP_OUTPUT6 = &H46

Global Const NIMC_ADC1 = &H51
Global Const NIMC_ADC2 = &H52
Global Const NIMC_ADC3 = &H53
Global Const NIMC_ADC4 = &H54
Global Const NIMC_ADC5 = &H55
Global Const NIMC_ADC6 = &H56
Global Const NIMC_ADC7 = &H57
Global Const NIMC_ADC8 = &H58

Global Const NIMC_IO_PORT1 = &H1
Global Const NIMC_IO_PORT2 = &H2
Global Const NIMC_IO_PORT3 = &H3
Global Const NIMC_IO_PORT4 = &H4
Global Const NIMC_RTSI_PORT = &H9

Global Const NIMC_PWM1 = &H1
Global Const NIMC_PWM2 = &H2

' These constants are used by flex_enable_axes to set the PID rate.
Global Const NIMC_PID_RATE_62_5 = 0
Global Const NIMC_PID_RATE_125 = 1
Global Const NIMC_PID_RATE_188 = 2
Global Const NIMC_PID_RATE_250 = 3
Global Const NIMC_PID_RATE_313 = 4
Global Const NIMC_PID_RATE_375 = 5
Global Const NIMC_PID_RATE_438 = 6
Global Const NIMC_PID_RATE_500 = 7

Global Const NIMC_FORWARD_DIRECTION = 0
Global Const NIMC_REVERSE_DIRECTION = 1

Global Const NIMC_OUTPUT = 0
Global Const NIMC_INPUT = 1

Global Const NIMC_READY_TO_RECEIVE = &H1
Global Const NIMC_DATA_IN_RDB = &H2             'Data in return data buffer

Global Const NIMC_PACKET_ERROR = &H10
Global Const NIMC_POWER_UP_RESET = &H20
Global Const NIMC_MODAL_ERROR_MSG = &H40
Global Const NIMC_HARDWARE_FAIL = &H80


'Read Axis Status bits
Global Const NIMC_RUN_STOP_BIT = &H1                  'Axis running bit
Global Const NIMC_PROFILE_COMPLETE_BIT = &H2          'Profile complete bit in 'per Axis' hardware status
Global Const NIMC_AXIS_OFF_BIT = &H4                  'Motor off bit
Global Const NIMC_FOLLOWING_ERROR_BIT = &H8           'Following error bit
Global Const NIMC_LIMIT_SWITCH_BIT = &H10             'Hardware Limit
Global Const NIMC_HOME_SWITCH_BIT = &H20              'Home switch bit
Global Const NIMC_SW_LIMIT_BIT = &H40                 'Software Limit
Global Const NIMC_VELOCITY_THRESHOLD_BIT = &H100      'Velocity threshold
Global Const NIMC_POS_BREAKPOINT_BIT = &H200          'Position Breakpoint
Global Const NIMC_HOME_FOUND_BIT = &H400              'Home Found
Global Const NIMC_INDEX_FOUND_BIT = &H800             'Index Found
Global Const NIMC_HIGH_SPEED_CAPTURE_BIT = &H1000     'High Speed capture
Global Const NIMC_DIRECTION_BIT = &H2000              'Direction
Global Const NIMC_BLEND_STATUS_BIT = &H4000           'Blend Status
Global Const NIMC_MOVE_COMPLETE_BIT = &H8000          'Move Complete

'Gearing Mode constants
Global Const NIMC_ABSOLUTE_GEARING = 0          'Absolute gearing
Global Const NIMC_RELATIVE_GEARING = 1        'Relative gearing

'Load Counts/Steps per Revolution constants
Global Const NIMC_COUNTS = 0                                                    ' Loading Count per Revolution
Global Const NIMC_STEPS = 1                                                     ' Loading Steps per Revolution

Global Const NIMC_MAX_FILENAME_LEN = 256 'Includes nulls byte

Global Const NIMC_ENABLE_1394_WATCHDOG = 1      'Enables the communication watchdog on the
                                                '1394 motherboard
Global Const NIMC_DISABLE_1394_WATCHDOG = 0     'Disables the communication watchdog on the
                                                '1394 motherboard

'Buffered Operations Constants
Global Const NIMC_GENERAL_PURPOSE_INPUT = 0
Global Const NIMC_GENERAL_PURPOSE_OUTPUT = 1
Global Const NIMC_POSITION_DATA = 2

'Buffer states
Global Const NIMC_BUFFER_NOT_EXIST = 0
Global Const NIMC_BUFFER_READY = 1
Global Const NIMC_BUFFER_ACTIVE = 2
Global Const NIMC_BUFFER_DONE = 3
Global Const NIMC_BUFFER_OLDDATASTOP = 4

'informationType Constants for getu32/setu32 functions.
Global Const NIMC_BUS_TIMEOUT = 256
Global Const NIMC_BP_WINDOW = 512
Global Const NIMC_PROGRAM_AUTOSTART = 768   'Attribute specifying the program that is enabled for auto start
Global Const NIMC_PULL_IN_WINDOW = 1024	
Global Const NIMC_PULL_IN_TRIES = 1025
Global Const NIMC_STICTION_MODE = 1030				'Attribute the mode of stiction compensation.
Global Const NIMC_STICTION_MAX_DEADBAND = 1031	' Attribute to set the stiction max deadband (where pid gains reconnected).
Global Const NIMC_STICTION_MIN_DEADBAND = 1032	' Attribute to set the stiction min deadband (where pid gains disconnected).
Global Const NIMC_STICTION_ITERM_OFFSET_FWD = 1033	' Attribute to set postive integral error offset (for forward direction).
Global Const NIMC_STICTION_ITERM_OFFSET_REV = 1034	' Attribute to set negative integral error offset (for reverse direction).

Global Const NIMC_OBJECT_TYPE_PROGRAM = 1
Global Const NIMC_OBJECT_TYPE_BUFFER = 2

'Pull in Constants (Closed Loop Stepper Only)
Global Const NIMC_MAX_PULL_IN_WINDOW = 32767
Global Const NIMC_MAX_PULL_IN_TRIES = 32767

'Stiction parameters (Servo only)
'Modes
Global Const NIMC_STICTION_OFF = 0
Global Const NIMC_STICTION_ZERO_DAC = 1
Global Const NIMC_STICTION_KILL = 2
