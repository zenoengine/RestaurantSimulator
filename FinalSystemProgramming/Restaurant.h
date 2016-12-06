#pragma once
#include <vector>
#include <queue>
#include "Customer.h"
#include <memory>
#include <time.h>
#include <windows.h>

using namespace std;




class Restaurant
{
	typedef void(*WORK)(void);

	struct Chair
	{
		HANDLE state;
		size_t chairId;
		Restaurant* pRestaurant;
	};

public:
	Restaurant();
	~Restaurant();

	void PrepareOpen();
	void Open();
	void Close();

private:
	shared_ptr<Customer> EnterCsutomer();

	void WaitForEmptyChair(size_t chairId);
	void MakeEmptyChair(size_t chairId);

	const int AVERAGE_CUSTOMER = 200;
	const int MAX_CHAIR_COUNT = 30;

	CRITICAL_SECTION mCriticalSecionDoor;

	queue<shared_ptr<Customer>> mWaitingCustomers;
	vector<Chair> mChairs;
	vector<HANDLE> mThreadHandles;

	static void ProvideFood(LPVOID arg);
	
	void EnterCriticalSecion();
	void LeaveCriticalSecion();
};