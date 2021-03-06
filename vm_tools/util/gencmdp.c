#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "grep.h"
#include "util.h"

typedef struct {
	const char	*cmd_name;
	const char	*func_name;
	const char	*help_message;
} CMD;

static char	folder[1024];
static char	bodyname[1024];
static char	headername[1024];
static char	bodypath[1024];
static char	headerpath[1024];
static char	tablepath[1024];

int get_token(char *path, char *key, char *token)
{
	FILE	*fp;
	char	*ptr, line[1024];
	int	hit = 0;

	if ((fp = fopen(path, "r")) == NULL) {
		printf("%s can't open\n", path);
		return -1;
	}
	while (fgets(line, sizeof(line), fp) != NULL) {
		if (match(key, line)) {
			trim(line);
			for (hit = 0, ptr = line ; *ptr ; ptr++) {
			       	if (*ptr == '=') {
					ptr++;
					hit = 1;
					break;
				}
			}
			while (*ptr == ' ' || *ptr == '\t')
				ptr++;
			if (hit) strcpy(token, ptr);
			printf("%s:[%s]\n", key, token);
			break;
		}
	}
	fclose(fp);
	return 0;
}

int create_cmdp_file(char *bodypath)
{
	FILE	*fp;

	if ((fp = fopen(bodypath, "r")) != NULL) {
		printf("%s, already exist\n", bodypath);
		return 0;
	}
	if ((fp = fopen(bodypath, "w")) == NULL) {
		printf("%s, can't create", bodypath);
		return -1;
	}
	fclose(fp);
	printf("%s created\n", bodypath);
	return 0;	
}

int create_hedr_file(char *headrpath)
{
	FILE	*fp;

	if ((fp = fopen(headerpath, "r")) != NULL) {
		printf("%s, already exist\n", headrpath);
		return 0;
	}
	if ((fp = fopen(headerpath, "w")) == NULL) {
		printf("%s, can't create", headrpath);
		return -1;
	}
	fclose(fp);
	printf("%s created\n", headerpath);
	return 0;	
}

int is_func_exist(char *bodypath, char *funcname)
{
	FILE	*fp;
	char	line[1024];
	int	hit = 0;

	if ((fp = fopen(bodypath, "r")) == NULL) {
		printf("%s, can't open\n", bodypath);
		exit(1);
	}
	while (fgets(line, sizeof(line), fp) != NULL) {
		if (match(funcname, line)) {
			hit = 1;
			break;
		}
	}
	fclose(fp);
	return hit;	
}

int create_func(char *bodypath, char *headerpath, char *funcname)
{
	FILE	*fp;

	if ((fp = fopen(bodypath, "a+")) == NULL) {
		printf("%s, can't open\n", bodypath);
		exit(1);
	}
	fprintf(fp, "\nint %s(char *line)\n{\nreturn 0;\n}", funcname); 
	fclose(fp);
	if ((fp = fopen(headerpath, "a+")) == NULL) {
		printf("%s, can't open\n", headerpath);
		exit(1);
	}
	fprintf(fp, "int %s(char *line);\n", funcname); 
	fclose(fp);
	return 0;	
}

int main(int argc, char **argv)
{
	FILE	*ifp, *ofp;
	char	*line;
	CSV_CTX	*ctx;

	if (argc < 2) {
		printf("usage: %s definition.txt\n", argv[0]);
		printf("%s, %s\n", __TIME__, __DATE__);
		return 0;
	}

	if (get_token(argv[1], "folder", folder) < 0)
		return 1;
	if (get_token(argv[1], "body", bodyname) < 0)
		return 1;
	if (get_token(argv[1], "header", headername) < 0)
		return 1;

	sprintf(bodypath, "%s%s", folder, bodyname);
	if (create_cmdp_file(bodypath) < 0)
		return 1;
	sprintf(headerpath, "%s%s", folder, headername);
	if (create_hedr_file(headerpath) < 0)
		return 1;

	if ((ifp = fopen(argv[1], "r")) == NULL) {
		printf("%s can't open\n", argv[1]);
		return 1;
	}

	sprintf(tablepath, "%s%s", folder, "cmdtbl.h");
	if ((ofp = fopen(tablepath, "w")) == NULL) {
		printf("%s can't open for write\n", tablepath);
		return 1;
	}
	fprintf(ofp, "CMD cmdtbl[] = {\n");

	ctx = new_csv();
	while ((line = csvgetline(ifp, ctx)) != NULL) {
		if (csvnfield(ctx) == 3) {
			if (is_func_exist(bodypath, csvfield(1, ctx))) {
				printf("%s exist\n", csvfield(1, ctx));
			} else {
				create_func(bodypath, headerpath, csvfield(1, ctx));
				printf("created %s\n", csvfield(1, ctx));
			}		
			fprintf(ofp, " { \"%s\", (FP_LINE*)%-24s, ARG_LINE, \"%s\"},\n",
					csvfield(0, ctx),
					csvfield(1, ctx),
					csvfield(2, ctx));
		}
	}
	fclose(ifp);
	fprintf(ofp, " { \"%s\", (FP_LINE*)%-24s, ARG_LINE, \"%s\"},\n",
			"?",
			" help",
			" help message");
	fprintf(ofp, " { \"%s\", (FP_LINE*)%-24s, ARG_LINE, \"%s\"},\n",
			"",
			"0",
			"");
	fprintf(ofp, "};\n");
	fclose(ofp);
	destroy_csv(ctx);
	return 0;
}
