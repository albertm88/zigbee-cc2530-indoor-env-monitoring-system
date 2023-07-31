#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "MyApp.h"
#include "DebugTrace.h"
#include "OSAL_Nv.h"
#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"
#include "LCD.h"

const cId_t MyApp_ClusterList[MYAPP_MAX_CLUSTERS] =
{
  MYAPP_TH_CLUSTERID,
  MYAPP_MQ_CLUSTERID,
  MYAPP_LT_CLUSTERID,
  MYAPP_PM_CLUSTERID
};

const SimpleDescriptionFormat_t MyApp_SimpleDesc =
{
  MYAPP_ENDPOINT,              //  int Endpoint;
  MYAPP_PROFID,                //  uint16 AppProfId[2];
  MYAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  MYAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  MYAPP_FLAGS,                 //  int   AppFlags:4;
  MYAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)MyApp_ClusterList,  //  byte *pAppInClusterList;
  0,         
  (cId_t *)NULL   
};

endPointDesc_t MyApp_epDesc;
devStates_t MyApp_NwkState;
byte MyApp_TaskID; 
byte MyApp_TransID; 

void MyApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void MyApp_SendTheMessage_BC( void );

void MyApp_Init( byte task_id )
{
  halUARTCfg_t uartConfig;
  MyApp_TaskID = task_id;
  MyApp_TransID = 0;

  MyApp_epDesc.endPoint = MYAPP_ENDPOINT;
  MyApp_epDesc.task_id = &MyApp_TaskID;
  MyApp_epDesc.simpleDesc = (SimpleDescriptionFormat_t *)&MyApp_SimpleDesc;
  MyApp_epDesc.latencyReq = noLatencyReqs;

  afRegister( &MyApp_epDesc );
  //���ڳ�ʼ��
  uartConfig.configured     = TRUE;
  uartConfig.baudRate       = HAL_UART_BR_115200;
  uartConfig.flowControl    = FALSE;
  uartConfig.callBackFunc   = NULL;
  HalUARTOpen(0, &uartConfig);
}

UINT16 MyApp_ProcessEvent( byte task_id, UINT16 events )
{
  afIncomingMSGPacket_t *MSGpkt;
  if ( events & SYS_EVENT_MSG )
  {
    MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( MyApp_TaskID );
    while ( MSGpkt )
    {
      switch ( MSGpkt->hdr.event )
      {

        case AF_INCOMING_MSG_CMD:
          MyApp_MessageMSGCB( MSGpkt );
          break;
        //case ZDO_STATE_CHANGE:
        //  MyApp_NwkState = (devStates_t)(MSGpkt -> hdr.status);
        //  if(MyApp_NwkState == DEV_ZB_COORD)
        //  {
        //    osal_start_timerEx(MyApp_TaskID, SEND_TO_ALL_EVENT, 5000);
        //  } 
        //  break;
        default:
          break;
      }

      // Release the memory
      osal_msg_deallocate( (uint8 *)MSGpkt );

      // Next
      MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( MyApp_TaskID );
    }

    // return unprocessed events
    return (events ^ SYS_EVENT_MSG);
  }
  
  //if(events & SEND_TO_ALL_EVENT)
  //{
  //     MyApp_SendTheMessage_BC();
  //     osal_start_timerEx(MyApp_TaskID, SEND_TO_ALL_EVENT, 5000);
  //     return (events ^ SEND_TO_ALL_EVENT);
  //}

  return 0;
}

void MyApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  char buf[20];
  switch ( pkt->clusterId )
  {
    case MYAPP_CLUSTERID:

      break;
    case MYAPP_TH_CLUSTERID:
      osal_memcpy(buf, pkt->cmd.Data, pkt->cmd.DataLength);
      
      HalUARTWrite(0, "T&H", 3);
      HalUARTWrite(0, pkt->cmd.Data, pkt->cmd.DataLength);
      HalUARTWrite(0, "\n", 1);
      break;
    case MYAPP_MQ_CLUSTERID:
      osal_memcpy(buf, pkt->cmd.Data, pkt->cmd.DataLength);
    
      HalUARTWrite(0, "MQ2", 3);
      HalUARTWrite(0, pkt->cmd.Data, pkt->cmd.DataLength);
      HalUARTWrite(0, "\n", 1);
      break;
    case MYAPP_PM_CLUSTERID:
      osal_memcpy(buf, pkt->cmd.Data, pkt->cmd.DataLength);
      
      HalUARTWrite(0, "PM", 3);
      HalUARTWrite(0, pkt->cmd.Data, pkt->cmd.DataLength);
      HalUARTWrite(0, "\n", 1);
      break;
    case MYAPP_LT_CLUSTERID:
      osal_memcpy(buf, pkt->cmd.Data, pkt->cmd.DataLength);
      
      HalUARTWrite(0, "LT", 3);
      HalUARTWrite(0, pkt->cmd.Data, pkt->cmd.DataLength);
      HalUARTWrite(0, "\n", 1);
      break;
  }
}


//void MyApp_SendTheMessage( void )
//{
//  char data[] = "";
//  afAddrType_t my_DstAddr;
//  AF_DataRequest( &my_DstAddr, &MyApp_epDesc,
//                       MYAPP_CLUSTERID,
//                       osal_strlen( data ) + 1,
//                       data,
//                       &MyApp_TransID,
//                       AF_DISCV_ROUTE, 
//                       AF_DEFAULT_RADIUS );
//}

