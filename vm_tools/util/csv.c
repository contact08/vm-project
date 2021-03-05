/*$Id$
 CSV handling utilities
 Excerpted from 'The Practice of Programming'
 by Brian W. Kernighan and Rob Pike
 modified by s.kim
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

#include "csv.h"
//#include "util.h"

enum { NOMEM = -2 };          /* out of memory signal */

//static char fieldsep[] = ","; /* field separator chars */

static char *advquoted(char *, char *fieldsep);
static int split(CSV_CTX *ctx);

/* endofline: check for and consume \r, \n, \r\n, or EOF */
static int endofline(FILE *fin, int c)
{
	int eol;

	eol = (c=='\r' || c=='\n');
	if (c == '\r') {
		c = getc(fin);
		if (c != '\n' && c != EOF)
			ungetc(c, fin);	/* read too far; put c back */
	}
	return eol;
}

/* reset: set variables back to starting values */
void csv_reset(CSV_CTX *ctx)
{
	free(ctx->line);	/* free(NULL) permitted by ANSI C */
	free(ctx->sline);
	free(ctx->field);
	ctx->line = NULL;
	ctx->sline = NULL;
	ctx->field = NULL;
	ctx->maxline = ctx->maxfield = ctx->nfield = 0;
}

/* csvgetline:  get one line, grow as needed */
/* sample input: "LU",86.25,"11/4/1998","2:19PM",+4.0625 */
char *csvgetline(FILE *fin, CSV_CTX *ctx)
{	
	int i, c;
	char *newl, *news;

	if (ctx->line == NULL) {			/* allocate on first call */
		//maxline = maxfield = 1;
		ctx->maxline = ctx->maxfield = 2048;	// make it bigger, s.kim
		ctx->line = (char *) malloc(ctx->maxline);
		ctx->sline = (char *) malloc(ctx->maxline);
		ctx->field = (char **) malloc(ctx->maxfield*sizeof(ctx->field[0]));
		if (ctx->line == NULL || ctx->sline == NULL || ctx->field == NULL) {
			csv_reset(ctx);
			return NULL;		/* out of memory */
		}
	}
	for (i=0; (c=getc(fin))!=EOF && !endofline(fin,c); i++) {
		if (i >= (ctx->maxline)-1) {	/* grow line */
			ctx->maxline *= 2;		/* double current size */
			newl = (char *) realloc(ctx->line, ctx->maxline);
			if (newl == NULL) {
				csv_reset(ctx);
				return NULL;
			}
			ctx->line = newl;
			news = (char *) realloc(ctx->sline, ctx->maxline);
			if (news == NULL) {
				csv_reset(ctx);
				return NULL;
			}
			ctx->sline = news;
		}
		ctx->line[i] = c;
	}
	ctx->line[i] = '\0';
	if (split(ctx) == NOMEM) {
		csv_reset(ctx);
		return NULL;			/* out of memory */
	}
	return (c == EOF && i == 0) ? NULL : ctx->line;
}

// returns pointer to next line or NULL, this is different from csvgetline()
char *csv_mem_getline(char *ptr, CSV_CTX *ctx)
{	
	int i, c;
	char *newl, *news;

	if (ctx->line == NULL) {			/* allocate on first call */
		ctx->maxline = ctx->maxfield = 2048;	// make it bigger, s.kim
		ctx->line = (char *) malloc(ctx->maxline);
		ctx->sline = (char *) malloc(ctx->maxline);
		ctx->field = (char **) malloc(ctx->maxfield*sizeof(ctx->field[0]));
		if (ctx->line == NULL || ctx->sline == NULL || ctx->field == NULL) {
			csv_reset(ctx);
			return NULL;		/* out of memory */
		}
	}
	for (i=0 ; (c = *ptr) != '\0' && c != '\n' ; i++, ptr++) {
		//m2log("#csv_mem_getline: %c(%x)", c, c & 255);
		if (*ptr == '\r') continue;	// ignore cr.
		if (i >= (ctx->maxline)-1) {	/* grow line */
			ctx->maxline *= 2;		/* double current size */
			newl = (char *) realloc(ctx->line, ctx->maxline);
			if (newl == NULL) {
				csv_reset(ctx);
				return NULL;
			}
			ctx->line = newl;
			news = (char *) realloc(ctx->sline, ctx->maxline);
			if (news == NULL) {
				csv_reset(ctx);
				return NULL;
			}
			ctx->sline = news;
		}
		ctx->line[i] = c;
	}
	ctx->line[i] = '\0';
	++ptr;
	if (split(ctx) == NOMEM) {
		csv_reset(ctx);
		return NULL;			/* out of memory */
	}
	//return (c == '\0' && i == 0) ? NULL : ctx->line;
	return (c == '\0' && i == 0) ? NULL : ptr;
}

