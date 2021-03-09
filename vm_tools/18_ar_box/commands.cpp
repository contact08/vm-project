#include "libbasic.h"
#include "libx3.h"

extern double ground;

int  adj_g(char *line)
{
	ground += 0.05;
	return 0;
}
int  adj_z(char *line)
{
	ground -= 0.05;
	return 0;
}
int  inc_var(char *line)
{
	ground += 0.02;
	return 0;
}
int  dec_var(char *line)
{
	ground -= 0.02;
	return 0;
}
