#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "VectorsLibrary.h"

TEST(VectorTests, ElementsAreTest)
{
	std::vector<int> v = generateNumbers(5, 3);
	// 0, 1, 2, 0, 1

	ASSERT_THAT(v, testing::ElementsAre(0, 1, 2, 0, 1));
}

TEST(VectorTests, RangeTest)
{
	using namespace testing;
	std::vector<int> v = generateNumbers(5, 3);

	// AllOff means "and" of all conditions, Ge means "greater or equal", Lt means "less than", Each means "for each element in the collection"
	ASSERT_THAT(v, Each(AllOf(Ge(0), Lt(3))));  
}

