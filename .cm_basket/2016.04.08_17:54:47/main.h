//--------------------------------------------------------------
//  main.h
//--------------------------------------------------------------
//HTTP-Server (per PHY : DP83848C)
//IP-Stack = lwIP
//
//RMII
//
//            PA1  = RMII_Ref_Clk
//            PA2  = ETH_MDIO
//            PA7  = RMII_CRS_DV
//            PB11 = RMII_TX_EN
//            PB12 = RMII_TXD0
//            PB13 = RMII_TXD1
//            PB14 = RMII_INT
//            PC1 =  ETH_MDC
//            PC4 =  RMII_RXD0
//            PC5 =  RMII_RXD1
//


#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
 extern "C" {
#endif
//--------------------------------------------------------------
// Includes

#include "genos/debug/debug_print.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "stm32f4x7_eth_bsp.h"
//#include "httpd.h"
#include "stm32f4x7_eth.h"
//#include "netconf.h"
//#include "stm32f4_discovery.h"
//#include "stm32f4xx_it.h"


//#define USE_DHCP
//--------------------------------------------------------------
// MAC-Adresse (02:00:00:00:00:00)
//--------------------------------------------------------------
#define MAC_ADDR0   2
#define MAC_ADDR1   0
#define MAC_ADDR2   0
#define MAC_ADDR3   0
#define MAC_ADDR4   0
#define MAC_ADDR5   0

//--------------------------------------------------------------
// statische IP-Adresse (192.168.1.110)
// (falls DHCP enable wird diese ignoriert)
//--------------------------------------------------------------
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   110

//--------------------------------------------------------------
// Netzmsaske (255.255.255.0)
//--------------------------------------------------------------
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

//--------------------------------------------------------------
// Gateway Adresse (192.168.1.1)
//--------------------------------------------------------------
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1


typedef enum {
  HTTP_SERVER_OK =0,
  HTTP_SERVER_ETH_MACDMA_ERR,
  HTTP_SERVER_ETH_PHYINT_ERR
}HTTP_SERVER_STATUS;

void Time_Update(void);


#ifdef __cplusplus
}
#endif
//--------------------------------------------------------------
#endif // __MAIN_H___
