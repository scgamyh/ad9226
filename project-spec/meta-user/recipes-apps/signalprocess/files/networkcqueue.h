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
struct networkQueueInfo *networkcreateQueue(void);
int networkqueue_push(struct networkQueueInfo *s, struct networkQueueInfo *temp);
int networkqueue_pop(struct networkQueueInfo *s, struct networkQueueInfo *temp);
int networkqueue_is_empty(struct networkQueueInfo *s);
int networkqueue_length(struct networkQueueInfo *s);
#endif
