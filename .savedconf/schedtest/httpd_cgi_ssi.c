/**
  ******************************************************************************
  * @file    httpd_cg_ssi.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Webserver SSI and CGI handlers
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <string.h>
#include <stdlib.h>
#include <stm32f4_discovery.h>

#include "lwip/debug.h"
#include "httpd.h"
#include "lwip/tcp.h"
#include "fs.h"
#include "main.h"
//#include "stm32_ub_http_server.h"


uint16_t SerA [20]={2048, 2681, 3252, 3705, 3996, 4098, 3996, 3705, 3252, 2681, 2048, 1415, 844,   391,  100,   0,    100,  391,  844,  1415};
//				    0800  0A79  0CB4  0E79  0F9C  1002  0F9C  0E79  0CB4  0A79  0800  0587  034C   0187  0064   0     0064  0187  034C  0587
uint16_t SerB [20]={274,  45,   10,   176,  526,  1024, 1622, 2263, 2882, 3418, 3821, 4050, 4086,  3920, 3570,  3072, 2474, 1833, 1214, 678};
//				   0112  002D  000A   00B0  020E  0400  0656  08D7  0B42  0D5A  0EED  0FD2  0FF6   0F50  0DF2   0C00  09AA  0729  04BE  02A6

uint16_t Dref [4]={0, 0, 0, 0};
uint8_t DdigitalL =0b00000000;
uint8_t DdigitalH =0b00000000;

/* we will use character "t","u"  as tag for CGI */
char const* TAGCHAR="t";
char const* TAGCHAR2="u";
char const** TAGS= { &TAGCHAR, &TAGCHAR2 };

uint16_t   nPacket=0;

/* CGI handler for LED control */ 
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

/* Html request for "/leds.cgi" will start LEDS_CGI_Handler */
const tCGI LEDS_CGI={"/leds.cgi", LEDS_CGI_Handler};

/* Cgi call table, only one CGI used */
tCGI CGI_TAB[1];

/************************************************************************
  * @brief  Data_Handler : SSI handler for Rx page HTML
  */
u16_t Data_Handler(int iIndex, char *pcInsert, int iInsertLen)
{
  if (iIndex ==0)
  {
    char DigitA1=0, DigitA2=0, DigitA3=0, DigitA4=0;
    char DigitB1=0, DigitB2=0, DigitB3=0, DigitB4=0;
    uint32_t Da, Db;
    uint32_t Mi=0;
    int i=0;
    nPacket++;  if (nPacket<9998) {Dref[2]=nPacket;}

    while (i<23)
    {
    if (i<20) { Da= SerA[i];  Db= SerB[i]; }
    if (i==20) { Da=  Dref[0];  Db= Dref[1];}
    if (i==21) { Da=  Dref[2];  Db=  Dref[3];}
    if (i==22) { Da= DdigitalH;  Db= DdigitalL;}


   //--  prepare data to be inserted in html
     DigitA1= Da/1000;
     DigitA2= (Da-( DigitA1*1000))/100 ;
     DigitA3= (Da-(( DigitA1*1000)+(DigitA2*100)))/10;
     DigitA4=  Da -(( DigitA1*1000)+(DigitA2*100)+ (DigitA3*10));

     DigitB1= Db/1000;
     DigitB2= (Db-(DigitB1*1000))/100 ;
     DigitB3= (Db-((DigitB1*1000)+(DigitB2*100)))/10;
     DigitB4=  Db -((DigitB1*1000)+(DigitB2*100)+ (DigitB3*10));

     //--  get digits to display
     *(pcInsert + Mi)   =   (char)(DigitA1+0x30);
     *(pcInsert +(Mi+ 1)) = (char)(DigitA2+0x30);
     *(pcInsert +(Mi+ 2)) = (char)(DigitA3+0x30);
     *(pcInsert +(Mi+ 3)) = (char)(DigitA4+0x30);
     *(pcInsert +(Mi+ 4)) = ' ';
     *(pcInsert +(Mi+ 5)) = (char)(DigitB1+0x30);
     *(pcInsert +(Mi+ 6)) = (char)(DigitB2+0x30);
     *(pcInsert +(Mi+ 7)) = (char)(DigitB3+0x30);
     *(pcInsert +(Mi+ 8)) = (char)(DigitB4+0x30);
     *(pcInsert +(Mi+ 9)) = '\n';

     Mi+=10;  i++;
    }

    /*  need to be inserted in html*/
    return strlen(pcInsert);
  }
  return 0;
}

/*#########################################################################
  * @brief  CGI handler for LEDs control 
  */
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{

 /* We have only one SSI handler iIndex = 0 */
  if (iIndex==0)
  {
    uint32_t i=0;

    /* All leds off */

    LEDOff(LED3);
    LEDOff(LED4);
    LEDOff(LED5);
    LEDOff(LED6);
    //----------------
    DdigitalL=0;   DdigitalH=0;

    /* Check cgi parameter : example GET /leds.cgi?led=2&led=4 */
    for (i=0; i<iNumParams; i++)
    {
      /* check parameter "led" */
      if (strcmp(pcParam[i] , "led")==0)   
      {
        /* switch led1 ON if 1 */
        if(strcmp(pcValue[i], "1") ==0) {
          GPIOD->BSRRL = GPIO_Pin_12;
          DdigitalL|=1;
        }
        /* switch led2 ON if 2 */
        else if(strcmp(pcValue[i], "2") ==0) {
          GPIOD->BSRRL = GPIO_Pin_13;
          DdigitalL|=2;
        }
        /* switch led3 ON if 3 */
        else if(strcmp(pcValue[i], "3") ==0) {
          GPIOD->BSRRL = GPIO_Pin_14;
          DdigitalL|=4;
        }
        /* switch led4 ON if 4 */
        else if(strcmp(pcValue[i], "4") ==0) {
          GPIOD->BSRRL = GPIO_Pin_15;
         DdigitalL|=8;
        }
        else if(strcmp(pcValue[i], "5") ==0) {
                DdigitalL|=16;
        }
        else if(strcmp(pcValue[i], "6") ==0) {
                        DdigitalL|=32;
        }
        else if(strcmp(pcValue[i], "7") ==0) {
                        DdigitalL|=64;
        }

        else if(strcmp(pcValue[i], "8") ==0) {
                        DdigitalL|=128;
        }
      }
        if (strcmp(pcParam[i] , "Var1")==0)
        {Dref[0] = atoi(pcValue[i]); }

        if (strcmp(pcParam[i] , "Var2")==0)  {
        	{Dref[1] = atoi(pcValue[i]); }
        }
    }
//    Dref[2]=nInt;
  }
  /* uri to send after cgi call*/
  return "/STM32F4x7Tx.html";   // STM32F4x7Rx.shtml

}

/**
 * Initialize SSI handlers
 */
void httpd_ssi_init(void)
{  
  /* configure SSI handlers (Tx page SSI) */
  http_set_ssi_handler(Data_Handler, (char const **)TAGS, 1);//1,2
}

/**
 * Initialize CGI handlers
 */
void httpd_cgi_init(void)
{ 
  /* configure CGI handlers (LEDs control CGI) */
  CGI_TAB[0] = LEDS_CGI;
  http_set_cgi_handlers(CGI_TAB, 1);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
