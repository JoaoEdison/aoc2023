/* Código escrito por João Edison Roso Manica */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAT_SIZE 200

char *mat;

void next_pos(i, j)
int *i, *j;
{
	static unsigned char l = 0;
	char next;
	
	if (!l) {
		next = mat[(*i+1)*MAT_SIZE + *j];
		if (next == 'J') {
			++*i;
			l = 3;
			return;
		}
		if (next == 'L') {
			++*i;
			l = 1;
			return;
		}
		if (next == '|') {
			++*i;
			l = 2;
			return;
		}
		next = mat[(*i-1)*MAT_SIZE + *j];
		if (next == '|') {
			--*i;
			l = 4;
			return;
		}
		if (next == 'F') {
			--*i;
			l = 1;	
			return;
		}
		if (next == '7') {
			--*i;
			l = 3;
			return;
		}
		next = mat[*i*MAT_SIZE + *j + 1];
		if (next == '-') {
			++*j;
			l = 1;
			return;
		}
		if (next == '7') {
			++*j;
			l = 2;
			return;
		}
		if (next == 'J') {
			++*j;
			l = 4;
			return;
		}
		--*j;
		if (next == '-')
			l = 3;
		else if (next == '7')
			l = 2;		
		else
			l = 3;
		return;
	}
	if (l == 1) {
		next = mat[*i*MAT_SIZE + ++*j];
		if (next == '7')
			l = 2;
		else if (next == 'J')
			l = 4;
		return;
	}
	if (l == 2) {
		next = mat[++*i*MAT_SIZE + *j];
		if (next == 'L')
			l = 1;
		else if (next == 'J')
			l = 3;
		return;
	}
	if (l == 3) {
		next = mat[*i*MAT_SIZE + --*j];
		if (next == 'F')
			l = 2;
		else if (next == 'L')
			l = 4;
		return;
	}
	next = mat[--*i*MAT_SIZE + *j];
	if (next == 'F')
		l = 1;
	else if (next == '7')
		l = 3;
}

si, sj;

loop()
{
	int i, j, len;

	i = si;
	j = sj;
	len = 0;
	do { next_pos(&i, &j); len++; } while (i != si || j != sj);
	return len>>1;
}

main()
{
	char c;
	int i, j, len;
	
	mat = malloc(MAT_SIZE*MAT_SIZE);
	i = j = 1;
	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			len = j;
			i++;
			j=1;
			continue;
		} else if (c == 'S') {
			si = i;
			sj = j;	
		}
		mat[i * MAT_SIZE + j++] = c;
	}
	for (i=0; i < len; i++)
		mat[i] = '.';
	for (i=0; i < len; i++)
		mat[i*MAT_SIZE] = '.';
	for (i=0; i <= len; i++)
		mat[i*MAT_SIZE + len] = '.';
	for (i=0; i <= len; i++)
		mat[len*MAT_SIZE + i] = '.';
	printf("%d\n", loop());
}
