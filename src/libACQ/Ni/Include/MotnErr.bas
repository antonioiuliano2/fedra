'=============================================================================================
' MotnErr.bas  -  Motion Control Error Codes
'
' Always use symbolic names and not explicit values when referring to
' specific error codes in your program.
'
' These error codes are also documented in Appendix A of the 
' Software Reference Manual and in the Function Reference Online Help
'
' Copyright 2000
' National Instruments Corporation.
' All rights reserved.
'
' Last Updated: 5/12/2000
'
'=============================================================================================

Global Const NIMC_noError                         = 0      ' No error.
Global Const NIMC_readyToReceiveTimeoutError      = -70001 ' Ready to Receive Timeout. The controller is still not ready to receive 
                                                           ' commands after the specified timeout period. This error may occur if the 
                                                           ' controller is busy processing previous commands. If this error persists, 
                                                           ' even when the controller should not be busy, contact National Instruments.
Global Const NIMC_currentPacketError              = -70002 ' Either this function is not supported by this type of controller, or the 
                                                           ' controller received an incomplete command packet and cannot execute the 
                                                           ' function.
Global Const NIMC_noReturnDataBufferError         = -70003 ' No data in the Return Data Buffer.  The kernel driver returns an error if 
                                                           ' it runs out of time waiting for the controller to return data to the 
                                                           ' Return Data Buffer. For FlexMotion controllers, this error can also be 
                                                           ' returned if the power-up state of the controller has not been cleared.
Global Const NIMC_halfReturnDataBufferError       = -70004 ' Partial readback packet. The data returned by the controller is 
                                                           ' incomplete. The kernel driver timed out after getting partial data.
Global Const NIMC_boardFailureError               = -70005 ' Most likely, your controller is not installed or configured properly.  If 
                                                           ' this error persists when you know your controller is installed and 
                                                           ' configured properly, it indicates an internal hardware failure.
Global Const NIMC_badResourceIDOrAxisError        = -70006 ' For ValueMotion, an invalid axis number was used. For FlexMotion, an 
                                                           ' invalid axis number or other resource ID (Vector Space, Encoder, I/O Port, 
                                                           ' and so on) was used.
Global Const NIMC_CIPBitError                     = -70007 ' A previous function is currently being executed, so the controller cannot 
                                                           ' accept this function until the previous function has completed.  If this 
                                                           ' problem persists, try putting a delay between the offending commands.
Global Const NIMC_previousPacketError             = -70008 ' The function called previous to this one is not supported by this type of 
                                                           ' controller.
Global Const NIMC_packetErrBitNotClearedError     = -70009 ' Packet error bit not cleared by terminator (hardware error).
Global Const NIMC_badCommandError                 = -70010 ' Command ID not recognized. Invalid command sent to the controller 
                                                           ' (FlexMotion only).
Global Const NIMC_badReturnDataBufferPacketError  = -70011 ' Corrupt readback data. The data returned by the motion controller is 
                                                           ' corrupt.
Global Const NIMC_badBoardIDError                 = -70012 ' Illegal board ID. You must use the board ID assigned to your controller in 
                                                           ' Measurement & Automation Explorer.
Global Const NIMC_packetLengthError               = -70013 ' Command packet length is incorrect.
Global Const NIMC_closedLoopOnlyError             = -70014 ' This command is valid only on closed-loop axes (closed-loop stepper and 
                                                           ' servo).
Global Const NIMC_returnDataBufferFlushError      = -70015 ' Unable to flush the Return Data Buffer.
Global Const NIMC_servoOnlyError                  = -70016 ' This command is valid only on servo axes.
Global Const NIMC_stepperOnlyError                = -70017 ' This command is valid only on stepper axes.
Global Const NIMC_closedLoopStepperOnlyError      = -70018 ' This command is valid only on closed-loop stepper axes.
Global Const NIMC_noBoardConfigInfoError          = -70019 ' Controller configuration information is missing or corrupt.
Global Const NIMC_countsNotConfiguredError        = -70020 ' Steps/rev and/or counts/rev (in ValueMotion, lines/rev) not loaded for 
                                                           ' this axis.
