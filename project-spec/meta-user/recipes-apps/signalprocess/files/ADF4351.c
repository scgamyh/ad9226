#include"ADF4351.h"
int gpiospifd = 0;
void initgpiospi()
{
printf("start gpiospifd mmap\r\n");
    if ((gpiospifd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC)) < 0) {
        printf("Unable to open /dev/mem: %s\n", strerror(errno));
        return NULL;
    }

 	GPIO_SPI = (uint32_t *) mmap(0, getpagesize() * 250, PROT_READ | PROT_WRITE, MAP_SHARED, gpiospifd, GPIOSPI_baseddr);
	
	

   if ((void *) GPIO_SPI == MAP_FAILED) {
        printf("mmap GPIO_SPI failed: %s\n", strerror(errno));
        exit(1);
    }
    printf("init gpiospifd mmap success!!!\r\n");
}

static void dds_delay(int length)
{
	length += 50;
	while (length>=0)
	length--;
}

void WriteToADF4351(unsigned char count, unsigned char *buf)
{
	unsigned	char	ValueToWrite = 0;
	unsigned	char	i = 0;
	unsigned	char	j = 0;
	
	usleep(100);
	CLR_SCLK();
     CLR_ADF4351();
     usleep(100);

	for(i=count;i>0;i--)
 	{
	 	ValueToWrite = *(buf + i - 1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				SET_DATA();	  //Send one to SDO pin
			}
			else
			{
				CLR_DATA();	  //Send zero to SDO pin
			}
            usleep(100);
			SET_SCLK();
            usleep(100);
			ValueToWrite <<= 1;	//Rotate data
			CLR_SCLK();
		}
	}
	CLR_DATA();
    usleep(100);
	SET_ADF4351();
    usleep(100);
    CLR_ADF4351();
	
}
void WriteToADF4356(unsigned char count, unsigned char *buf)
{
	unsigned	char	ValueToWrite = 0;
	unsigned	char	i = 0;
	unsigned	char	j = 0;
	
	usleep(100);
	CLR_SCLK();
     CLR_ADF4356();
     usleep(100);

	for(i=count;i>0;i--)
 	{
	 	ValueToWrite = *(buf + i - 1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				SET_DATA();	  //Send one to SDO pin
			}
			else
			{
				CLR_DATA();	  //Send zero to SDO pin
			}
            usleep(100);
			SET_SCLK();
            usleep(100);
			ValueToWrite <<= 1;	//Rotate data
			CLR_SCLK();
		}
	}
	CLR_DATA();
    usleep(100);
	SET_ADF4356();
    usleep(100);
    CLR_ADF4356();
	
}
void Set_ADF4351_Reg(unsigned int R0,unsigned int R1,unsigned int R2,unsigned int R3,unsigned int R4,unsigned int R5)
{
	 CLR_ADF4351();
	 sleep(1);
	 SET_ADF4351();
	  sleep(1);
	 CLR_ADF4351();
	sleep(1);
	unsigned char Reg_Buf[4] = {0};
	Reg_Buf[3] = R5>>24 & 0xff;
	Reg_Buf[2] = R5>>16 & 0xff;     
	Reg_Buf[1] = R5>>8  & 0xff;	
 	Reg_Buf[0] = R5     & 0xff;
	WriteToADF4351(4,Reg_Buf);		

	Reg_Buf[3] = R4>>24 & 0xff;
	Reg_Buf[2] = R4>>16 & 0xff;     
	Reg_Buf[1] = R4>>8  & 0xff;	
 	Reg_Buf[0] = R4     & 0xff;			
	WriteToADF4351(4,Reg_Buf);		

	Reg_Buf[3] = R3>>24 & 0xff;
	Reg_Buf[2] = R3>>16 & 0xff;     
	Reg_Buf[1] = R3>>8  & 0xff;	
 	Reg_Buf[0] = R3     & 0xff;        
	WriteToADF4351(4,Reg_Buf);	

	Reg_Buf[3] = R2>>24 & 0xff;
	Reg_Buf[2] = R2>>16 & 0xff;     
	Reg_Buf[1] = R2>>8  & 0xff;	
 	Reg_Buf[0] = R2     & 0xff;			
	WriteToADF4351(4,Reg_Buf);		

	Reg_Buf[3] = R1>>24 & 0xff;
	Reg_Buf[2] = R1>>16 & 0xff;     
	Reg_Buf[1] = R1>>8  & 0xff;	
 	Reg_Buf[0] = R1     & 0xff;		
	WriteToADF4351(4,Reg_Buf);	
							   

	Reg_Buf[3] = R0>>24 & 0xff;
	Reg_Buf[2] = R0>>16 & 0xff;     
	Reg_Buf[1] = R0>>8  & 0xff;	
 	Reg_Buf[0] = R0     & 0xff;
	WriteToADF4351(4,Reg_Buf);
}


