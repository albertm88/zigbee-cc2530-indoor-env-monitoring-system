#ifndef MYAPP_H
#define MYAPP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZComDef.h"

#define MYAPP_ENDPOINT           10

#define MYAPP_PROFID             0x0F04
#define MYAPP_DEVICEID           0x0001
#define MYAPP_DEVICE_VERSION     0
#define MYAPP_FLAGS              0

#define MYAPP_MAX_CLUSTERS       5
#define MYAPP_CLUSTERID           1
#define MYAPP_TH_CLUSTERID       2
#define MYAPP_MQ_CLUSTERID       3
#define MYAPP_LT_CLUSTERID       4
#define MYAPP_PM_CLUSTERID       5
   

// Send Message Timeout
#define MYAPP_SEND_MSG_TIMEOUT   5000     // Every 5 seconds

// Application Events (OSAL) - These are bit weighted definitions.
#define SEND_DATA_EVENT          0x01
#define SEND_TO_ALL_EVENT        0x02

     
extern void MyApp_Init( byte task_id );

extern UINT16 MyApp_ProcessEvent( byte task_id, UINT16 events );

#ifdef __cplusplus
}
#endif

#endif /* MYAPP_H */