Global Const NIMC_systemResetError                = -70021 ' System reset did not occur in maximum time allowed.
Global Const NIMC_functionSupportError            = -70022 ' This command is not supported by this controller or operating system.
Global Const NIMC_parameterValueError             = -70023 ' One of the parameters passed into the function has an illegal value.
Global Const NIMC_motionOnlyError                 = -70024 ' Motion command sent to an Encoder board.
Global Const NIMC_returnDataBufferNotEmptyError   = -70025 ' The Return Data Buffer is not empty. Commands that expect data returned 
                                                           ' from the controller cannot be sent until the Return Data Buffer is 
                                                           ' cleared.
Global Const NIMC_modalErrorsReadError            = -70026 ' The Motion Error Hander.flx VI discovered modal error(s) in the modal 
                                                           ' error stack.  These error(s) can be viewed in the Modal Error(s) Out 
                                                           ' Indicator/terminal of this VI.
Global Const NIMC_processTimeoutError             = -70027 ' Under Windows NT, a function call made to the motion controller timed out 
                                                           ' waiting for driver access. For ethernet controllers, a function call made 
                                                           ' to the motion controller timed out waiting for return data.  This can 
                                                           ' happen due to a slow or lost connection with the ethernet box product.
Global Const NIMC_insufficientSizeError           = -70028 ' The resource is not large enough to supported the specified operation.
Global Const NIMC_reserved4Error                  = -70029 ' Reserved for obsolete product.
Global Const NIMC_reserved5Error                  = -70030 ' Reserved for obsolete product.
Global Const NIMC_reserved6Error                  = -70031 ' Reserved for obsolete product.
Global Const NIMC_reserved7Error                  = -70032 ' Reserved for obsolete product.
Global Const NIMC_badPointerError                 = -70033 ' A NULL pointer has been passed into a function inappropriately.
Global Const NIMC_wrongReturnDataError            = -70034 ' Incorrect data has been returned by the controller.  This data does not 
                                                           ' correspond to the expected data for the command sent to the controller.
Global Const NIMC_watchdogTimeoutError            = -70035 ' A fatal error has occurred on the controller. You must reset the 
                                                           ' controller by power cycling your computer. Contact National Instruments 
                                                           ' technical support if this problem persists.
Global Const NIMC_invalidRatioError               = -70036 ' A specified ratio is invalid.
Global Const NIMC_irrelevantAttributeError        = -70037 ' The specified attribute is not relevant.
Global Const NIMC_internalSoftwareError           = -70038 ' An unexpected error has occurred internal to the driver.  Please contact 
                                                           ' National Instruments with the name of the function or VI that returned 
                                                           ' this error.
Global Const NIMC_1394WatchdogEnableError         = -70039 ' The communication watchdog on the 1394 motherboard could not be started.
Global Const NIMC_availableForUse40               = -70040 ' Available
Global Const NIMC_boardIDInUseError               = -70041 ' The boardID is already in use by another controller.  If trying to change 
                                                           ' a controllers boardID, select a boardID not in use.
Global Const NIMC_availableForUse42               = -70042 ' Available
Global Const NIMC_availableForUse43               = -70043 ' Available
Global Const NIMC_availableForUse44               = -70044 ' Available
Global Const NIMC_availableForUse45               = -70045 ' Available
Global Const NIMC_availableForUse46               = -70046 ' Available
Global Const NIMC_availableForUse47               = -70047 ' Available
Global Const NIMC_availableForUse48               = -70048 ' Available
Global Const NIMC_downloadChecksumError           = -70049 ' There was an error during check sum on a file being downloaded to the 
                                                           ' FlexMotion controller.
Global Const NIMC_reserved50Error                 = -70050 ' Reserved.
Global Const NIMC_firmwareDownloadError           = -70051 ' Firmware download failed.  Reset the controller and try downloading again.
Global Const NIMC_FPGAProgramError                = -70052 ' Internal Error.  The FPGA failed to program.  Reset the controller and try 
                                                           ' again. If the problem persists, contact National Instruments technical 
                                                           ' support.
