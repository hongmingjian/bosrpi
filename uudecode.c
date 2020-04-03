/**
 *
 * UUDECODE
 * Copyright (C) 1998 Clem Dye
 * Copyright (C) 2007 Hong MingJian
 *
 */
#include <string.h>

#define DEC(c)  (((c) - ' ') & 077)

int uudecode (unsigned char *where, char *filename)
{
	char *p;
	unsigned char *out;
	p = out = where;

	{
		int i;
		char *q;
		do {
			q = p;
			while(*p++ != '\n')
				;
		} while(strncmp(q, "begin ", 6));

		q += 6; //skip "begin "

		while(*q != ' ') q++; //skip mode
		                 q++; //skip space

		i = 0;
		while(*q != '\r' && *q != '\n') {
			filename[i] = *q;
			q++;
			i++;
			if(i == 15)
				break;
		}
		filename[i] = 0;
	}

	while (1) {
		int n;

		while(*p == '\n')
			p++;

		if (!strncmp(p, "end\n", 4))
			break;

		if (!strncmp(p, "end\r\n", 5))
			break;

		n = DEC (*p);

		if (n <= 0)
			break;

		for (++p; n > 0; p += 4, n -= 3) {
			if (n >= 3)	{
				*out++ = DEC (p[0]) << 2 | DEC (p[1]) >> 4;
				*out++ = DEC (p[1]) << 4 | DEC (p[2]) >> 2;
				*out++ = DEC (p[2]) << 6 | DEC (p[3]);
			} else {
				if (n >= 1)	{
					*out++ = DEC (p[0]) << 2 | DEC (p[1]) >> 4;
				}
				if (n >= 2)	{
					*out++ = DEC (p[1]) << 4 | DEC (p[2]) >> 2;
				}
			}
		}

		/*discard rest of the line*/
		while(*p != '\n')
			p++;
	}

	return (out-where);
}

#if __TESTING__

#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	long size;
	char *buf;
	char filename[16];


	fprintf(stdout, "\\n=%x, \\r=%x\n", '\n', '\r');

	fp = fopen("kernel.uue", "rb");
	if(fp == NULL) {
		fprintf(stderr, "error fopen");
		return 1;
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	fprintf(stdout, "size=%d\n", size);

	buf = (char *)malloc(size);
	{
		int read = fread(buf, 1, size, fp);
		if( read != size) {
			fprintf(stderr, "error fread (%d expected, %d read)\n", size, read);
			free(buf);
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);

	fp = fopen("kernel.uud", "wb");
	if(fp == NULL) {
		fprintf(stderr, "error fopen");
		free(buf);
		return 1;
	}

	size = uudecode(buf, filename);
	fwrite(buf, 1, size, fp);

	fprintf(stdout, "%d bytes decoded.\n", size);

	free(buf);
	fclose(fp);
	return 0;
}

#endif

