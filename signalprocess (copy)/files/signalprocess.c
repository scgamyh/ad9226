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
#include "threadwork.h"
#include <stdlib.h>
static volatile uint32_t *GPIO_ATEENCTRL ;
#define MAXDATASIZE 65536

int main()
{

	
   //初始化socket 与上位机进行控制数据通信 
	
    ssize_t len;
  
    struct sockaddr_in my_addr;   //服务器网络地址信息结构体
    struct sockaddr_in remote_addr; //客户端网络地址结构体
    uint32_t sin_size;
    uint8_t buf[MAXDATASIZE];  //数据传送的缓冲区

    memset(&my_addr, 0,sizeof(my_addr)); //数据初始化--清零
    my_addr.sin_family = AF_INET; //设置为IP通信
    my_addr.sin_addr.s_addr = inet_addr("192.168.0.10");//服务器IP地址--允许连接到所有本地地址上
    my_addr.sin_port = htons(LocalRecverPort); //服务器端口号
    /*创建服务器端套接字--IPv4协议，面向无连接通信，UDP协议*/
    if((server_sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket error");
        return -1;
    }

    /*将套接字绑定到服务器的网络地址上*/
    if (bind(server_sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("bind erroraaaaaaaaaaaaaaaaaa");
        return -1;
    }
    sin_size = sizeof(struct sockaddr_in);
	/*****initial resource ***********************/
   
  
    	ThreadArg.fftlength = fftbuffer_size;
   
	detectangle.mode = 0;
	detectangle.Currentarraydirection = 0;
	detectangle.correctphase[0][0] = 0.0;
	detectangle.correctphase[0][1] = 0.0;
	detectangle.correctphase[0][2] = 0.0;
	detectangle.correctphase[1][0] = 0.0;
	detectangle.correctphase[1][1] = 0.0;
	detectangle.correctphase[1][2] = 0.0;
	detectangle.frequence = 5260000000;
	detectangle.bandwidth = 0;//默认带宽1MHz



     issamp = 1;
  

   //init gpio resource 
	
	int gpioateenctrlfd;
	if ((gpioateenctrlfd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0) {
        printf("Unable to open /dev/mem: %s\n", strerror(errno));
        return NULL;
    }

 	GPIO_ATEENCTRL = (uint32_t *) mmap(0, getpagesize() * 250, PROT_READ | PROT_WRITE, MAP_SHARED, gpioateenctrlfd, GPIOATEENCTRL_baseddr);
	
	

   if ((void *) GPIO_ATEENCTRL == MAP_FAILED) {
        printf("mmap GPIO_ATEENCTRL failed: %s\n", strerror(errno));
        exit(1);
    }
	*GPIO_ATEENCTRL = 0xFF;
	sleep(1);

    /******初始化链表队列*********/
 printf("start to work\r\n");  
	queue = createQueue();
	//线程 信号量初始化
    //天线数据采样
    pthread_t threadsample;
    //天线数据处理
    pthread_t threaddatadeal;
    //更新处理机工作状态
     // pthread_t threadStateUpdate;
 
    int ret1;
    int ret2;
    

    ret1 = pthread_create(&threadsample,NULL, &samplethreadrun,NULL);//创建采集线程
    if(ret1 < 0)
    {
        printf("threadsample create error!\n");
        exit(1);
    }
 sleep(1);
    ret2 = pthread_create(&threaddatadeal,NULL, &signaldealthreadrun,NULL);//创建数据处理线程
    if(ret2 < 0)
    {
        printf("threaddatadeal create error!\n");
        exit(1);
    }	
    printf(" waiting for a packet...\n"); 
    /*接收客户端的控制数据对处理系统进行控制   -recvfrom是无连接的*/
	while(1){
	if (((len = recvfrom(server_sockfd, buf, MAXDATASIZE, MSG_DONTWAIT, (struct sockaddr *)&remote_addr, &sin_size)) > 0)) {
		printf(" received packet from:%s,buf:%s,length:%d \r\n",inet_ntoa(remote_addr.sin_addr),buf,sin_size);
		//printf("buf[0]:%d,\r\n",(int)buf[0]);  
        	//常规工作模式  采集通道数据
        	if(((buf[0]&0xFF)==0x55)&&((buf[1]&0xFF)==0xB0))
        	{
            
        	 //	detectangle.mode =3;
         	 	//解析阵列类型
         	 	if((buf[5]&0xFF) == 0x00)//水平阵列
         		{ 
			*GPIO_ATEENCTRL = 0x00;
			detectangle.Currentarraydirection = 0;
                 	usleep(100);


         	 	} else  if((buf[5]&0xFF) == 0x01)//垂直阵列
             		{
		 	*GPIO_ATEENCTRL = 0xFF;
			detectangle.Currentarraydirection = 1;
 			 usleep(100);
			}
           
        	 }else if(((buf[0]&0xFF)==0x55)&&((buf[1]&0xFF)==0xB1))//校准相位
        	{
            
         	detectangle.mode =0;
          	//解析阵列类型
          	if((buf[4]&0xFF) == 0x00)//水平阵列
        	 { 
		*GPIO_ATEENCTRL = 0x00;
		detectangle.Currentarraydirection = 0;
                 usleep(100);

          	} else  if((buf[4]&0xFF) == 0x01)//垂直阵列
             	{
		 *GPIO_ATEENCTRL = 0xFF;
		 detectangle.Currentarraydirection = 1;
 		 usleep(100);
		}
 		detectangle.frequence = ((((unsigned short)buf[6]&0xFF)<<8)|((unsigned short)buf[7]&0xFF))*100000.00;
		detectangle.bandwidth = 0.0;//默认带宽1MHz
		 printf("mode:%d,detectangle.Currentarraydirection:%d,detectangle.frequence:%lf,detectangle.bandwidth:%lf;\r\n",detectangle.mode,detectangle.Currentarraydirection,detectangle.frequence,detectangle.bandwidth);
         	}
		else if(((buf[0]&0xFF)==0x55)&&((buf[1]&0xFF)==0xB2))//
       		 {
            
        
         	 //解析阵列类型
         	 if((buf[4]&0xFF) == 0x00)//水平阵列
        	 { 
			*GPIO_ATEENCTRL = 0x00;
			detectangle.Currentarraydirection = 0;
                	 usleep(1000);

         	 } else  if((buf[4]&0xFF) == 0x01)//垂直阵列
             {
		 *GPIO_ATEENCTRL = 0xFF;
		detectangle.Currentarraydirection = 1;
 		 usleep(1000);
		}
	 if((buf[5]&0xFF) == 0x00)//单音信号 
         { 
		 detectangle.mode =1;
	detectangle.frequence = ((((unsigned short)buf[6]&0xFF)<<8)|((unsigned short)buf[7]&0xFF))*100000.00;
	detectangle.bandwidth = 0.0;//默认带宽1MHz
                 usleep(1000);

          } else  if((buf[5]&0xFF) == 0x01)//宽带信号
             {
		 detectangle.mode =2;
		detectangle.frequence =  ((((unsigned short)buf[6]&0xFF)<<8)|((unsigned short)buf[7]&0xFF))*100000.00;
		detectangle.bandwidth = (buf[8]&0xFF)*1000000;//默认带宽1MHz
 		 usleep(1000);
		
		}

            printf("mode:%d,detectangle.Currentarraydirection:%d,detectangle.frequence:%lf,detectangle.bandwidth:%lf;\r\n",detectangle.mode,detectangle.Currentarraydirection,detectangle.frequence,detectangle.bandwidth);
         }
	printf("exit recv \r\n");
    
	}
	usleep(1);
}


	pthread_join(threadsample,NULL); //等待线程1退出
    pthread_join(threaddatadeal,NULL);//等待线程2退出
  
    if(sem_destroy(&semsamplethread) == -1) //删除信号量
    {
        printf("delete semsamplethread error!\n");
        exit(1);
    }
    
    if(sem_destroy(&semsignaldealthread) == -1) //删除信号量
    {
        printf("delete semsignaldealthread error!\n");
        exit(1);
    }
	
	
	return 0;
}



