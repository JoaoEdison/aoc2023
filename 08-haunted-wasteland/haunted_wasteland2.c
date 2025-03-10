/* Código escrito por João Edison Roso Manica */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../base-structures/math.c"

struct node {
	char key[4], left[4], right[4];
	struct node *ptrs[2];
	struct node *next;
};

struct list_nodes {
	struct node *head, *tail;
} table[65] = { 0 };

#define HASH(s) ((s[0]+s[1]+s[2])%'A')

struct node *create_node(key, left, right)
char key[], left[], right[];
{
	struct node *ptrn;
	int hash;
	
	hash = HASH(key);
	if (!table[hash].head) {
		ptrn = table[hash].tail = table[hash].head = malloc(sizeof(struct node));
	} else {
		ptrn = table[hash].tail->next = malloc(sizeof(struct node));
		table[hash].tail = ptrn;
	}
	ptrn->next = NULL;
	strcpy(ptrn->key, key);
	strcpy(ptrn->left, left);
	strcpy(ptrn->right, right);
	ptrn->ptrs[0] = ptrn->ptrs[1] = NULL;
	return ptrn;
}

struct node *find_key(key)
char key[];
{
	struct node *ptr;
	int hash;

	hash = HASH(key);
	for (ptr=table[hash].head; ptr; ptr = ptr->next)
		if (!strcmp(key, ptr->key))
			return ptr;
	return NULL;
}

main()
{
	struct node *newn, *as[100];
	char current[4], left[4], right[4];
	char *instruction, *instructions;
	size_t len;
	int end_as, i, j;
	positive_int a, b, *steps;

	instructions = NULL;
	len = 0;
	getline(&instructions, &len, stdin);
	getchar();
	end_as = 0;
	while (scanf("%3s = (%3s,%3s)", current, left, right) != EOF) {
		newn = create_node(current, left, right);
		if (current[2] == 'A')
			as[end_as++] = newn;
	}
	steps = calloc(end_as, sizeof(positive_int));
	for (i=0; i < end_as; i++)
		for (instruction = instructions;;instruction++) {
			if (*instruction == '\n')
				instruction = instructions;
			if (!as[i]->ptrs[0])
				as[i]->ptrs[0] = find_key(as[i]->left);
			if (!as[i]->ptrs[1])
				as[i]->ptrs[1] = find_key(as[i]->right);
			as[i] = as[i]->ptrs[*instruction == 'R'];
			steps[i]++;
			if (as[i]->key[2] == 'Z')
				break;
		}
	for (i = 0; i < end_as-1; i++)
		for (j = 0; j < end_as-i-1; j++) {
			a = steps[j];
			b = steps[j+1];
			steps[j] = b / greatest_common_divisor(a,b) * a;
		}
	printf("%lu\n", *steps);
}
