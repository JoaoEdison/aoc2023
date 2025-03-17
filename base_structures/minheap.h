#include <stdlib.h>

typedef struct {
	void **arr;
	int end, size;
    int (*compar)(void *, void *);
} minheap;

#define create_minheap(H, M, RULE) \
	do { H.end = 0; H.size = M; H.arr = malloc(sizeof(void *) * M); H.compar = RULE; } while (0)

void insert_minheap(minheap *h, void *key);
void print_minheap(minheap *h);
void *pop_minheap(minheap *h);

#define top_minheap(h) h->arr[0]
#define is_empty_minheap(h) (h->end == 0)
#define clear_minheap(h) h->end = 0

#define free_minheap(H) free(H.arr)
#define free_minheap_ptr(H) free(H->arr)
