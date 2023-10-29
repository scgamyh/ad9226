#ifndef THREADWORK_H_
#define THREADWORK_H_

#define  LocalRecverPort 5555
#define  Channel1dataport 60000
#define  Channel2dataport 60001
#define  Channel3dataport 60002
#define  Channel4dataport 60003

#define  Channel1maxholddataport 60004
#define  Channel2maxholddataport 60005
#define  Channel3maxholddataport 60006
#define  Channel4maxholddataport 60007

#define axigpioctrl_baseddr         0x41200000


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>         
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h> 
#include <unistd.h> 
#include <netdb.h>  
#include <net/if.h>  
#include <sys/ioctl.h>  
#include <sys/types.h>  
#include <sys/time.h> 
#include <semaphore.h>
#include "cqueue.h"
#include "global.h"
#include <math.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/mman.h>
#include "libaxidma.h"
#include <net/if.h>  // IFF_RUNNING
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if_arp.h>

#include"threadpool.h"
#include<pthread.h>
#include<stdlib.h>
#include <unistd.h>
#include"networkcqueue.h"
#include"ADF4351.h"
#define GPIOADF4351EN_baseddr         0x41200000
#define GPIOATEENCTRL_baseddr         0x41220000
#define GPIORESERVE_baseddr           0x41210000
#define AD9226_SAMPLENUM (10000)
#define   C 299792458
#define FFTCORRECTVALUE 0
#define BASEDISTANCE 0.028497381939
#define  PI 3.1415926
#define  MidFre 5263600000.00
#define MidBandwidth 58823528.00
#define WindowsSize 6
extern int issamp;
extern int issendSwitch;
extern sem_t semsamplethread;
extern sem_t semsignaldealthread;
extern bool sampflag ;
extern bool signaldealthreadrunflag ;
extern int centerfre;
extern bool  clearmaxholdflag;
extern bool  maxholdstate;
extern struct threadarg ThreadArg;
extern struct QueueInfo * queue;
extern struct networkQueueInfo * networkqueue;
extern int isworkflag;
extern int workmode;
extern int8_t ReferenceThresholdPower;
extern struct detectanglearg detectangle;
extern  int32_t server_sockfd;
extern   uint32_t *GPIO_APPITUDE ;
// 获取本机ip  eth_inf网卡名称 调用方法get_local_ip("apcli0", ip);
char* get_local_ip(const char *eth_inf,char *ip_buf);
void* samplethreadrun(void * arg);
void* getdatathreadrun(void * arg);
void* signaldealthreadrun(void * arg);
void* stateUpdatathreadrun(void * arg);
void* recviverstateUpdatathreadrun(void * arg);
void* CHECKRECVIVERrun(void * arg);
void setRadio_mid_appitude();
int check_nic(char *nic_name);
void* networkthread(void * arg);
#endif