Global Const NIMC_DSPInitializationError          = -70053 ' Internal Error.  The DSP failed to initialize.  Reset the controller and 
                                                           ' try again. If the problem persists, contact National Instruments technical 
                                                           ' support.
Global Const NIMC_corrupt68331FirmwareError       = -70054 ' Corrupt onboard microprocessor firmware detected.  Download new firmware.
Global Const NIMC_corruptDSPFirmwareError         = -70055 ' Corrupt DSP firmware detected.  Download new DSP firmware.
Global Const NIMC_corruptFPGAFirmwareError        = -70056 ' Corrupt FPGA firmware detected.  Download new FPGA firmware.
Global Const NIMC_interruptConfigurationError     = -70057 ' Internal Error.  Host interrupt configuration failed and interrupt support 
                                                           ' is disabled.
Global Const NIMC_IOInitializationError           = -70058 ' Internal Error.  The I/O structure on the controller failed to initialize. 
                                                           '  Reset the controller and try again. If the problem persists, contact 
                                                           ' National Instruments technical support.
Global Const NIMC_flashromCopyError               = -70059 ' Error copying to the FLASH ROM.
Global Const NIMC_corruptObjectSectorError        = -70060 ' The objects stored in FLASH ROM are corrupt.
Global Const NIMC_bufferInUseError                = -70061 ' The specified buffer is in use.
Global Const NIMC_oldDataStopError                = -70062 ' The Read Buffer or Write Buffer function was unable to complete 
                                                           ' sucessfully because old data was encountered in the course of reading from 
                                                           ' or writing to the buffer.
Global Const NIMC_bufferConfigurationError        = -70063 ' The buffer has not been configured properly.  Buffer type cannot be 
                                                           ' changed.  The buffer might exist in ROM, but has not yet been configured 
                                                           ' since the last power cycle.
Global Const NIMC_illegalBufferOperation          = -70064 ' This operation is invalid at this time.  You cannot write to a ROM buffer.
Global Const NIMC_illegalContouringError          = -70065 ' This operation cannot be executed while a contouring operation is in 
                                                           ' progress.
Global Const NIMC_availableForUse66               = -70066 ' None
Global Const NIMC_availableForUse67               = -70067 ' None
Global Const NIMC_availableForUse68               = -70068 ' None
Global Const NIMC_availableForUse69               = -70069 ' None
Global Const NIMC_availableForUse70               = -70070 ' None
Global Const NIMC_availableForUse71               = -70071 ' None
Global Const NIMC_boardInPowerUpResetStateError   = -70072 ' The FlexMotion controller is in power up reset state.  Please execute the 
                                                           ' Clear Power Up Status function or VI before sending any commands to the 
                                                           ' controller.
Global Const NIMC_boardInShutDownStateError       = -70073 ' The controller cannot accept this function, as it has been shut down.
Global Const NIMC_shutDownFailedError             = -70074 ' The controller failed to shut down.  This could be because it failed to 
                                                           ' disable the DACs, the encoders, or the ADCs, or because it could not reset 
                                                           ' the I/O back to user defaults. 
Global Const NIMC_hostFIFOBufferFullError         = -70075 ' Communication FIFO buffer between the host computer and the controller is 
                                                           ' full.
Global Const NIMC_noHostDataError                 = -70076 ' Communications error.  The controller did not receive any data in the 
                                                           ' command packet from the host computer.
Global Const NIMC_corruptHostDataError            = -70077 ' Communications error.  The controller received corrupt data in the packet 
                                                           ' from the host computer.
Global Const NIMC_invalidFunctionDataError        = -70078 ' Invalid function data has been passed.  This is usually a parameter out of 
                                                           ' range, or an illegal combination of parameter values.
Global Const NIMC_autoStartFailedError            = -70079 ' The controller could not run the onboard program on auto start.  When you 
                                                           ' enable auto start, make sure that you specify a valid program number and 
                                                           ' that the program is saved in FLASH ROM.
