#include "cqueue.h"
/*创建队列，外部释放内存*/
struct netwokrQueueInfo *networkcreateQueue(void)
{
	struct networkQueueInfo *queue = NULL;
	int counttt = 0;
	while((NULL == queue)&&(counttt<100))
{
		queue = (struct networkQueueInfo *)malloc(sizeof(struct networkQueueInfo));
		counttt++;
		//usleep(10);
}
	if (counttt == 100)
	{
		printf("malloc failed\n");
		return NULL;
	}else{
		printf("list malloc success.\r\n");
	}
	queue->next = NULL;
	return queue;
}
/*入队列，0表示成，非0表示出错*/
int networkqueue_push(struct networkQueueInfo *s,struct networkQueueInfo *temp)
{
	/*用来保存尾部指针*/
	struct	networkQueueInfo *temp1 = NULL;
	while ((temp1==NULL))
	{
		temp1 = (struct networkQueueInfo *)malloc(sizeof(struct networkQueueInfo));
	}
	/*找到链表的尾部*/
	while (s->next != NULL)
	{
		s = s->next;
	}
	memcpy(temp1,temp,sizeof(struct networkQueueInfo));
	temp1->next = s->next;
	s->next = temp1;
	//free(temp);
	return SUCCESS;
}
/*出队列*/
int networkqueue_pop(struct networkQueueInfo *s, struct networkQueueInfo *data)
{
	/*首先判断队列是否为空*/
	if (networkqueue_is_empty(s))
		return FAILURE;
	/*找出队列顶元素*/
	usleep(1);
	data->parm.mode = s->next->parm.mode;
	data->parm.numbuf1 = s->next->parm.numbuf1;
	data->parm.numbuf2 = s->next->parm.numbuf2;
	data->parm.numbuf3 = s->next->parm.numbuf3;
	data->parm.numbuf4 = s->next->parm.numbuf4;
	data->parm.numbuf5 = s->next->parm.numbuf5;
	data->parm.mode0valid = s->next->parm.mode0valid;
	data->parm.mode1valid = s->next->parm.mode1valid;
	data->parm.mode2valid = s->next->parm.mode2valid;
	data->parm.mode3valid = s->next->parm.mode3valid;
	data->parm.maxagp = s->next->parm.maxagp;
	//printf("wssdsgsggsget  numbuf1:%d,numbuf2:%d,numbuf3:%d,numbuf4:%d,numbuf5:%d,  \r\n",s->next->parm.numbuf1,s->next->parm.numbuf2,s->next->parm.numbuf3,s->next->parm.numbuf4,s->next->parm.numbuf5);
	memcpy(data->parm.buf1,s->next->parm.buf1,data->parm.numbuf1); 
	memcpy(data->parm.buf2,s->next->parm.buf2,data->parm.numbuf2); 
	memcpy(data->parm.buf3,s->next->parm.buf3,data->parm.numbuf3); 
	memcpy(data->parm.buf4,s->next->parm.buf4,data->parm.numbuf4); 
	memcpy(data->parm.buf5,s->next->parm.buf5,data->parm.numbuf5); 
	//memcpy(data->parm.channel1fftdata,s->next->parm.channel1fftdata,fftbuffer_size*sizeof(double)); 
	//memcpy(data->parm.channel2fftdata,s->next->parm.channel2fftdata,fftbuffer_size*sizeof(double));
	//memcpy(data->parm.channel3fftdata,s->next->parm.channel3fftdata,fftbuffer_size*sizeof(double));
	//memcpy(data->parm.channel4fftdata,s->next->parm.channel4fftdata,fftbuffer_size*sizeof(double));
	//printf("get data !!!!!!!!!!!!!!!!!!!!!! \r\n");
/**拷贝完成释放指针***/
	/*保存等下需要free的指针*/
	struct networkQueueInfo *temp = s->next;
	/*更换队列顶的位置*/
	s->next = s->next->next;

	/*释放队列顶节点内存*/
	if (temp != NULL)/*先判断指针是否为空*/
		{
            free(temp);
        }
	temp = NULL;
	return SUCCESS;
}


/*判断队列是否为空，空返回1，未空返回0*/
int networkqueue_is_empty(struct networkQueueInfo *s)
{
	/*队列顶指针为空，则队列为空*/
	if (s->next == NULL)
	{
		//printf("队列为空\n");
		return true;
	}

	return false;
}
int networkqueue_length(struct networkQueueInfo *s)
{
	int length = 0;
	/*找到链表的尾部*/
	while (s->next != NULL)
	{
		s = s->next;
		length ++;
	}
	return length;
}