/* split: split line into fields */
static int split(CSV_CTX *ctx)
{
	char *p, **newf;
	char *sepp; /* pointer to temporary separator character */
	int sepc;   /* temporary separator character */
	char	*ptr;

	ctx->nfield = 0;
	if (ctx->line[0] == '\0')
		return 0;
	strcpy(ctx->sline, ctx->line);
	p = ctx->sline;
	do {
		if (ctx->nfield >= ctx->maxfield) {
			ctx->maxfield *= 2;		/* double current size */
			newf = (char **) realloc(ctx->field, 
						ctx->maxfield * sizeof(ctx->field[0]));
			if (newf == NULL)
				return NOMEM;
			ctx->field = newf;
		}
#if 1
		if (*p == '"') {
			ptr = p+1;
			while (*ptr != '"') {
				if (*ptr == ',') *ptr = '.';
				ptr++;
			}
			sepp = advquoted(++p, ctx->fieldsep);		/* skip initial quote */
		} else
			sepp = p + strcspn(p, ctx->fieldsep);
#else
		sepp = p + strcspn(p, ctx->fieldsep);
#endif
		sepc = sepp[0];
		sepp[0] = '\0';				/* terminate field */
		ctx->field[(ctx->nfield)++] = p;
		p = sepp + 1;
	} while (sepc == ctx->fieldsep[0]);

	return ctx->nfield;
}

/* advquoted: quoted field; return pointer to next separator */
static char *advquoted(char *p, char *fieldsep)
{
	int i, j;

	for (i = j = 0; p[j] != '\0'; i++, j++) {
		if (p[j] == '"' && p[++j] != '"') {
			/* copy up to next separator or \0 */
			int k = strcspn(p+j, fieldsep);
			memmove(p+i, p+j, k);
			i += k;
			j += k;
			break;
		}
		p[i] = p[j];
	}
	p[i] = '\0';
	return p + j;
}

/* csvfield:  return pointer to n-th field */
char *csvfield(int n, CSV_CTX *ctx)
{
	if (n < 0 || n >= ctx->nfield) return NULL;
	return ctx->field[n];
}

/* csvnfield:  return number of fields */ 
int csvnfield(CSV_CTX *ctx)
{
	return ctx->nfield;
}

CSV_CTX	*new_csv()
{
	CSV_CTX	*csv;

	if ((csv = (CSV_CTX*)malloc(sizeof(CSV_CTX))) != NULL) {
		memset(csv, 0, sizeof(CSV_CTX));
		csv->fieldsep = ",";
	}
	return csv;
}

void destroy_csv(CSV_CTX *csv)
{
	free(csv);
}

#if 0
/* csvtest main: test CSV library */
int main(int argc, char **argv)
{
	int i;
	char *line;
	int	nlines = 0;
	FILE	*fp;
	CSV_CTX	*ctx;

	if (argc < 2) {
		printf("usage: %s csvfile\n", argv[0]);
		exit(1);
	}
	if ((ctx = new_csv()) == NULL) exit(1);

	if ((fp = fopen(argv[1], "r")) == NULL) {
		printf("%s can't open\n", argv[1]);
		exit(1);
	}

	while ((line = csvgetline(fp, ctx)) != NULL) {
		++nlines;
		for (i = 0; i < csvnfield(ctx); i++) {
			printf("field[%d] = %s\n", i, csvfield(i, ctx));
		}
	}
	fclose(fp);
	printf("lines scanned: %u\n", nlines);

	destroy_csv(ctx);

	return 0;
}
#endif
