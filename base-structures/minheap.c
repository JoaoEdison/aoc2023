#include <assert.h>
#include "minheap.h"

static int (*greater)();

HeapMin *create_minheap(m, rule)
int (*rule)();
{
    HeapMin *h = malloc(sizeof(HeapMin));
    h->end = 0;
    h->size = m;
    h->arr = malloc(sizeof(void *) * m);
    greater = rule;
    return h;
}

static void heapify_bottom(h, i)
HeapMin *h; 
{
    void *temp;
    int parent;
    
    for (parent = (i-1)/2; greater(h->arr[parent], h->arr[i]) > 0;
          i = parent,
          parent = (i-1)/2) {
        temp = h->arr[parent];
        h->arr[parent] = h->arr[i];
        h->arr[i] = temp;
    }
}

void insert_minheap(h, key)
HeapMin *h;
void *key;
{
    assert(h->size > h->end);
    h->arr[h->end] = key;
    heapify_bottom(h, h->end);
    h->end++;
}

static void heapify_top(h, parent)
HeapMin *h;
{
    int left = parent*2+1;
    int right = parent*2+2;
    int min;
    void *temp;

    min = (left < h->end && left > 0 &&
            greater(h->arr[left], h->arr[parent]) < 0)? left : parent;
    
    if (right < h->end && right > 0 &&
         greater(h->arr[right], h->arr[min]) < 0)
        min = right;

    while (min != parent) {
        temp = h->arr[min];
        h->arr[min] = h->arr[parent];
        h->arr[parent] = temp;

        parent = min;

        left = parent*2+1;
        right = parent*2+2;

        min = (left < h->end && left > 0 &&
                greater(h->arr[left], h->arr[parent]) < 0)? left : parent;
        
        if (right < h->end && right > 0 &&
             greater(h->arr[right], h->arr[min]) < 0)
            min = right;
    }
}

void *pop_minheap(HeapMin *h)
{
    void *pop_minheap;
    
    if (!h->end)
        return NULL;
    pop_minheap = h->arr[0];
    h->arr[0] = h->arr[h->end-1];
    h->end--;
    heapify_top(h, 0);
    return pop_minheap;
}
