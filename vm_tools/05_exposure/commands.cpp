#include <stdio.h>
#include <ISCSDKLib.h>
#include "libbasic.h"
extern ISCSDKLib	*sdk;

static unsigned	expval = 0;
unsigned set_exposure(unsigned n);

unsigned get_expval(void)
{
	return expval;
}

int  exp2mnual(char *line)
{
	int	n;
	HERE;
	n = sdk->GetExposureValue(&expval);
	check_if_0("GetExposureValue", n);
	printf("exposure: %d\n", expval);
	n = sdk->SetShutterControlMode(false);
	check_if_0("SetShutterControlMode", n);
	return 0;
}

int  exp2single(char *line)
{
	int	n;
	HERE;
	n = sdk->SetShutterControlMode(1);
	check_if_0("SetShutterControlMode", n);
	expval = 0;
	return 0;
}

int  exp2double(char *line)
{
	int	n;
	HERE;
	n = sdk->SetShutterControlMode(2);
	check_if_0("SetShutterControlMode", n);
	expval = 0;
	return 0;
}

int  exp2double_no_merge(char *line)
{
	int	n;
	HERE;
	n = sdk->SetShutterControlMode(3);
	check_if_0("SetShutterControlMode", n);
	expval = 0;
	return 0;
}

int  incexp(char *line)
{
	HERE;
	if ((expval += 10) > 480)
		expval = 479;
	expval = set_exposure(expval);
	return 0;
}

int  decexp(char *line)
{
	HERE;
	if ((expval -= 10) < 1)
		expval = 2;
	expval = set_exposure(expval);
	return 0;
}
