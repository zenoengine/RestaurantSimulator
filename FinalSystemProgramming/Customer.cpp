#include "Customer.h"
#include "Util.h"
#include <Windows.h>
#include <iostream>

Customer::Customer(CustomerType type)
	: mType(type)
	, mTimeToEat(RandomRange())
{
}

void Customer::Eat()
{
	std::cout << "Entered " << std::endl;
	std::cout << "timeToEat : " << mTimeToEat << std::endl;

	Sleep(mTimeToEat);
}