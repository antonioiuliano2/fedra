#ifndef _SYSAL_CONNECTION_
#define _SYSAL_CONNECTION_

#define SYSAL_MAX_CONN_NAME_LEN	64

typedef char ConnectionName[SYSAL_MAX_CONN_NAME_LEN];

#define SYSAL_CONN_TYPE_NULL		0
#define SYSAL_CONN_TYPE_INTERFACE	1

typedef int HSySalHANDLE;

typedef struct
{
	int Type;
	ConnectionName Name;
//	IUnknown *pUnknown;
	} SySalConnection;

#define SYSAL_ASYNC_STATUS_IDLE		0
#define SYSAL_ASYNC_STATUS_RUNNING	1
#define SYSAL_ASYNC_STATUS_PAUSED	2

#define SYSAL_READYSTATUS_READY						0
#define SYSAL_READYSTATUS_UNSUPPORTED_INTERFACE		1
#define SYSAL_READYSTATUS_INCOMPLETE_CONFIG			2

#endif
