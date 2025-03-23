/* Código escrito por João Edison Roso Manica */
#ifndef QUEUE_H
#define QUEUE_H

#include <string.h>
#include <assert.h>

#define QUEUE_GENERATE(TYPE, SIZE) \
typedef struct {\
    TYPE *arr;\
    int start, end;\
} queue;\
\
void insert_queue(q, p)\
queue *q;\
TYPE *p;\
{\
    assert(q->end < SIZE);\
    memcpy(&q->arr[q->end], p, sizeof(TYPE));\
    q->end = (q->end+1) % SIZE;\
    assert(q->end != q->start);\
}\
\
TYPE *dequeue(q)\
queue *q;\
{\
    TYPE *p;\
    \
    p = &q->arr[q->start];\
    if (q->start == q->end)\
        return NULL;\
    q->start = (q->start+1) % SIZE;\
    return p;\
}
#endif
