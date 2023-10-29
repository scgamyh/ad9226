#include"threadwork.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/mman.h>
#include "libaxidma.h"

static volatile uint32_t *gpioled ;
//axidma operate
static axidma_dev_t g_AxiDmaDev;
array_t* g_apRxChannels;
void* g_pAD9226DataBuf;
struct detectanglearg detectangle;
 int32_t server_sockfd;
unsigned int R[6] = {0x580005,0x9A003C,0x4B3,0x4F42,0x8008011,0x4E8000};




bool isenable ;
int issendSwitch ;
int  isconnflag ;
int issamp ;
int workmode ;
int centerfre ;
sem_t semsamplethread;
sem_t semsignaldealthread;

struct threadarg ThreadArg;
struct QueueInfo *queue;
pthread_mutex_t mutex;   

static const char *device = "/dev/spidev1.0";
static uint32_t spimode = 0;
static uint8_t bits = 8;
static uint32_t speed = 20000000;
static uint16_t delay;

uint8_t default_rx[32] = {0 };
uint8_t XOR_check(char *buf,int length)
{
	int i =0;
	uint8_t result = 0;
	for(i = 0;i<length-1;i++)
	{
		result = result^(buf[i]);
	}
	return result;
}
//如果网卡已连上网线，返回0，否则返回-1.
int check_nic(char *nic_name)
{
        struct ifreq ifr;
        int skfd = socket(AF_INET, SOCK_DGRAM, 0);

        strcpy(ifr.ifr_name, nic_name);
        if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
        {
                return -1;
        }
	close(skfd);
        if(ifr.ifr_flags & IFF_RUNNING)
                return 0;  // 网卡已插上网线
else return -1;
}

static void transfer(int fd)
{
	int ret;
	int status;
	struct spi_ioc_transfer xfer;
 	unsigned char buf[32],*bp;
	
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	buf[0] = (R[0]>>24)&0xFF;
	buf[1] = (R[0]>>16)&0xFF;
	buf[2] = (R[0]>>8)&0xFF;
	buf[3] = (R[0])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	sleep(1);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R[1]>>24)&0xFF;
	buf[1] = (R[1]>>16)&0xFF;
	buf[2] = (R[1]>>8)&0xFF;
	buf[3] = (R[1])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	sleep(1);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R[2]>>24)&0xFF;
	buf[1] = (R[2]>>16)&0xFF;
	buf[2] = (R[2]>>8)&0xFF;
	buf[3] = (R[2])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	sleep(1);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R[3]>>24)&0xFF;
	buf[1] = (R[3]>>16)&0xFF;
	buf[2] = (R[3]>>8)&0xFF;
	buf[3] = (R[3])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	sleep(1);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R[4]>>24)&0xFF;
	buf[1] = (R[4]>>16)&0xFF;
	buf[2] = (R[4]>>8)&0xFF;
	buf[3] = (R[4])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	sleep(1);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R[5]>>24)&0xFF;
	buf[1] = (R[5]>>16)&0xFF;
	buf[2] = (R[5]>>8)&0xFF;
	buf[3] = (R[5])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	sleep(1);

	
}
void spiconfig(){
int fd,ret =0;

	fd = open(device, O_RDWR);
	if (fd < 0)
		printf("can't open device\r\n");
	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE32, &spimode);
	if (ret == -1)
		printf("can't set spi mode\r\n");

	ret = ioctl(fd, SPI_IOC_RD_MODE32, &spimode);
	if (ret == -1)
		printf("can't get spi mode \r\n");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		printf("can't set bits per word \r\n");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		printf("can't get bits per word \r\n");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		printf("can't set max speed hz\r\n");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		printf("can't get max speed hz \r\n");

	printf("spi mode: 0x%x\n", spimode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	
	transfer(fd);
	transfer(fd);
	
	
	
      
	
}
void callbackAfterRecive(int channelid,void* data)
{
  // printf("INFO: callback func triggerd,channelid: %d\n",channelid);
			 int listlength =  queue_length(queue);
			//printf("length:%d \r\n",listlength);	
		   if(listlength<10)
	   	   { 
			
			memcpy(ThreadArg.ad9226fftdata,g_pAD9226DataBuf,sizeof(unsigned int)*4096*4);
   			pthread_mutex_lock(&mutex);
   			queue_push(queue, &ThreadArg);
   			pthread_mutex_unlock(&mutex); 
		}
	
}

// 获取本机ip  eth_inf网卡名称 调用方法get_local_ip("apcli0", ip);
char* get_local_ip(const char *eth_inf,char *ip_buf)
{
    int sd;
    struct sockaddr_in sin;
    struct ifreq ifr;
 
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sd)
    {
        printf("socket error: %s\n", strerror(errno));
       // return 0;
    }
 
    strncpy(ifr.ifr_name, eth_inf, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;
 
    // if error: No such device  
    if (ioctl(sd, SIOCGIFADDR, &ifr) < 0)
    {
        printf("ioctl error: %s\n", strerror(errno));
        close(sd);
       // return -1;
    }
    
    printf("interfac: %s, ip: %s\n", eth_inf, inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr)); 
 	strcpy(ip_buf, inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
    close(sd);
    return ip_buf;
}


     //互斥信号量
