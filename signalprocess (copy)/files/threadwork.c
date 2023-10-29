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
#include <math.h>
#include <time.h>
#define CLOCKS_PER_SEC  1000000l
static volatile uint32_t *GPIO_ADF4351EN ;

static volatile uint32_t *GPIO_RESERVE ;
//axidma operate
static axidma_dev_t g_AxiDmaDev;
array_t* g_apRxChannels;
void* g_pAD9226DataBuf;
struct detectanglearg detectangle;
 int32_t server_sockfd;
unsigned int R[6] = {0x580005,0x8A0024,0x4004B3,0x2005FC2,0x8008011,0x658000};
unsigned int R1[6] = {0x580005,0x9A0024,0x4004B3,0x2005FC2,0x8008011,0x388000};
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
static const char *device1 = "/dev/spidev1.1";
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
	//xfer.cs_change = 1,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("1 can't send spi1.0 message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
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
	//xfer.cs_change = 1,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("2 can't send spi1.0 message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
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
	//xfer.cs_change = 1,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("3 can't send spi1.0 message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
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
	//xfer.cs_change = 1,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("4 can't send spi1.0 message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
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
	//xfer.cs_change = 1,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("5 can't send spi1.0 message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
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
	//xfer.cs_change = 1,
	//send spi message
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("6 can't send spi1.0 message \r\n");
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);

	
}
static void transfer1(int fd)
{
	int ret;
	int status;
	struct spi_ioc_transfer xfer;
 	unsigned char buf[32],*bp;
	
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	buf[0] = (R1[0]>>24)&0xFF;
	buf[1] = (R1[0]>>16)&0xFF;
	buf[2] = (R1[0]>>8)&0xFF;
	buf[3] = (R1[0])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//xfer.cs_change = 0,
	//send spi message
	//status  = write(fd,buf,4);
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi1.1 message  status:%d\r\n",status);
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[1]>>24)&0xFF;
	buf[1] = (R1[1]>>16)&0xFF;
	buf[2] = (R1[1]>>8)&0xFF;
	buf[3] = (R1[1])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//xfer.cs_change = 0,
	//send spi message
	//status  = write(fd,buf,4);
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi1.1 message  status:%d\r\n",status);
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[2]>>24)&0xFF;
	buf[1] = (R1[2]>>16)&0xFF;
	buf[2] = (R1[2]>>8)&0xFF;
	buf[3] = (R1[2])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//xfer.cs_change = 0,
	////send spi message
	//status  = write(fd,buf,4);
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi1.1 message  status:%d\r\n",status);
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[3]>>24)&0xFF;
	buf[1] = (R1[3]>>16)&0xFF;
	buf[2] = (R1[3]>>8)&0xFF;
	buf[3] = (R1[3])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//xfer.cs_change = 0,
	//send spi message
	//status  = write(fd,buf,4);
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi1.1 message  status:%d\r\n",status);
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[4]>>24)&0xFF;
	buf[1] = (R1[4]>>16)&0xFF;
	buf[2] = (R1[4]>>8)&0xFF;
	buf[3] = (R1[4])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//xfer.cs_change = 0,
	//send spi message
	//status  = write(fd,buf,4);
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi1.1 message  status:%d\r\n",status);
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[5]>>24)&0xFF;
	buf[1] = (R1[5]>>16)&0xFF;
	buf[2] = (R1[5]>>8)&0xFF;
	buf[3] = (R1[5])&0xFF;
	xfer.tx_buf = (unsigned long) buf;
	xfer.len = 4;
	xfer.delay_usecs = delay,
	xfer.speed_hz = speed,
	xfer.bits_per_word = bits,
	//xfer.cs_change = 0,
	//send spi message
	//status  = write(fd,buf,4);
	status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 1)
		printf("can't send spi1.1 message  status:%d\r\n",status);
	//printf("reponse(%2d,%2d): \r\n",4,status);
	usleep(100);

	
}
void spiconfig(){
int fd,ret =0;

	fd = open(device, O_RDWR);
	if (fd < 0)
		printf("can't open spi1.0 device\r\n");
	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE32, &spimode);
	if (ret == -1)
		printf("can't set spi1.0 mode\r\n");

	ret = ioctl(fd, SPI_IOC_RD_MODE32, &spimode);
	if (ret == -1)
		printf("can't get spi1.0 mode \r\n");

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

	printf("spi1.0 mode: 0x%x\n", spimode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	
	transfer(fd);
	transfer(fd);
	close(fd);
	//transfer(fd);
	
	
	
      
	
}
void spiconfig1(){
int fd,ret =0;

	fd = open(device1, O_RDWR);
	if (fd < 0)
		printf("can't open device1.1\r\n");
	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE32, &spimode);
	if (ret == -1)
		printf("can't set spi1.1 mode\r\n");

	ret = ioctl(fd, SPI_IOC_RD_MODE32, &spimode);
	if (ret == -1)
		printf("can't get spi1.1 mode \r\n");

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

	printf("spi1.1 mode: 0x%x\n", spimode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
	
	transfer1(fd);
	transfer1(fd);
	close(fd);
	//transfer(fd);
	
	
	
      
	
}
void callbackAfterRecive(int channelid,void* data)
{
  // printf("INFO: callback func triggerd,channelid: %d\n",channelid);
			 int listlength =  queue_length(queue);
			//printf("length:%d \r\n",listlength);	
		   if(listlength<3)
	   	   { 
			
			memcpy(ThreadArg.ad9226fftdata,g_pAD9226DataBuf,sizeof(unsigned int)*fftbuffer_size*8);
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

//static volatile uint32_t *GPIO_ATEENCTRL ;
//static volatile uint32_t *GPIO_RESERVE ;
 	int gpioadf4351fd;
	
	int gpioreservefd;
	printf("start gpioadf4351fd mmap\r\n");
	
	printf("start gpioreservefd mmap\r\n");
    if ((gpioadf4351fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0) {
        printf("Unable to open /dev/mem: %s\n", strerror(errno));
        return NULL;
    }

 	GPIO_ADF4351EN = (uint32_t *) mmap(0, getpagesize() * 250, PROT_READ | PROT_WRITE, MAP_SHARED, gpioadf4351fd, GPIOADF4351EN_baseddr);
	
	

   if ((void *) GPIO_ADF4351EN == MAP_FAILED) {
        printf("mmap GPIO_ADF4351EN failed: %s\n", strerror(errno));
        exit(1);
    }

if ((gpioreservefd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0) {
        printf("Unable to open /dev/mem: %s\n", strerror(errno));
        return NULL;
    }

 	GPIO_RESERVE = (uint32_t *) mmap(0, getpagesize() * 250, PROT_READ | PROT_WRITE, MAP_SHARED, gpioreservefd, GPIORESERVE_baseddr);
	
	

   if ((void *) GPIO_RESERVE == MAP_FAILED) {
        printf("mmap GPIO_RESERVE failed: %s\n", strerror(errno));
        exit(1);
    }
//使能ADF4351芯片	
	*GPIO_ADF4351EN = 0xFF;
	*GPIO_RESERVE = 0xFF;
	
	sleep(1);
	//init spi adf4531
	spiconfig();
	printf("spi1.0 init success!!!!\r\n");
	sleep(1);
        spiconfig1();
	printf("spi1.1 init success!!!!\r\n");
	sleep(1);

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
    g_pAD9226DataBuf = axidma_malloc(g_AxiDmaDev,(AD9226_SAMPLENUM)*8*4);
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

        	    usleep(1);
		  
					//read ad9226 data
			axidma_oneway_transfer(g_AxiDmaDev,g_apRxChannels->data[0],g_pAD9226DataBuf,(AD9226_SAMPLENUM)*8*4,false);
					
		   
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
	double channel1_realtemp[fftbuffer_size] ={0.0};
	double channel1_imagtemp[fftbuffer_size] ={0.0};
	double channel2_realtemp[fftbuffer_size] ={0.0};
	double channel2_imagtemp[fftbuffer_size] ={0.0};
	double channel3_realtemp[fftbuffer_size] ={0.0};
	double channel3_imagtemp[fftbuffer_size] ={0.0};
	double channel4_realtemp[fftbuffer_size] ={0.0};
	double channel4_imagtemp[fftbuffer_size] ={0.0};
	double channel1_appitude[fftbuffer_size] ={0.0};
	double channel2_appitude[fftbuffer_size] ={0.0};
	double channel3_appitude[fftbuffer_size] ={0.0};
	double channel4_appitude[fftbuffer_size] ={0.0};
	 clock_t start, finish;
       double duration;
       /* 测量一个事件持续的时间*/
      
      
    
      
     
 	while (1)//等待数据更新
 	{
		
		
		//printf("init rsource success\r\n");
   		 pthread_mutex_lock(&mutex);
 		 re_list =   queue_pop(queue, &data);
    		 pthread_mutex_unlock(&mutex); 
		//usleep(10000);

		if(re_list == SUCCESS)
		{	
		 	//start = clock();
			for(int i = 0;i < fftbuffer_size/2;i++)
    			{

			channel1_realtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+0*2)));
			channel1_imagtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+0*2+1)));
			channel1_appitude[i] = 20*log10(sqrt(channel1_realtemp[i]*channel1_realtemp[i]+channel1_imagtemp[i]*channel1_imagtemp[i])/fftbuffer_size)-60;
			channel2_realtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+1*2)));
			channel2_imagtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+1*2+1)));
			channel2_appitude[i] = 20*log10(sqrt(channel2_realtemp[i]*channel2_realtemp[i]+channel2_imagtemp[i]*channel2_imagtemp[i])/fftbuffer_size)-60;
			channel3_realtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+2*2)));
			channel3_imagtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+2*2+1)));
			channel3_appitude[i] = 20*log10(sqrt(channel3_realtemp[i]*channel3_realtemp[i]+channel3_imagtemp[i]*channel3_imagtemp[i])/fftbuffer_size)-60;
			channel4_realtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+3*2)));
			channel4_imagtemp[i] = (int)((*((unsigned int*)(data.ad9226fftdata)+i*8+3*2+1))); 
			channel4_appitude[i] = 20*log10(sqrt(channel4_realtemp[i]*channel4_realtemp[i]+channel4_imagtemp[i]*channel4_imagtemp[i])/fftbuffer_size)-60;
			if(!isnormal(channel1_appitude[i]))
				channel1_appitude[i] = channel1_appitude[i-1];
			if(!isnormal(channel2_appitude[i]))
				channel2_appitude[i] = channel2_appitude[i-1];
			if(!isnormal(channel3_appitude[i]))
				channel3_appitude[i] = channel3_appitude[i-1];
			if(!isnormal(channel4_appitude[i]))
				channel4_appitude[i] = channel4_appitude[i-1];
   			}

			int count_num =10;
			int fftnum = 0;
			int bytecount =13;
			buf1[0]=0x55;
			buf1[1]=0xA0;
			//数据总长度
			buf1[4]=0x01;
			buf1[5]=(detectangle.Currentarraydirection&0xFF);
			buf1[6]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0))&0xFF;
			buf1[7]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0)>>8)&0xFF;
			buf1[8]=((unsigned short)((MidBandwidth/100000.00)/2.0))&0xFF;
			buf1[9]=((unsigned short)((MidBandwidth/100000.00)/2.0)>>8)&0xFF;
			buf1[12]=0;

			buf2[0]=0x55;
			buf2[1]=0xA0;
			//数据总长度
			buf2[4]=0x02;
			buf2[5]=(detectangle.Currentarraydirection&0xFF);
			buf2[6]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0))&0xFF;
			buf2[7]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0)>>8)&0xFF;
			buf2[8]=((unsigned short)((MidBandwidth/100000.00)/2.0))&0xFF;
			buf2[9]=((unsigned short)((MidBandwidth/100000.00)/2.0)>>8)&0xFF;
			
			buf2[12]=0;

			buf3[0]=0x55;
			buf3[1]=0xA0;
			//数据总长度
			buf3[4]=0x03;
			buf3[5]=(detectangle.Currentarraydirection&0xFF);
			buf3[6]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0))&0xFF;
			buf3[7]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0)>>8)&0xFF;
			buf3[8]=((unsigned short)((MidBandwidth/100000.00)/2.0))&0xFF;
			buf3[9]=((unsigned short)((MidBandwidth/100000.00)/2.0)>>8)&0xFF;
			
			buf3[12]=0;

			buf4[0]=0x55;
			buf4[1]=0xA0;
			//数据总长度
			buf4[4]=0x04;
			buf4[5]=(detectangle.Currentarraydirection&0xFF);
			buf4[6]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0))&0xFF;
			buf4[7]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0)>>8)&0xFF;
			buf4[8]=((unsigned short)((MidBandwidth/100000.00)/2.0))&0xFF;
			buf4[9]=((unsigned short)((MidBandwidth/100000.00)/2.0)>>8)&0xFF;
			
			buf4[12]=0;
			
			for(int i =0;i<fftbuffer_size/2;i++)
			{
				buf1[(i)*2 +13] = (short)(channel1_appitude[i]*100.00)&0xFF;
				buf1[(i)*2 +13+1] = (( short)(channel1_appitude[i]*100.00)>>8)&0xFF;
				
				buf2[(i)*2 +13] = ( short)(channel2_appitude[i]*100.00)&0xFF;
				buf2[(i)*2 +13+1] = (( short)(channel2_appitude[i]*100.00)>>8)&0xFF;
				buf3[(i)*2 +13] = ( short)(channel3_appitude[i]*100.00)&0xFF;
				buf3[(i)*2 +13+1] = (( short)(channel3_appitude[i]*100.00)>>8)&0xFF;
				buf4[(i)*2 +13] = ( short)(channel4_appitude[i]*100.00)&0xFF;
				buf4[(i)*2 +13+1] = (( short)(channel4_appitude[i]*100.00)>>8)&0xFF;
			//printf("i:%d,channel1_appitude:%lf,channel2_appitude:%lf,\r\n",i,channel1_appitudecrrect[i],channel2_appitudecrrect[i]);
			
				fftnum++;	
			}
			buf1[10]=fftnum&0xFF;
			buf1[11]=(fftnum>>8)&0xFF;
			buf2[10]=fftnum&0xFF;
			buf2[11]=(fftnum>>8)&0xFF;
			buf3[10]=fftnum&0xFF;
			buf3[11]=(fftnum>>8)&0xFF;
			buf4[10]=fftnum&0xFF;
			buf4[11]=(fftnum>>8)&0xFF;
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
			sendto(lfd,buf1,bytecount,MSG_NOSIGNAL,(struct sockaddr*)&saddr1,sizeof(saddr1));	
			sendto(lfd,buf2,bytecount,MSG_NOSIGNAL,(struct sockaddr*)&saddr2,sizeof(saddr2));	
			sendto(lfd,buf3,bytecount,MSG_NOSIGNAL,(struct sockaddr*)&saddr3,sizeof(saddr3));	
			sendto(lfd,buf4,bytecount,MSG_NOSIGNAL,(struct sockaddr*)&saddr4,sizeof(saddr4));
			}
				
			else{
			printf("connect:faild \r\n");
				}
 		//finish = clock();
       		//duration = (double)(finish - start) / CLOCKS_PER_SEC;
       		//printf( "calcu run time :%f ms\r\n", duration*1000.00 );
			if((detectangle.mode == 0))//单音信号测试 寻找峰值点

			{ 
				double frequence =  detectangle.frequence;
				int  midcnt =   (frequence -(MidFre-MidBandwidth/4.00))/(MidBandwidth/2)*(fftbuffer_size/2.00);
				int startcnt = midcnt - 2.0E6/(MidBandwidth/2.0)*(fftbuffer_size/2.00);
				int endcnt = midcnt + 2.0E6/(MidBandwidth/2.0)*(fftbuffer_size/2.00);
				//printf("midcnt:%d,startcnt:%d,endcnt:%d;\r\n",midcnt,startcnt,endcnt);
				int maxpos1 =0;
				double maxappitude1 =-200;
				int maxpos2 =0;
				double maxappitude2 =-200;
				int maxpos3 =0;
				double maxappitude3 =-200;
				int maxpos4 =0;
				double maxappitude4 =-200;

				for(int i =40;i<fftbuffer_size/2;i++)
				{
					if(maxappitude1<channel1_appitude[i])
					{
						maxappitude1 = channel1_appitude[i];
						maxpos1 = i;
					}
					if(maxappitude2<channel2_appitude[i])
					{
						maxappitude2 = channel2_appitude[i];
						maxpos2 = i;
					}
					if(maxappitude3<channel3_appitude[i])
					{
						maxappitude3 = channel3_appitude[i];
						maxpos3 = i;
					}
					if(maxappitude4<channel4_appitude[i])
					{
						maxappitude4 = channel4_appitude[i];
						maxpos4 = i;
					}
			//printf("i:%d,appitude1:%lf,\r\n",i,channel1_appitude[i]);

				}
				//计算四通道相位差

   				 double differ12 =atan2((channel1_realtemp[maxpos1]*channel2_imagtemp[maxpos2]-channel1_imagtemp[maxpos1]*channel2_realtemp[maxpos2]),channel1_imagtemp[maxpos1]*channel2_imagtemp[maxpos2]+channel1_realtemp[maxpos1]*channel2_realtemp[maxpos2])*180.00/PI;
   				 double differ13 =atan2((channel1_realtemp[maxpos1]*channel3_imagtemp[maxpos3]-channel1_imagtemp[maxpos1]*channel3_realtemp[maxpos3]),channel1_imagtemp[maxpos1]*channel3_imagtemp[maxpos3]+channel1_realtemp[maxpos1]*channel3_realtemp[maxpos3])*180.00/PI;
 				 double differ14 =atan2((channel1_realtemp[maxpos1]*channel4_imagtemp[maxpos4]-channel1_imagtemp[maxpos1]*channel4_realtemp[maxpos4]),channel1_imagtemp[maxpos1]*channel4_imagtemp[maxpos4]+channel1_realtemp[maxpos1]*channel4_realtemp[maxpos4])*180.00/PI;
				if(differ12<0.00)
					differ12= differ12+360.00;
				//differ12 =differ12-180.00;
				if(differ13<0.00)
					differ13= differ13+360.00;
				//differ13 =differ13-180.00;
				if(differ14<0.00)
					differ14= differ14+360.00;
				//differ14 =differ14-180.00;
				printf("aaaa  differ12:%lf,differ13:%lf ,differ14:%lf\r\n",differ12,differ13,differ14);
				unsigned short frequencea = (double)((maxpos1/(fftbuffer_size/2.0)*MidBandwidth/2.0/1.0E6+5245294118/1.0E6)*10.00);
				printf("frequence:%d,maxpos1:%d \r\n",frequencea,maxpos1);
				int bytecount =16;
				buf1[0]=0x55;
				buf1[1]=0xA1;
				buf1[2]=bytecount&0xFF;
				buf1[3]=(bytecount>>8)&0xFF;
				//数据总长度
				buf1[4]= detectangle.Currentarraydirection&0x00FF;
				//buf1[4] = 0x01;
				//printf("detectangle.Currentarraydirection:%d,buf1[4]:%d   \r\n",detectangle.Currentarraydirection,buf1[4]);
				buf1[5]=0x00;
				buf1[6]=(frequencea)&0xFF;
				buf1[7]=((frequencea)>>8)&0xFF;
				buf1[8]=0x00;
				buf1[9]=(( unsigned short)(differ12*100.00))&0xFF;
				buf1[10]=((unsigned short)(differ12*100.00)>>8)&0xFF;
				buf1[11]=((unsigned short)(differ13*100.00))&0xFF;
				buf1[12]=((unsigned short)(differ13*100.00)>>8)&0xFF;
				buf1[13]=((unsigned short)(differ14*100.00))&0xFF;
				buf1[14]=((unsigned short)(differ14*100.00)>>8)&0xFF;
				buf1[bytecount-1] =XOR_check(buf1,bytecount);
				 if(check_nic("eth0") == 0)
        		{
			//printf("connect:success \r\n");
			close(lfd);
			lfd = socket(PF_INET, SOCK_DGRAM, 0);
			bind(lfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
			sendto(server_sockfd,buf1,bytecount,MSG_NOSIGNAL,(struct sockaddr*)&saddr0,sizeof(saddr0));
			}
			else{
			printf("connect:faild \r\n");
				}
				
    			}
			else if((detectangle.mode == 1)) //单音信号测向模式
			{
				double frequence =  detectangle.frequence;
				int  midcnt =   (frequence -(MidFre-MidBandwidth/4.00))/(MidBandwidth/2)*(fftbuffer_size/2.00);
				int startcnt = midcnt - 2.0E6/(MidBandwidth/2.0)*(fftbuffer_size/2.00);
				int endcnt = midcnt + 2.0E6/(MidBandwidth/2.0)*(fftbuffer_size/2.00);
				int maxpos1 =0;
				double maxappitude1 =-200;
				int maxpos2 =0;
				double maxappitude2 =-200;
				int maxpos3 =0;
				double maxappitude3 =-200;
				int maxpos4 =0;
				double maxappitude4 =-200;
				for(int i =startcnt;i<endcnt;i++)
				{
					if(maxappitude1<channel1_appitude[i])
					{
						maxappitude1 = channel1_appitude[i];
						maxpos1 = i;
					}
					if(maxappitude2<channel2_appitude[i])
					{
						maxappitude2 = channel2_appitude[i];
						maxpos2 = i;
					}
					if(maxappitude3<channel3_appitude[i])
					{
						maxappitude3 = channel3_appitude[i];
						maxpos3 = i;
					}
					if(maxappitude4<channel4_appitude[i])
					{
						maxappitude4 = channel4_appitude[i];
						maxpos4 = i;
					}

				}
				//printf("maxpos1:%d,maxappitude1:%lf \r\n",maxpos1,maxappitude1);
				//printf("maxpos2:%d,maxappitude2:%lf \r\n",maxpos2,maxappitude2);
				//计算四通道相位差

   				 double differ12 =atan2((channel1_realtemp[maxpos1]*channel2_imagtemp[maxpos2]-channel1_imagtemp[maxpos1]*channel2_realtemp[maxpos2]),channel1_imagtemp[maxpos1]*channel2_imagtemp[maxpos2]+channel1_realtemp[maxpos1]*channel2_realtemp[maxpos2])*180.00/PI;
   				 double differ13 =atan2((channel1_realtemp[maxpos1]*channel3_imagtemp[maxpos3]-channel1_imagtemp[maxpos1]*channel3_realtemp[maxpos3]),channel1_imagtemp[maxpos1]*channel3_imagtemp[maxpos3]+channel1_realtemp[maxpos1]*channel3_realtemp[maxpos3])*180.00/PI;
 				 double differ14 =atan2((channel1_realtemp[maxpos1]*channel4_imagtemp[maxpos4]-channel1_imagtemp[maxpos1]*channel4_realtemp[maxpos4]),channel1_imagtemp[maxpos1]*channel4_imagtemp[maxpos4]+channel1_realtemp[maxpos1]*channel4_realtemp[maxpos4])*180.00/PI;
				if(differ12<0.0)
					differ12= differ12+360.00;
				//differ12 =differ12-180.00;
				if(differ13<0.0)
					differ13= differ13+360.0;
				//differ13 =differ13-180.0;
				if(differ14<0.0)
					differ14= differ14+360.0;
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
				k2 = round((3.0*differ12-differ13)/360.0);
				k3 = round((4.0*differ12-differ14)/360.0);
				double differreal14 =0.0;
				double differreal13 =0.0;
				differreal14 = differ14/180.0 + 2.0*PI*k3;
				differreal13 = differ13/180.0 + 2.0*PI*k2;
				double angle1 = asin(differ12/180.0*(C/MidFre)/(2.00*PI*1*BASEDISTANCE))*180.00/PI;
				double angle2 = asin(differreal13*(C/MidFre)/(2.0*PI*3.0*BASEDISTANCE))*180.00/PI;
				double angle3 = asin(differreal14*(C/MidFre)/(2.0*PI*4.0*BASEDISTANCE))*180.00/PI;
				//printf("current differ12:%lf,differ13:%lf,differ14:%lf   \r\n",differ12,differ13,differ14);
				//printf("current k1:%d,k2:%d,k3:%d   \r\n",k1,k2,k3);
				//printf("current angle1:%lf,angle2:%lf,angle3:%lf   \r\n",angle1,angle2,angle3);
				unsigned short frequencea = (double)((maxpos1/(fftbuffer_size/2.0)*MidBandwidth/2.0/1.0E6+5245294118/1.0E6)*10.00);
				int bytecount =12;
				buf1[0]=0x55;
				buf1[1]=0xA2;
				buf1[2]=bytecount&0xFF;
				buf1[3]=(bytecount>>8)&0xFF;
				//数据总长度
				buf1[4]=detectangle.Currentarraydirection&0xFF;
				//buf1[4]= 0x01;
				//printf("buf1[4]:%d,\r\n",buf1[4]);
				buf1[5]=0x00;
				buf1[6]=((unsigned short)frequencea)&0xFF;
				buf1[7]=(((unsigned short)frequencea)>>8)&0xFF;
				buf1[8]=0x00;
				buf1[9]=(( short)(angle1*100.0))&0xFF;
				buf1[10]=(( short)(angle1*100.0)>>8)&0xFF;
				
				buf1[bytecount-1] =XOR_check(buf1,bytecount);
			 if(check_nic("eth0") == 0)
        		{
			//printf("connect:success \r\n");
				close(lfd);
				lfd = socket(PF_INET, SOCK_DGRAM, 0);
				bind(lfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
				sendto(server_sockfd,buf1,bytecount,MSG_NOSIGNAL,(struct sockaddr*)&saddr0,sizeof(saddr0));
			}
			else{
			printf("connect:faild \r\n");
				}
				
 			}else if((detectangle.mode == 2))//宽带信号测向模式
			{
				//获取测向频率和带宽
				double frequence =  detectangle.frequence;
				double bandwidth = detectangle.bandwidth;
				int  midcnt =   (frequence -(MidFre-MidBandwidth/4.00))/(MidBandwidth/2)*(fftbuffer_size/2.00);
				int startcnt = midcnt - detectangle.bandwidth/(MidBandwidth/2)*(fftbuffer_size/2.00);
				int endcnt = midcnt + detectangle.bandwidth/(MidBandwidth/2)*(fftbuffer_size/2.00);
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

   				double workdiffer12 =atan2((channel1_realtemp[i]*channel2_imagtemp[i]-channel1_imagtemp[i]*channel2_realtemp[i]),channel1_imagtemp[i]*channel2_imagtemp[i]+channel1_realtemp[i]*channel2_realtemp[i])*180.00/3.14159265;
   				double workdiffer13 =atan2((channel1_realtemp[i]*channel3_imagtemp[i]-channel1_imagtemp[i]*channel3_realtemp[i]),channel1_imagtemp[i]*channel3_imagtemp[i]+channel1_realtemp[i]*channel3_realtemp[i])*180.00/3.14159265;
 				double workdiffer14 =atan2((channel1_realtemp[i]*channel4_imagtemp[i]-channel1_imagtemp[i]*channel4_realtemp[i]),channel1_imagtemp[i]*channel4_imagtemp[i]+channel1_realtemp[i]*channel4_realtemp[i])*180.00/3.14159265;
				if(workdiffer12<0.00)
					workdiffer12= workdiffer12+360.00;
				//workdiffer12 =workdiffer12-180.00;
				if(workdiffer13<0.00)
					workdiffer13 = workdiffer13+360.00;
				//workdiffer13 =workdiffer13-180.00;
				if(workdiffer14<0.0)
					workdiffer14= workdiffer14+360.0;
				//workdiffer14 =workdiffer14-180.00;
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
				k2 = round((3.0*differ12-differ13)/360.0);
				k3 = round((4.0*differ12-differ14)/360.0);
				double differreal14 =0.0;
				double differreal13 =0.0;
				differreal14 = differ14/180.00 + 2.0*PI*k3;
				differreal13 = differ13/180.00 + 2.0*PI*k2;
				double angle1 = asin(differ12/180.0*(C/MidFre)/(2*PI*1*BASEDISTANCE))*180.00/PI;
				double angle2 = asin(differreal13*(C/MidFre)/(2.0*PI*3.0*BASEDISTANCE))*180.00/PI;
				double angle3 = asin(differreal14*(C/MidFre)/(2.0*PI*4.0*BASEDISTANCE))*180.00/PI;
				//printf("current differ12:%lf,differ13:%lf,differ14:%lf   \r\n",differ12,differ13,differ14);
				//printf("current k1:%d,k2:%d,k3:%d   \r\n",k1,k2,k3);
				//printf("current angle1:%lf,angle2:%lf,angle3:%lf   \r\n",angle1,angle2,angle3);
				int bytecount =12;
				buf1[0]=0x55;
				buf1[1]=0xA2;
				buf1[2]=bytecount&0xFF;
				buf1[3]=(bytecount>>8)&0xFF;
				//数据总长度
				buf1[4]=detectangle.Currentarraydirection&0xFF;
				buf1[5]=0x01;
				buf1[6]=(( unsigned short)(frequence/100000.0))&0xFF;
				buf1[7]=((( unsigned short)(frequence/100000.0))>>8)&0xFF;
				buf1[8]=((int8_t)(detectangle.bandwidth/1000000.0))&0xFF;
				buf1[9]=(( short)(angle1*100.00))&0xFF;
				buf1[10]=(( short)(angle1*100.00)>>8)&0xFF;
				buf1[bytecount-1] =XOR_check(buf1,bytecount);
 			if(check_nic("eth0") == 0)
        		{
			//printf("connect:success \r\n");
			close(lfd);
			lfd = socket(PF_INET, SOCK_DGRAM, 0);
			bind(lfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
			sendto(server_sockfd,buf1,bytecount,MSG_NOSIGNAL,(struct sockaddr*)&saddr0,sizeof(saddr0));
			}
			else{
			printf("connect:faild \r\n");
				}
				
			}
	
	
 	 	
   	}
}
 
 close(lfd);
  

}

