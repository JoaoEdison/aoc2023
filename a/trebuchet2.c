/* Código escrito por João Edison Roso Manica */
#include <stdio.h>

char *numbers[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
char *numbers_rev[] = {"eno", "owt", "eerht", "ruof", "evif", "xis", "neves", "thgie", "enin"};

main()
{
	char *lineptr, *ptr, *ptr2, *ptr3;
	size_t n;
	int i, number, other, sum;
	
	lineptr = NULL;
	n = sum = 0;	
	while (getline(&lineptr, &n, stdin) != EOF) {
		for (ptr=lineptr; *ptr != '\n'; ptr++) {
			if (*ptr >= '0' && *ptr <= '9') {
				number = *ptr - '0';
				break;
			}
			for (i=0; i < 9; i++) {
				ptr3 = ptr;
				for (ptr2=numbers[i]; *ptr2; ptr3++, ptr2++)
					if (*ptr3 != *ptr2)
						break;
				if (!*ptr2) {
					number = i+1;
					goto end;
				}
			}
		}
end:
		while (*ptr) ptr++;
		do {
			ptr--;
			if (*ptr >= '0' && *ptr <= '9') {
				other = *ptr - '0';
				break;
			}
			for (i=0; i < 9; i++) {
				ptr3 = ptr;
				for (ptr2=numbers_rev[i]; *ptr2; ptr3--, ptr2++)
					if (*ptr3 != *ptr2)
						break;
				if (!*ptr2) {
					other = i+1;
					goto end2;
				}
			}
		} while (ptr != lineptr);
end2:
		sum += number * 10 + other;
	}
	printf("%d\n", sum);
}
