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
#include"threadpool.h"
#include<pthread.h>
#include<stdlib.h>
#include <unistd.h>
#include "fft.h"
#define CLOCKS_PER_SEC  1000000l
static volatile uint32_t *GPIO_ADF4351EN ;
 clock_t start;
 uint32_t *GPIO_APPITUDE ;
//axidma operate
static axidma_dev_t g_AxiDmaDev;
array_t* g_apRxChannels;
void* g_pAD9226DataBuf;
struct detectanglearg detectangle;
 int32_t server_sockfd;
 unsigned int R[6] = {0x580005,0x9A0024,0x4004B3,0x2004FC2,0x8008011,0x388000};
unsigned int R1[14] = {0xD,0x15FC,0x61200B,0xC00CBA,0xD08BCC9,0x15596568,0x60000E7,0x350100F6,0x800025,0x34008B84,0x3,0x12,0x1,0x300CB0};
bool isenable ;
bool  clearmaxholdflag = false;
bool  maxholdstate = true;
int issendSwitch ;
int  isconnflag ;
int issamp ;
int workmode ;
int centerfre ;
bool  getflag = false;
sem_t semsamplethread;
sem_t semsignaldealthread;
int8_t ReferenceThresholdPower = -75;
struct threadarg ThreadArg;
struct QueueInfo *queue;
struct networkQueueInfo *networkqueue;
pthread_mutex_t mutex;   
pthread_mutex_t mutexnetwork;  
static const char *device = "/dev/spidev1.0";//4351
static const char *device1 = "/dev/spidev1.1";//4356
static uint32_t spimode = 0;
static uint8_t bits = 8;
static uint32_t speed = 1000;
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
void *Memcpy(void *dest, const void *src, size_t count)  
{  
  // cout<<"sizeof(dest)是："<<sizeof(dest)<<endl;
	int bytelen=count/sizeof(dest);	/*按CPU位宽拷贝*/
	int slice=count%sizeof(dest);	/*剩余的按字节拷贝*/
	unsigned int* d = (unsigned int*)dest;  
    unsigned int* s = (unsigned int*)src;  

 if (((int)dest > ((int)src+count)) || (dest < src))  
    {  
    while (bytelen--)  
        *d++ = *s++;  
	while (slice--)  
        *(char *)d++ = *(char *)s++; 
    }  
 else /* overlap重叠 */  
    {  
    d = (unsigned int*)((unsigned int)dest + count - 4); /*指针位置从末端开始，注意偏置 */  
    s = (unsigned int*)((unsigned int)src + count -4);  
    while (bytelen --)  
        *d-- = *s--;  
	d++;s++;
	char * d1=(char *)d;
	char * s1=(char *)s;
	d1--;s1--;
	while (slice --)  
        *(char *)d1-- = *(char *)s1--; 
    }  
 return dest;  
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

	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[6]>>24)&0xFF;
	buf[1] = (R1[6]>>16)&0xFF;
	buf[2] = (R1[6]>>8)&0xFF;
	buf[3] = (R1[6])&0xFF;
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

	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[7]>>24)&0xFF;
	buf[1] = (R1[7]>>16)&0xFF;
	buf[2] = (R1[7]>>8)&0xFF;
	buf[3] = (R1[7])&0xFF;
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
	
	buf[0] = (R1[8]>>24)&0xFF;
	buf[1] = (R1[8]>>16)&0xFF;
	buf[2] = (R1[8]>>8)&0xFF;
	buf[3] = (R1[8])&0xFF;
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
	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[9]>>24)&0xFF;
	buf[1] = (R1[9]>>16)&0xFF;
	buf[2] = (R1[9]>>8)&0xFF;
	buf[3] = (R1[9])&0xFF;
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
	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[10]>>24)&0xFF;
	buf[1] = (R1[10]>>16)&0xFF;
	buf[2] = (R1[10]>>8)&0xFF;
	buf[3] = (R1[10])&0xFF;
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

	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[11]>>24)&0xFF;
	buf[1] = (R1[11]>>16)&0xFF;
	buf[2] = (R1[11]>>8)&0xFF;
	buf[3] = (R1[11])&0xFF;
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
	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[12]>>24)&0xFF;
	buf[1] = (R1[12]>>16)&0xFF;
	buf[2] = (R1[12]>>8)&0xFF;
	buf[3] = (R1[12])&0xFF;
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
	usleep(100);
	memset(&xfer,0,sizeof(xfer));
	memset(buf,0,sizeof(buf));
	
	buf[0] = (R1[13]>>24)&0xFF;
	buf[1] = (R1[13]>>16)&0xFF;
	buf[2] = (R1[13]>>8)&0xFF;
	buf[3] = (R1[13])&0xFF;
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

	fd = open(device, O_RDWR);
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
void* getdatathreadrun(void * arg){
while(1)
{
	usleep(1);
	
if(getflag)
{
	//pthread_mutex_lock(&mutex);
	if(queue_length(queue)<30){
	pthread_mutex_lock(&mutex);
	queue_push(queue, g_pAD9226DataBuf,fftbuffer_size*8*4);	
	pthread_mutex_unlock(&mutex);
	}
	//pthread_mutex_lock(&mutexgetdata);
	
	//pthread_mutex_unlock(&mutexgetdata); 
	//clock_t finish= clock();
   // double duration = (double)(finish - start) / CLOCKS_PER_SEC;
    //printf( "calcu run time :%f ms\r\n", duration*1000.0000 );

	}
}
}
void callbackAfterRecive(int channelid,void* data)
{
  // printf("INFO: callback func triggerd,channelid: %d\n",channelid);
	
	//pthread_mutex_lock(&mutexgetdata);
	getflag = true;
	//pthread_mutex_unlock(&mutexgetdata);  
	usleep(100);
	getflag = false;
	
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
      start= clock(); 
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

 	GPIO_APPITUDE = (uint32_t *) mmap(0, getpagesize() * 250, PROT_READ | PROT_WRITE, MAP_SHARED, gpioreservefd, GPIORESERVE_baseddr);
	
	

   if ((void *) GPIO_APPITUDE == MAP_FAILED) {
        printf("mmap GPIO_RESERVE failed: %s\n", strerror(errno));
        exit(1);
    }
//使能ADF4351芯片	
	*GPIO_ADF4351EN = 0xFF;
	*GPIO_APPITUDE = 0x00;

	//init spi adf4531
	initgpiospi();

	
	//Set_ADF4351_Reg(0x388000,0x8008011,0x2004FC2,0x4004B3,0x9A0024,0x580005); //1.13G 2.5mA
Set_ADF4351_Reg(0x2D0010,0x8008029,0x5E42,0x4B3,0x9C803C,0x580005); //1.13G 2.5mA
	sleep(1);
	//Set_ADF4351_Reg(0x2D0010,0x8008029,0x5E42,0x4B3,0x9C803C,0x580005); //1.13G 2.5mA
	
	Set_ADF4356_Reg(0x300CB0,0x1,0x12,0x3,0x34008B84,0x800025,0x350100F6,0x60000E7,
									0x15596568,0xD08BCC9,0xC00CBA,0x61200B,0x15FC,0xD);
	//Set_ADF4356_Reg(0x300CB0,0x1,0x12,0x40000003,0x3400BD84,0x800025,0x350540F6,0x40000F7,
			//						0x15596568,0xD08BCC9,0xC00CBA,0x161200B,0x15FC,0xD);	
	printf("ADF4351 ADF4356 init success!!!!\r\n");
	sleep(1);
   // spiconfig();
	//printf("spi1.1 init success!!!!\r\n");
	//sleep(1);

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
 
    
    axidma_stop_transfer(g_AxiDmaDev,g_apRxChannels->data[0]);
	
    axidma_set_callback(g_AxiDmaDev,g_apRxChannels->data[0],callbackAfterRecive,NULL);
	printf("preper to start adc\n");
    //memset(ThreadArg.ad9226fftdata,0,AD9226_SAMPLENUM*8);
 
    printf("the ADC has start...\n");
	
    struct timeval timeStart, timeEnd; 
 	
	while(1)
	{
		// 判断是否工作

	
        while(( issamp ==1))
		{

        	    usleep(1);
		  
    // printf("the ADC  start.  sample..\n");

					//read ad9226 data
			axidma_oneway_transfer(g_AxiDmaDev,g_apRxChannels->data[0],g_pAD9226DataBuf,(AD9226_SAMPLENUM)*8*4,false);
					
		   
		}	
	}
}int LinearSmooth72(double* input, long size) {
	
	double output[fftbuffer_size] = {0.0};
 
	if (size < 7)
	{
		for (int i = 0; i <= size - 1; i++)
		{
			output[i] = input[i];
		}
	}
	else
	{
		output[0] = (32.0 * input[0] + 15.0 * input[1] + 3.0 * input[2] - 4.0 * input[3] -
			6.0 * input[4] - 3.0 * input[5] + 5.0 * input[6]) / 42.0;
		output[1] = (5.0 * input[0] + 4.0 * input[1] + 3.0 * input[2] + 2.0 * input[3] +
			input[4] - input[6]) / 14.0;
		output[2] = (1.0 * input[0] + 3.0 * input[1] + 4.0 * input[2] + 4.0 * input[3] +
			3.0 * input[4] + 1.0 * input[5] - 2.0 * input[6]) / 14.0;
		for (int i = 3; i <= size - 4; i++)
		{
			output[i] = (-2.0 * (input[i - 3] + input[i + 3]) +
				3.0 * (input[i - 2] + input[i + 2]) +
				6.0 * (input[i - 1] + input[i + 1]) + 7.0 * input[i]) / 21.0;
		}
		output[size - 3] = (1.0 * input[size - 1] + 3.0 * input[size - 2] + 4.0 * input[size - 3] +
			4.0 * input[size - 4] + 3.0 * input[size - 5] + 1.0 * input[size - 6] - 2.0 * input[size - 7]) / 14.0;
		output[size - 2] = (5.0 * input[size - 1] + 4.0 * input[size - 2] + 3.0 * input[size - 3] +
			2.0 * input[size - 4] + input[size - 5] - input[size - 7]) / 14.0;
		output[size - 1] = (32.0 * input[size - 1] + 15.0 * input[size - 2] + 3.0 * input[size - 3] -
			4.0 * input[size - 4] - 6.0 * input[size - 5] - 3.0 * input[size - 6] + 5.0 * input[size - 7]) / 42.0;
	}
 
	for (int i = 0; i < size; i++)
	{
		input[i] = output[i];
	}

 
	return 0;
}

	
void task(void *arg){
	
	//printf("init work thread rsource success\r\n");

	struct	thread_arg *r = NULL;
	while ((r==NULL))
	{
		r = (struct thread_arg *)malloc(sizeof(struct thread_arg));
	}
	// thread_arg *r = malloc(sizeof(thread_arg));
	// memset(r->fftdata0,0,fftbuffer_size*8*4+6);
	 memcpy(r,arg,sizeof(struct thread_arg));
	 
    char buf1[fftbuffer_size*2+100]={0};
	char buf2[fftbuffer_size*2+100]={0};
	char buf3[fftbuffer_size*2+100]={0};
	char buf4[fftbuffer_size*2+100]={0};
	char buf5[fftbuffer_size*2+100]={0};
	int buf1bytecount =0;
	int buf2bytecount =0;
	int buf3bytecount =0;
	int buf4bytecount =0;
	int buf5bytecount =0;
	//模式数据有效标志
	int mode0valid = 0;
    int mode1valid = 0;
    int mode2valid = 0;
	int mode3valid = 0;
    	struct timeval timeStart, timeEnd; 
	double runTime=0;
	//printf("init rsource success\r\n");
	int re_list = -1;
	double maxagp = -1000.00;
	double avgreal12[fftbuffer_size] ={0.0};
	double avgimag12[fftbuffer_size] ={0.0};
	double avgreal13[fftbuffer_size] ={0.0};
	double avgimag13[fftbuffer_size] ={0.0};
	double avgreal14[fftbuffer_size] ={0.0};
	double avgimag14[fftbuffer_size] ={0.0};
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


	double channel1_appitudeslide[fftbuffer_size] ={0.0};
	double channel2_appitudeslide[fftbuffer_size] ={0.0};
	double channel3_appitudeslide[fftbuffer_size] ={0.0};
	double channel4_appitudeslide[fftbuffer_size] ={0.0};
	struct	networkQueueInfo *temp ;
	while ((temp==NULL))
	{
		temp = (struct networkQueueInfo *)malloc(sizeof(struct networkQueueInfo));
	}
	
	 //clock_t start, finish;
      // double duration;
       /* 测量一个事件持续的时间*/
		
		//取出fft数据
		
		 	//start = clock();
		for(int i = 0;i < fftbuffer_size/2;i++)
    			{
		channel1_realtemp[i] = (int)((*((unsigned int*)(r->fftdata0)+i*8+0*2)));
			channel1_imagtemp[i] = (int)((*((unsigned int*)(r->fftdata0)+i*8+0*2+1)));
			channel1_appitude[i] = 20*log10(sqrt(channel1_realtemp[i]*channel1_realtemp[i]+channel1_imagtemp[i]*channel1_imagtemp[i])/fftbuffer_size)-60;
			channel2_realtemp[i] = (int)((*((unsigned int*)(r->fftdata0)+i*8+1*2)));
			channel2_imagtemp[i] = (int)((*((unsigned int*)(r->fftdata0)+i*8+1*2+1)));
			channel2_appitude[i] = 20*log10(sqrt(channel2_realtemp[i]*channel2_realtemp[i]+channel2_imagtemp[i]*channel2_imagtemp[i])/fftbuffer_size)-60;
			channel3_realtemp[i] = (int)((*((unsigned int*)(r->fftdata0)+i*8+2*2)));
			channel3_imagtemp[i] = (int)((*((unsigned int*)(r->fftdata0)+i*8+2*2+1)));
			channel3_appitude[i] = 20*log10(sqrt(channel3_realtemp[i]*channel3_realtemp[i]+channel3_imagtemp[i]*channel3_imagtemp[i])/fftbuffer_size)-60;
			channel4_realtemp[i] = (int)((*((unsigned int*)(r->fftdata0)+i*8+3*2)));
			channel4_imagtemp[i] = (int)((*((unsigned int*)(r->fftdata0)+i*8+3*2+1))); 
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
			for(int i = 0;i < fftbuffer_size/2;i++)
    		{
				if(i<WindowsSize)
			{
				for(int z = 0;z<WindowsSize;z++)
				{
					channel1_appitudeslide[i] +=channel1_appitude[z];
					channel2_appitudeslide[i] +=channel2_appitude[z];
					channel3_appitudeslide[i] +=channel3_appitude[z];
					channel4_appitudeslide[i] +=channel4_appitude[z];
				}
				
			} else	if((WindowsSize<i)&&((i<(fftbuffer_size/2-WindowsSize))))
			{
				for(int z = i-WindowsSize/2;z<i+WindowsSize/2;z++)
				{
					
					channel1_appitudeslide[i] +=channel1_appitude[z];
					channel2_appitudeslide[i] +=channel2_appitude[z];
					channel3_appitudeslide[i] +=channel3_appitude[z];
					channel4_appitudeslide[i] +=channel4_appitude[z];
				}
			}
			 else	if(i>(fftbuffer_size/2-WindowsSize))
			{
				for(int z = i-WindowsSize;z<i;z++)
				{
					
					channel1_appitudeslide[i] +=channel1_appitude[z];
					channel2_appitudeslide[i] +=channel2_appitude[z];
					channel3_appitudeslide[i] +=channel3_appitude[z];
					channel4_appitudeslide[i] +=channel4_appitude[z];
				}
			}
				channel1_appitudeslide[i] = channel1_appitudeslide[i]/WindowsSize;
				channel2_appitudeslide[i] = channel2_appitudeslide[i]/WindowsSize;
				channel3_appitudeslide[i] = channel3_appitudeslide[i]/WindowsSize;
				channel4_appitudeslide[i] = channel4_appitudeslide[i]/WindowsSize;

			}
				for(int i = 0;i < fftbuffer_size/2;i++)
				{
					channel1_appitude[i] = channel1_appitudeslide[i];
					channel2_appitude[i] = channel2_appitudeslide[i];
					channel3_appitude[i] = channel3_appitudeslide[i];
					channel4_appitude[i] = channel4_appitudeslide[i];
				}
			int count_num =10;
			int fftnum = 0;
			int bytecount =14;
			buf1[0]=0x55;
			buf1[1]=0xA0;
			//数据总长度
			buf1[4]=0x01;
			buf1[5]=(detectangle.Currentarraydirection&0xFF);
			buf1[6]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0))&0xFF;
			buf1[7]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0)>>8)&0xFF;
			buf1[8]=((unsigned short)((MidBandwidth/100000.00)/2.0))&0xFF;
			buf1[9]=((unsigned short)((MidBandwidth/100000.00)/2.0)>>8)&0xFF;
			buf1[12]=((int)ReferenceThresholdPower)&0xFF;
			if(*GPIO_APPITUDE == 0x00)
			buf1[13] = 0xD8;
			else if(*GPIO_APPITUDE == 0x01)
			buf1[13] = 0xEC;
			else if(*GPIO_APPITUDE == 0x03)
			buf1[13] = 0x00;
			buf2[0]=0x55;
			buf2[1]=0xA0;
			//数据总长度
			buf2[4]=0x02;
			buf2[5]=(detectangle.Currentarraydirection&0xFF);
			buf2[6]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0))&0xFF;
			buf2[7]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0)>>8)&0xFF;
			buf2[8]=((unsigned short)((MidBandwidth/100000.00)/2.0))&0xFF;
			buf2[9]=((unsigned short)((MidBandwidth/100000.00)/2.0)>>8)&0xFF;
			
			buf2[12]=((int)ReferenceThresholdPower)&0xFF;
			if(*GPIO_APPITUDE == 0x00)
			buf2[13] = 0xD8;
			else if(*GPIO_APPITUDE == 0x01)
			buf2[13] = 0xEC;
			else if(*GPIO_APPITUDE == 0x03)
			buf2[13] = 0x00;
			buf3[0]=0x55;
			buf3[1]=0xA0;
			//数据总长度
			buf3[4]=0x03;
			buf3[5]=(detectangle.Currentarraydirection&0xFF);
			buf3[6]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0))&0xFF;
			buf3[7]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0)>>8)&0xFF;
			buf3[8]=((unsigned short)((MidBandwidth/100000.00)/2.0))&0xFF;
			buf3[9]=((unsigned short)((MidBandwidth/100000.00)/2.0)>>8)&0xFF;
			
			buf3[12]=((int)ReferenceThresholdPower)&0xFF;
			if(*GPIO_APPITUDE == 0x00)
			buf3[13] = 0xD8;
			else if(*GPIO_APPITUDE == 0x01)
			buf3[13] = 0xEC;
			else if(*GPIO_APPITUDE == 0x03)
			buf3[13] = 0x00;
			buf4[0]=0x55;
			buf4[1]=0xA0;
			//数据总长度
			buf4[4]=0x04;
			buf4[5]=(detectangle.Currentarraydirection&0xFF);
			buf4[6]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0))&0xFF;
			buf4[7]=((unsigned short)((MidFre-MidBandwidth/4.0)/100000.0)>>8)&0xFF;
			buf4[8]=((unsigned short)((MidBandwidth/100000.00)/2.0))&0xFF;
			buf4[9]=((unsigned short)((MidBandwidth/100000.00)/2.0)>>8)&0xFF;
			
			buf4[12]=((int)ReferenceThresholdPower)&0xFF;
			if(*GPIO_APPITUDE == 0x00)
			buf4[13] = 0xD8;
			else if(*GPIO_APPITUDE == 0x01)
			buf4[13] = 0xEC;
			else if(*GPIO_APPITUDE == 0x03)
			buf4[13] = 0x00;
			for(int i =0;i<fftbuffer_size/2;i++)
			{
				buf1[(i)*2 +14] = (short)(channel1_appitude[i]*100.00)&0xFF;
				buf1[(i)*2 +14+1] = (( short)(channel1_appitude[i]*100.00)>>8)&0xFF;
				//printf("i:%d,avg1:%d  \r\n",channel1_appitude,(short)(avg_appitude[0][i]*100.00 ));
				buf2[(i)*2 +14] = ( short)(channel2_appitude[i]*100.00)&0xFF;
				buf2[(i)*2 +14+1] = (( short)(channel2_appitude[i]*100.00)>>8)&0xFF;
				buf3[(i)*2 +14] = ( short)(channel3_appitude[i]*100.00)&0xFF;
				buf3[(i)*2 +14+1] = (( short)(channel3_appitude[i]*100.00)>>8)&0xFF;
				buf4[(i)*2 +14] = ( short)(channel4_appitude[i]*100.00)&0xFF;
				buf4[(i)*2 +14+1] = (( short)(channel4_appitude[i]*100.00)>>8)&0xFF;
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
			 buf1bytecount =bytecount;
			 buf2bytecount =bytecount;
			 buf3bytecount =bytecount;
			 buf4bytecount =bytecount;

			if((detectangle.mode == 0))//单音信号测试 寻找峰值点

			{ 
				double frequence =  detectangle.frequence;
				int  midcnt =   (frequence -(MidFre-MidBandwidth/4.00))/(MidBandwidth/2)*(fftbuffer_size/2.0);
				int startcnt = midcnt - 0.2E6/(MidBandwidth/2.0)*(fftbuffer_size/2.0);
				int endcnt = midcnt + 0.2E6/(MidBandwidth/2.0)*(fftbuffer_size/2.0);


				//printf("midcnt:%d,startcnt:%d,endcnt:%d;\r\n",midcnt,startcnt,endcnt);
				int maxpos1 =0;
				double maxappitude1 =-200;
				int maxpos2 =0;
				double maxappitude2 =-200;
				int maxpos3 =0;
				double maxappitude3 =-200;
				int maxpos4 =0;
				double maxappitude4 =-200;

				for(int i =startcnt;i<=endcnt;i++)
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
				if(((maxappitude1>ReferenceThresholdPower)&&(maxappitude2>ReferenceThresholdPower)/*&&(maxappitude3>ReferenceThresholdPower)&&(maxappitude4>ReferenceThresholdPower))*/&&(maxpos1==maxpos2)/*&&(maxpos1==maxpos3)&&(maxpos1==maxpos4)*/))
				{
					//printf("max1:%lf,max2:%lf,  \r\n",maxappitude1,maxappitude2);
				//	printf("aaaa  differ12:%lf,differ13:%lf ,differ14:%lf\r\n",differ12,differ13,differ14);
					mode1valid =0;
					mode0valid = 1;
					 mode2valid =0;
				}else
				{
					mode1valid =0;
					mode0valid = 0;
					mode2valid =0;
					
				}
				//printf("aaaa  differ12:%lf,differ13:%lf ,differ14:%lf\r\n",differ12,differ13,differ14);
				unsigned short frequencea = (double)((((double)maxpos1)/(fftbuffer_size/2.0)*MidBandwidth/2.0/1.0E6+5245294118/1.0E6+3.6)*10.00);
				//printf("frequence:%d,maxpos1:%d \r\n",frequencea,maxpos1);
				buf5bytecount =16;
				buf5[0]=0x55;
				buf5[1]=0xA1;
				buf5[2]=buf5bytecount&0xFF;
				buf5[3]=(buf5bytecount>>8)&0xFF;
				//数据总长度
				buf5[4]= detectangle.Currentarraydirection&0x00FF;
				//buf1[4] = 0x01;
				//printf("detectangle.Currentarraydirection:%d,buf1[4]:%d   \r\n",detectangle.Currentarraydirection,buf1[4]);
				buf5[5]=0x00;
				buf5[6]=(frequencea)&0xFF;
				buf5[7]=((frequencea)>>8)&0xFF;
				buf5[8]=0x00;
				buf5[9]=(( unsigned short)((differ12+180)*100.00))&0xFF;
				buf5[10]=((unsigned short)((differ12+180)*100.00)>>8)&0xFF;
				buf5[11]=((unsigned short)((differ13+180)*100.00))&0xFF;
				buf5[12]=((unsigned short)((differ13+180)*100.00)>>8)&0xFF;
				buf5[13]=((unsigned short)((differ14+180)*100.00))&0xFF;
				buf5[14]=((unsigned short)((differ14+180)*100.00)>>8)&0xFF;
				
				uint8_t result = 0;
				for( int i = 0;i<buf5bytecount-1;i++)
				{
					result = result^(buf5[i]);
				}
				buf5[buf5bytecount-1] =result;
    			}
			else if((detectangle.mode == 1)) //单音信号测向模式
			{
				double frequence =  detectangle.frequence;
				int  midcnt =   (frequence -(MidFre-MidBandwidth/4.00))/(MidBandwidth/2)*(fftbuffer_size/2.0);
				int startcnt = midcnt - 0.2E6/(MidBandwidth/2.0)*(fftbuffer_size/2.0);
				int endcnt = midcnt + 0.2E6/(MidBandwidth/2.0)*(fftbuffer_size/2.0);
				int maxpos1 =0;
				double maxappitude1 =-200;
				int maxpos2 =0;
				double maxappitude2 =-200;
				int maxpos3 =0;
				double maxappitude3 =-200;
				int maxpos4 =0;
				double maxappitude4 =-200;
				for(int i =startcnt;i<=endcnt;i++)
				//for(int i =20;i<fftbuffer_size/2-100;i++)
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
				//differ12 = 148.4458;
				//differ13 = 85.2952;
				//differ14 = -126.3859;
				k2 = round((3.0*differ12-differ13)/360.0);
				k3 = round((4.0*differ12-differ14)/360.0);
				double differreal14 =0.0;
				double differreal13 =0.0;
				differreal14 = differ14 + 2.0*180.00*k3;
				differreal13 = differ13 + 2.0*180.00*k2;
				
				double angle1 = -asin(differ12/180.00*PI*(C/MidFre)/(2*PI*1*BASEDISTANCE))*180.00/PI;
				double angle2 = -asin(differreal13/180.00*PI*(C/MidFre)/(2*PI*3.0*BASEDISTANCE))*180.00/PI;
				double angle3 = -asin(differreal14/180.00*PI*(C/MidFre)/(2*PI*4.0*BASEDISTANCE))*180.00/PI;
				if(angle1<0.0)
				angle1 = angle1+360.0;
				if(angle2<0.0)
				angle2 = angle2+360.0;
				if(angle3<0.0)
				angle3 = angle3+360.0;
				//printf("current k1:%d,k2:%d,k3:%d   \r\n",k1,k2,k3);
				//printf("current angle1:%lf,angle2:%lf,angle3:%lf   \r\n",angle1,angle2,angle3);

				//printf("maxpos1:%d,maxpos2:%d,maxpos3:%d,maxpos4:%d   \r\n",maxpos1,maxpos2,maxpos3,maxpos4);
				if(((maxappitude1>ReferenceThresholdPower)&&(maxappitude2>ReferenceThresholdPower)/*&&(maxappitude3>ReferenceThresholdPower)&&(maxappitude4>ReferenceThresholdPower))*/&&(maxpos1==maxpos2)/*&&(maxpos1==maxpos3)&&(maxpos1==maxpos4)*/))
				{
					//printf("maxpos1:%d,maxpos2:%d,maxpos3:%d,maxpos4:%d   \r\n",maxpos1,maxpos2,maxpos3,maxpos4);
					// printf("aaaa  differ12:%lf,differ13:%lf ,differ14:%lf\r\n",differ12,differ13,differ14);
					// printf("current angle1:%lf,angle2:%lf,angle3:%lf   \r\n",angle1,angle2,angle3);
					// printf("current angle1:%lf,angle2:%lf,angle3:%lf   \r\n",angle1,angle2,angle3);
					mode1valid =1;
					mode0valid = 0;
					mode2valid =0;
				}else
				{
					mode1valid =0;
					mode0valid = 0;
					mode2valid =0;
					
				}
				//printf("current differ12:%lf,differ13:%lf,differ14:%lf   \r\n",differ12,differ13,differ14);
				//printf("current k1:%d,k2:%d,k3:%d   \r\n",k1,k2,k3);
				
				unsigned short frequencea = (double)((((double)maxpos1)/(fftbuffer_size/2.0)*MidBandwidth/2.0/1.0E6+5245294118/1.0E6+3.6)*10.00);
				buf5bytecount =12;
				buf5[0]=0x55;
				buf5[1]=0xA2;
				buf5[2]=buf5bytecount&0xFF;
				buf5[3]=(buf5bytecount>>8)&0xFF;
				//数据总长度
				buf5[4]=detectangle.Currentarraydirection&0xFF;
				//buf1[4]= 0x01;
				//printf("buf1[4]:%d,\r\n",buf1[4]);
				buf5[5]=0x00;
				buf5[6]=((unsigned short)frequencea)&0xFF;
				buf5[7]=(((unsigned short)frequencea)>>8)&0xFF;
				buf5[8]=((uint8_t)(detectangle.bandwidth/20000.0))&0xFF;
				buf5[9]=(( short)(angle1*100.0))&0xFF;
				buf5[10]=(( short)(angle1*100.0)>>8)&0xFF;
				
				uint8_t result = 0;
				for( int i = 0;i<buf5bytecount-1;i++)
				{
					result = result^(buf5[i]);
				}
				buf5[buf5bytecount-1] =result;
				
				
 			}else if((detectangle.mode == 2))//宽带信号测向模式
			{
				//获取测向频率和带宽
				double frequence =  detectangle.frequence;
				double bandwidth = detectangle.bandwidth;
				int  midcnt =   (frequence -(MidFre-MidBandwidth/4.00))/(MidBandwidth/2)*(fftbuffer_size/2.0);
				int startcnt = midcnt - detectangle.bandwidth/(MidBandwidth/2)*(fftbuffer_size/2.0)/2.00;
				int endcnt = midcnt + detectangle.bandwidth/(MidBandwidth/2)*(fftbuffer_size/2.0)/2.00;
				int maxpos1 =0;
			//计算平均幅度是否大于-80dbm
			double avgapt1 = 0.0 ;
			double avgapt2 = 0.0 ;
			double avgapt3 = 0.0 ;
			double avgapt4 = 0.0 ;
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
					//计算四通道相位差
				avgapt1   += channel1_appitude[i];
				avgapt2   += channel2_appitude[i];
				avgapt3   += channel3_appitude[i];
				avgapt4   += channel4_appitude[i];
   				double workdiffer12 =atan2((channel1_realtemp[i]*channel2_imagtemp[i]-channel1_imagtemp[i]*channel2_realtemp[i]),channel1_imagtemp[i]*channel2_imagtemp[i]+channel1_realtemp[i]*channel2_realtemp[i])*180.00/3.14159265;
   				double workdiffer13 =atan2((channel1_realtemp[i]*channel3_imagtemp[i]-channel1_imagtemp[i]*channel3_realtemp[i]),channel1_imagtemp[i]*channel3_imagtemp[i]+channel1_realtemp[i]*channel3_realtemp[i])*180.00/3.14159265;
 				double workdiffer14 =atan2((channel1_realtemp[i]*channel4_imagtemp[i]-channel1_imagtemp[i]*channel4_realtemp[i]),channel1_imagtemp[i]*channel4_imagtemp[i]+channel1_realtemp[i]*channel4_realtemp[i])*180.00/3.14159265;
				
				differ12 = differ12 +workdiffer12;
				differ13 = differ13 +workdiffer13;
				differ14 = differ14 +workdiffer14;
				count ++;
				}
				
					
				
				differ12 = differ12/count;
				differ13 = differ13/count;
				differ14 = differ14/count;
				avgapt1 = avgapt1/count;
				avgapt2 = avgapt2/count;
				avgapt3 = avgapt3/count;
				avgapt4 = avgapt4/count;
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
				
				if(((avgapt1>ReferenceThresholdPower)&&(avgapt2>ReferenceThresholdPower)/*&&(avgapt3>ReferenceThresholdPower)&&(avgapt4>ReferenceThresholdPower)*/))
				{
					//printf("avgapt1:%d,avgapt2:%d,avgapt3:%d,avgapt4:%d   \r\n",avgapt1,avgapt2,avgapt3,avgapt4);
					printf("aaaa  differ12:%lf,differ13:%lf ,differ14:%lf\r\n",differ12,differ13,differ14);
					mode1valid =0;
					mode0valid = 0;
					mode2valid =1;
				}else
				{
					mode1valid =0;
					mode0valid = 0;
					mode2valid =0;
				}
				int k1 = 0;
				int k2 = 0;
				int k3 = 0;
				k2 = round((3.0*differ12-differ13)/360.0);
				k3 = round((4.0*differ12-differ14)/360.0);
				double differreal14 =0.0;
				double differreal13 =0.0;
				differreal14 = differ14 + 2.0*180.00*k3;
				differreal13 = differ13 + 2.0*180.00*k2;
				
				double angle1 = -asin(differ12/180.00*PI*(C/MidFre)/(2*PI*1*BASEDISTANCE))*180.00/PI;
				double angle2 = -asin(differreal13/180.00*PI*(C/MidFre)/(2*PI*3.0*BASEDISTANCE))*180.00/PI;
				double angle3 = -asin(differreal14/180.00*PI*(C/MidFre)/(2*PI*4.0*BASEDISTANCE))*180.00/PI;
				if(angle1<0.0)
				angle1 = angle1+360.0;
				if(angle2<0.0)
				angle2 = angle2+360.0;
				if(angle3<0.0)
				angle3 = angle3+360.0;
				//printf("current differ12:%lf,differ13:%lf,differ14:%lf   \r\n",differ12,differ13,differ14);
				//printf("current k1:%d,k2:%d,k3:%d   \r\n",k1,k2,k3);
				//printf("current angle1:%lf,angle2:%lf,angle3:%lf   \r\n",angle1,angle2,angle3);
			 buf5bytecount =12;
				buf5[0]=0x55;
				buf5[1]=0xA2;
				buf5[2]=buf5bytecount&0xFF;
				buf5[3]=(buf5bytecount>>8)&0xFF;
				//数据总长度
				buf5[4]=detectangle.Currentarraydirection&0xFF;
				buf5[5]=0x01;
				buf5[6]=(( unsigned short)(frequence/100000.0))&0xFF;
				buf5[7]=((( unsigned short)(frequence/100000.0))>>8)&0xFF;
				buf5[8]=((uint8_t)(detectangle.bandwidth/20000.0))&0xFF;
				buf5[9]=(( short)(angle1*100.00))&0xFF;
				buf5[10]=(( short)(angle1*100.00)>>8)&0xFF;
					
				
				uint8_t result = 0;
				for( int i = 0;i<buf5bytecount-1;i++)
				{
					result = result^(buf5[i]);
				}
				buf5[buf5bytecount-1] =result;
			
				
			}
			else if((detectangle.mode == 3))
		
		{
			//进行能量检测 搜索步进1MHz。根据最高峰值位置进行判断  最大目标 30个   峰值点附近0.2MHZ带宽进行测向
			double SNR = 0.2;
		int maxposstart[MAXAIMNUM] ={0};
		int maxposstop[MAXAIMNUM] ={0};
		
		
		
		int aimnum = 0;
		
		
		//start to detect
		//计算信号功率均值
		
		
		bool  startdetectflag = true;
		
		double avgappitude12[fftbuffer_size/2] = {0.0};
		double avgappitude13[fftbuffer_size/2] = {0.0};
		double avgappitude14[fftbuffer_size/2] = {0.0};
		double avgappitudeslide12[fftbuffer_size/2] = {0.0};
		double avgappitudeslide13[fftbuffer_size/2] = {0.0};
		double avgappitudeslide14[fftbuffer_size/2] = {0.0};
		double maxagp = -1000.00;
		//一次删选得到初次 异常信号
		//对信号进行共轭相乘 去除四个通道的白噪声 剔除噪声信号  保留共有信号
		for(int i = 0;i<fftbuffer_size/2;i++ )
		{
				avgimag12[i] = channel1_realtemp[i]*channel2_imagtemp[i]-channel1_imagtemp[i]*channel2_realtemp[i];
				avgreal12[i] = channel1_imagtemp[i]*channel2_imagtemp[i]+channel1_realtemp[i]*channel2_realtemp[i];
				avgimag13[i] = channel1_realtemp[i]*channel3_imagtemp[i]-channel1_imagtemp[i]*channel3_realtemp[i];
				avgreal13[i] = channel1_imagtemp[i]*channel3_imagtemp[i]+channel1_realtemp[i]*channel3_realtemp[i];
				avgimag14[i] = channel1_realtemp[i]*channel4_imagtemp[i]-channel1_imagtemp[i]*channel4_realtemp[i];
				avgreal14[i] = channel1_imagtemp[i]*channel4_imagtemp[i]+channel1_realtemp[i]*channel4_realtemp[i];
				avgappitude12[i] = sqrt(avgimag12[i]*avgimag12[i]+avgreal12[i]*avgreal12[i])/(( sqrt(channel1_realtemp[i]*channel1_realtemp[i]+channel1_imagtemp[i]*channel1_imagtemp[i]))*(sqrt(channel2_realtemp[i]*channel2_realtemp[i]+channel2_imagtemp[i]*channel2_imagtemp[i])));
				avgappitude13[i] = sqrt(avgimag13[i]*avgimag13[i]+avgreal13[i]*avgreal13[i])/(( sqrt(channel1_realtemp[i]*channel1_realtemp[i]+channel1_imagtemp[i]*channel1_imagtemp[i]))*(sqrt(channel3_realtemp[i]*channel3_realtemp[i]+channel3_imagtemp[i]*channel3_imagtemp[i])));
				avgappitude12[i] = sqrt(avgimag14[i]*avgimag14[i]+avgreal14[i]*avgreal14[i])/(( sqrt(channel1_realtemp[i]*channel1_realtemp[i]+channel1_imagtemp[i]*channel1_imagtemp[i]))*(sqrt(channel4_realtemp[i]*channel4_realtemp[i]+channel4_imagtemp[i]*channel4_imagtemp[i])));
				printf("i:%d,avgimag12:%lf ,avgreal12:%lf,avgappitude12:%lf ,temp:%lf\r\n",i,avgimag12[i],avgreal12[i],sqrt(avgimag12[i]*avgimag12[i]+avgreal12[i]*avgreal12[i]),(( sqrt(channel1_realtemp[i]*channel1_realtemp[i]+channel1_imagtemp[i]*channel1_imagtemp[i]))*(sqrt(channel2_realtemp[i]*channel2_realtemp[i]+channel2_imagtemp[i]*channel2_imagtemp[i]))));
				if(i>100)
				if(maxagp<=channel1_appitude[i])
				{
					maxagp=channel1_appitude[i];
				}
		}
		//	数据平滑 
		
		double  maxref = -10000.00;
		int paxrefpos1 = 0 ;
		for(int i = 0;i < fftbuffer_size/2;i++)
    		{
				if(i<WindowsSize)
			{
				for(int z = 0;z<WindowsSize;z++)
				{
					avgappitudeslide12[i] +=avgappitude12[z];	
				}
				
			} else	if((WindowsSize<i)&&((i<(fftbuffer_size/2-WindowsSize))))
			{
				for(int z = i-WindowsSize/2;z<i+WindowsSize/2;z++)
				{
					avgappitudeslide12[i] +=avgappitude12[z];	
				}
			}
			 else	if(i>(fftbuffer_size/2-WindowsSize))
			{
				for(int z = i-WindowsSize;z<i;z++)
				{	
					avgappitudeslide12[i] +=avgappitude12[z];	
				}
			}
				avgappitudeslide12[i] = avgappitudeslide12[i]/WindowsSize;
				if(i>100)
				if( maxref<=avgappitudeslide12[i] )
				{
					maxref=avgappitudeslide12[i] ;
					paxrefpos1 = i;
				}
				
			}
		//	printf("paxrefpos:%d,\r\n",paxrefpos);
		int currentpos = 30;
		for(int i  = 10;i<fftbuffer_size/2;i++)
		{
			/* code */
			if(startdetectflag)
			{
				if(avgappitudeslide12[i]/maxref>=SNR)
				{
				maxposstart[aimnum] = i;
				startdetectflag = false;
				//printf("has detected aim start \r\n");
				}
			}
			if(!startdetectflag)
			{
				if(avgappitudeslide12[i]/maxref<SNR)
				{
				maxposstop[aimnum] = i;
				aimnum ++;
				startdetectflag = true;
				//printf("has detected aim end \r\n");
				}
			}
		}
		int maxposstartcorrect[MAXAIMNUM] ={0};
		int maxposstopcorrect[MAXAIMNUM] ={0};
		int aimnumcorrect = 0;
		double angle1[MAXAIMNUM] ={0.0};
		double angle2[MAXAIMNUM] ={0.0};
		double angle3[MAXAIMNUM] ={0.0};
		//二次删选  合并信号
		//两个信号 间距小于100个点为一个信号
		for(int i = 0;i<aimnum;i++)
		{
			if(i == 0)
			{
				maxposstartcorrect[aimnumcorrect] = maxposstart[i];
				maxposstopcorrect[aimnumcorrect] = maxposstop[i];
			}
			if((i<(aimnum-1))&&(i>0))
			
			{
				if((maxposstart[i]-maxposstopcorrect[aimnumcorrect])>200)
			{
				//
				aimnumcorrect ++;
				mode3valid = 1;
				maxposstartcorrect[aimnumcorrect] = maxposstart[i];
				maxposstopcorrect[aimnumcorrect] = maxposstop[i];
				
				//printf("current aimnumcorrect:%d,maxposstartcorrect:%d,maxposstopcorrect:%d \r\n",aimnumcorrect,maxposstartcorrect[aimnumcorrect],maxposstopcorrect[aimnumcorrect]);
			}
			else if((maxposstart[i]-maxposstopcorrect[aimnumcorrect])<200){
				maxposstopcorrect[aimnumcorrect] = maxposstop[i];
				//printf("current aimnumcorrect:%d,maxposstartcorrect:%d,maxposstopcorrect:%d \r\n",aimnumcorrect,maxposstartcorrect[aimnumcorrect],maxposstopcorrect[aimnumcorrect]);
			}
			}
			else if(aimnum==1){
				mode3valid = 1;
				maxposstartcorrect[aimnumcorrect] = maxposstart[i];
				maxposstopcorrect[aimnumcorrect] = maxposstop[i];
				aimnumcorrect ++;
			}

		}
		//打印探测到的信号
		for(int z = 0;z<aimnum;z++)
		{
			printf("   i:%d,start pos:%d, end pos:%d \r\n",z,maxposstart[z],maxposstop[z]);
			
		}
		//判断峰值点的位置是否差值在小范围内
		//三次剔除错值
		int correcttimes3 = 0;
		for(int z = 0;z<aimnumcorrect;z++)
		{
			printf("corrrect   i:%d,start pos:%d, end pos:%d \r\n",z,maxposstartcorrect[z],maxposstopcorrect[z]);
			int posref1 = 0;
			int posref2 = 0;
			int posref3 = 0;
			int posref4 = 0;
			double ref1 =  -200.00;
			double ref2 =  -200.00;
			double ref3 =  -200.00;
			double ref4 =  -200.00;
			for(int i = maxposstartcorrect[z];i<maxposstopcorrect[z];i++)
			{
				if(ref1<=channel1_appitude[i])
				{
					ref1 = channel1_appitude[i];
					posref1 = i;
				}
				if(ref2<=channel2_appitude[i])
				{
					ref2 = channel2_appitude[i];
					posref2 = i;
				}
				if(ref3<=channel3_appitude[i])
				{
					ref3 = channel3_appitude[i];
					posref3 = i;
				}
				if(ref4<=channel4_appitude[i])
				{
					ref4 = channel4_appitude[i];
					posref4 = i;
				}
			}
			if((abs(posref1-posref2)==0)||(abs(posref1-posref2)==0)||(abs(posref1-posref2)==0))
			{
				correcttimes3++;
			}
			
		}
		//开始测向
		
	if(abs(correcttimes3-aimnumcorrect)>0)		
	{	
		for(int z = correcttimes3;z<aimnumcorrect;z++)
		{
			double differ12 = 0.0 ,differ13 = 0.0,differ14 = 0.0;
			int count = 0; 
			for(int i = maxposstartcorrect[z];i<maxposstopcorrect[z];i++)
			{
					//计算四通道相位差
   				double workdiffer12 =atan2((channel1_realtemp[i]*channel2_imagtemp[i]-channel1_imagtemp[i]*channel2_realtemp[i]),channel1_imagtemp[i]*channel2_imagtemp[i]+channel1_realtemp[i]*channel2_realtemp[i])*180.00/3.14159265;
   				double workdiffer13 =atan2((channel1_realtemp[i]*channel3_imagtemp[i]-channel1_imagtemp[i]*channel3_realtemp[i]),channel1_imagtemp[i]*channel3_imagtemp[i]+channel1_realtemp[i]*channel3_realtemp[i])*180.00/3.14159265;
 				double workdiffer14 =atan2((channel1_realtemp[i]*channel4_imagtemp[i]-channel1_imagtemp[i]*channel4_realtemp[i]),channel1_imagtemp[i]*channel4_imagtemp[i]+channel1_realtemp[i]*channel4_realtemp[i])*180.00/3.14159265;
				
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
				else if(detectangle.Currentarraydirection == 1)
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
				differreal14 = differ14 + 2.0*180.00*k3;
				differreal13 = differ13 + 2.0*180.00*k2;
				
				 angle1[z] = -asin(differ12/180.00*PI*(C/MidFre)/(2*PI*1*BASEDISTANCE))*180.00/PI;
				 angle2[z] = -asin(differreal13/180.00*PI*(C/MidFre)/(2*PI*3.0*BASEDISTANCE))*180.00/PI;
				 angle3[z] = -asin(differreal14/180.00*PI*(C/MidFre)/(2*PI*4.0*BASEDISTANCE))*180.00/PI;
				if(angle1[z]<0.0)
				angle1[z] = angle1[z]+360.0;
				if(angle2[z]<0.0)
				angle2[z] = angle2[z]+360.0;
				if(angle3[z]<0.0)
				angle3[z] = angle3[z]+360.0;
				printf("i:%d,angle1:%lf,angle2:%lf,angle3:%lf  \r\n",z,angle1[z],angle2[z],angle3[z]);
		}
	}
		}
	
			


	temp->parm.mode = detectangle.mode ;
	temp->parm.numbuf1 = buf1bytecount;
	temp->parm.numbuf2 = buf2bytecount;
	temp->parm.numbuf3 = buf3bytecount;
	temp->parm.numbuf4 = buf4bytecount;
	temp->parm.numbuf5 = buf5bytecount;
	temp->parm.mode0valid = mode0valid;
	temp->parm.mode1valid = mode1valid;
	temp->parm.mode2valid = mode2valid;
	temp->parm.mode3valid = mode3valid;
	temp->parm.maxagp = maxagp;
    memcpy(temp->parm.buf1,buf1,buf1bytecount);
	memcpy(temp->parm.buf2,buf2,buf2bytecount);
	memcpy(temp->parm.buf3,buf3,buf3bytecount);
	memcpy(temp->parm.buf4,buf4,buf4bytecount);
	memcpy(temp->parm.buf5,buf5,buf5bytecount);
	//memcpy(temp->parm.channel1fftdata,channel1_appitude,fftbuffer_size/2*sizeof(double));
	//memcpy(temp->parm.channel2fftdata,channel2_appitude,fftbuffer_size/2*sizeof(double));
	//memcpy(temp->parm.channel3fftdata,channel3_appitude,fftbuffer_size/2*sizeof(double));
	//memcpy(temp->parm.channel4fftdata,channel4_appitude,fftbuffer_size/2*sizeof(double));
	if(networkqueue_length(networkqueue)<60)
	{
			pthread_mutex_lock(&mutexnetwork);
		
			networkqueue_push(networkqueue,temp );
			pthread_mutex_unlock(&mutexnetwork);
	}
//	printf("deal complet !!! \r\n");
    free(r);
	free(temp);
	free(arg);
 
}
void* networkthread(void * arg){
		printf("start init network resource \r\n");
	//循环检查网络缓存数据链表
	int mode = -2;
   char buf1[fftbuffer_size*2+100];
	char buf2[fftbuffer_size*2+100];
	char buf3[fftbuffer_size*2+100];
	char buf4[fftbuffer_size*2+100];
	char buf5[fftbuffer_size*2+100];
	double maxagpbuf[100] = {-1000.00};
	int numbuf1 = 0 ;
	int numbuf2 = 0 ;
	int numbuf3 = 0 ;
	int numbuf4 = 0 ;
	int numbuf5 = 0 ;
	int mode0valid = 0;
    int mode1valid = 0;
    int mode2valid = 0;
	//初始化网络资源
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
	struct sockaddr_in saddr1,saddr2,saddr3,saddr4,saddr0,saddr5,saddr6,saddr7,saddr8;
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
	struct	networkQueueInfo *temp = NULL;
	while ((temp==NULL))
	{
		temp = (struct networkQueueInfo *)malloc(sizeof(struct networkQueueInfo));
	}
	
	int agpbufnum = 0;
		///	maxholdbuf4[12]=0;
	while(1){
	//printf("network  length:%d \r\n",networkqueue_length(networkqueue));
		usleep(1);
	
		//if(networkqueue_length(networkqueue)>55)
			{
				if(check_nic("eth0") == 0)
				{
					close(lfd);
					lfd = socket(PF_INET, SOCK_DGRAM, 0);
					bind(lfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
				//	for(int i = 0;i<50;i++)
				{
					pthread_mutex_lock(&mutexnetwork);

					int relist = networkqueue_pop(networkqueue, temp);
					pthread_mutex_unlock(&mutexnetwork);
			//printf("get  numbuf1:%d,numbuf2:%d,numbuf3:%d,numbuf4:%d,numbuf5:%d,  \r\n",numbuf1,numbuf2,numbuf3,numbuf4,numbuf5);
	
        		
			
					if(relist == 0)
					{
					//	printf(" get network data :success \r\n");
					// 	if(temp->parm.mode == 3)
					// {	maxagpbuf[agpbufnum] = temp->parm.maxagp ;
					// 	agpbufnum ++;
					// 	if(agpbufnum == 100)
					// 	{
					// 		agpbufnum = 0;
					// 	}
					// 	double  maxfref = -2000.00;
					// 	for(int i = 0;i<100;i++)
					// 	{
					// 	if(maxfref<=maxagpbuf[i])
					// 	{
					// 		maxfref=maxagpbuf[i];
					// 	}
					// 	}
					// 	if(maxfref>0.0)
					// 	{
					// 		 *GPIO_APPITUDE = 0x00;
					// 	}
					// 	else if(maxfref>-10.0)
					// 	{
					// 		 *GPIO_APPITUDE = 0x01;
					// 	}
					// 	else if(maxfref<-60)
					// 	{
					// 		 *GPIO_APPITUDE = 0x03;
					// 	}
					// 	else if(maxfref<-50)
					// 	{
					// 		 *GPIO_APPITUDE = 0x01;
					// 	}
					// }
						sendto(lfd,temp->parm.buf1,temp->parm.numbuf1,0,(struct sockaddr*)&saddr1,sizeof(saddr1));	
						sendto(lfd,temp->parm.buf2,temp->parm.numbuf2,0,(struct sockaddr*)&saddr2,sizeof(saddr2));	
						sendto(lfd,temp->parm.buf3,temp->parm.numbuf3,0,(struct sockaddr*)&saddr3,sizeof(saddr3));	
						sendto(lfd,temp->parm.buf4,temp->parm.numbuf4,0,(struct sockaddr*)&saddr4,sizeof(saddr4));	
						if(temp->parm.mode0valid||temp->parm.mode1valid||temp->parm.mode2valid||temp->parm.mode3valid)
						sendto(lfd,temp->parm.buf5,temp->parm.numbuf5,0,(struct sockaddr*)&saddr0,sizeof(saddr0));
					}
			//printf("temp->parm.mode0valid:%d,temp->parm.mode1valid:%d,temp->parm.mode2valid:%d\r\n",temp->parm.mode0valid,temp->parm.mode1valid,temp->parm.mode2valid);
			
			
				}
				}else{
			printf("network   connect:faild \r\n");
				}
			}
	}
	// close(lfd);
}
void* signaldealthreadrun(void * arg)
{
	//int *args = (int*)arg;
  //  printf("managment thread num: %d,  ...\r\n",*args);
	
	
	
	printf("init thread pool rsource success\r\n");
	int re_list = -1;
	
	 //clock_t start, finish;
      // double duration;
       /* 测量一个事件持续的时间*/
   struct   thread_arg  threadarg;
	 
      
     puts("Making threadpool with 5 threads");
 threadpool thpool = thpool_init(5);


 int i = 0;
 


 	while (1)//等待数据更新
 	{
		
		usleep(1);
		//printf("current atten:%d,\r\n",*GPIO_APPITUDE);
		//printf("list length :%d \r\n",queue_length(queue));
		//if(queue_length(queue)>10){
	    //for(int i = 0;i<5;i++)    
   		{
			pthread_mutex_lock(&mutex);
 		re_list =   queue_pop(queue, threadarg.fftdata0,fftbuffer_size*8*4);
		pthread_mutex_unlock(&mutex); 
		if( re_list == 0) 
		thpool_add_work(thpool, task, (void*)(uintptr_t)&threadarg);
		}
	
		//}
		
	}
	thpool_wait(thpool);
 puts("Killing threadpool");
 thpool_destroy(thpool);	
}


