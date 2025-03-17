/* Código escrito por João Edison Roso Manica.
 * Baseado na solução de jonathanpaulson (https://github.com/jonathanpaulson/AdventOfCode/blob/master/2023/17.py)
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../base_structures/minheap.c"

#define MTX_SIZE 256
#define TABLE_SIZE 1187
#define QUEUE_SIZE (1<<14)

struct iteration {
	unsigned char ac, d;
	int x,y,dist;
};
struct node {
	struct iteration data;
	struct node *next;
};

unsigned char matx[MTX_SIZE*MTX_SIZE];
len, wid;
struct list {
	struct node *head, *tail;
} table[TABLE_SIZE] = { 0 };

struct iteration *insert_set(x,y,d,ac)
unsigned char ac,d;
{
	struct node *newit, *next;
	int idx;

	idx = (x*3+y*5+ac*7+d*11)%TABLE_SIZE;
	for (next=table[idx].head; next; next = next->next)
		if (next->data.x == x && next->data.y == y && next->data.d == d && next->data.ac == ac)
			return NULL;
	if (!(newit = malloc(sizeof(struct node)))) {
		fputs("MEMORY LIMIT.\n", stderr);
		exit(1);
	}
	newit->data.x = x; newit->data.y = y; newit->data.d = d; newit->data.ac = ac; newit->next = NULL;
	if (table[idx].tail) {
		table[idx].tail->next = newit; 
		table[idx].tail = newit;
	} else
		table[idx].head = table[idx].tail = newit;
	return &newit->data;
}

minheap queue;

void add_edge(x,y,d,ac,dist)
unsigned char ac,d;
{
	struct iteration *next;
	
	if ((next = insert_set(x,y,d,ac))) {
		next->dist = dist + matx[x*MTX_SIZE+y];
		insert_minheap(&queue, next);
	}
}

void shortest()
{
	struct iteration curr, *ptri;
	char jumpo, jumpac;
	
	ptri = insert_set(0,0,4,0);
	ptri->dist = 0;
	insert_minheap(&queue, ptri);
	jumpo = 5;
	while ((ptri = pop_minheap(&queue))) {
		curr = *ptri;
		jumpac = 0;
		if (curr.d == 0){
			jumpo = 2;
			if (curr.ac == 3)
				jumpac = 1;
		}else if (curr.d == 1){
			jumpo = 1;
			if (curr.ac == 3)
				jumpac = 2;
		}else if (curr.d == 2){
			jumpo = 4;
			if (curr.ac == 3)
				jumpac = 3;
		}else if (curr.d == 3){
			jumpo = 3;
			if (curr.ac == 3)
				jumpac = 4;
		}
		if (curr.x+1 < len && jumpo != 1 && jumpac != 1)
			add_edge(curr.x+1, curr.y, 0, curr.d==0? curr.ac+1 : 1, curr.dist);
		if (curr.x-1 >= 0 && jumpo!= 2 && jumpac != 2)
			add_edge(curr.x-1, curr.y, 1, curr.d==1? curr.ac+1 : 1, curr.dist);
		if (curr.y-1 >= 0 && jumpo != 3 && jumpac != 3)
			add_edge(curr.x, curr.y-1, 2, curr.d==2? curr.ac+1 : 1, curr.dist);
		if (curr.y+1 < wid && jumpo != 4 && jumpac != 4)
			add_edge(curr.x, curr.y+1, 3, curr.d==3? curr.ac+1 : 1, curr.dist);
	}
}

compar(a,b)
const void *a, *b;
{
	return ((struct iteration*)a)->dist - ((struct iteration*)b)->dist;
}

main()
{
	struct node *next;
	int i,min;
	char c;

	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			len++;
			i = wid;
			wid = 0;
			continue;
		}
		matx[len*MTX_SIZE+wid++] = c - '0';
	}
	wid = i;
	create_minheap(queue, QUEUE_SIZE, compar);
	shortest();
	free_minheap(queue);
	min = INT_MAX;
	for (i=0;i<TABLE_SIZE;i++)
		for (next=table[i].head; next; next = next->next)
			if (next->data.x == len-1 && next->data.y == wid-1 && min > next->data.dist)
				min = next->data.dist;
	printf("%d\n", min);
}
