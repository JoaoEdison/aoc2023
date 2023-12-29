#include <stdio.h>
#include <stdlib.h>

#define SIZE 150

typedef unsigned char positive;

struct endp {
	positive x,y;
} *sparr1, *sparr2;

char matx[SIZE][SIZE];
positive points[SIZE*SIZE] = { 0 };
wid,len;
split1l,split2l;

void walkin(direction, x, y)
char direction;
{
	int i;
	
L1:
	if (x == wid || y == len || y == -1 || x == -1)
		return;
	points[x*SIZE+y]=1;
	switch (direction) {
		case '>':
			if (matx[x][y] == '\\') {
				direction='v';
				x++;
			} else if (matx[x][y] == '/') {
				direction='^';
				x--;
			} else if (matx[x][y] == '|') {
				y++;
				for (i=0; i < split1l; i++)
					if (sparr1[i].x == x && sparr1[i].y == y)
						return;
				sparr1[split1l].x = x;
				sparr1[split1l++].y = y;
				walkin('v', x+1, y-1);
				walkin('^', x-1, y-1);
				return;
			} else 
				y++;
			break;
		case '<':
			if (matx[x][y] == '\\') {
				direction='^';
				x--;
			} else if (matx[x][y] == '/') {
				direction='v';
				x++;
			} else if (matx[x][y] == '|') {
				y--;
				for (i=0; i < split1l; i++)
					if (sparr1[i].x == x && sparr1[i].y == y)
						return;
				sparr1[split1l].x = x;
				sparr1[split1l++].y = y;
				walkin('v', x+1, y+1);
				walkin('^', x-1, y+1);
				return;
			} else
				y--;
			break;
		case '^':
			if (matx[x][y] == '\\') {
				direction='<';
				y--;
			} else if (matx[x][y] == '/') {
				direction='>';
				y++;
			} else if (matx[x][y] == '-') {
				x--;
				for (i=0; i < split2l; i++)
					if (sparr2[i].x == x && sparr2[i].y == y)
						return;
				sparr2[split2l].x = x;
				sparr2[split2l++].y = y;
				walkin('<', x+1, y-1);
				walkin('>', x+1, y+1);
				return;
			} else
				x--;
			break;
		case 'v':
			if (matx[x][y] == '\\') {
				direction='>';
				y++;
			} else if (matx[x][y] == '/') {
				direction='<';
				y--;
			} else if (matx[x][y] == '-') {
				x++;
				for (i=0; i < split2l; i++)
					if (sparr2[i].x == x && sparr2[i].y == y)
						return;
				sparr2[split2l].x = x;
				sparr2[split2l++].y = y;
				walkin('<', x-1, y-1);
				walkin('>', x-1, y+1);
				return;
			} else
				x++;
			break;
	}
	goto L1;
}

main()
{
	int i, j, ans, split1, split2;
	char c;
	
	wid=len=0;
	split1=split2=0;
	split1l=0;split2l=0;
	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			j=wid;
			wid=0;
			len++;
			continue;
		} else if (c == '|')
			split1++;	
		else if (c == '-')
			split2++;
		matx[len][wid++] = c;
	}
	sparr1 = malloc(sizeof(struct endp) * split1);
	sparr2 = malloc(sizeof(struct endp) * split2);
	wid = j;
	walkin('>', 0, 0);
	ans = 0;
	for (i=0; i < len; i++)
		for (j=0; j < wid; j++)
			if (points[i*SIZE+j])
				ans++;
	printf("%d\n", ans);
}
