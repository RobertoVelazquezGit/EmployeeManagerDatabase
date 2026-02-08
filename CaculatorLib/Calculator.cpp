#include "pch.h"
#include "Calculator.h"

Calculator::Calculator(IMathUtils& math)
    : mMath(math)
{
}

int Calculator::compute()
{
    return mMath.add(2, 3);
}
