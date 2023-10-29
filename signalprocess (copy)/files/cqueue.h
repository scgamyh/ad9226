#ifndef CQUEUE_H_
#define CQUEUE_H_
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include <string.h>
#include <math.h>
#include "global.h"
/*函数声明*/
struct QueueInfo *createQueue(void);
int queue_push(struct QueueInfo *s, struct threadarg *parm);
int queue_pop(struct QueueInfo *s, struct threadarg *parm);
int queue_top(struct QueueInfo *s, struct threadarg *parm);
int queue_is_empty(struct QueueInfo *s);
int queue_length(struct QueueInfo *s);
#endif
