#ifndef _SYSAL_CONFIG_
#define _SYSAL_CONFIG_

#define SYSAL_MAXCONFIG_NAME_LEN	64
#define SYSAL_MAXCONFIG_ENTRY_NAME_LEN	64
#define SYSAL_MAXCONFIG_ENTRY_VALUE_LEN	64

typedef struct
{
	char Name[SYSAL_MAXCONFIG_NAME_LEN];
	int CountOfItems;
	char *pItems;
	} SySalConfig;

#endif
