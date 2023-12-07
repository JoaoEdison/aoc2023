/* Código escrito por João Edison Roso Manica */
#include <stdio.h>
#include <stdlib.h>

struct one_hand {
	char cards[6];
	int bid;
	unsigned char type;
} hands[2000];

hash_fn(card)
char card;
{
	if (card == 'A')
		return 0;
	if (card == 'K')
		return 1;
	if (card == 'Q')
		return 2;
	if (card == 'J')
		return 3;
	if (card == 'T')
		return 4;
	return '9'-card+5;
}

hand_type(cards)
char cards[];
{
	int i, count[13];
	int max1, max2;
	char *p;

	for (i=0; i < 13; i++)
		count[i] = 0;
	for (p=cards; *p; p++)
		count[hash_fn(*p)]++;
	max2 = max1 = -1;
	for (i=0; i < 13; i++)
		if (max1 < count[i]) {
			max2 = max1;
			max1 = count[i];
		} else if (max2 < count[i])
			max2 = count[i];
	if (max1 == 5)
		return 1;
	if (max1 == 4)
		return 2;
	if (max1 == 3 && max2 == 2)
		return 3;
	if (max1 == 3)
		return 4;
	if (max1 == 2 && max2 == 2)
		return 5;
	if (max1 == 2)
		return 6;
	return 7;
}

sort_cards(x, y)
const void *x, *y;
{
	int i, v1, v2;
	char *p1, *p2;

	if (((struct one_hand*)x)->type < ((struct one_hand*)y)->type)
		return 1;
	if (((struct one_hand*)x)->type > ((struct one_hand*)y)->type)
		return -1;
	p1 = ((struct one_hand*)x)->cards;
	p2 = ((struct one_hand*)y)->cards;
	for (; *p1; i++, p1++, p2++) {
		v1 = hash_fn(*p1);
		v2 = hash_fn(*p2);
		if (v1 < v2)
			return 1;
		else if (v1 > v2)
			return -1;
	}
	return 0;
}

main()
{
	int total_hands;
	int i;
	int res;

	total_hands = 0;
	while (scanf("%s %d\n", hands[total_hands].cards, &hands[total_hands].bid) != EOF)
		total_hands++;
	for (i=0; i < total_hands; i++)
		hands[i].type = hand_type(hands[i].cards);
	qsort(hands, total_hands, sizeof(struct one_hand), sort_cards);
	res = hands[0].bid;
	for (i=1; i < total_hands; i++)
		res += hands[i].bid * (i+1);
	printf("%d\n", res);
}
