#ifndef ___motncnst_h___
#define ___motncnst_h___

/*****************************************************************************************
   This file contains standard definitions used by the Value Motion and Flex Motion 
   motion controller board families.

   The sections in this file are:

      Data Types
      Board Types
      Board Class (servo or stepper).
      Board Family
      Bus Type
      Loop Mode
      Axis Numbers

*****************************************************************************************/

/*********************************** Data Types *****************************************/
#ifndef __LINUX__
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
#else
	#ifndef __KERNEL__
	typedef unsigned char u8;
	typedef unsigned short u16;
	typedef unsigned long u32;
	#endif
#endif

typedef char            i8;
typedef short           i16;
typedef long            i32;
typedef double 			f64;
typedef float			f32;
typedef long            tBoolean;

#define  NIMC_FALSE     0
#define  NIMC_TRUE      1
/********************************** Board Types *****************************************/
                        
#define  PC_SERVO_2A       7
#define  PC_SERVO_4A       3
#define  PC_STEP_2OX       8
#define  PC_STEP_4OX       4
#define  PC_STEP_2CX       9
#define  PC_STEP_4CX       5  
#define  PC_FLEXMOTION_6C  16

#define  PCI_SERVO_2A      17
#define  PCI_SERVO_4A      11
#define  PCI_STEP_2OX      18
#define  PCI_STEP_4OX      12
#define  PCI_STEP_2CX      19
#define  PCI_STEP_4CX      13
#define  PCI_FLEXMOTION_6C 24
#define	PCI_7344				28
#define	PCI_7324				29
#define	PCI_7314				30
#define  PCI_7334				32
#define  PCI_7332				33
#define  PCI_7348          34
#define  PCI_7342				37
#define  PCI_7346				38

#define  PXI_7312    22    // Step-2OX
#define  PXI_7314    20    // Step-4OX 
#define  PXI_7322    23    // Step-2CX
#define  PXI_7324    21  
#define  PXI_7332    26   
#define  PXI_7334    25    
#define  PXI_7344		27
#define  PXI_7348    35
#define  PXI_7342		36
#define  PXI_7346    39

#define	FW_7344		31		//1394 7344
#define	ENET_7344	41		// Ethernet 7344
#define	SER_7344		51		// Serial 7344

// existing boards no longer being shipped.
#define  PC_CONTROL        0
#define  PC_STEP_OL        1
#define  PC_STEP_CL        2
#define  PC_ENCODER_4A     6
#define  PCI_ENCODER_4A    14

#define  PC_SERVO_1P       PC_SERVO_2A

// *******************************************************************
// There are currently 31 boards, and the PC_SERVO_1P is NOT included.
// *******************************************************************


//////////////////////// ValueMotion Board Class (servo or stepper)//////////////////////////////////////
#define  NIMC_SERVO 0
#define  NIMC_OPEN_LOOP_STEPPER  1
#define  NIMC_CLOSED_LOOP_STEPPER  2

//////////////////////// FlexMotion Board Class (6C or 7344)//////////////////////////////////////
#define  NIMC_FLEX_6C		0
#define  NIMC_FLEX_7344		1
#define	NIMC_FLEX_7334		2

//////////////////////// Board Class (servo or stepper)//////////////////////////////////////
#define  SERVO 0
#define  STEPPER  1
#define  SERVO_STEPPER 2

///////////////////////////////// Board Family ///////////////////////////////////////////////
#define  NIMC_FLEX_MOTION  0
#define  NIMC_VALUE_MOTION 1

//////////////////////////////////// Bus Type ///////////////////////////////////////////////
#define  NIMC_ISA_BUS   0
#define  NIMC_PCI_BUS  1
#define  NIMC_PXI_BUS  2
#define  NIMC_UNKNOWN_BUS 3
#define	NIMC_1394_BUS	4
#define	NIMC_ENET_BUS	5
#define	NIMC_SERIAL_BUS	6

