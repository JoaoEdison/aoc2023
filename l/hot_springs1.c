/*escrever o testador!!!*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SET_SIZE 500
#define STACK_SIZE 1000

char line[32+1];
int qtdg, groups[20];

struct iteration {
	unsigned spring, group, encode, gsize;
} *stack;
unsigned *set;

arrangements()
{
	static struct iteration curr;
	int i, count, stack_idx;
	
	count = stack_idx = 0;
	stack[stack_idx].spring = 0;
	stack[stack_idx].group = 0;
	stack[stack_idx].encode = 0;
	stack[stack_idx++].gsize = 0;
	while (stack_idx) {
		memcpy(&curr, &stack[--stack_idx], sizeof(struct iteration));
		while (line[curr.spring] && curr.group < qtdg) {
			if (line[curr.spring] == '#') {
				curr.encode |= 1<<curr.spring;
				curr.gsize++;
				if (curr.gsize > groups[curr.group])
					goto end;
			} else if (line[curr.spring] == '.') {
				if (curr.gsize) {
					if (curr.gsize == groups[curr.group])
						curr.group++;
					else
						goto end;
				}
				curr.gsize = 0;
			} else if (line[curr.spring] == '?') {
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
					if (line[curr.spring+1] == '?')
						curr.spring++;
					else if (line[curr.spring+1] == '#')
						goto end;
				} else if (curr.gsize > groups[curr.group])
					goto end;
			}
			curr.spring++;
		}
		if (curr.gsize && curr.gsize == groups[curr.group] && (line[curr.spring-1] == '#' || line[curr.spring-1] == '?')) {
			curr.group++;
			curr.encode |= 1<<(curr.spring-1);
		}
		while (line[curr.spring])
			if (line[curr.spring++] == '#')
				goto end;
		if (curr.group == qtdg && !line[curr.spring]) {
			for (i=0; i < count; i++)
				if (set[i] == curr.encode)
					goto end;
			set[count++] = curr.encode;
			if (count == SET_SIZE) {
				puts("Maximum capacity reached in set.");
				exit(1);
			}
		}
end:;
	}
	return count;
}

main()
{
	char c;
	int i, sum;
	
	stack = malloc(sizeof(struct iteration) * STACK_SIZE);
	set = malloc(sizeof(unsigned) * SET_SIZE);
	sum=i=0;	
	for (;;) {
		c = getchar();
		if (c == ' ') {
			line[i] = '\0';
			qtdg=i=0;
			for (;;) {
				scanf("%d", &groups[qtdg++]);
				c = getchar();
				if (c == '\n' || c == EOF)
					break;
			}
			sum += arrangements();
			if (c == EOF)
				break;
			else
				continue;
		} else if (c == EOF)
			break;
		line[i++] = c;
	}
	printf("%d\n", sum);
}
