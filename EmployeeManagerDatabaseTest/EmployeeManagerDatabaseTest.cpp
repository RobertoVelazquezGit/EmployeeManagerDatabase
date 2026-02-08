#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <map>

#include "IDatabaseConnection.hpp"
#include "Employee.hpp"
#include "EmployeeManager.hpp"


class MockDatabaseConnection : public IDatabaseConnection
{
public:
	
    MockDatabaseConnection(std::string serverAddress);

    //MOCK_METHODn n=0,10
    MOCK_METHOD0(connect, void());  // MOCKMETHOD(void, connect, ())
    MOCK_METHOD0(disconnect, void()); // MOCKMETHOD(void, disconnect, ())

    MOCK_CONST_METHOD1(getSalary, float(int));  // MOCKMETHOD(float, getSalary, (int), (const) )      
    MOCK_METHOD2(updateSalary, void(int, float)); // MOCKMETHOD(void, updateSalary, (int, float) )   

    MOCK_CONST_METHOD1(getSalariesRange, std::vector<Employee>(float));  // MOCKMETHOD(std::vector<Employee>, getSalariesRange, (float), (const) )
    MOCK_CONST_METHOD2(getSalariesRange, std::vector<Employee>(float, float));  // MOCKMETHOD(std::vector<Employee>, getSalariesRange, (float, float), (const) )

    // Sintax if comma , MOCK_METHOD((std::map<std::string, float>), something, ()) 

    void someMemberMethod(std::string param)
    {
        std::cout << "Member method called with param = " << param << "\n";
        throw std::runtime_error("Dummy error!");
    }
};

MockDatabaseConnection::MockDatabaseConnection(std::string serverAddress) : IDatabaseConnection(serverAddress)
{

}

TEST(TestEmployeeManager, TestConnection)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());

    EmployeeManager employeeManager(&dbConnection);
}

TEST(TestEmployeeManager, TestConnectionError)
{
    MockDatabaseConnection dbConnection("DummyAddress");
    // WillOnce is behavior specification   
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::Throw(std::runtime_error("Dummy error")));
    // commented out never called EXPECT_CALL(dbConnection, disconnect());

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

ACTION(myThrow)
{
    std::cout << "In myThrow action" << std::endl;
    throw std::runtime_error("My Dummy errorr");
}

TEST(TestEmployeeManager, TestConnectionErrorAction)
{
    MockDatabaseConnection dbConnection("DummyAddress");
    EXPECT_CALL(dbConnection, connect()).WillOnce(myThrow());

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

void someFreeFunction()
{
    std::cout << "In someFreeFunction" << std::endl;
    throw std::runtime_error("Free function Dummy error");
}

TEST(TestEmployeeManager, TestConnectionErrorInvokeFunction)
{
    MockDatabaseConnection dbConnection("DummyAddress");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::Invoke(someFreeFunction));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

TEST(TestEmployeeManager, TestConnectionErrorInvokeLambda)
{
    MockDatabaseConnection dbConnection("DummyAddress");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::Invoke(
        []() {
            std::cout << "In lambda function" << std::endl;
            throw std::runtime_error("Lambda Dummy error");
        }));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}

TEST(TestEmployeeManager, TestConnectionErrorInvokeMethod)
{
    MockDatabaseConnection dbConnection("DummyAddress");
    auto boundMethod = std::bind(&MockDatabaseConnection::someMemberMethod, &dbConnection, "Some param");
    EXPECT_CALL(dbConnection, connect()).WillOnce(testing::InvokeWithoutArgs(boundMethod));

    ASSERT_THROW(EmployeeManager employeeManager(&dbConnection), std::runtime_error);
}


TEST(TestEmployeeManager, TestUpdateSalary)
{
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());
    EXPECT_CALL(dbConnection, updateSalary(/*50*/testing::_, /*6000*/testing::_)).Times(1);  // testing::_ means any value , is a wildcard or matcher

    EmployeeManager employeeManager(&dbConnection);

    employeeManager.setSalary(50, 6000);
}

TEST(TestEmployeeManager, TestGetSalary)
{
    const int employeeId = 50;
    const float salary = 6100.0;
    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());
    EXPECT_CALL(dbConnection, getSalary(testing::_)).Times(1).WillOnce(testing::Return(salary));

    EmployeeManager employeeManager(&dbConnection);

    float returnedSalary = employeeManager.getSalary(employeeId);

    ASSERT_EQ(salary, returnedSalary);
}

TEST(TestEmployeeManager, TestGetSalaryInRange)
{
    const int low = 5000, high = 8000;
    std::vector<Employee> returnedVector{ Employee{1, 5600, "John"},
                                    Employee{2, 7000, "Jane"},
                                    Employee{3, 6600, "Alex"} };

    MockDatabaseConnection dbConnection("dummyConnection");
    EXPECT_CALL(dbConnection, connect());
    EXPECT_CALL(dbConnection, disconnect());
    EXPECT_CALL(dbConnection, getSalariesRange(low, high)).WillOnce(testing::Return(returnedVector));

    EmployeeManager employeeManager(&dbConnection);

    std::map<std::string, float> returnedMap = employeeManager.getSalariesBetween(low, high);

    for (auto it = returnedMap.begin(); it != returnedMap.end(); ++it)
    {
        std::cout << it->first << " " << it->second << '\n';
        ASSERT_THAT(it->second, testing::AnyOf(testing::Gt(low), testing::Lt(high - 3000)));  // pass
        //ASSERT_THAT(it->second, testing::Gt(low + 1000));  // not pass
		//ASSERT_THAT(it->second, testing::AllOf(testing::Gt(low), testing::Lt(high - 1000)));  // not pass
    }
}

void realCallback(int n, int m) {
	std::cout << "In realCallback with values: " << n << ", " << m << "\n"; 
}

// Commented out #define ENABLE_ORIGINAL_EXAMPLE_CALLBACKTEST 1   
#ifdef ENABLE_ORIGINAL_EXAMPLE_CALLBACKTEST
TEST(TestEmployeeManager, CallbackTest)
{
    MockDatabaseConnection dbConnection("dummyConnection");

    testing::MockFunction<void(int, int)> mockFunction;
    dbConnection.setOnConnect(mockFunction.AsStdFunction());
    EXPECT_CALL(mockFunction, Call(testing::_, testing::_));

	// Fails because the callback is not called in the constructor of EmployeeManager, but we can call it manually to test the callback mechanism   
    dbConnection.connect();
}
#else
TEST(TestEmployeeManager, CallbackTest)
{
    MockDatabaseConnection dbConnection("dummyConnection");

    testing::MockFunction<void(int, int)> mockFunction;
    dbConnection.setOnConnect(mockFunction.AsStdFunction());
    EXPECT_CALL(mockFunction, Call(testing::_, testing::_));
    EXPECT_CALL(dbConnection, connect())
        .WillOnce(testing::InvokeWithoutArgs([&dbConnection]() {
        dbConnection.IDatabaseConnection::connect();
            }));

    dbConnection.connect();
}
#endif

