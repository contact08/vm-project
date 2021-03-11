#include <stdio.h>
#include <string.h>
#include "commands.h"

enum {
	ARG_VOID,	// no arg
	ARG_LINE,	// string arg
	ARG_NUM1,	// 1 number
	ARG_NUM2,	// 2 numbers
};

typedef struct {
	const char	*name;
	int	(*funcptr)(char*);
	int	arg_type;
	const char	*comment;
} CMD;

int help(char *ptr);

typedef int FP(int);
typedef int FP_LINE(char*);
typedef int FP_VOID();
typedef int FP_NUM1(int);
typedef int FP_NUM2(int, int);
typedef int FP_TOKN(char *, int);

#include "cmdtbl.h"

int	help(char *ptr)
{
	int	i;

	printf("compiled: %s %s\n", __DATE__, __TIME__);	
	printf("------------------------------------------------------------------\n");	
	for (i = 0 ; strcmp("", cmdtbl[i].name) ; i++) {
		printf("%-16s | %s\r\n", cmdtbl[i].name, cmdtbl[i].comment);
	}
	printf("------------------------------------------------------------------\n");	
	return 0;
}

int cmd_proc(char *cmd)
{
	int	i, ret = -1;		
	for (i = 0 ; strcmp("", cmdtbl[i].name) ; i++) {
		if (!strcmp(cmd, cmdtbl[i].name)) {
#if 0
			if (cmdtbl[i].arg_type == ARG_VOID) {
				ret = ((FP_VOID*)cmdtbl[i].funcptr)();
			} else if (cmdtbl[i].arg_type == ARG_NUM1) { 
				cmdtbl[i].funcptr(n);
			} else if (cmdtbl[i].arg_type == ARG_NUM2) { 
				((FP_NUM2*)cmdtbl[i].funcptr)(n, n2);
			} else if (cmdtbl[i].arg_type == ARG_TOKN) { 
				((FP_TOKN*)cmdtbl[i].funcptr)(token2, n2);
			}
#else
			if (cmdtbl[i].arg_type == ARG_LINE) {
				ret = ((FP_LINE*)cmdtbl[i].funcptr)(NULL);
			}
#endif
		}
	}
	return ret;
}
