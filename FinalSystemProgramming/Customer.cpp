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
	Sleep((DWORD)mTimeToEat);
}