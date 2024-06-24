/* Código escrito por João Edison Roso Manica */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ARR_SIZE 5000
struct point {
	int x,y;
	unsigned char s;
} *points;
int endp;

sortps(x,y)
const void *x,*y;
{
	const struct point *p1, *p2;
	
	p1 = x;
	p2 = y;
	if (p1->x == p2->x)
		return p1->y-p2->y;
	return p1->x-p2->x;
}

int lline;

inner_area()
{
	int i,j,k,area,in;
	
	k=area=0;
	for (i=0;i<lline;i++) {
		in=0;
		for (j=k;points[j+1].x==i && j+1<endp;j++){
			if (points[j].s) in = !in;
			area += (points[j+1].y-points[j].y-1)*in;
		}
		if (j+1==endp)
			break;
		k=j+1;
	}
	return area;
}

main()
{
	char buffer[8],d;
	int len,x,y,px,py,i,area;
	int lcol,pcol,pline;
	char l;
	
	points = malloc(sizeof(struct point) * ARR_SIZE);
	lcol=lline=endp=0;
	pcol=pline=INT_MAX;
	l=px=py=x=y=0;
	while (scanf("%c %d (#%7s\n", &d, &len, buffer)!=EOF){
		switch (d) {
		case 'R':
			y+=len;
			if (y>lcol) lcol=y;
			points[endp-1].s = l=='D';
			while (py<y){
				py++;
				points[endp].x = x;
				points[endp].y = py;
				points[endp++].s = 0;
				if (endp==ARR_SIZE){ fputs("MAX CAPACITY\n",stderr); return 1; }
			}
			l='R';
			break;
		case 'L':
			y-=len;
			if (y<pcol) pcol=y;
			points[endp-1].s = l=='D';
			while (py>y){
				py--;
				points[endp].x = x;
				points[endp].y = py;
				points[endp++].s = 0;
				if (endp==ARR_SIZE){ fputs("MAX CAPACITY\n",stderr); return 1; }
			}
			l='L';
			break;
		case 'U':
			x-=len;
			if (x<pline) pline=x;
			points[endp-1].s = l=='L'||l=='R';
			while (px>x){
				px--;
				points[endp].x = px;
				points[endp].y = y;
				points[endp++].s = 1;
				if (endp==ARR_SIZE){ fputs("MAX CAPACITY\n",stderr); return 1; }
			}
			points[endp-1].s = 0;
			l='U';
			break;
		case 'D':
			x+=len;
			if (x>lline) lline=x;
			while (px<x){
				px++;
				points[endp].x = px;
				points[endp].y = y;
				points[endp++].s = 1;
				if (endp==ARR_SIZE){ fputs("MAX CAPACITY\n",stderr); return 1; }
			}
			points[endp-1].s = 0;
			l='D';
			break;
		}
	}
	if (pline < 0) {
		for (i=0;i<endp;i++)
			points[i].x -= pline;
		lline-=pline;
	}
	if (pcol < 0) {
		for (i=0;i<endp;i++)
			points[i].y -= pcol;
		lcol-=pcol;
	}
	qsort(points, endp, sizeof(struct point), sortps);
	area = inner_area();
	area += endp;
	printf("%d\n", area);
}
