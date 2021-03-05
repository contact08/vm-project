
typedef struct {
	char *line;    /* input chars */
	char *sline;   /* line copy used by split */
	int  maxline;  /* size of line[] and sline[] */
	char **field;  /* field pointers */
	int  maxfield; /* size of field[] */
	int  nfield;   /* number of fields in field[] */
	char *fieldsep;
} CSV_CTX;

/* csv.h: interface for csv library */
#ifdef __cplusplus
extern "C" {
#endif

char *csvgetline(FILE *f, CSV_CTX *ctx); /* read next input line */
char *csvfield(int n, CSV_CTX *ctx);	  /* return field n */
int csvnfield(CSV_CTX *ctx);		  /* return number of fields */
void csv_reset(CSV_CTX *ctx);
char *csv_mem_getline(char *ptr, CSV_CTX *ctx);  /* read next line from mem. */

CSV_CTX	*new_csv();
void destroy_csv(CSV_CTX *csv);

#ifdef __cplusplus
}
#endif
