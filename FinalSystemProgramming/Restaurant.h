#pragma once
#include <vector>
#include <queue>
#include <list>
#include <memory>
#include <time.h>
#include <windows.h>
#include <utility>

#include "Customer.h"
#include "Logger.h"

using namespace std;

class Restaurant
{
	typedef void(*WORK)(void);

	struct Chair
	{
		bool bCanUseState;
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

	Chair* FindEmptyChair();
	
	void SitToChair(shared_ptr<Customer> spCustomer, size_t chairId);
	void MakeEmptyChair(size_t chairId);

	const int AVERAGE_CUSTOMER = 200;
	const int MAX_CHAIR_COUNT = 30;
	const int MAX_STAFF_COUNT = 30;

	CRITICAL_SECTION mCriticalSecionDoor;

	queue<shared_ptr<Customer>> mWaitingCustomers;
	list<pair<shared_ptr<Customer>, int>> mEattingCustomers;
	
	vector<Chair> mChairs;
	vector<HANDLE> mThreadHandles;
	
	static void StaffWorkProcess(LPVOID arg);

	void Lock();
	void Unlock();

	size_t CheckEmptyChairCount();

	Logger mLogger;

	void WriteLog(const char* szTitle = "");

	size_t mClock;
};