#include "cqueue.h"
/*创建队列，外部释放内存*/
struct QueueInfo *createQueue(void)
{
	struct QueueInfo *queue = NULL;
	int counttt = 0;
	while((NULL == queue)&&(counttt<100))
{
		queue = (struct QueueInfo *)malloc(sizeof(struct QueueInfo));
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
int queue_push(struct QueueInfo *s, void* adcdata,int size)
{
	/*用来保存尾部指针*/
	struct	QueueInfo *temp = NULL;
	while ((temp==NULL))
	{
		temp = (struct QueueInfo *)malloc(sizeof(struct QueueInfo));
	}
	/*找到链表的尾部*/
	while (s->next != NULL)
	{
		s = s->next;
	}
	
    	memcpy(temp->parm.ad9226fftdata,adcdata,size);
	temp->next = s->next;
	s->next = temp;
	return SUCCESS;
}
/*出队列*/
int queue_pop(struct QueueInfo *s,   int *adcdata0,int size)
{
	/*首先判断队列是否为空*/
	if (queue_is_empty(s))
		return FAILURE;
	/*找出队列顶元素*/
	memcpy(adcdata0,s->next->parm.ad9226fftdata,size); 
/**拷贝完成释放指针***/
	/*保存等下需要free的指针*/
	struct QueueInfo *temp = s->next;
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

/*访问队列顶元素*/
int queue_top(struct QueueInfo *s, struct threadarg *parm)
{
	/*首先判断队列是否为空*/
	if (queue_is_empty(s))
	{
		return FAILURE;
	}
	return SUCCESS;
}
/*判断队列是否为空，空返回1，未空返回0*/
int queue_is_empty(struct QueueInfo *s)
{
	/*队列顶指针为空，则队列为空*/
	if (s->next == NULL)
	{
		//printf("队列为空\n");
		return true;
	}

	return false;
}
int queue_length(struct QueueInfo *s)
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
