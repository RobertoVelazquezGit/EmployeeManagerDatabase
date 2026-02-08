#include <gtest/gtest.h>
#include <gmock/gmock.h>

// This is a console c++ project with gtest and gmock configured
// Nugget packages used: googletestmock.v.141  
// EmployeeManagerDatabaseLibrary (custom static library project containing EmployeeManager, Employee, IDatabaseConnection)
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
