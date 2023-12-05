#include <stdio.h>
#include <limits.h>

long seeds[20];
seed_len;

struct {
	long dest_start, source_start, len; 
} mappings[60];

map_seeds()
{
	int i, j, map_len, last;

	while (getchar()!='\n');
	map_len = 0;
	while ((last = scanf("%ld %ld %ld\n", &mappings[map_len].dest_start, &mappings[map_len].source_start, &mappings[map_len].len))) {
		if (last == EOF)
			break;
		map_len++;
	}
	for (i=0; i < seed_len; i++)
		for (j=0; j < map_len; j++) {
			if (seeds[i] >= mappings[j].source_start &&
			    seeds[i] <= mappings[j].source_start+mappings[j].len) {
				seeds[i] = seeds[i] - mappings[j].source_start + mappings[j].dest_start;
				break;
			}
				
		}
	return last != EOF;
}

main()
{
	int i;
	long min;
	
	min = INT_MAX;
	seed_len = -1;
	scanf("seeds:");
	while (scanf(" %ld", &seeds[++seed_len]));
	while (map_seeds());
	for (i=0; i < seed_len; i++)
		if (min > seeds[i])
			min = seeds[i];
	printf("%ld\n", min);
}
