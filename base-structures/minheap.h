#include <stdlib.h>

typedef struct {
	void **arr;
	int end, size;
} HeapMin;

HeapMin *create_minheap(int size, int (*cmp)());
void insert_minheap(HeapMin *h, void *key);
void print_minheap(HeapMin *h);
void *pop_minheap(HeapMin *h);

#define top_minheap(h) h->arr[0]
#define is_empty_minheap(h) (h->end == 0)
#define clear_minheap(h) h->end = 0
#define free_minheap(h) free(h->arr)
