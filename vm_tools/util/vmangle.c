#include <stdio.h>
#include <math.h>

#define PI ((double)3.14159265359)
#define D2P(X) ((double)(X*PI)/180.0)
#define R2D(X) ((double)(X*180.0)/PI)

#define OLDW ((double)640)
#define NEWW ((double)752)

int main(int argc, char **argv)
{
	double	oldview, newview, oldangle, newangle;
	double	degree;

	if (argc != 2) {
		printf("usage: %s vmxx20pp2_angle_in_degree\n", argv[0]);
		printf("%s, %s\n", __TIME__, __DATE__);
		return 1;
	}
	if (sscanf(argv[1], "%lf", &oldangle) != 1) {
		printf("error: %s\n", argv[1]);
		return 1;
	}
	oldview = tan(D2P(oldangle/2.0));
	newview = oldview * NEWW/OLDW;
	newangle = R2D(atan(newview)) * 2.0;

	printf("old angle: %lf\n", oldangle);
	printf("new angle: %lf\n", newangle);
	return 0;
}