//////////////////////////////////// Loop Mode ///////////////////////////////////////////////
#define  OPEN_LOOP   0
#define  CLOSED_LOOP  1

////////////////////////////////////// Axis Numbers //////////////////////////////////////////
#define  TWO_AXIS   2
#define  THREE_AXIS   3
#define  FOUR_AXIS   4
#define  SIX_AXIS   6
#define	EIGHT_AXIS   8

//////////////////////////////////// OS types supported //////////////////////////////////////
#define NIMC_UNKNOWN_OS	0xFFFF
#define NIMC_WIN95		0
#define NIMC_WIN98		2
#define NIMC_WINNT		3
#define NIMC_WIN2000		4
#define NIMC_PHARLAP		5
#define NIMC_LINUX		6

/************************************ MISC ********************************************/
#define  MAX_TRIGGER_NUMBER 4
#define  MAX_NUM_AXIS_VALUEMOTION 4
#define  MAX_NUM_AXIS_FLEXMOTION 6
#define  NIMC_MAXBOARDS 32

#define  NIMC_BOARD_FAMILY                1100
#define  NIMC_BOARD_TYPE                  1120
#define  NIMC_BUS_TYPE                    1130
#define  NIMC_CLOSED_LOOP_CAPABLE         1150
#define  NIMC_NOT_APPLICABLE              1500
#define  NIMC_NUM_AXES                    1510
#define  NIMC_VALUEMOTION_BOARD_CLASS     2020
#define  NIMC_BOARDTYPE_TO_VALUEMOTION_BOARD_CLASS     2150
#define	NIMC_BOOT_VERSION                3010
#define	NIMC_FIRMWARE_VERSION            3020
#define	NIMC_DSP_VERSION                 3030
#define	NIMC_FPGA_VERSION                3040
#define  NIMC_FPGA1_VERSION					3040
#define  NIMC_FPGA2_VERSION					3050
#define  NIMC_FLEXMOTION_BOARD_CLASS		2030
#define	NIMC_CONTROLLER_SERIAL_NUMBER		2040


//ValueMotion Axis Status bits
#define  NIMC_VALUEMOTION_PROFILE_COMPLETE	0x400 //profile complete bit for ValueMotion
#define  NIMC_VALUEMOTION_MOTOR_OFF				4     //motor off bit for ValueMotion
#define  NIMC_VALUEMOTION_MOTOR_RUNNING		0x80  //motor running bit for ValueMotion
#define	NIMC_VALUEMOTION_LIMIT_SWITCH 1
#define  NIMC_VALUEMOTION_HOME_SWITCH	2
#define  NIMC_VALUEMOTION_FOLLOWING_ERROR		0x20
#define	NIMC_VALUEMOTION_DIRECTION	0x1000
#define  NIMC_VALUEMOTION_INDEX	0x8

//ValueMotion communication status bits
#define NIMC_VALUEMOTION_HARDWARE_FAIL		0x40
#define NIMC_VALUEMOTION_FUNCTION_ERROR	0x10
#define NIMC_VALUEMOTION_PREVIOUS_FUNCTION_ERROR 0x80


#define	NIMC_KP		0
#define	NIMC_KI		1
#define	NIMC_IL		2
#define	NIMC_KD		3
#define	NIMC_TD		4
#define	NIMC_KV		5
#define	NIMC_AFF		6
#define	NIMC_VFF		7
#define	NIMC_KS_POS 8
#define	NIMC_KS_NEG 9

#define	NIMC_BOTH 0
#define	NIMC_ACCELERATION 1
#define	NIMC_ACCEL NIMC_ACCELERATION
#define	NIMC_DECELERATION 2
#define	NIMC_DECEL NIMC_DECELERATION

#define	NIMC_RUN_STOP_STATUS 0
#define	NIMC_MOTOR_OFF_STATUS 1 
#define	NIMC_VELOCITY_THRESHOLD_STATUS 2
#define	NIMC_MOVE_COMPLETE_STATUS 3

