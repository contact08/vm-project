#include "libbasic.h"
#include "libx3.h"

#define EXTRN extern
#include "global.h"

static	CUI_VAR	*var;

CUI_VAR	cui_ground = {
	"ground level in m",
	CUI_DOUBLE,
	(void*)(&arbox[0]->ground),
	0.05,
	+2.0,	
	-2.0,
};

CUI_VAR	cui_altitude = {
	"platform altitude in m",
	CUI_DOUBLE,
	(void*)&(arbox[0]->altitude),
	0.05,
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

CUI_VAR	cui_rolling = {
	"set rolling angle of arbox",
	CUI_INT,
	(void*)(&rolling_deg),
	1,
	 180.0,	
	-180.0,
};

int  adj_g(char *line)
{
	cui_ground.ptr = &(arbox[index_arbox]->ground);
	var = &cui_ground;
	return 0;
}

int  adj_h(char *line)
{
	cui_altitude.ptr = &(arbox[index_arbox]->altitude);
	var = &cui_altitude;
	return 0;
}

int  adj_m(char *line)
{
	var = &cui_impose;
	inc_cui_var(var);
	return 0;
}

CUI_VAR	cui_arbox = {
	"current arbox",
	CUI_INT,
	(void*)&(index_arbox),
	1,
	1.0,	
	0,
};

int  adj_b(char *line)
{
	var = &cui_arbox;
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

int  adj_r(char *line)
{
	var = &cui_rolling;
	return 0;
}
