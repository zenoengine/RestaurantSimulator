#include "Restaurant.h"
#include <map>
#include <iostream>
#include <assert.h>

Restaurant::Restaurant()
{
}

Restaurant::~Restaurant()
{
}

void Restaurant::PrepareOpen()
{
	InitializeCriticalSection(&mCriticalSecionDoor);

	srand((unsigned int)time(NULL));

	const int avergaeError = rand() % 10 - 5;
	int todayCustomerCount = AVERAGE_CUSTOMER + avergaeError;

	//make reservation index;
	vector<size_t> reservationTickets;
	const int reservationCutomerCount = 5;
	size_t index = 0;
	while (reservationTickets.size() < reservationCutomerCount)
	{
		size_t index = rand() % todayCustomerCount;
		auto reservationIndex = find(reservationTickets.begin(), reservationTickets.end(), index);
		bool bFoundReservation = reservationIndex != reservationTickets.end();
		if (!bFoundReservation)
		{
			reservationTickets.push_back(index);
		}
	}

	sort(reservationTickets.begin(), reservationTickets.end());

	//create customers and set reservation customers;
	for (int idx = 0; idx < todayCustomerCount; idx++)
	{
		shared_ptr<Customer> customer = make_shared<Customer>(CT_NORMAL);

		if (!reservationTickets.empty())
		{
			size_t ticketIndex = reservationTickets.front();
			if (ticketIndex == idx)
			{
				customer->SetCustomerType(CT_RESERVATION);
				reservationTickets.erase(reservationTickets.begin());
			}
		}


		mWaitingCustomers.push(customer);
	}

	for (size_t idx = 0; idx < MAX_CHAIR_COUNT; idx++)
	{
		Chair chair;
		chair.chairId = idx;
		chair.pRestaurant = this;
		chair.state = CreateMutex(0, FALSE, 0);
		mChairs.push_back(chair);
	}

	for (size_t idx = 0; idx < MAX_CHAIR_COUNT; idx++)
	{
		auto a = mChairs[idx];
		HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ProvideFood, (LPVOID)&mChairs[idx], CREATE_SUSPENDED, 0);
		mThreadHandles.push_back(handle);
	}

}

void Restaurant::Open()
{
	for (size_t idx = 0; idx < mThreadHandles.size(); idx++)
	{
		ResumeThread(mThreadHandles[idx]);
	}

	WaitForMultipleObjects((DWORD)mThreadHandles.size(), &mThreadHandles[0], TRUE, INFINITE);
}

void Restaurant::Close()
{
	for (auto handle : mThreadHandles)
	{
		CloseHandle(handle);
	}

	for (auto chair : mChairs)
	{
		CloseHandle(chair.state);
	}

	while (!mWaitingCustomers.empty())
	{
		mWaitingCustomers.pop();
	}

	mThreadHandles.clear();
	mChairs.clear();

	DeleteCriticalSection(&mCriticalSecionDoor);
}

shared_ptr<Customer> Restaurant::EnterCsutomer()
{
	shared_ptr<Customer> spCustomer = nullptr;

	EnterCriticalSection(&mCriticalSecionDoor);
	if (!mWaitingCustomers.empty())
	{
		spCustomer = mWaitingCustomers.front();
		mWaitingCustomers.pop();
	}
	LeaveCriticalSection(&mCriticalSecionDoor);

	return spCustomer;
}

void Restaurant::WaitForEmptyChair(size_t chairId)
{
	WaitForSingleObject(mChairs[chairId].state, INFINITE);
}

void Restaurant::MakeEmptyChair(size_t chairId)
{
	ReleaseMutex(mChairs[chairId].state);
}

void Restaurant::ProvideFood(LPVOID arg)
{
	Chair* pChair = static_cast<Chair*>(arg);
	Restaurant* pRestaurant = pChair->pRestaurant;
	if (!pChair || !pRestaurant)
	{
		assert(false);
		return;
	}

	while (1)
	{
		size_t chairId = pChair->chairId;
		pRestaurant->WaitForEmptyChair(chairId);

		shared_ptr<Customer> spCustomer = pRestaurant->EnterCsutomer();

		if (spCustomer == nullptr)
		{
			return;
		}

		spCustomer->Eat();
		pRestaurant->MakeEmptyChair(chairId);
	}
}