#define	NIMC_DECEL_STOP 0
#define	NIMC_HALT_STOP 1
#define	NIMC_KILL_STOP 2

#define	NIMC_LIMIT_INPUTS 0 
#define	NIMC_SOFTWARE_LIMITS 1

#define	NIMC_OBJECT_SAVE 0
#define	NIMC_OBJECT_DELETE 1
#define	NIMC_OBJECT_FREE 2

#define	NIMC_SEND_COMMAND 0
#define	NIMC_SEND_AND_READ 1
#define	NIMC_READ_RDB 2

#define	NIMC_WAIT 0
#define	NIMC_WAIT_OR 1

#define  NIMC_OPEN_LOOP 0
#define  NIMC_CLOSED_LOOP 1

#define  NIMC_MATCH_ALL 0
#define  NIMC_MATCH_ANY 1

#define	NIMC_PROGRAM_DONE 0
#define	NIMC_PROGRAM_PLAYING 1
#define	NIMC_PROGRAM_PAUSED 2
#define	NIMC_PROGRAM_STORING 3

//Condition Codes for FlexMotion flex_wait_on_condition() and
//flex_jump_label_on_condition()
#define  NIMC_CONDITION_LESS_THAN	                  0	// Less Than
#define  NIMC_CONDITION_EQUAL	                        1	// Equal	
#define  NIMC_CONDITION_LESS_THAN_OR_EQUAL	         2  // Less Than or Equal to	
#define  NIMC_CONDITION_GREATER_THAN	               3	// Greater Than		
#define  NIMC_CONDITION_NOT_EQUAL	                  4	// Not Equal
#define  NIMC_CONDITION_GREATER_THAN_OR_EQUAL	      5	// Greater Than or Equal
#define  NIMC_CONDITION_TRUE	                        6	// TRUE (default)	
#define  NIMC_CONDITION_HOME_FOUND	                  7	// Home Found sequence completed
#define  NIMC_CONDITION_INDEX_FOUND	                  8  // Index Found on Axes	
#define  NIMC_CONDITION_HIGH_SPEED_CAPTURE	         9	// High Speed Capt on Axes
#define  NIMC_CONDITION_POSITION_BREAKPOINT	         10	// Position breakpoint on Axes				
#define  NIMC_CONDITION_ANTICIPATION_TIME_BREAKPOINT  11	// Anticipation Time Breakpoint	
#define  NIMC_CONDITION_VELOCITY_THRESHOLD          	12	// Velocity Breakpoint			
#define  NIMC_CONDITION_MOVE_COMPLETE               	13	// Move Complete				
#define  NIMC_CONDITION_PROFILE_COMPLETE            	14	// Profile Complete				
#define  NIMC_CONDITION_BLEND_COMPLETE              	15	// Blend Complete				
#define  NIMC_CONDITION_MOTOR_OFF                   	16	// Motor Off					
#define  NIMC_CONDITION_HOME_INPUT_ACTIVE           	17	// Home Active					
#define  NIMC_CONDITION_LIMIT_INPUT_ACTIVE          	18	// Limit Active					
#define  NIMC_CONDITION_SOFTWARE_LIMIT_ACTIVE         19 // Software Limit Active
#define  NIMC_CONDITION_PROGRAM_COMPLETE            	20	// Program has run to completion
#define  NIMC_CONDITION_IO_PORT_MATCH              	21	// IO port match				

// Operation Modes
#define	NIMC_ABSOLUTE_POSITION     0
#define  NIMC_RELATIVE_POSITION     1
#define  NIMC_VELOCITY              2
#define  NIMC_RELATIVE_TO_CAPTURE   3
#define  NIMC_MODULUS_POSITION      4
#define	NIMC_ABSOLUTE_CONTOURING	5
#define	NIMC_RELATIVE_CONTOURING	6

// ValueMotion breakpoint constants
#define	NIMC_POSITION_BREAKPOINT 0
#define	NIMC_ANTICIPATION_TIME_BREAKPOINT 1

