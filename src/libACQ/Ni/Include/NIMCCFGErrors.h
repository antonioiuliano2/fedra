//------------------------------------------------------------------------------------------
// NIMCCFGErrors.h  -  NI-Motion Configuration Error Codes
//
// Copyright 2000
// National Instruments Corporation.
// All rights reserved.
//
//------------------------------------------------------------------------------------------
#ifndef __NIMCCFGErrors_h__
#define __NIMCCFGErrors_h__
//------------------------------------------------------------------------------------------
#define NIMCCFG_noError										0			//No Error - function was successful
#define NIMCCFG_getFailedError							-78001	//Could not get value from database
#define NIMCCFG_setFailedError							-78002	//Could not set value to database
#define NIMCCFG_irrelevantAttributeError				-78003	//Attribute specified for get/set operation
																				//was not valid (undefined)
#define NIMCCFG_invalidResourceSpecifedError			-78004	//The axis number or resource number such aa
																				//the ADC channel, enoder number etc., for which  
																				//the get/set operation eas specified was not valid
#define NIMCCFG_outOfRangeError							-78005	//The value being persisted to the storage is
																				//out of range
#define NIMCCFG_insufficientSizeError					-78006	//The size of tha array is insufficient to return the
																				//data required
#define NIMCCFG_invalidSettingsNameSpecifiedError  -78007	//An invalid settings name was specified
#define NIMCCFG_createFailedError						-78008	//Failed to create a new logical settings
#define NIMCCFG_deleteFailedError						-78009	//Failed to delete the logical settings specified.  
																				//This may be because the setting is currently being used by a motion device.
#define NIMCCFG_objectInUseError							-78010   //The object being deleted in in use
#define NIMCCFG_commitFailedError						-78011   //The changes could not be persisted to the configuration
#define NIMCCFG_importFailedError						-78012   //The initialization settings could not be imported successfully
#define NIMCCFG_exportFailedError						-78013   //The initialization settings could not be exported successfully

//--------------------------------------------------------------------------------------------
#endif //__NIMCCFGErrors_h__