Global Const NIMC_returnDataBufferFullError       = -70080 ' The Return Data Buffer on the controller is full.
Global Const NIMC_reserved81Error                 = -70081 ' Reserved - Never used for DSP messaging error.
Global Const NIMC_reserved82Error                 = -70082 ' Reserved - Never used for position wrap-around error.
Global Const NIMC_DSPXmitBufferFullError          = -70083 ' Internal error. The transmit buffer of the DSP is full. Messages from DSP 
                                                           ' to the onboard microprocessor are being delayed or lost.
Global Const NIMC_DSPInvalidCommandError          = -70084 ' Internal error.  The DSP received an illegal command.
Global Const NIMC_DSPInvalidDeviceError           = -70085 ' Internal error.  The DSP received a command with an invalid Device ID.
Global Const NIMC_availableForUse86               = -70086 ' None
Global Const NIMC_availableForUse87               = -70087 ' None
Global Const NIMC_availableForUse88               = -70088 ' None
Global Const NIMC_availableForUse89               = -70089 ' None
Global Const NIMC_availableForUse90               = -70090 ' None
Global Const NIMC_availableForUse91               = -70091 ' None
Global Const NIMC_DSPXmitDataError                = -70092 ' Internal error.  The data returned by the DSP is incomplete or corrupt.
Global Const NIMC_DSPCommunicationsError          = -70093 ' Internal error. A command from the onboard microprocessor to the DSP was 
                                                           ' corrupt and ignored.
Global Const NIMC_DSPMessageBufferEmptyError      = -70094 ' Internal Error.  This is an internal message to indicate that the there is 
                                                           ' no more data in the internal message buffer.
Global Const NIMC_DSPCommunicationsTimeoutError   = -70095 ' Internal error.  There was an internal timeout while sending commands to 
                                                           ' the DSP.
Global Const NIMC_passwordError                   = -70096 ' The password used for this function is incorrect.
Global Const NIMC_mustOnMustOffConflictError      = -70097 ' There is a conflict between the mustOn and mustOff values set for this 
                                                           ' function.
Global Const NIMC_reserved98Error                 = -70098 ' Was BAD_IO_DIR_ERROR
Global Const NIMC_reserved99Error                 = -70099 ' Was OUTCOMP_ERROR - Counter/timer breakpoint is set for an invalid I/O bit.
Global Const NIMC_IOEventCounterError             = -70100 ' Problem with the I/O Event Counter.
Global Const NIMC_reserved101Error                = -70101 ' Was PAI_TM_ERROR - Problem with the I/O timer gate.
Global Const NIMC_wrongIODirectionError           = -70102 ' The I/O bit configuration does not agree with its port's direction setting.
Global Const NIMC_wrongIOConfigurationError       = -70103 ' I/O bit configuration is not possible for that pin.
Global Const NIMC_outOfEventsError                = -70104 ' Internal error.  The number of events pending have reached the maximum 
                                                           ' allowed.
Global Const NIMC_availableForUse105              = -70105 ' None
Global Const NIMC_outputDeviceNotAssignedError    = -70106 ' No DAC or stepper output is assigned to this axis.
Global Const NIMC_availableForUse107              = -70107 ' None
Global Const NIMC_PIDUpdateRateError              = -70108 ' PID rate specified is too fast for the number of axes and/or encoders 
                                                           ' enabled.
Global Const NIMC_feedbackDeviceNotAssignedError  = -70109 ' No primary feedback device (encoder or ADC) is assigned to a servo or 
                                                           ' closed-loop stepper axis.