// FlexMotion breakpoint constants
#define	NIMC_BREAKPOINT_OFF			0
#define	NIMC_ABSOLUTE_BREAKPOINT	1
#define	NIMC_RELATIVE_BREAKPOINT	2
#define	NIMC_MODULO_BREAKPOINT		3

#define	NIMC_NO_CHANGE					0
#define	NIMC_RESET_BREAKPOINT		1
#define	NIMC_SET_BREAKPOINT			2
#define	NIMC_TOGGLE_BREAKPOINT		3

#define NIMC_OPERATION_SINGLE		0
#define NIMC_OPERATION_BUFFERED		1

// FlexMotion regeneration mode constants for write_buffer
#define NIMC_REGENERATION_NO_CHANGE		0
#define NIMC_REGENERATION_LAST_WRITE	1

// FlexMotion high-speed capture constants
#define NIMC_HS_NON_INVERTING_LEVEL			0
#define NIMC_HS_INVERTING_LEVEL				1
#define NIMC_HS_LOW_TO_HIGH_EDGE				2
#define NIMC_HS_HIGH_TO_LOW_EDGE				3
#define NIMC_HS_NON_INVERTING_DI				4
#define NIMC_HS_INVERTING_DI					5

// These are used for getting error descriptions
#define NIMC_ERROR_ONLY			0
#define NIMC_FUNCTION_NAME_ONLY		1
#define NIMC_RESOURCE_NAME_ONLY		2
#define NIMC_COMBINED_DESCRIPTION	3

// ADC range constants
#define NIMC_ADC_UNIPOLAR_5	0
#define NIMC_ADC_BIPOLAR_5		1
#define NIMC_ADC_UNIPOLAR_10	2
#define NIMC_ADC_BIPOLAR_10	3

// RTSI constants (destination)
#define NIMC_RTSI0				0
#define NIMC_RTSI1				1
#define NIMC_RTSI2				2
#define NIMC_RTSI3				3
#define NIMC_RTSI4				4
#define NIMC_RTSI5				5
#define NIMC_RTSI6				6
#define NIMC_PXI_STAR_TRIGGER	7
#define NIMC_HS_CAPTURE1		8
#define NIMC_HS_CAPTURE2		9
#define NIMC_HS_CAPTURE3		10
#define NIMC_HS_CAPTURE4		11

// RTSI constants (source)
#define NIMC_TRIGGER_INPUT		8
#define NIMC_BREAKPOINT1		9
#define NIMC_BREAKPOINT2		10
#define NIMC_BREAKPOINT3		11
#define NIMC_BREAKPOINT4		12
#define NIMC_RTSI_SOFTWARE_PORT	13
#define NIMC_DONT_DRIVE			14
#define NIMC_BREAKPOINT5		15
#define NIMC_BREAKPOINT6		16
#define NIMC_PHASE_A1			17
#define NIMC_PHASE_A2			18
#define NIMC_PHASE_A3			19
#define NIMC_PHASE_A4			20
#define NIMC_PHASE_A5			21
#define NIMC_PHASE_A6			22
#define NIMC_PHASE_B1			23
#define NIMC_PHASE_B2			24
#define NIMC_PHASE_B3			25
#define NIMC_PHASE_B4			26
#define NIMC_PHASE_B5			27
#define NIMC_PHASE_B6			28
#define NIMC_INDEX1				29
#define NIMC_INDEX2				30
#define NIMC_INDEX3				31
#define NIMC_INDEX4				32
#define NIMC_INDEX5				33
#define NIMC_INDEX6				34

/* These are the base CTRL values for resourses.*/
#define NIMC_AXIS_CTRL				0x00
#define NIMC_VECTOR_SPACE_CTRL   0x10
#define NIMC_ENCODER_CTRL			0x20
#define NIMC_DAC_CTRL				0x30
#define NIMC_STEP_OUTPUT_CTRL		0x40
#define NIMC_ADC_CTRL				0x50
#define NIMC_IO_PORT_CTRL			0x00
#define NIMC_PWM_CTRL				0x00
#define NIMC_PROGRAM_CTRL			0x00

