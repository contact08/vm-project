#include "libbasic.h"
#include "libx3.h"

extern double ground;
extern double pfheight;

static	double	*ptr = &ground;

int  adj_g(char *line)
{
	ptr = &ground;
	return 0;
}
int  adj_h(char *line)
{
	ptr = &pfheight;
	return 0;
}
int  inc_var(char *line)
{
	*ptr += 0.01;
	return 0;
}
int  dec_var(char *line)
{
	*ptr -= 0.01;
	return 0;
}
