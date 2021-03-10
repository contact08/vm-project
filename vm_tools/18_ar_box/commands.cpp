#include "libbasic.h"
#include "libx3.h"

extern ARBOX *arbox;
extern int impose;

static	CUI_VAR	*var;

CUI_VAR	cui_ground = {
	"ground level in m",
	CUI_DOUBLE,
	(void*)(&arbox->ground),
	0.1,
	+2.0,	
	-2.0,
};

CUI_VAR	cui_altitude = {
	"platform altitude in m",
	CUI_DOUBLE,
	(void*)&(arbox->altitude),
	0.1,
	+2.0,	
	-2.0,
};

CUI_VAR	cui_impose = {
	"impose depth or not",
	CUI_INT,
	(void*)(&impose),
	1,
	1.0,	
	0.0,
};

int  adj_g(char *line)
{
	printf("ptr: %p\n", cui_ground.ptr);
	cui_ground.ptr = &(arbox->ground);
	printf("ptr: %p\n", cui_ground.ptr);
	var = &cui_ground;
	return 0;
}

int  adj_h(char *line)
{
	printf("ptr: %p\n", cui_altitude.ptr);
	cui_altitude.ptr = &(arbox->altitude);
	printf("ptr: %p\n", cui_altitude.ptr);
	var = &cui_altitude;
	return 0;
}

int  adj_m(char *line)
{
	var = &cui_impose;
	inc_cui_var(var);
	return 0;
}

int  inc_var(char *line)
{
	inc_cui_var(var);
	return 0;
}

int  dec_var(char *line)
{
	dec_cui_var(var);
	return 0;
}
