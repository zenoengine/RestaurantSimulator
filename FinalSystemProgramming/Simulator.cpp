#include "Simulator.h"
#include <map>
#include <iostream>

Simulator::Simulator()
{
}

Simulator::~Simulator()
{
}

void Simulator::Init()
{
	srand((size_t)time(NULL));
	
	const int avergaeError = rand() % 10 - 5;
	int todayCustomerCount = AVERAGE_CUSTOMER + avergaeError;

	for (int idx = 0; idx < todayCustomerCount; idx++)
	{
		customers.push_back(make_shared<Customer>(CT_NORMAL));
	}

	vector<size_t> reservationIndices;
	const int reservationCutomerCount = 5;
	size_t index = 0;
	while(reservationIndices.size() < reservationCutomerCount)
	{
		size_t index = rand()%todayCustomerCount;
		auto reservationIndex = find(reservationIndices.begin(), reservationIndices.end(), index);
		bool bFoundReservation = reservationIndex != reservationIndices.end();
		if (!bFoundReservation)
		{
			reservationIndices.push_back(index);
			customers[index]->SetCustomerType(CT_RESERVATION);
		}
	}

	for (auto customer : customers)
	{
		cout << "Type : "<< customer->GetCustomerType() << endl;
		cout << "TimeToEat : "<< customer->GetTimeToEat() << endl;
		cout << " " << endl;
	}
}
