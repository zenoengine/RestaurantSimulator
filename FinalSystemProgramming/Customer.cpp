#include "Customer.h"
#include "Util.h"
#include <Windows.h>
#include <iostream>

size_t Customer::customerIdGenerator = 0;

Customer::Customer(CustomerType type)
	: mType(type)
	, mTimeToEat(RandomRange(10,50))
{
	mCustomerId = customerIdGenerator;
	customerIdGenerator++;
}

void Customer::Eat()
{
	mTimeToEat--;
}

bool Customer::IsFinish()
{
	return mTimeToEat <= 0;
}

size_t Customer::GetId()
{
	return mCustomerId;
}
