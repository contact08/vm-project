#include <stdio.h>
#include <ISCSDKLib.h>
#include "libbasic.h"
#include "libx3.h"
#define EXTRN extern
#include "global.h"
#include "commands.h"

static	CUI_VAR	*var;

CUI_VAR	cui_near = {
	"near side distance (cm)",
	CUI_INT,
	(void*)(&near_cm),
	1.0,
	500.0,	
	50.0,
};


CUI_VAR	cui_far = {
	"far side distance (cm)",
	CUI_INT,
	(void*)(&far_cm),
	1.0,
	500.0,	
	50.0,
};


int  set_near(char *line)
{
	var = &cui_near;
	return 0;
}
int  set_far(char *line)
{
	var = &cui_far;
	return 0;
}
int  inczdist(char *line)
{
	inc_cui_var(var);
	return 0;
}
int  deczdist(char *line)
{
	dec_cui_var(var);
	return 0;
}
