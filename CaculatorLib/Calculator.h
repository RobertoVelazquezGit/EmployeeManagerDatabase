#pragma once
#include "IMathUtils.h"

class Calculator
{
public:
    explicit Calculator(IMathUtils& math);

    int compute();

private:
    IMathUtils& mMath;
};

