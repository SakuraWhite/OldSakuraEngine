#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "../../../../public/simple_math/transformation/vector/vector_2d.h"
#include <stdio.h>

fvector_2d::fvector_2d(float in_x, float in_y)
	:x(in_x)
	,y(in_y)
{

}

float fvector_2d::dot(const fvector_2d& a)
{
	return x * a.x + y * a.y;
}

char *fvector_2d::to_string(char* buff)
{
	sprintf(buff,"(x=%f,y=%f)",x,y);
	return buff;
}
