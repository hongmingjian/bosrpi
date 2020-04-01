unsigned int __udivmodsi4(unsigned int num, unsigned int den, unsigned int * rem_p)
{
	unsigned int quot = 0, qbit = 1;

	if (den == 0)
	{
		return 0;
	}

	/*
	 * left-justify denominator and count shift
	 */
	while ((signed int) den >= 0)
	{
		den <<= 1;
		qbit <<= 1;
	}

	while (qbit)
	{
		if (den <= num)
		{
			num -= den;
			quot += qbit;
		}
		den >>= 1;
		qbit >>= 1;
	}

	if (rem_p)
		*rem_p = num;

	return quot;
}

unsigned int __aeabi_uidiv(unsigned int num, unsigned int den)
{
	return __udivmodsi4(num, den, 0);
}

unsigned int __aeabi_uidivmod(unsigned int num, unsigned int den)
{
	unsigned int quot = __udivmodsi4(num, den, 0);
	__asm__ __volatile__ ("sub r1, %0, %1\n\t":  : "r" (num), "r" (den*quot) : "r1" );
	return quot;
}