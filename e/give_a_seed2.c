#include <stdio.h>
#include <limits.h>

typedef unsigned long long seedt;

seedt seeds[20];
seed_len, total_maps;

struct mapping {
	seedt dest_start, source_start, len; 
};

struct {
	struct mapping maps[60];
	int map_len;
} mappings_arr[10];

get_maps()
{
	int map_len, last;

	while (getchar()!='\n');
	map_len = 0;
	while ((last = scanf("%llu %llu %llu\n", &mappings_arr[total_maps].maps[map_len].dest_start,
					      &mappings_arr[total_maps].maps[map_len].source_start,
					      &mappings_arr[total_maps].maps[map_len].len))) {
		if (last == EOF)
			break;
		map_len++;
	}
	mappings_arr[total_maps].map_len = map_len;
	return last != EOF;
}

main()
{
	int i, j, k, l;
	seedt min, seed;
	
	min = LONG_MAX;
	seed_len = -1;
	scanf("seeds:");
	while (scanf(" %llu", &seeds[++seed_len]));
	while (get_maps())
		total_maps++;
	total_maps++;
	for (i=0; i < seed_len; i+=2)
		for (j=seeds[i]; j < seeds[i]+seeds[i+1]; j++) {
			seed = j;
			if (seed < 0)
				printf("%d\n", i);
			for (k=0; k < total_maps; k++)
				for (l=0; l < mappings_arr[k].map_len; l++)
					if (seed >= mappings_arr[k].maps[l].source_start &&
					    seed <= mappings_arr[k].maps[l].source_start+mappings_arr[k].maps[l].len) {
						seed = seed - mappings_arr[k].maps[l].source_start + mappings_arr[k].maps[l].dest_start;
						break;
					}
			if (min > seed) {
				min = seed;
				printf("%llu\n", min);
			}
		}
}
