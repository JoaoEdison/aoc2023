#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
	char key[4], left[4], right[4];
	struct node *ptrs[2];
	struct node *next;
};

struct {
	struct node *head, *tail;
} table[65] = { 0 };

#define HASH(s) ((s[0]+s[1]+s[2])%'A')

void create_node(key, left, right)
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
	struct node *next;
	char current[4], left[4], right[4];
	char *instruction, *instructions;
	size_t len;
	int steps;
	
	instructions = NULL;
	len = 0;
	getline(&instructions, &len, stdin);
	getchar();
	while (scanf("%3s = (%3s,%3s)", current, left, right) != EOF)
		create_node(current, left, right);
	steps = 0;
	next = find_key("AAA");
	for (instruction = instructions;;instruction++) {
		if (*instruction == '\n')
			instruction = instructions;
		next->ptrs[0] = find_key(next->left);
		next->ptrs[1] = find_key(next->right);
		next = next->ptrs[*instruction == 'R'];
		steps++;
		if (!strcmp(next->key, "ZZZ"))
			break;
	}
	printf("%d\n", steps);
}
