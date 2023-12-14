/* Código escrito por João Edison Roso Manica */
/* How '?###???????? 3,2,1' => 506250? */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define LINE_LEN 32+1
#define SET_SIZE 500
#define STACK_SIZE 1000

typedef long unsigned positive;

char new_line[LINE_LEN];
char line[LINE_LEN];
int qtdg, groups[20];

struct iteration {
	positive spring, group, encode, gsize;
	unsigned char line;
} *stack, *big_stack;
positive *set, *big_set;

int big_stack_idx, big_set_size;

void print_b(n)
positive n;
{
	while (n) {
		putchar('0' + (n & 1));
		n >>= 1;
	}
	putchar('\n');
}

positive multiplied()
{
	int first_comb, i;

	big_set_size = 0;
	strcpy(new_line, "?");
	strcat(new_line, line);
	puts(new_line);
	big_stack_idx = 0;
	big_stack[0].spring = 0;
	big_stack[0].group = 0;
	big_stack[0].encode = 0;
	big_stack[0].gsize = 0;
	big_stack[0].line = 1;
	first_comb = arrangements(big_stack[0]);
	while (big_stack_idx--)
		arrangements(big_stack[big_stack_idx]);
	for (i=0; i< big_set_size; i++)
		print_b(big_set[i]);
	return first_comb * pow(big_set_size, 4);
}

arrangements(state)
struct iteration state;
{
	static struct iteration curr;
	char *ptrl;
	int i, j, count, stack_idx;
	
	count = stack_idx = 0;
	stack[stack_idx].group = 0;
	stack[stack_idx].spring = state.spring;
	stack[stack_idx].encode = state.encode;
	stack[stack_idx++].gsize = state.gsize;
	ptrl = state.line? line : new_line; 
	while (stack_idx) {
		memcpy(&curr, &stack[--stack_idx], sizeof(struct iteration));
		while (ptrl[curr.spring] && curr.group < qtdg) {
			if (ptrl[curr.spring] == '#') {
				curr.encode |= 1<<curr.spring;
				curr.gsize++;
				if (curr.gsize > groups[curr.group])
					goto end;
			} else if (ptrl[curr.spring] == '.') {
				if (curr.gsize) {
					if (curr.gsize == groups[curr.group])
						curr.group++;
					else
						goto end;
				}
				curr.gsize = 0;
			} else if (ptrl[curr.spring] == '?') {
				if (curr.gsize && curr.gsize < groups[curr.group]) {
					curr.encode |= 1<<curr.spring;
					curr.gsize++;
					curr.spring++;
					continue;
				}
				if (curr.gsize && curr.gsize == groups[curr.group]) {
					stack[stack_idx].spring = curr.spring+1;
					stack[stack_idx].group = curr.group+1;
					stack[stack_idx].encode = curr.encode;
					stack[stack_idx].gsize = 0;
					stack_idx++;
					if (stack_idx == STACK_SIZE) {
						puts("Maximum capacity reached in stack.");
						exit(1);
					}
				} else if (!curr.gsize) {
					stack[stack_idx].spring = curr.spring+1;
					stack[stack_idx].group = curr.group;
					stack[stack_idx].encode = curr.encode;
					stack[stack_idx].gsize = 0;
					stack_idx++;
					if (stack_idx == STACK_SIZE) {
						puts("Maximum capacity reached in stack.");
						exit(1);
					}
				}
				curr.encode |= 1<<curr.spring;
				curr.gsize++;
				if (curr.gsize == groups[curr.group]) {
					curr.group++;
					curr.gsize = 0;
					if (ptrl[curr.spring+1] == '?')
						curr.spring++;
					else if (ptrl[curr.spring+1] == '#')
						goto end;
				} else if (curr.gsize > groups[curr.group])
					goto end;
			}
			curr.spring++;
		}
		if (curr.gsize && curr.gsize == groups[curr.group] && (ptrl[curr.spring-1] == '#' || ptrl[curr.spring-1] == '?')) {
			curr.group++;
			curr.encode |= 1<<(curr.spring-1);
		}
		while (ptrl[curr.spring])
			if (ptrl[curr.spring++] == '#')
				goto end;
		if (curr.group == qtdg && !ptrl[curr.spring]) {
			for (i=0; i < count; i++)
				if (set[i] == curr.encode)
					goto end;
			set[count++] = curr.encode;
			if (count == SET_SIZE) {
				puts("Maximum capacity reached in set.");
				exit(1);
			}
			if (state.line) {
				big_stack[big_stack_idx] = curr;
				big_stack[big_stack_idx].line = 0;
				big_stack[big_stack_idx].spring = 0;
				big_stack[big_stack_idx].encode = 0;
				if (curr.gsize && curr.gsize == groups[qtdg-1]) {
					big_stack[big_stack_idx].gsize = 0;
					big_stack[big_stack_idx].spring = 1;
				}
				big_stack_idx++;
				if (big_stack_idx == STACK_SIZE) {
					puts("Maximum capacity reached in stack.");
					exit(1);
				}
			} else
				for (i=0; i < count; i++) {
					for (j=0; j < big_set_size; j++)
						if (set[i] == big_set[j])
							break;
					if (j == big_set_size)
						big_set[big_set_size++] = set[i];
					if (big_set_size == SET_SIZE) {
						puts("Maximum capacity reached in set.");
						exit(1);
					}
				}
		}
end:;
	}
	return count;
}

main()
{
	char c;
	int i;
	positive sum, x;
	
	stack = malloc(sizeof(struct iteration) * STACK_SIZE);
	big_stack = malloc(sizeof(struct iteration) * STACK_SIZE);
	set = malloc(sizeof(positive) * SET_SIZE);
	big_set = malloc(sizeof(positive) * SET_SIZE);
	sum=i=0;	
	for (;;) {
		c = getchar();
		if (c == ' ') {
			line[i] = '\0';
			qtdg=0;
			for (;;) {
				scanf("%d", &groups[qtdg++]);
				c = getchar();
				if (c == '\n' || c == EOF)
					break;
			}
			x = multiplied();
			sum += x; 
			printf("%lu\n", x);
			i=0;
			if (c == EOF)
				break;
			else
				continue;
		} else if (c == EOF)
			break;
		line[i++] = c;
	}
	printf("%lu\n", sum);
}
