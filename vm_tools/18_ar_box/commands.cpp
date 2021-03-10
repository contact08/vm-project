#include "libbasic.h"
#include "libx3.h"

extern double ground;
extern double pfheight;
extern int impose;

static	CUI_VAR	*var;

CUI_VAR	cui_ground = {
	"ground level in m",
	CUI_DOUBLE,
	(void*)(&ground),
	0.1,
	+2.0,	
	-2.0,
};

CUI_VAR	cui_altitude = {
	"platform altitude in m",
	CUI_DOUBLE,
	(void*)(&pfheight),
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
	var = &cui_ground;
	return 0;
}

int  adj_h(char *line)
{
	var = &cui_altitude;
	return 0;
}

int  adj_m(char *line)
{
	var = &cui_impose;
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
