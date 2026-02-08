#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Calculator.h"  
#include "IMathUtils.h"

using ::testing::Return;

class MockMathUtils : public IMathUtils
{
public:
    MOCK_METHOD2(add, int(int, int));
};

TEST(CalculatorTest, UsesMathUtilsWrapper)
{
    MockMathUtils mockMath;

    EXPECT_CALL(mockMath, add(2, 3))
        .WillOnce(Return(10));

    Calculator calc(mockMath);

    EXPECT_EQ(calc.compute(), 10);
}
