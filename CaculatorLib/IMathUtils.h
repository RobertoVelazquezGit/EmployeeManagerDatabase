#pragma once

class IMathUtils
{
public:
    virtual ~IMathUtils() = default;
    virtual int add(int a, int b) = 0;
};

