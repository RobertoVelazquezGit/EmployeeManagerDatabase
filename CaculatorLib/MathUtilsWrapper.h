#pragma once
#include "IMathUtils.h"
class MathUtilsWrapper :
    public IMathUtils
{
public:
    int add(int a, int b) override;
};

