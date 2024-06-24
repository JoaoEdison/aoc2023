#include <stdio.h>
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
	int parent = (i-1)/2;

	if (greater(h->arr[parent], h->arr[i]) > 0) {
		temp = h->arr[parent];
		h->arr[parent] = h->arr[i];
		h->arr[i] = temp;
		heapify_bottom(h, parent);
	}
}

void insert_minheap(h, key)
HeapMin *h;
void *key;
{
	if (h->end < h->size) {
		h->arr[h->end] = key;
		heapify_bottom(h, h->end);
		h->end++;
	} else {
		fputs("Heap Full.\n", stderr);
		exit(1);
	}
}

static void heapify_top(h, parent)
HeapMin *h;
{
	int left = parent*2+1;
	int right = parent*2+2;
	int min;
	void *temp;

	if (left >= h->end || left < 0)
		left = -1;
	if (right >= h->end || right < 0)
		right = -1;

	min = (left != -1 && greater(h->arr[left], h->arr[parent]) < 0)? left : parent;
	
	if (right != -1 && greater(h->arr[right], h->arr[min]) < 0)
		min = right;

	if (min != parent) {
		temp = h->arr[min];
		h->arr[min] = h->arr[parent];
		h->arr[parent] = temp;
		heapify_top(h, min);
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
