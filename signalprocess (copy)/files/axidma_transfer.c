#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h> 
#include <sys/mman.h>
#include "libaxidma.h"
 
#define AD9226_SAMPLENUM (8200)

 

//axidma operate
axidma_dev_t g_AxiDmaDev;
array_t* g_apRxChannels;
void* g_pAD9226DataBuf;

 
 
void callbackAfterRecive(int channelid,void* data)
{
   printf("INFO: callback func triggerd,channelid: %d\n",channelid);
usleep(1);
 //for(int i = 0;i < 10;i++)
  //  {

//	for(int z =0;z<4;z++)
    //    printf("SAMPLE NUM:%d,channel id:%d,real:%d ,imag:%d \r\n",i,z,(int)((*((unsigned int*)(g_pAD9226DataBuf)+i*8+z*2))),(int)((*((unsigned int*)//(g_pAD9226DataBuf)+i*8+z*2+1))));
  //  }
  //  for(int i = 4090;i < 4120;i++)
  //  {

//	for(int z =0;z<4;z++)
   //     printf("SAMPLE NUM:%d,channel id:%d,real:%d ,imag:%d \r\n",i,z,(int)((*((unsigned int*)(g_pAD9226DataBuf)+i*8+z*2))),(int)((*((unsigned int*)(g_pAD9226DataBuf)+i*8+z*2+1))));
 //   }
  //  printf("\n");
}
 
int main()
{
    //init axidma device
    g_AxiDmaDev = axidma_init();
    if(g_AxiDmaDev == NULL)
    {
        printf("Error: can not initialize the AXI DMA device\n");
        return 0;
    }
 
    //get avaliable rx channels
    g_apRxChannels = axidma_get_dma_rx(g_AxiDmaDev);
    if(g_apRxChannels->len < 1)
    {
        printf("Error: no receive channels found\n");
        return 0;
    }
 
    //print avaliable channels
    for(int i = 0; i < g_apRxChannels->len;i++)
    {
        printf("INFO: receive channel ID: %d\n",g_apRxChannels->data[i]);
    }
 
  
  
 
    //start DMA recive
    //per channel data num = 1920 ,per data 16bit,total has 8 adc channel data
    g_pAD9226DataBuf = axidma_malloc(g_AxiDmaDev,AD9226_SAMPLENUM*2*8*2);
    printf("the init dma recive buf:\n");
    printf("\n");
 
    printf("preper to start adc\n");
    axidma_stop_transfer(g_AxiDmaDev,g_apRxChannels->data[0]);
    axidma_set_callback(g_AxiDmaDev,g_apRxChannels->data[0],callbackAfterRecive,NULL);
    
 
    printf("the ADC has start...\n");
    
    while(1){
	
	axidma_oneway_transfer(g_AxiDmaDev,g_apRxChannels->data[0],g_pAD9226DataBuf,AD9226_SAMPLENUM*2*8*2,false);
	}
 
   
 
    axidma_destroy(g_AxiDmaDev);
    
    return 0;
 
}
