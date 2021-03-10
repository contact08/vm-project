#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <linux/types.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "libbasic.h"
#include "libx3.h"

void inc_cui_var(CUI_VAR *var)
{
	switch(var->type) {
	   case CUI_INT:
		*(int*)var->ptr += (int)var->step;
		if (*(int*)(var->ptr) > (int)var->max) {
			*(int*)var->ptr = (int)var->min;
		}
		break;
	   case CUI_UINT:
		*(unsigned*)var->ptr += (unsigned)var->step;
		if (*(unsigned*)(var->ptr) > (unsigned)var->max) {
			*(unsigned*)var->ptr = (unsigned)var->min;
		}
		break;
	   case CUI_DOUBLE:
		*(double*)var->ptr += (double)var->step;
		if (*(double*)(var->ptr) > (double)var->max) {
			*(double*)var->ptr = (double)var->min;
		}
		break;
	   case CUI_FLOAT:
		*(float*)var->ptr += (float)var->step;
		if (*(float*)(var->ptr) > (float)var->max) {
			*(float*)var->ptr = (float)var->min;
		}
		break;
	   default:	
			break;
	}
}

void dec_cui_var(CUI_VAR *var)
{
	switch(var->type) {
	   case CUI_INT:
		*(int*)var->ptr -= (int)var->step;
		if (*(int*)(var->ptr) < (int)var->min) {
			*(int*)var->ptr = (int)var->max;
		}
		break;
	   case CUI_UINT:
		*(unsigned*)var->ptr -= (unsigned)var->step;
		if (*(unsigned*)(var->ptr) < (unsigned)var->min) {
			*(unsigned*)var->ptr = (unsigned)var->max;
		}
		break;
	   case CUI_DOUBLE:
		*(double*)var->ptr -= (double)var->step;
		if (*(double*)(var->ptr) < (double)var->min) {
			*(double*)var->ptr = (double)var->max;
		}
		break;
	   case CUI_FLOAT:
		*(double*)var->ptr -= (double)var->step;
		if (*(float*)(var->ptr) < (float)var->min) {
			*(float*)var->ptr = (float)var->max;
		}
		break;
	   default:	
			break;
	}
}
