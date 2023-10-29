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



#define  SAMPLETIMES  1
#define PI 3.14159265358979
#define SUCCESS 0
#define FAILURE -1
#define fftbuffer_size 4096//Resolution 58.823528mhz/8192 
#define  MAXAIMNUM 1000
#define  ArrayNumber 4
typedef struct Complex
{
   double real;
   double image;
} CPLX;
// typedef struct thread_arg{
//     int num;
//     int fftdata0[fftbuffer_size*4*8+6]; 
// } ;
 struct networkthread_arg{
   int mode;
   int  numbuf1 ;
   int  numbuf2 ;
   int  numbuf3 ;
   int  numbuf4 ;
    int  numbuf5 ;
    int mode0valid;
    int mode1valid;
    int mode2valid;
    int mode3valid;
    double maxagp ;
    char buf1[fftbuffer_size*2+200];
	char buf2[fftbuffer_size*2+200];
	char buf3[fftbuffer_size*2+200];
	char buf4[fftbuffer_size*2+200];
    char buf5[fftbuffer_size*2+200];
  
} ;
typedef struct{
   int mode;
   int  numbuf1 ;
   int  numbuf2 ;
   int  numbuf3 ;
   int  numbuf4 ;
    int  numbuf5 ;
    double maxagp ;
    char buf1[fftbuffer_size*2+100];
	char buf2[fftbuffer_size*2+100];
	char buf3[fftbuffer_size*2+100];
	char buf4[fftbuffer_size*2+100];
    char buf5[fftbuffer_size*2+100];
} networkthreadarg;
struct threadarg
{
int            fftlength  ;
double         bandwidth ;
double         frequence ;//HZ
int 	       work_mode ;//2:wideband wave    1: single wave    0:test mode

int            Currentarraydirection;// 0 horital  1 vertical
short   ad9226fftdata[fftbuffer_size*4*8+100];

double  packet ;
};
struct detectanglearg{
int mode ;//   1: single wave 单音信号 测向模式    0:相位差校准 mode   2:宽带信号测向模式  3  Unattended mode无人值守模式
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
/*定义队列结构*/
typedef struct networkQueueInfo
{
   struct networkthread_arg parm; /*队列存储的数据*/
   struct networkQueueInfo *next; /*指向队列的下一个元素*/
};
#endif