void Set_ADF4356_Reg(unsigned int R0,unsigned int R1,unsigned int R2,unsigned int R3,unsigned int R4,unsigned int R5,unsigned int R6,
		unsigned int R7,unsigned int R8,unsigned int R9,unsigned int R10,unsigned int R11,unsigned int R12,unsigned int R13)
{
	 CLR_ADF4356();
	 sleep(1);
	 SET_ADF4356();
	  sleep(1);
	 CLR_ADF4356();
	sleep(1);
	unsigned char Reg_Buf[4] = {0};
	Reg_Buf[3] = R13>>24 & 0xff;
	Reg_Buf[2] = R13>>16 & 0xff;     
	Reg_Buf[1] = R13>>8  & 0xff;	
 	Reg_Buf[0] = R13     & 0xff;
	WriteToADF4356(4,Reg_Buf);
	
	Reg_Buf[3] = R12>>24 & 0xff;
	Reg_Buf[2] = R12>>16 & 0xff;     
	Reg_Buf[1] = R12>>8  & 0xff;	
 	Reg_Buf[0] = R12     & 0xff;
	WriteToADF4356(4,Reg_Buf);
	
	
	Reg_Buf[3] = R11>>24 & 0xff;
	Reg_Buf[2] = R11>>16 & 0xff;     
	Reg_Buf[1] = R11>>8  & 0xff;	
 	Reg_Buf[0] = R11     & 0xff;
	WriteToADF4356(4,Reg_Buf);
	
	
	Reg_Buf[3] = R10>>24 & 0xff;
	Reg_Buf[2] = R10>>16 & 0xff;     
	Reg_Buf[1] = R10>>8  & 0xff;	
 	Reg_Buf[0] = R10     & 0xff;
	WriteToADF4356(4,Reg_Buf);
	
	//delay_ms(10);
	
	Reg_Buf[3] = R9>>24 & 0xff;
	Reg_Buf[2] = R9>>16 & 0xff;     
	Reg_Buf[1] = R9>>8  & 0xff;	
 	Reg_Buf[0] = R9     & 0xff;
	WriteToADF4356(4,Reg_Buf);
	
	//delay_ms(10);
	
	Reg_Buf[3] = R8>>24 & 0xff;
	Reg_Buf[2] = R8>>16 & 0xff;     
	Reg_Buf[1] = R8>>8  & 0xff;	
 	Reg_Buf[0] = R8     & 0xff;
	WriteToADF4356(4,Reg_Buf);
	
	//delay_ms(10);
	
	Reg_Buf[3] = R7>>24 & 0xff;
	Reg_Buf[2] = R7>>16 & 0xff;     
	Reg_Buf[1] = R7>>8  & 0xff;	
 	Reg_Buf[0] = R7     & 0xff;
	WriteToADF4356(4,Reg_Buf);
	
	//delay_ms(10);
	
	Reg_Buf[3] = R6>>24 & 0xff;
	Reg_Buf[2] = R6>>16 & 0xff;     
	Reg_Buf[1] = R6>>8  & 0xff;	
 	Reg_Buf[0] = R6     & 0xff;
	WriteToADF4356(4,Reg_Buf);
	
	//delay_ms(10);
	
	Reg_Buf[3] = R5>>24 & 0xff;
	Reg_Buf[2] = R5>>16 & 0xff;     
	Reg_Buf[1] = R5>>8  & 0xff;	
 	Reg_Buf[0] = R5     & 0xff;
	WriteToADF4356(4,Reg_Buf);

	//delay_ms(10);

	Reg_Buf[3] = R4>>24 & 0xff;
	Reg_Buf[2] = R4>>16 & 0xff;     
	Reg_Buf[1] = R4>>8  & 0xff;	
 	Reg_Buf[0] = R4     & 0xff;			
	WriteToADF4356(4,Reg_Buf);

	//delay_ms(10);

	Reg_Buf[3] = R3>>24 & 0xff;
	Reg_Buf[2] = R3>>16 & 0xff;     
	Reg_Buf[1] = R3>>8  & 0xff;	
 	Reg_Buf[0] = R3     & 0xff;        
	WriteToADF4356(4,Reg_Buf);

	//delay_ms(10);

	Reg_Buf[3] = R2>>24 & 0xff;
	Reg_Buf[2] = R2>>16 & 0xff;     
	Reg_Buf[1] = R2>>8  & 0xff;	
 	Reg_Buf[0] = R2     & 0xff;			
	WriteToADF4356(4,Reg_Buf);

	//delay_ms(10);

	Reg_Buf[3] = R1>>24 & 0xff;
	Reg_Buf[2] = R1>>16 & 0xff;     
	Reg_Buf[1] = R1>>8  & 0xff;	
 	Reg_Buf[0] = R1     & 0xff;		
	WriteToADF4356(4,Reg_Buf);

	//delay_us(2);
							   

	Reg_Buf[3] = R0>>24 & 0xff;
	Reg_Buf[2] = R0>>16 & 0xff;     
	Reg_Buf[1] = R0>>8  & 0xff;	
 	Reg_Buf[0] = R0     & 0xff;
	
	WriteToADF4356(4,Reg_Buf);
}
