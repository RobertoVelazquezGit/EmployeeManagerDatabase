#include "pch.h"
#include "framework.h"


#include "IDatabaseConnection.hpp"
#include <iostream>

IDatabaseConnection::IDatabaseConnection(std::string serverAddress) : mServerAddress{serverAddress}
{
}


void IDatabaseConnection::connect()
{
  std::cout << "Connecting to database server " << mServerAddress << "\n";
  //initialize real DB connection
  if(mOnConnect)
  {
	  std::cout << "Invoking onConnect callback\n";	
	  mOnConnect(5, 6); //dummy values
  }
}

void IDatabaseConnection::disconnect()
{
 std::cout << "Disconnecting from database\n";
 //close the real connection
}

void IDatabaseConnection::setOnConnect(Callback onConnect)
{
	mOnConnect = onConnect;
}	