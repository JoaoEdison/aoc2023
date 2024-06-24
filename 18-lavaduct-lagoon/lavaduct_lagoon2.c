/* Código escrito por João Edison Roso Manica */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define ARR_SIZE 5000
struct point {
	long x,y;
} *points;
int endp;

long inner_area()
{
	double aux,area;
	int i;

	for (i=0;i+1<endp;i++)
		// Trapezoid formula
		if ((aux=(points[i].y+points[i+1].y)*(points[i].x-points[i+1].x))!=.0) {
			aux/=2.0;
			area += aux;
		}
	if ((aux=(points[i].y+points[0].y)*(points[i].x-points[0].x))!=.0)
		aux/=2.0;
	return fabs(area+aux);
}

main()
{
	char buffer[8],d;
	long len,x,y,area;
	long pcol,pline;
	long perimeter;
	int i;
	
	points = malloc(sizeof(struct point) * ARR_SIZE);
	endp=0;
	pcol=pline=LONG_MAX;
	perimeter=x=y=0;
	while (scanf("%c %ld (#%7s\n", &d, &len, buffer)!=EOF){
		d = buffer[5];
		buffer[5] = '\0';
		len = strtol(buffer, NULL, 16);
		switch (d) {
		case '0':
			y+=len;
			break;
		case '2':
			y-=len;
			if (y<pcol) pcol=y;
			break;
		case '3':
			x-=len;
			if (x<pline) pline=x;
			break;
		case '1':
			x+=len;
			break;
		}
		points[endp].x = x;
		points[endp++].y = y;
		perimeter+=len;
		if (endp==ARR_SIZE){ fputs("MAX CAPACITY\n",stderr); return 1; }
	}
	if (pline < 0)
		for (i=0;i<endp;i++)
			points[i].x -= pline;
	if (pcol < 0)
		for (i=0;i<endp;i++)
			points[i].y -= pcol;
	area = inner_area();
	// Pick's theorem (+1?????)
	area += (perimeter>>1)+1;
	printf("%ld\n", area);
}
