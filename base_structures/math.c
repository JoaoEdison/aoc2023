/* Código escrito por João Edison Roso Manica. */
typedef unsigned long positive_int;

positive_int greatest_common_divisor(x, y)
positive_int x, y;
{
	while (x && y)
		if (x > y)
			x %= y;
		else
			y %= x;
	return x? x : y;
}

#include <assert.h>

unsigned brent(f, end, mu)
long *f;
unsigned *mu;
{
    long tortoise, hare;
    unsigned hare_i, tortoise_i, power, lam;
    
	// Needs at least 2 in array f.
	assert(end>1);

    power = lam = 1;
    tortoise = f[0];
    hare_i = 1;
    hare = f[1];
    while (tortoise != hare) {
        if (power == lam) {
            tortoise = hare;
            power<<=1;
            lam = 0;
        }
        if (++hare_i == end)
            return 0;
        hare = f[hare_i];
        lam++;
    }
	
	tortoise = f[0];
	tortoise_i = 0;
	hare = f[lam];
	hare_i = lam;
	for (*mu = 0; tortoise != hare; ++*mu) {
		tortoise = f[tortoise_i++];
		hare = f[hare_i++];
	}
	return lam;
}