#define NIMC_AXIS1	0x01
#define NIMC_AXIS2	0x02
#define NIMC_AXIS3	0x03
#define NIMC_AXIS4	0x04
#define NIMC_AXIS5	0x05
#define NIMC_AXIS6	0x06

#define NIMC_VECTOR_SPACE1	0x11
#define NIMC_VECTOR_SPACE2	0x12
#define NIMC_VECTOR_SPACE3	0x13

#define NIMC_ENCODER1	0x21
#define NIMC_ENCODER2	0x22
#define NIMC_ENCODER3	0x23
#define NIMC_ENCODER4	0x24
#define NIMC_ENCODER5	0x25
#define NIMC_ENCODER6	0x26

#define NIMC_DAC1	0x031
#define NIMC_DAC2	0x032
#define NIMC_DAC3	0x033
#define NIMC_DAC4	0x034
#define NIMC_DAC5	0x035
#define NIMC_DAC6	0x036

#define NIMC_STEP_OUTPUT1	0x41
#define NIMC_STEP_OUTPUT2	0x42
#define NIMC_STEP_OUTPUT3	0x43
#define NIMC_STEP_OUTPUT4	0x44
#define NIMC_STEP_OUTPUT5	0x45
#define NIMC_STEP_OUTPUT6	0x46

#define NIMC_ADC1	0x51
#define NIMC_ADC2	0x52
#define NIMC_ADC3	0x53
#define NIMC_ADC4	0x54
#define NIMC_ADC5	0x55
#define NIMC_ADC6	0x56
#define NIMC_ADC7	0x57
#define NIMC_ADC8	0x58

#define NIMC_IO_PORT1	0x01
#define NIMC_IO_PORT2	0x02
#define NIMC_IO_PORT3	0x03

#define NIMC_PWM1		0x01
#define NIMC_PWM2		0x02

// These constants are used by flex_enable_axes to set the PID rate.
#define NIMC_PID_RATE_62_5		0
#define NIMC_PID_RATE_125		1
#define NIMC_PID_RATE_188		2
#define NIMC_PID_RATE_250		3
#define NIMC_PID_RATE_313		4
#define NIMC_PID_RATE_375		5
#define NIMC_PID_RATE_438		6
#define NIMC_PID_RATE_500		7

#define NIMC_FORWARD_DIRECTION	0 
#define NIMC_REVERSE_DIRECTION	1

#define NIMC_OUTPUT	0 
#define NIMC_INPUT	1

//Communication Status Register bits
#define NIMC_READY_TO_RECEIVE 0x01		//Ready to receive
#define NIMC_DATA_IN_RDB		0x02		//Data in return data buffer
#define NIMC_PACKET_ERROR     0x10		//Packet error
#define NIMC_POWER_UP_RESET	0x20		//Power up Reset
#define NIMC_MODAL_ERROR_MSG  0x40		//Modal error message
#define NIMC_HARDWARE_FAIL		0x80		//Hardware Fail bit


//Read Axis Status bits
#define NIMC_RUN_STOP_BIT				0x0001			//Axis running bit
#define NIMC_PROFILE_COMPLETE_BIT	0x0002			//Profile complete bit in 'per Axis' hardware status
#define NIMC_AXIS_OFF_BIT				0x0004			//Motor off bit
#define NIMC_FOLLOWING_ERROR_BIT		0x0008			//Following error bit
#define NIMC_LIMIT_SWITCH_BIT			0x0010			//Hardware Limit
#define NIMC_HOME_SWITCH_BIT			0x0020			//Home switch bit
#define NIMC_SW_LIMIT_BIT				0x0040			//Software Limit
#define NIMC_VELOCITY_THRESHOLD_BIT	0x0100			//Velocity threshold
#define NIMC_POS_BREAKPOINT_BIT		0x0200			//Position Breakpoint
#define NIMC_HOME_FOUND_BIT			0x0400       //Home Found
#define NIMC_INDEX_FOUND_BIT			0x0800			//Index Found
#define NIMC_HIGH_SPEED_CAPTURE_BIT	0x1000      //High Speed capture
#define NIMC_DIRECTION_BIT				0x2000		//Direction
#define NIMC_BLEND_STATUS_BIT			0x4000      //Blend Status
#define NIMC_MOVE_COMPLETE_BIT		0x8000		//Move Complete 

