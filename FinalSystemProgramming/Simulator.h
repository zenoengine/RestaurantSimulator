#pragma once
#include <vector>
#include "Customer.h"
#include <memory>
#include <time.h>

using namespace std;

class Simulator
{
public:
	Simulator();
	~Simulator();

	void Init();

private:
	const int AVERAGE_CUSTOMER = 200;
	
	vector<shared_ptr<Customer>> customers;
};