void* samplethreadrun(void * arg)
{

	//init spi adf4531
	spiconfig();
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
    g_pAD9226DataBuf = axidma_malloc(g_AxiDmaDev,(AD9226_SAMPLENUM)*4*4);
    printf("the init dma recive buf:\n");
    printf("\n");
 
    printf("preper to start adc\n");
    axidma_stop_transfer(g_AxiDmaDev,g_apRxChannels->data[0]);
    axidma_set_callback(g_AxiDmaDev,g_apRxChannels->data[0],callbackAfterRecive,NULL);

 
    printf("the ADC has start...\n");
	
    struct timeval timeStart, timeEnd; 
 	
	while(1)
	{
		// 判断是否工作
        while(( issamp ==1))
		{

        	    usleep(600);
					//read ad9226 data
			axidma_oneway_transfer(g_AxiDmaDev,g_apRxChannels->data[0],g_pAD9226DataBuf,(AD9226_SAMPLENUM)*4*4,false);
					
		   
		}	
	}
}
	
   

void* signaldealthreadrun(void * arg)
{

	
	static int recvpackget = 0;
	//network initial 
	char ipbuf[16] ="";
	char ethinf[5] ="eth0";
	get_local_ip(ethinf,ipbuf);
	//创建一个套结字
	int lfd,ret;
 	struct sockaddr_in my_addr; 
 	memset(&my_addr, 0,sizeof(my_addr)); //数据初始化--清零
    	my_addr.sin_family = AF_INET; //设置为IP通信
    	my_addr.sin_addr.s_addr = inet_addr("192.168.0.10");//
    	my_addr.sin_port = htons(Channel1dataport); //端口号
	//lfd = socket(AF_INET,SOCK_DGRAM,0);
 	if((lfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    	{
       	 perror("socket error");
        return -1;
    	}
	 /*将套接字绑定到服务器的网络地址上*/
    	if (bind(lfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
    	{
        perror("bind erroraaaaaaaaaaaaaaaaaa");
        return -1;
    	}
	//封装套结字地址结构
	struct sockaddr_in saddr1,saddr2,saddr3,saddr4,saddr0;
	//封装套结字地址结构
	//远程主机 
	saddr0.sin_family = AF_INET;
	saddr0.sin_port = htons(LocalRecverPort);
	saddr0.sin_addr.s_addr = inet_addr("192.168.0.20");

	saddr1.sin_family = AF_INET;
	saddr1.sin_port = htons(Channel1dataport);
	saddr1.sin_addr.s_addr = inet_addr("192.168.0.20");

	saddr2.sin_family = AF_INET;
	saddr2.sin_port = htons(Channel2dataport);
	saddr2.sin_addr.s_addr = inet_addr("192.168.0.20");

	saddr3.sin_family = AF_INET;
	saddr3.sin_port = htons(Channel3dataport);
	saddr3.sin_addr.s_addr = inet_addr("192.168.0.20");
	saddr4.sin_family = AF_INET;
	saddr4.sin_port = htons(Channel4dataport);
	saddr4.sin_addr.s_addr = inet_addr("192.168.0.20");
	char buf1[fftbuffer_size*2+100]={0};
	char buf2[fftbuffer_size*2+100]={0};
	char buf3[fftbuffer_size*2+100]={0};
	char buf4[fftbuffer_size*2+100]={0};
    	struct timeval timeStart, timeEnd; 
	double runTime=0;
	struct threadarg data;
	data.fftlength = fftbuffer_size;
	printf("init rsource success\r\n");
	int re_list;
 	while (1)//等待数据更新
 	{

		usleep(20000);
		//printf("init rsource success\r\n");
   		 pthread_mutex_lock(&mutex);
 		 re_list =   queue_pop(queue, &data);
    		 pthread_mutex_unlock(&mutex); 
		//printf("init rsource success\r\n");

		if(re_list == SUCCESS)
		{	double channel1_realtemp[fftbuffer_size] ={0.0};
			double channel1_imagtemp[fftbuffer_size] ={0.0};
			double channel2_realtemp[fftbuffer_size] ={0.0};
			double channel2_imagtemp[fftbuffer_size] ={0.0};
			double channel3_realtemp[fftbuffer_size] ={0.0};
			double channel3_imagtemp[fftbuffer_size] ={0.0};
			double channel4_realtemp[fftbuffer_size] ={0.0};
			double channel4_imagtemp[fftbuffer_size] ={0.0};

			double channel1_real[fftbuffer_size] ={0.0};
			double channel1_imag[fftbuffer_size] ={0.0};
			double channel2_real[fftbuffer_size] ={0.0};
			double channel2_imag[fftbuffer_size] ={0.0};
			double channel3_real[fftbuffer_size] ={0.0};
			double channel3_imag[fftbuffer_size] ={0.0};
			double channel4_real[fftbuffer_size] ={0.0};
			double channel4_imag[fftbuffer_size] ={0.0};
			double channel1_appitude[fftbuffer_size] ={0.0};
			double channel2_appitude[fftbuffer_size] ={0.0};
			double channel3_appitude[fftbuffer_size] ={0.0};
			double channel4_appitude[fftbuffer_size] ={0.0};
			double channel1_timeappitude[fftbuffer_size] ={0.0};
			double channel2_timeappitude[fftbuffer_size] ={0.0};
			double channel3_timeappitude[fftbuffer_size] ={0.0};
			double channel4_timeappitude[fftbuffer_size] ={0.0};
			
			for(int i = 0;i < fftbuffer_size;i++)
    			{

			channel1_realtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+0*2)));

			channel1_imagtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+0*2+1)));
			//channel1_appitude[i] = 20*log10(sqrt(channel1_realtemp[i]*channel1_realtemp[i]+channel1_imagtemp[i]*channel1_imagtemp[i])/fftbuffer_size/6500000.00);
			channel2_realtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+1*2)));
			channel2_imagtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+1*2+1)));
			//channel2_appitude[i] = 20*log10(sqrt(channel2_realtemp[i]*channel2_realtemp[i]+channel2_imagtemp[i]*channel2_imagtemp[i])/fftbuffer_size/6500000.00);
			channel3_realtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+2*2)));
			channel3_imagtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+2*2+1)));
			//channel3_appitude[i] = 20*log10(sqrt(channel3_realtemp[i]*channel3_realtemp[i]+channel3_imagtemp[i]*channel3_imagtemp[i])/fftbuffer_size/6500000.00);
			channel4_realtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+3*2)));
			channel4_imagtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+3*2+1))); 
			//channel4_appitude[i] = 20*log10(sqrt(channel4_realtemp[i]*channel4_realtemp[i]+channel4_imagtemp[i]*channel4_imagtemp[i])/fftbuffer_size/6500000.00);
  			channel1_timeappitude[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*4+0*2)));	
			channel2_timeappitude[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*4+0*2+1)));	
			channel3_timeappitude[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*4+1*2)));
			channel4_timeappitude[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*4+1*2+1)));
}
			
			for(int i =0;i<fftbuffer_size;i++)
			{
				channel1_real[i] = channel1_realtemp[i];
				channel1_imag[i] = channel1_imagtemp[i];
				
				
				channel1_appitude[i] = 20*log10(sqrt(channel1_real[i]*channel1_real[i]+channel1_imag[i]*channel1_imag[i])/fftbuffer_size)-150;
				
				
				channel2_real[i] = channel2_realtemp[i];
				channel2_imag[i] = channel2_imagtemp[i];
				
				channel2_appitude[i] = 20*log10(sqrt(channel2_real[i]*channel2_real[i]+channel2_imag[i]*channel2_imag[i])/fftbuffer_size)-150;
				
				
				channel3_real[i] = channel3_realtemp[i];
				channel3_imag[i] = channel3_imagtemp[i];
				
				channel3_appitude[i] = 20*log10(sqrt(channel3_real[i]*channel3_real[i]+channel3_imag[i]*channel3_imag[i])/fftbuffer_size)-150;
				
				
				channel4_real[i] = channel4_realtemp[i];
				channel4_imag[i] = channel4_imagtemp[i];
				
				channel4_appitude[i] = 20*log10(sqrt(channel4_real[i]*channel4_real[i]+channel4_imag[i]*channel4_imag[i])/fftbuffer_size)-150;
				
			}
			//前面10Mhz数据   以5MHZ为中心互换位置
			double channel1_realcrrectemp[fftbuffer_size] ={0.0};
			double channel1_imagcrrectemp[fftbuffer_size] ={0.0};
			double channel2_realcrrectemp[fftbuffer_size] ={0.0};
			double channel2_imagcrrectemp[fftbuffer_size] ={0.0};
			double channel3_realcrrectemp[fftbuffer_size] ={0.0};
			double channel3_imagcrrectemp[fftbuffer_size] ={0.0};
			double channel4_realcrrectemp[fftbuffer_size] ={0.0};
			double channel4_imagcrrectemp[fftbuffer_size] ={0.0};
			double channel1_realcrrect[fftbuffer_size] ={0.0};
			double channel1_imagcrrect[fftbuffer_size] ={0.0};
			double channel2_realcrrect[fftbuffer_size] ={0.0};
			double channel2_imagcrrect[fftbuffer_size] ={0.0};
			double channel3_realcrrect[fftbuffer_size] ={0.0};
			double channel3_imagcrrect[fftbuffer_size] ={0.0};
			double channel4_realcrrect[fftbuffer_size] ={0.0};
			double channel4_imagcrrect[fftbuffer_size] ={0.0};
			double channel1_appitudecrrect[fftbuffer_size] ={0.0};
			double channel2_appitudecrrect[fftbuffer_size] ={0.0};
			double channel3_appitudecrrect[fftbuffer_size] ={0.0};
			double channel4_appitudecrrect[fftbuffer_size] ={0.0};
			int count_num =(int)(fftbuffer_size/5);
			//printf("count_num:%d \r\n",count_num);
			int fftnum = 0;
			//for(int i = 0;i<
			for(int i = 0;i<count_num;i++)
			{

			channel1_realcrrect[i] = channel1_real[fftbuffer_size/5-i-1];
			channel1_imagcrrect[i] = channel1_imag[fftbuffer_size/5-i-1];	
			channel1_appitudecrrect[i] = channel1_appitude[fftbuffer_size/5-i-1];

			
			channel2_realcrrect[i] = channel2_real[fftbuffer_size/5-i-1];
			channel2_imagcrrect[i] = channel2_imag[fftbuffer_size/5-i-1];
			channel2_appitudecrrect[i] = channel2_appitude[fftbuffer_size/5-i-1];

			

			channel3_realcrrect[i] = channel3_real[fftbuffer_size/5-i-1];
			channel3_imagcrrect[i] = channel3_imag[fftbuffer_size/5-i-1];
			channel3_appitudecrrect[i] = channel3_appitude[fftbuffer_size/5-i-1];

			
			channel4_realcrrect[i] = channel4_real[fftbuffer_size/5-i-1];
			channel4_imagcrrect[i] = channel4_imag[fftbuffer_size/5-i-1];
			channel4_appitudecrrect[i] = channel4_appitude[fftbuffer_size/5-i-1];
			
			channel1_realcrrect[i+fftbuffer_size/5] = channel1_real[fftbuffer_size-i-1];
			channel1_imagcrrect[i+fftbuffer_size/5] = channel1_imag[fftbuffer_size-i-1];	
			channel1_appitudecrrect[i+fftbuffer_size/5] = channel1_appitude[fftbuffer_size-i-1];

			
			channel2_realcrrect[i+fftbuffer_size/5] = channel2_real[fftbuffer_size-i-1];
			channel2_imagcrrect[i+fftbuffer_size/5] = channel2_imag[fftbuffer_size-i-1];
			channel2_appitudecrrect[i+fftbuffer_size/5] = channel2_appitude[fftbuffer_size-i-1];

			

			channel3_realcrrect[i+fftbuffer_size/5] = channel3_real[fftbuffer_size-i-1];
			channel3_imagcrrect[i+fftbuffer_size/5] = channel3_imag[fftbuffer_size-i-1];
			channel3_appitudecrrect[i+fftbuffer_size/5] = channel3_appitude[fftbuffer_size-i-1];

			
			channel4_realcrrect[i+fftbuffer_size/5] = channel4_real[fftbuffer_size-i-1];
			channel4_imagcrrect[i+fftbuffer_size/5] = channel4_imag[fftbuffer_size-i-1];
			channel4_appitudecrrect[i+fftbuffer_size/5] = channel4_appitude[fftbuffer_size-i-1];
			
//printf("i:%d,channel1_appitude:%lf,channel2_appitude:%lf,\r\n",i,channel1_appitude[i],channel2_appitude[i]);
//printf("i:%d,channel1_appitude:%lf,channel2_appitude:%lf,\r\n",i,channel1_appitudecrrect[i],channel2_appitudecrrect[i]);
			
			}
			
			channel4_appitudecrrect[818] = channel4_appitudecrrect[817];
			channel3_appitudecrrect[818] = channel3_appitudecrrect[817];
			channel2_appitudecrrect[818] = channel2_appitudecrrect[817];
			channel1_appitudecrrect[818] = channel1_appitudecrrect[817];
			int bytecount =12;
			buf1[0]=0x55;
			buf1[1]=0xA0;
			//数据总长度
			buf1[4]=0x01;
			buf1[5]=(detectangle.Currentarraydirection&0xFF);
			buf1[6]=(15855)&0xFF;
			buf1[7]=(15855>>8)&0xFF;
			buf1[8]=20;
			
			buf1[11]=0;

			buf2[0]=0x55;
			buf2[1]=0xA0;
			//数据总长度
			buf2[4]=0x02;
			buf2[5]=(detectangle.Currentarraydirection&0xFF);
			buf2[6]=(15855)&0xFF;
			buf2[7]=(15855>>8)&0xFF;
			buf2[8]=20;
			
			buf2[11]=0;

			buf3[0]=0x55;
			buf3[1]=0xA0;
			//数据总长度
			buf3[4]=0x03;
			buf3[5]=(detectangle.Currentarraydirection&0xFF);
			buf3[6]=(15855)&0xFF;
			buf3[7]=(15855>>8)&0xFF;
			buf3[8]=20;
			
			buf3[11]=0;

			buf4[0]=0x55;
			buf4[1]=0xA0;
			//数据总长度
			buf4[4]=0x04;
			buf4[5]=(detectangle.Currentarraydirection&0xFF);
			buf4[6]=(15855)&0xFF;
			buf4[7]=(15855>>8)&0xFF;
			buf4[8]=20;
			
			buf4[11]=0;
			
			for(int i =0;i<fftbuffer_size;i++)
			//for(int i =0;i<2*count_num;i++)
			{
				
				//buf1[(i)*2 +12] = (short)(channel1_appitudecrrect[i]*100.00)&0xFF;
				//buf1[(i)*2 +12+1] = (( short)(channel1_appitudecrrect[i]*100.00)>>8)&0xFF;
				
				//buf2[(i)*2 +12] = ( short)(channel2_appitudecrrect[i]*100.00)&0xFF;
				//buf2[(i)*2 +12+1] = (( short)(channel2_appitudecrrect[i]*100.00)>>8)&0xFF;
				//buf3[(i)*2 +12] = ( short)(channel3_appitudecrrect[i]*100.00)&0xFF;
				//buf3[(i)*2 +12+1] = (( short)(channel3_appitudecrrect[i]*100.00)>>8)&0xFF;
				//buf4[(i)*2 +12] = ( short)(channel4_appitudecrrect[i]*100.00)&0xFF;
				//buf4[(i)*2 +12+1] = (( short)(channel4_appitudecrrect[i]*100.00)>>8)&0xFF;
				buf1[(i)*2 +12] = (short)(channel1_timeappitude[i])&0xFF;
				buf1[(i)*2 +12+1] = (( short)(channel1_timeappitude[i])>>8)&0xFF;
				
				buf2[(i)*2 +12] = ( short)(channel2_timeappitude[i])&0xFF;
				buf2[(i)*2 +12+1] = (( short)(channel2_timeappitude[i])>>8)&0xFF;
				buf3[(i)*2 +12] = ( short)(channel3_timeappitude[i])&0xFF;
				buf3[(i)*2 +12+1] = (( short)(channel3_timeappitude[i])>>8)&0xFF;
				buf4[(i)*2 +12] = ( short)(channel4_timeappitude[i])&0xFF;
				buf4[(i)*2 +12+1] = (( short)(channel4_timeappitude[i])>>8)&0xFF;
			printf("i:%d,channel1_timeappitude:%lf,channel2_timeappitude:%lf,\r\n",i,channel1_timeappitude[i],channel2_timeappitude[i]);
			
				fftnum++;	
			}
			buf1[9]=fftnum&0xFF;
			buf1[10]=(fftnum>>8)&0xFF;
			buf2[9]=fftnum&0xFF;
			buf2[10]=(fftnum>>8)&0xFF;
			buf3[9]=fftnum&0xFF;
			buf3[10]=(fftnum>>8)&0xFF;
			buf4[9]=fftnum&0xFF;
			buf4[10]=(fftnum>>8)&0xFF;
			bytecount = bytecount+fftnum*2 +1;
			buf1[2]=bytecount&0xFF;
			buf1[3]=(bytecount>>8)&0xFF;
			buf2[2]=bytecount&0xFF;
			buf2[3]=(bytecount>>8)&0xFF;
			buf3[2]=bytecount&0xFF;
			buf3[3]=(bytecount>>8)&0xFF;
			buf4[2]=bytecount&0xFF;
			buf4[3]=(bytecount>>8)&0xFF;
			
			buf1[bytecount-1] =XOR_check(buf1,bytecount);
			buf2[bytecount-1] =XOR_check(buf2,bytecount);
			buf3[bytecount-1] =XOR_check(buf3,bytecount);
			buf4[bytecount-1] =XOR_check(buf4,bytecount);
			 if(check_nic("eth0") == 0)
        		{
			//printf("connect:success \r\n");
			close(lfd);
			lfd = socket(PF_INET, SOCK_DGRAM, 0);
			bind(lfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
			sendto(lfd,buf1,bytecount,0,(struct sockaddr*)&saddr1,sizeof(saddr1));	
			sendto(lfd,buf2,bytecount,0,(struct sockaddr*)&saddr2,sizeof(saddr2));	
			sendto(lfd,buf3,bytecount,0,(struct sockaddr*)&saddr3,sizeof(saddr3));	
			sendto(lfd,buf4,bytecount,0,(struct sockaddr*)&saddr4,sizeof(saddr4));	
			}
			//else{
			//printf("connect:faild \r\n");
			//	}
			if((detectangle.mode == 0))//单音信号测试 寻找峰值点

			{ 
				int maxpos1 =0;
				double maxappitude1 =-200;
				int maxpos2 =0;
				double maxappitude2 =-200;
				int maxpos3 =0;
				double maxappitude3 =-200;
				int maxpos4 =0;
				double maxappitude4 =-200;
				for(int i =0;i<count_num*2;i++)
				{
					if(maxappitude1<channel1_appitudecrrect[i])
					{
						maxappitude1 = channel1_appitudecrrect[i];
						maxpos1 = i;
					}
					if(maxappitude2<channel2_appitudecrrect[i])
					{
						maxappitude2 = channel2_appitudecrrect[i];
						maxpos2 = i;
					}
					if(maxappitude3<channel3_appitudecrrect[i])
					{
						maxappitude3 = channel3_appitudecrrect[i];
						maxpos3 = i;
					}
					if(maxappitude4<channel4_appitudecrrect[i])
					{
						maxappitude4 = channel4_appitudecrrect[i];
						maxpos4 = i;
					}
			//printf("i:%d,appitude1:%lf,\r\n",i,channel1_appitude[i]);

				}
				printf("maxpos1:%d,maxappitude1:%lf \r\n",maxpos1,maxappitude1);
				printf("maxpos2:%d,maxappitude2:%lf \r\n",maxpos2,maxappitude2);
				printf("maxpos3:%d,maxappitude3:%lf \r\n",maxpos3,maxappitude3);
				printf("maxpos4:%d,maxappitude4:%lf \r\n",maxpos4,maxappitude4);
				//计算四通道相位差

   				 double differ12 =atan2((channel1_realcrrect[maxpos1]*channel2_imagcrrect[maxpos2]-channel1_imagcrrect[maxpos1]*channel2_realcrrect[maxpos2]),channel1_imagcrrect[maxpos1]*channel2_imagcrrect[maxpos2]+channel1_realcrrect[maxpos1]*channel2_realcrrect[maxpos2])*180.00/PI;
   				 double differ13 =atan2((channel1_realcrrect[maxpos1]*channel3_imagcrrect[maxpos3]-channel1_imagcrrect[maxpos1]*channel3_realcrrect[maxpos3]),channel1_imagcrrect[maxpos1]*channel3_imagcrrect[maxpos3]+channel1_realcrrect[maxpos1]*channel3_realcrrect[maxpos3])*180.00/PI;
 				 double differ14 =atan2((channel1_realcrrect[maxpos1]*channel4_imagcrrect[maxpos4]-channel1_imagcrrect[maxpos1]*channel4_realcrrect[maxpos4]),channel1_imagcrrect[maxpos1]*channel4_imagcrrect[maxpos4]+channel1_realcrrect[maxpos1]*channel4_realcrrect[maxpos4])*180.00/PI;
				//if(differ12<0.00)
				//	differ12= differ12+360.00;
				//differ12 =differ12-180.00;
				//if(differ13<0.00)
				//	differ13= differ13+360.00;
				//differ13 =differ13-180.00;
				//if(differ14<0.00)
				//	differ14= differ14+360.00;
				//differ14 =differ14-180.00;
				printf("differ12:%lf,differ13:%lf ,differ14:%lf\r\n",differ12,differ13,differ14);
				short frequencea = (double)((maxpos1/count_num/2.0*20)*10.00);
				//printf("frequence:%d \r\n",frequencea);
				int bytecount =16;
				buf1[0]=0x55;
				buf1[1]=0xA1;
				buf1[2]=bytecount&0xFF;
				buf1[3]=(bytecount>>8)&0xFF;
				//数据总长度
				buf1[4]=(detectangle.Currentarraydirection&0xFF);
				buf1[5]=0x00;
				buf1[6]=(frequencea)&0xFF;
				buf1[7]=((frequencea)>>8)&0xFF;
				buf1[8]=0x00;
				buf1[9]=(( short)(differ12*100.00))&0xFF;
				buf1[10]=(( short)(differ12*100.00)>>8)&0xFF;
				buf1[11]=(( short)(differ13*100.00))&0xFF;
				buf1[12]=(( short)(differ13*100.00)>>8)&0xFF;
				buf1[13]=(( short)(differ14*100.00))&0xFF;
				buf1[14]=(( short)(differ14*100.00)>>8)&0xFF;
				buf1[bytecount-1] =XOR_check(buf1,bytecount);
				sendto(server_sockfd,buf1,bytecount,0,(struct sockaddr*)&saddr0,sizeof(saddr0));
    			}
			else if((detectangle.mode == 1)) //单音信号测向模式
			{
				int maxpos1 =0;
				double maxappitude1 =-200;
				int maxpos2 =0;
				double maxappitude2 =-200;
				int maxpos3 =0;
				double maxappitude3 =-200;
				int maxpos4 =0;
				double maxappitude4 =-200;
				for(int i =0;i<count_num*2;i++)
				{
					if(maxappitude1<channel1_appitudecrrect[i])
					{
						maxappitude1 = channel1_appitudecrrect[i];
						maxpos1 = i;
					}
					if(maxappitude2<channel2_appitudecrrect[i])
					{
						maxappitude2 = channel2_appitudecrrect[i];
						maxpos2 = i;
					}
					if(maxappitude3<channel3_appitudecrrect[i])
					{
						maxappitude3 = channel3_appitudecrrect[i];
						maxpos3 = i;
					}
					if(maxappitude4<channel4_appitudecrrect[i])
					{
						maxappitude4 = channel4_appitudecrrect[i];
						maxpos4 = i;
					}

				}
				//printf("maxpos1:%d,maxappitude1:%lf \r\n",maxpos1,maxappitude1);
				//printf("maxpos2:%d,maxappitude2:%lf \r\n",maxpos2,maxappitude2);
				//计算四通道相位差

   				 double differ12 =atan2((channel1_realcrrect[maxpos1]*channel2_imagcrrect[maxpos2]-channel1_imagcrrect[maxpos1]*channel2_realcrrect[maxpos2]),channel1_imagcrrect[maxpos1]*channel2_imagcrrect[maxpos2]+channel1_realcrrect[maxpos1]*channel2_realcrrect[maxpos2])*180.00/PI;
   				 double differ13 =atan2((channel1_realcrrect[maxpos1]*channel3_imagcrrect[maxpos3]-channel1_imagcrrect[maxpos1]*channel3_realcrrect[maxpos3]),channel1_imagcrrect[maxpos1]*channel3_imagcrrect[maxpos3]+channel1_realcrrect[maxpos1]*channel3_realcrrect[maxpos3])*180.00/PI;
 				 double differ14 =atan2((channel1_realcrrect[maxpos1]*channel4_imagcrrect[maxpos4]-channel1_imagcrrect[maxpos1]*channel4_realcrrect[maxpos4]),channel1_imagcrrect[maxpos1]*channel4_imagcrrect[maxpos4]+channel1_realcrrect[maxpos1]*channel4_realcrrect[maxpos4])*180.00/PI;
				//if(differ12<0.0)
				//	differ12= differ12+360.00;
				//differ12 =differ12-180.00;
				//if(differ13<0.0)
				//	differ13= differ13+360.0;
				//differ13 =differ13-180.0;
				//if(differ14<0.0)
				//	differ14= differ14+360.0;
				//differ14 =differ14-180.0;
				//判断当前阵列模式  四通道相位差修正
				if(detectangle.Currentarraydirection == 0)
				{
					differ12 = differ12 - detectangle.correctphase[0][0];
					differ13 = differ13 - detectangle.correctphase[0][1];
					differ14 = differ14 - detectangle.correctphase[0][2];
				}
				else
				{
					differ12 = differ12 - detectangle.correctphase[1][0];
					differ13 = differ13 - detectangle.correctphase[1][1];
					differ14 = differ14 - detectangle.correctphase[1][2];
				}
				int k1 = 0;
				int k2 = 0;
				int k3 = 0;
				k2 = round((2.0*differ12-differ13)/360.0);
				k3 = round((3.0*differ12-differ14)/360.0);
				double differreal14 =0.0;
				double differreal13 =0.0;
				differreal14 = differ14/180.0 + 2.0*PI*k3;
				differreal13 = differ13/180.0 + 2.0*PI*k2;
				double angle1 = asin(differ12/180.0*(C/Fre)/(2.00*PI*1*BASEDISTANCE))*180.00/PI;
				double angle2 = asin(differreal13*(C/Fre)/(2.0*PI*2.0*BASEDISTANCE))*180.00/PI;
				double angle3 = asin(differreal14*(C/Fre)/(2.0*PI*3.0*BASEDISTANCE))*180.00/PI;
				printf("current differ12:%lf,differ13:%lf,differ14:%lf   \r\n",differ12,differ13,differ14);
				printf("current k1:%d,k2:%d,k3:%d   \r\n",k1,k2,k3);
				printf("current angle1:%lf,angle2:%lf,angle3:%lf   \r\n",angle1,angle2,angle3);
				double frequencea = (double)((maxpos1/4096.0*10)*10.00);
				int bytecount =12;
				buf1[0]=0x55;
				buf1[1]=0xA2;
				buf1[2]=bytecount&0xFF;
				buf1[3]=(bytecount>>8)&0xFF;
				//数据总长度
				buf1[4]=(detectangle.Currentarraydirection&0xFF);
				buf1[5]=0x00;
				buf1[6]=((short)frequencea)&0xFF;
				buf1[7]=(((short)frequencea)>>8)&0xFF;
				buf1[8]=0x00;
				buf1[9]=(( short)(angle3*100.0))&0xFF;
				buf1[10]=(( short)(angle3*100.0)>>8)&0xFF;
				
				buf1[bytecount-1] =XOR_check(buf1,bytecount);
				sendto(server_sockfd,buf1,bytecount,0,(struct sockaddr*)&saddr0,sizeof(saddr0));
 			}else if((detectangle.mode == 2))//宽带信号测向模式
			{
				//获取测向频率和带宽
				double frequence =  detectangle.frequence;
				double bandwidth = detectangle.bandwidth;
				int  midcnt =   (frequence -(1570000000.0-50000000.00/2.00))/50000000.00*4096.0;
				int startcnt = midcnt - bandwidth/50000000.00*4096.000;
				int endcnt = midcnt + bandwidth/50000000.00*4096.000;
				int maxpos1 =0;
				double maxappitude1 =-200;
				int maxpos2 =0;
				double maxappitude2 =-200;
				int maxpos3 =0;
				double maxappitude3 =-200;
				int maxpos4 =0;
				double maxappitude4 =-200;
 				double differ12 = 0.0 ,differ13 = 0.0,differ14 = 0.0;
				int count = 0; 
				for(int i = startcnt;i<endcnt;i++)
				{
					//计算四通道相位差

   				double workdiffer12 =atan2((channel1_realcrrect[i]*channel2_imagcrrect[i]-channel1_imagcrrect[i]*channel2_realcrrect[i]),channel1_imagcrrect[i]*channel2_imagcrrect[i]+channel1_realcrrect[i]*channel2_realcrrect[i])*180.00/3.14159265;
   				double workdiffer13 =atan2((channel1_realcrrect[i]*channel3_imagcrrect[i]-channel1_imagcrrect[i]*channel3_realcrrect[i]),channel1_imagcrrect[i]*channel3_imagcrrect[i]+channel1_realcrrect[i]*channel3_realcrrect[i])*180.00/3.14159265;
 				double workdiffer14 =atan2((channel1_realcrrect[i]*channel4_imagcrrect[i]-channel1_imagcrrect[i]*channel4_realcrrect[i]),channel1_imagcrrect[i]*channel4_imagcrrect[i]+channel1_realcrrect[i]*channel4_realcrrect[i])*180.00/3.14159265;
				if(workdiffer12<0.00)
					workdiffer12= workdiffer12+360.00;
				workdiffer12 =workdiffer12-180.00;
				if(workdiffer13<0.00)
					workdiffer13 = workdiffer13+360.00;
				workdiffer13 =workdiffer13-180.00;
				if(workdiffer14<0.0)
					workdiffer14= workdiffer14+360.0;
				workdiffer14 =workdiffer14-180.00;
				differ12 = differ12 +workdiffer12;
				differ13 = differ13 +workdiffer13;
				differ14 = differ14 +workdiffer14;
				count ++;
				}
				differ12 = differ12/count;
				differ13 = differ13/count;
				differ14 = differ14/count;
				
				//判断当前阵列模式  四通道相位差修正
				if(detectangle.Currentarraydirection == 0)
				{
					differ12 = differ12 - detectangle.correctphase[0][0];
					differ13 = differ13 - detectangle.correctphase[0][1];
					differ14 = differ14 - detectangle.correctphase[0][2];
				}
				else
				{
					differ12 = differ12 - detectangle.correctphase[1][0];
					differ13 = differ13 - detectangle.correctphase[1][1];
					differ14 = differ14 - detectangle.correctphase[1][2];
				}
				

				int k1 = 0;
				int k2 = 0;
				int k3 = 0;
				k2 = round((2.0*differ12-differ13)/360.0);
				k3 = round((3.0*differ12-differ14)/360.0);
				double differreal14 =0.0;
				double differreal13 =0.0;
				differreal14 = differ14/180.00 + 2.0*PI*k3;
				differreal13 = differ13/180.00 + 2.0*PI*k2;
				double angle1 = asin(differ12/180.0*(C/Fre)/(2*PI*1*BASEDISTANCE))*180.00/PI;
				double angle2 = asin(differreal13*(C/Fre)/(2.0*PI*2.0*BASEDISTANCE))*180.00/PI;
				double angle3 = asin(differreal14*(C/Fre)/(2.0*PI*3.0*BASEDISTANCE))*180.00/PI;
				printf("current differ12:%lf,differ13:%lf,differ14:%lf   \r\n",differ12,differ13,differ14);
				printf("current k1:%d,k2:%d,k3:%d   \r\n",k1,k2,k3);
				printf("current angle1:%lf,angle2:%lf,angle3:%lf   \r\n",angle1,angle2,angle3);
				int bytecount =12;
				buf1[0]=0x55;
				buf1[1]=0xA2;
				buf1[2]=bytecount&0xFF;
				buf1[3]=(bytecount>>8)&0xFF;
				//数据总长度
				buf1[4]=(detectangle.Currentarraydirection&0xFF);
				buf1[5]=0x01;
				buf1[6]=(( short)(frequence/100000.0))&0xFF;
				buf1[7]=((( short)(frequence/100000.0))>>8)&0xFF;
				buf1[8]=((int8_t)(detectangle.bandwidth/1000000.0))&0xFF;
				buf1[9]=(( short)(angle3*100.00))&0xFF;
				buf1[10]=(( short)(angle3*100.00)>>8)&0xFF;
				buf1[bytecount-1] =XOR_check(buf1,bytecount);
				sendto(server_sockfd,buf1,bytecount,0,(struct sockaddr*)&saddr0,sizeof(saddr0));
			}
	
	
 	 	
   	}
}
 
 close(lfd);
  

}

