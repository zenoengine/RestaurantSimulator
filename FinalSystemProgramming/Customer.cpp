#include "Customer.h"
#include "Util.h"

Customer::Customer(CustomerType type)
	: mType(type)
	, mTimeToEat(RandomRange())
{
}

