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
