#ifndef ADF4351_H_
#define ADF4351_H_
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
#include <sys/mman.h>
#include <math.h>
#include <time.h>
#include<pthread.h>
#include<stdlib.h>
#include <unistd.h>
#define GPIOSPI_baseddr          0x41230000

static volatile uint32_t *GPIO_SPI ;
#define SET_ADF4351()		*GPIO_SPI = 0x04
#define CLR_ADF4351()		*GPIO_SPI = 0x00

//K2
#define	SET_SCLK()		*GPIO_SPI = 0x01|(*GPIO_SPI)
#define	CLR_SCLK()		*GPIO_SPI = 0x0E&(*GPIO_SPI)

//PC4
#define SET_ADF4356()		*GPIO_SPI = 0x08
#define CLR_ADF4356()		*GPIO_SPI = 0x00

//K3
#define SET_DATA()		*GPIO_SPI = 0x02|(*GPIO_SPI)
#define CLR_DATA()		*GPIO_SPI = 0x0D&(*GPIO_SPI)
void initgpiospi();
static void dds_delay(int length);
void WriteToADF(unsigned char count, unsigned char *buf);
void Set_ADF4356_Reg(unsigned int R0,unsigned int R1,unsigned int R2,unsigned int R3,unsigned int R4,unsigned int R5,unsigned int R6,unsigned int R7,
unsigned int R8,unsigned int R9,unsigned int R10,unsigned int R11,unsigned int R12,unsigned int R13);
void Set_ADF4351_Reg(unsigned int R0,unsigned int R1,unsigned int R2,unsigned int R3,unsigned int R4,unsigned int R5);
#endif