//Gearing Mode constants
#define NIMC_ABSOLUTE_GEARING		0	//Absolute gearing
#define NIMC_RELATIVE_GEARING		1	//Relative gearing

//Load Counts/Steps per Revolution constants
#define NIMC_COUNTS	0
#define NIMC_STEPS	1

#define NIMC_MAX_FILENAME_LEN		256 // includes nulls byte

#define NIMC_ENABLE_1394_WATCHDOG	1  //Enables the communication watchdog on the 
													//1394 motherboard
#define NIMC_DISABLE_1394_WATCHDOG	0  //Disables the communication watchdog on the 
													//1394 motherboard

// Buffered Operations Constants
#define	NIMC_GENERAL_PURPOSE_INPUT		0
#define	NIMC_GENERAL_PURPOSE_OUTPUT		1
#define	NIMC_POSITION_DATA				2
#define NIMC_BREAKPOINT_DATA			3
#define NIMC_HS_CAPTURE_READBACK		4

// Contouring Points Constants
#define NIMC_MIN_CONTOURING_INTERVAL	10		// 10 ms
#define NIMC_MAX_CONTOURING_INTERVAL	90.5	// PID rate of 500 us with splinePoints = 181
#define NIMC_MAX_SPLINE_POINTS			181		// sqrt(2^15) -- 
												// Precondition on DSP code, DX^2 <= 2^15

// Buffer states
#define	NIMC_BUFFER_NOT_EXIST		0
#define	NIMC_BUFFER_READY			1
#define	NIMC_BUFFER_ACTIVE			2
#define	NIMC_BUFFER_DONE			3
#define	NIMC_BUFFER_OLDDATASTOP		4

// informationType Constants for getu32/setu32 functions.
#define	NIMC_BUS_TIMEOUT	256		// Attribute specifying the time out value for remote device
#define	NIMC_BP_WINDOW		512		// Attribute specifying the window for enabling breakpoint
#define	NIMC_PROGRAM_AUTOSTART	768		// Attribute specifying the program that is enabled for auto start
#define	NIMC_PULL_IN_WINDOW	1024	// Attribute specifying the deadband around the target position for a 
												// pull in move (closed loop stepper only).
#define	NIMC_PULL_IN_TRIES	1025	// Attribute specifying the number of tries to pull in a closed loop
												// stepper move in the target position is not reached.
#define	NIMC_STICTION_MODE				1030	// Attribute the mode of stiction compensation.
#define	NIMC_STICTION_MAX_DEADBAND		1031	// Attribute to set the stiction max deadband (where pid gains reconnected).
#define	NIMC_STICTION_MIN_DEADBAND		1032	// Attribute to set the stiction min deadband (where pid gains disconnected).
#define	NIMC_STICTION_ITERM_OFFSET_FWD	1033	// Attribute to set postive integral error offset (for forward direction).
#define	NIMC_STICTION_ITERM_OFFSET_REV	1034	// Attribute to set negative integral offset (for reverse direction).

//Object Types
#define NIMC_OBJECT_TYPE_PROGRAM 1
#define NIMC_OBJECT_TYPE_BUFFER 2

// Pull in Constants (Closed Loop Stepper Only)
#define NIMC_MAX_PULL_IN_WINDOW	32767
#define NIMC_MAX_PULL_IN_TRIES	32767

// Stiction parameters (Servo only)
// Modes
#define NIMC_STICTION_OFF			0
#define NIMC_STICTION_ZERO_DAC	1
#define NIMC_STICTION_KILL			2


#endif // ___motncnst_h___
