#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <math.h>
#include <complex.h>
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





#define SUCCESS 0
#define FAILURE -1
#define fftbuffer_size 4096 //Resolution 58.823528mhz/8192 

#define  ArrayNumber 4



struct threadarg
{
int            fftlength  ;
double         bandwidth ;
double         frequence ;//HZ
int 	       work_mode ;//2:wideband wave    1: single wave    0:test mode

int            Currentarraydirection;// 0 horital  1 vertical
unsigned int   ad9226fftdata[fftbuffer_size*4*2];

double  packet ;
};
struct detectanglearg{
int mode ;//2:wideband wave    1: single wave    0:test mode
int Currentarraydirection; //0 horital  1 vertical
double correctphase[2][3];//存储相位校准角度
double frequence ;
double bandwidth;
};
/*定义队列结构*/
typedef struct QueueInfo
{
   struct threadarg parm; /*队列存储的数据*/
   struct QueueInfo *next; /*指向队列的下一个元素*/
};
#endif
