//MQ-2
#include "OSAL.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"

#include "MyApp.h"
#include "DebugTrace.h"


#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"


#define DIGITAL_IN P0_4
#define ANALOG_IN P0_6

const cId_t MyApp_ClusterList[MYAPP_MAX_CLUSTERS] =
{
  MYAPP_CLUSTERID
};

const SimpleDescriptionFormat_t MyApp_SimpleDesc =
{
  MYAPP_ENDPOINT,              //  int Endpoint;
  MYAPP_PROFID,                //  uint16 AppProfId[2];
  MYAPP_DEVICEID,              //  uint16 AppDeviceId[2];
  MYAPP_DEVICE_VERSION,        //  int   AppDevVer:4;
  MYAPP_FLAGS,                 //  int   AppFlags:4;
  0,      
  (cId_t *)NULL,  
  MYAPP_MAX_CLUSTERS,          //  byte  AppNumInClusters;
  (cId_t *)MyApp_ClusterList   //  byte *pAppInClusterList;
};

endPointDesc_t MyApp_epDesc;
byte MyApp_TaskID; 
byte MyApp_TransID; 
devStates_t MyApp_NwkState;

void MyApp_MessageMSGCB( afIncomingMSGPacket_t *pckt );
void MyApp_SendTheMessage( void );
void MyApp_SendTheMessage_BC( void );

void MyApp_Init( byte task_id )
{
  halUARTCfg_t uartConfig;
  MyApp_TaskID = task_id;
  MyApp_NwkState = DEV_INIT;
  MyApp_TransID = 0;
  P0SEL &= 0x50; //P0_4//P0_6
  P0DIR &= 0x50;
  // Fill out the endpoint description.
  MyApp_epDesc.endPoint = MYAPP_ENDPOINT;
  MyApp_epDesc.task_id = &MyApp_TaskID;
  MyApp_epDesc.simpleDesc
            = (SimpleDescriptionFormat_t *)&MyApp_SimpleDesc;
  MyApp_epDesc.latencyReq = noLatencyReqs;

  // Register the endpoint description with the AF
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
        case ZDO_STATE_CHANGE:
          MyApp_NwkState = (devStates_t)(MSGpkt->hdr.status);
          if (MyApp_NwkState == DEV_END_DEVICE)
          {
            osal_set_event(MyApp_TaskID, SEND_DATA_EVENT);
          }
          break;

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
  
  if( events & SEND_DATA_EVENT )
  {
    MyApp_SendTheMessage_BC();
    osal_start_timerEx(MyApp_TaskID, SEND_DATA_EVENT, 1000);
    return (events ^ SEND_DATA_EVENT);
  }
  // Discard unknown events
  return 0;
}

void MyApp_SendTheMessage()
{
  byte state,str[3];
  if(DIGITAL_IN == 1)
  {
    state = 1 + 0x30;
  }
  else
  {
    MicroWait(10000);
    if(DIGITAL_IN == 1)
    {
      state = 0 + 0x30;
    }
  }
  osal_memcpy(str, "M", 1); 
  osal_memcpy(&str[1], (byte *)state, 2); 
  
  afAddrType_t my_DstAddr;
  my_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
  my_DstAddr.endPoint = MYAPP_ENDPOINT;
  my_DstAddr.addr.shortAddr = 0x0000;
  AF_DataRequest( &my_DstAddr, &MyApp_epDesc,
                       MYAPP_MQ_CLUSTERID,
                       sizeof(str),
                       str,
                       &MyApp_TransID,
                       AF_DISCV_ROUTE, 
                       AF_DEFAULT_RADIUS );
}

void MyApp_MessageMSGCB( afIncomingMSGPacket_t *pkt )
{
  char *recvbuf;
  switch ( pkt->clusterId )
  {
    case MYAPP_MQ_CLUSTERID:
      //message
      break;
    case MYAPP_BC_CLUSTERID:
      osal_memcpy(recvbuf, pkt -> cmd.Data, pkt -> cmd.DataLength);
      if(osal_memcpy(recvbuf, "start", 5))
      {
        MyApp_SendTheMessage_BC();
      }
      break;
  }
}
         
void MyApp_SendTheMessage_BC( void )
{
  unsigned char data[] = "MQ-2 GOT";
  afAddrType_t my_DstAddr;
  my_DstAddr.addrMode = (afAddrMode_t)Addr16Bit;
  my_DstAddr.endPoint = MYAPP_ENDPOINT;
  my_DstAddr.addr.shortAddr = 0x0000;
  AF_DataRequest( &my_DstAddr, &MyApp_epDesc,
                       MYAPP_BC_CLUSTERID,
                       osal_strlen( data ) + 1,
                       data,
                       &MyApp_TransID,
                       AF_DISCV_ROUTE, 
                       AF_DEFAULT_RADIUS );
  HalLedSet(HAL_LED_1, HAL_LED_MODE_OFF);
  HalLedSet(HAL_LED_2, HAL_LED_MODE_OFF);
  HalLedSet(HAL_LED_3, HAL_LED_MODE_ON);
}
