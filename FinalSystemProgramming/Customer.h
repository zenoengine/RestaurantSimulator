#pragma once

enum CustomerType
{
	CT_NORMAL = 0,
	CT_RESERVATION
};

class Customer
{
private:
	Customer() = delete;
	Customer(const Customer &) = delete;

	size_t mTimeToEat = 0;
	CustomerType mType = CT_NORMAL;
	
	static size_t customerIdGenerator;
	size_t mCustomerId = 0;

public:
	Customer(CustomerType type);
	~Customer() {};

	inline const size_t GetTimeToEat() const { return mTimeToEat; }
	inline const CustomerType GetCustomerType() const { return mType; }
	inline const void SetCustomerType(CustomerType type) { mType = type; }

	void Eat();
	bool IsFinish();
	size_t GetId();
};
