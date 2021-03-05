/* 
 remove eprintf.h functions for portability. by s.kim
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grep.h"

#if 0

/* grep main: search for regexp in files */
int main(int argc, char *argv[])
{
	int i, nmatch;
	FILE *f;

	if (argc < 2)
		printf("usage: %s regexp [file ...]", argv[0]);
	nmatch = 0;
	if (argc == 2) {
		if (grep(argv[1], stdin, NULL))
			nmatch++;
	} else {
		for (i = 2; i < argc; i++) {
			f = fopen(argv[i], "r");
			if (f == NULL) {
				printf("can't open %s:", argv[i]);
				continue;
			}
			if (grep(argv[1], f, argc>3 ? argv[i] : NULL) > 0)
				nmatch++;
			fclose(f);
		}
	}
	return nmatch == 0;
}

#endif

/* matchhere: search for regexp at beginning of text */
int matchhere(char *regexp, char *text)
{
	if (regexp[0] == '\0')
		return 1;
	if (regexp[1] == '*')
		return matchstar(regexp[0], regexp+2, text);
	if (regexp[0] == '$' && regexp[1] == '\0')
		return *text == '\0';
	if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
		return matchhere(regexp+1, text+1);
	return 0;
}

static char	*matchp;

/* match: search for regexp anywhere in text */
int match(char *regexp, char *text)
{
	if (regexp[0] == '^')
		return matchhere(regexp+1, text);
	do {	/* must look even if string is empty */
		if (matchhere(regexp, text)) {
			matchp = text;	//+++
			return 1;
		}
	} while (*text++ != '\0');
	return 0;
}

/* match: search for regexp anywhere in text */
char *get_matchp(void)
{
	return matchp;
}

/* matchstar: search for c*regexp at beginning of text */
int matchstar(int c, char *regexp, char *text)
{
	do {	/* a * matches zero or more instances */
		if (matchhere(regexp, text))
			return 1;
	} while (*text != '\0' && (*text++ == c || c == '.'));
	return 0;
}
/*
int main(int argc, char **argv)
{
	int	i, j, n1, n2, n3, n4;
	FILE	*fp;
	char	*ptr, line[1024*2], token[1024*2], kword[1024];
	char	ch, temp[1024];
	for (i = 1 ; i < 14 ; i++) {
		sprintf(kword, "U%d", i);
		printf("----%s----\n", kword);
		if ((fp =fopen("BTLPCTP2.net", "r")) == NULL) return 1;
		while (fgets(line, sizeof(line), fp)) {
			if (match(kword, line)) {
				ptr= line+4;
				while (*ptr) {
					if (matchhere(kword, ptr)) {
						sscanf(ptr, "U%d-%d", &n1, &n2);
						break;
					} else
						ptr++;
				}
				if (i == n1) {
					sprintf(temp, "[U%d-%d", n1, n2);
					if (!strncmp(line+4, temp, strlen(temp))) {
						ptr = line+4;
						while (*ptr != ',' && *ptr == ']')
							++ptr;
						++ptr;
						printf("%U%02d-%02d,%s", n1, n2, ptr);
					} else {
						for (j = 0 ; ; j++) {				
							ch = *(line+5+j);
							if (ch == ',' || ch == ']')
								break;
							temp[j] = ch;
						}
						temp[j] = 0;
						printf("U%02d-%02d,%s\n", n1, n2, temp);
					}
				}
			}
		}
		fclose(fp);
	}
	return 0;
}
*/