Global Const NIMC_reserved110Error                = -70110 ' None - was duplicate - same as 106 -NIMC_outputDeviceNotAssignedError
Global Const NIMC_availableForUse111              = -70111 ' None
Global Const NIMC_availableForUse112              = -70112 ' None
Global Const NIMC_noMoreRAMError                  = -70113 ' No RAM available for object (program or buffer) storage.
Global Const NIMC_reserved114Error                = -70114 ' Was NIMC_nestedProgramError.
Global Const NIMC_jumpToInvalidLabelError         = -70115 ' A Jump to Label on Condition function in a program had an invalid label.
Global Const NIMC_invalidConditionCodeError       = -70116 ' Condition selected is invalid.
Global Const NIMC_homeLimitNotEnabledError        = -70117 ' Find Home function cannot execute because the Home and/or Limit inputs are 
                                                           ' not enabled.
Global Const NIMC_findHomeError                   = -70118 ' Find home was not successful because the motor stopped before the find 
                                                           ' home switch was found.
Global Const NIMC_limitSwitchActiveError          = -70119 ' The desired move cannot be completed because the limit input is active in 
                                                           ' the direction of travel.
Global Const NIMC_availableForUse120              = -70120 ' None
Global Const NIMC_positionRangeError              = -70121 ' Absolute target position loaded would cause the move length to be out of 
                                                           ' the +/-31 bit range allowed for a single move segment.
Global Const NIMC_encoderDisabledError            = -70122 ' The encoder is disabled.  The encoder must be enabled to read it.
Global Const NIMC_moduloBreakpointError           = -70123 ' The breakpoint value loaded exceeds the modulo range.
Global Const NIMC_findIndexError                  = -70124 ' Find Index sequence did not find the index successfully.
Global Const NIMC_wrongModeError                  = -70125 ' The function was not executed because it was attempted at an illegal time.
Global Const NIMC_axisConfigurationError          = -70126 ' An axis cannot be configured while enabled.  Disable the axis and then 
                                                           ' configure it.
Global Const NIMC_pointsTableFullError            = -70127 ' The points table for cubic splining is full.
Global Const NIMC_available128Error               = -70128 ' Was used for TABLE_ALLOC_ERROR but NIMC_noMoreRAMError should be used 
                                                           ' instead.
Global Const NIMC_axisDisabledError               = -70129 ' A disabled axis has been commanded to move.  Enable the axis before 
                                                           ' executing a move on it.
Global Const NIMC_memoryRangeError                = -70130 ' An invalid memory location is being addressed on the controller.
Global Const NIMC_inPositionUpdateError           = -70131 ' Internal error.  The axis position could not be read for in-position 
                                                           ' verification.
Global Const NIMC_targetPositionUpdateError       = -70132 ' Internal error.  The DSP was too busy to update the target position.
Global Const NIMC_pointRequestMissingError        = -70133 ' Internal error.  The internal points request buffer is missing a request.
Global Const NIMC_internalSamplesMissingError     = -70134 ' Internal error.  The internal samples buffer is missing samples.
Global Const NIMC_reserved135Error                = -70135 ' Was NIMC_axisEnabledError
Global Const NIMC_eventTimeoutError               = -70136 ' A wait operation timed out or a read function timed out.
Global Const NIMC_objectReferenceError            = -70137 ' An attempt was made to reference a nonexistent program object or buffer 
                                                           ' object.  Or, the object number is already in use by an object of a 
                                                           ' different type.  Choose a different object number, or free/delete the 
                                                           ' object currently owning that object number.
Global Const NIMC_outOfMemoryError                = -70138 ' Not enough FLASH ROM space to save this object (program or buffer).
Global Const NIMC_registryFullError               = -70139 ' Object registry is full.  The number of programs and buffers has reached 
                                                           ' the limit.  Free some programs or buffers from RAM or ROM using the Object 
                                                           ' Memory Management function.
Global Const NIMC_noMoreProgramPlayerError        = -70140 ' All program players (maximum 10) are in use storing/playing programs.
Global Const NIMC_programOverruleError            = -70141 ' A Start, Blend, Find Home, or Find Index function being executed from an 
                                                           ' onboard program has been overruled by a Stop Motion function from the host 
                                                           ' computer. The program is left in the PAUSED state.  Execute the 
                                                           ' Pause/Resume Program function to continue.
