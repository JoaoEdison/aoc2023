/* Código escrito por jonathanpaulson (https://github.com/jonathanpaulson/AdventOfCode/blob/master/2023/12.py)
 * Adaptado por João Edison Roso Manica*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DP_SIZE 200000

typedef unsigned long positive;

char line[200];
qtdg, groups[50];

struct block {
	char *current;
	int group, damaged_size;
	positive permu;
} *blocks;
bsize;

in_block(current, group, damaged_size)
char *current;
{
	int i; 
	
	for (i=0; i < bsize; i++)
		if (blocks[i].current == current &&
		    blocks[i].group == group && 
		    blocks[i].damaged_size == damaged_size)
			return i;
	return i;
}

positive arrangements(current, group, damaged_size)
char *current;
{
	positive ans;
	int i;

	if ((i = in_block(current, group, damaged_size)) < bsize)
		return blocks[i].permu;
	if (!*current) {
		if (group == qtdg && !damaged_size)
			return 1;
		else if (group == qtdg-1 && groups[group] == damaged_size)
			return 1;
		return 0;
	}
	ans = 0;
	if (*current == '.' || *current == '?') {
		if (!damaged_size)
			ans += arrangements(current+1, group, 0);
		else if (group < qtdg && groups[group] == damaged_size)
			ans += arrangements(current+1, group+1, 0);
	}
	if (*current == '#' || *current == '?')
		ans += arrangements(current+1, group, damaged_size+1);
	blocks[bsize].current = current;
	blocks[bsize].group = group;
	blocks[bsize].damaged_size = damaged_size;
	blocks[bsize++].permu = ans;
	if (bsize == DP_SIZE) {
		fprintf(stderr, "Maximum capacity at blocks.\n");
		exit(1);
	}
	return ans;
}

main()
{
	char c, aux[30];
	int i, j, modulo;
	positive answer;
	
	blocks = malloc(sizeof(struct block) * DP_SIZE);
	answer=i=0;
	for (;;) {
		c = getchar();
		if (c == ' ') {
			aux[i] = '\0';
			modulo = 0;
			for (;;) {
				scanf("%d", &groups[modulo++]);
				c = getchar();
				if (c == '\n' || c == EOF)
					break;
			}
			strcpy(line, aux);
			qtdg = modulo;
			for (i=0; i < 4; i++) {
				strcat(line, "?");
				strcat(line, aux);
				for (j=0; j < modulo; j++) {
					groups[qtdg] = groups[qtdg%modulo];
					qtdg++;
				}
			}
			bsize = 0;
			answer += arrangements(line, 0, 0);
			i = 0;
			if (c == EOF)
				break;
			else
				continue;
		} else if (c == EOF)
			break;
		aux[i++] = c;
	}
	printf("%lu\n", answer);
}
