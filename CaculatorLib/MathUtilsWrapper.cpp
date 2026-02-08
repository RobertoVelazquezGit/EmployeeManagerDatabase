#include "pch.h"
#include "MathUtilsWrapper.h"
#include "MathUtils.h"

int MathUtilsWrapper::add(int a, int b)
{
	return MathUtils::add(a, b);  //static method of MathUtils class is called	
}
