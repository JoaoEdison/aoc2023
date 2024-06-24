/* Código escrito por João Edison Roso Manica */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEN 256
#define LABEL_SIZE 10

struct node {
	char label[LABEL_SIZE];
	int focal;
	struct node *next;
};
struct list {
	struct node *head, *tail;
} boxes[LEN] = { 0 };

hash(step)
char *step;
{
	int value;
	
	for (value = 0; *step; step++)
		value = ((*step)+value)*17%LEN;
	return value;
}

void add(label, focal)
char label[];
{
	struct list *l;
	struct node *newnod;
	
	l = &boxes[hash(label)];
	newnod = malloc(sizeof(struct node));
	strcpy(newnod->label, label);
	newnod->focal = focal;
	newnod->next = NULL;
	if (l->tail) {
		l->tail->next = newnod;
		l->tail = newnod;
	} else
		l->tail = l->head = newnod;	
}

void removenod(label)
char label[];
{
	struct node *next, *prev;
	struct list *l;
	
	prev = NULL;
	l = &boxes[hash(label)];
	next = l->head;
	for (; next; prev = next, next = next->next)
		if (!strcmp(next->label, label)) {
			if (prev)
				prev->next = next->next;
			if (next == l->tail)
				l->tail = prev;
			if (next == l->head)
				l->head = next->next;
			free(next);
			return;
		}
}

struct node *find(label)
char label[];
{
	struct node *next;

	next = boxes[hash(label)].head;
	for (; next; next = next->next)
		if (!strcmp(next->label, label))
			return next;
	return NULL;
}

summation()
{
	struct node *curr, *prev;
	int i, j, ans;
	
	ans = 0;	
	for (i=0; i < LEN; i++) {
		curr = boxes[i].head;
		for (j=0; curr; j++) {
			ans += (1+i)*(j+1)*curr->focal;
			prev = curr;
			curr = curr->next;
			free(prev);
		}
	}
	return ans;
}

main()
{
	char buf[LABEL_SIZE];
	struct node *lens;
	int focal, i;
	char c;
	
	i = 0;	
	while ((c = getchar()) != EOF) {
		if (c == ',' || c == '\n')
			i = 0;
		else if (c == '=') {
			focal = getchar()-'0';
			buf[i] = '\0';
			if ((lens = find(buf)))
				lens->focal = focal;
			else
				add(buf, focal);
		} else if (c == '-') {
			buf[i] = '\0';
			removenod(buf);
		} else
			buf[i++] = c;
	}
	printf("%d\n", summation());
}
