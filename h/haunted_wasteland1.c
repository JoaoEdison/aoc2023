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

int max = 1;

void create_node(key, left, right)
char key[], left[], right[];
{
	struct node *ptrn;
	int hash;
	
	hash = HASH(key);
	if (max < hash)
		max = hash;
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

void bind_table(curr)
struct node *curr;
{
	curr->ptrs[0] = find_key(curr->left);
	curr->ptrs[1] = find_key(curr->right);
}

main()
{
	char *instructions;
	char current[4], left[4], right[4];
	size_t len;
	struct node *next;
	char *instruction;
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
		bind_table(next);
		next = next->ptrs[*instruction == 'R'];
		steps++;
		if (!strcmp(next->key, "ZZZ"))
			break;
	}
	printf("%d\n", steps);
}