Global Const NIMC_followingErrorOverruleError     = -70142 ' A Start, Blend, Find Home, or Find Index function being executed from an 
                                                           ' onboard program has been overruled due to a following error condition.  
                                                           ' The program is left in the PAUSED state.  Execute the Pause/Resume Program 
                                                           ' function to continue.
Global Const NIMC_reserved143Error                = -70143 ' None - Was used for I/O interrupt stuck on condition.
Global Const NIMC_illegalVariableError            = -70144 ' An illegal general-purpose variable is being used.
Global Const NIMC_illegalVectorSpaceError         = -70145 ' The vector space being used does not have enough axes assigned to it.
Global Const NIMC_noMoreSamplesError              = -70146 ' There are no samples to read.  Execute Acquire Trajectory Data before 
                                                           ' trying to read samples.
Global Const NIMC_slaveAxisKilledError            = -70147 ' Gearing cannot be enabled because the slave axis is in a killed state.  
                                                           ' Issue a halt stop or decel stop with the Stop Motion function on the slave 
                                                           ' axis to energize it.
Global Const NIMC_ADCDisabledError                = -70148 ' The ADC is disabled.  The ADC channel must be enabled to read it.
Global Const NIMC_operationModeError              = -70149 ' Axes that are a part of a vector space are either in velocity mode or have 
                                                           ' different operation modes.
Global Const NIMC_followingErrorOnFindHomeError   = -70150 ' Find Home sequence did not find home successfully because the axis tripped 
                                                           ' on following error.
Global Const NIMC_invalidVelocityError            = -70151 ' The vector velocity is not valid.  The resulting angular velocity is out 
                                                           ' of range.  Change the vector velocity for the arc move.
Global Const NIMC_invalidAccelerationError        = -70152 ' The vector acceleration is not valid.  The resulting angular acceleration 
                                                           ' is out of range.  Change the vector acceleration for the arc move.
Global Const NIMC_samplesBufferFullError          = -70153 ' Internal error.  The internal samples buffer is full. 
Global Const NIMC_illegalVectorError              = -70154 ' The input or return vector being used is invalid.
Global Const NIMC_QSPIFailedError                 = -70155 ' Internal error.  The internal QSPI serial bus failed and ADC values cannot 
                                                           ' be read.
Global Const NIMC_reserved156Error                = -70156 ' None - Duplicate - same as Error 27
Global Const NIMC_pointsBufferFullError           = -70157 ' Internal error.  The internal point request buffer is full. 
Global Const NIMC_axisInitializationError         = -70158 ' Internal Error.  The internal axis data structures failed to initialize.  
                                                           ' Reset the controller and try again. If the problem persists, contact 
                                                           ' National Instruments technical support.
Global Const NIMC_encoderInitializationError      = -70159 ' Internal Error.  The internal encoder data structures failed to 
                                                           ' initialize.  Reset the controller and try again. If the problem persists, 
                                                           ' contact National Instruments technical support.
Global Const NIMC_stepChannelInitializationError  = -70160 ' Internal Error.  The internal stepper output data structures failed to 
                                                           ' initialize.  Reset the controller and try again. If the problem persists, 
                                                           ' contact National Instruments technical support.
Global Const NIMC_blendFactorConflictError        = -70161 ' Axes, which are part of a vector space, have different blend factors.  
                                                           ' Make sure that all the axes in the vector space have the same blend 
                                                           ' factor.
Global Const NIMC_torqueOffsetError               = -70162 ' The torque offset is outside of the torque limit range.
Global Const NIMC_invalidLimitRangeError          = -70163 ' The negative (lower) limit is greater than or equal to the positive 
                                                           ' (upper) limit.
Global Const NIMC_ADCConfigurationError           = -70164 ' ADCs cannot be enabled or disabled while axes are enabled with analog 
                                                           ' feedback.  Also, ADC ranges cannot be changed on ADC channels being used 
                                                           ' for analog feedback while axes are enabled.


Global Const  NIMC_badDeviceOrAxisError =  NIMC_badResourceIDOrAxisError
