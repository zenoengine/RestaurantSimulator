#include "Restaurant.h"
#include <map>
#include <iostream>
#include <assert.h>
#include <string>

#include "Util.h"

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
		chair.bCanUseState = true;
		mChairs.push_back(chair);
	}

	for (size_t idx = 0; idx < MAX_STAFF_COUNT; idx++)
	{
		HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StaffWorkProcess, this, CREATE_SUSPENDED, 0);
		mThreadHandles.push_back(handle);
	}

	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = 0;

}

void Restaurant::Open()
{
	for (size_t idx = 0; idx < mThreadHandles.size(); idx++)
	{
		ResumeThread(mThreadHandles[idx]);
	}

	while (true)
	{
		if (mWaitingCustomers.size() == 0 && mEattingCustomers.size() == 0)
		{
			break;
		}

		Sleep(1);

		list<pair<shared_ptr<Customer>, int>> finishedList;
		for (auto customerInfo : mEattingCustomers)
		{
			auto customer = customerInfo.first;

			if (customer->IsFinish())
			{

				finishedList.push_back(customerInfo);
				continue;
			}

			customer->Eat();
		}

		for (auto customerInfo : finishedList)
		{
			size_t charId = customerInfo.second;
			MakeEmptyChair(charId);
			mEattingCustomers.remove(customerInfo);

			string leaveLog = "Leave ";
			leaveLog += "customerId :" + to_string(customerInfo.first->GetId());
			WriteLog(leaveLog.c_str());
		}

		if (mClock % 30 == 0)
		{
			size_t count = CheckEmptyChairCount();
			string checkEmptyChairLog = "empty Char Count : " + to_string(count);
			WriteLog(checkEmptyChairLog.c_str());
		}

		mClock++;
	}

	WaitForMultipleObjects((DWORD)mThreadHandles.size(), &mThreadHandles[0], TRUE, INFINITE);
}

void Restaurant::Close()
{

	for (auto handle : mThreadHandles)
	{
		CloseHandle(handle);
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

	if (!mWaitingCustomers.empty())
	{
		spCustomer = mWaitingCustomers.front();
		mWaitingCustomers.pop();
		string enterLog = "Enter Customer : ";
		enterLog += to_string(spCustomer->GetId());
		WriteLog(enterLog.c_str());
	}

	return spCustomer;
}

Restaurant::Chair* Restaurant::FindEmptyChair()
{
	Chair* pChair = nullptr;

	for (size_t index = 0; index < mChairs.size(); index++)
	{
		if (mChairs[index].bCanUseState)
		{
			pChair = &mChairs[index];
			break;
		}
	}

	return pChair;
}

void Restaurant::SitToChair(shared_ptr<Customer> spCustomer, size_t chairId)
{
	string startEatLog = "Start Eat And Customer Id :" + to_string(spCustomer->GetId());
	WriteLog(startEatLog.c_str());
 	mChairs[chairId].bCanUseState = false;
	mEattingCustomers.push_back(make_pair(spCustomer, chairId));
}

void Restaurant::MakeEmptyChair(size_t chairId)
{
	mChairs[chairId].bCanUseState = true;
}

void Restaurant::StaffWorkProcess(LPVOID arg)
{
	Restaurant* pRestaurant = static_cast<Restaurant*>(arg);
	if (!pRestaurant)
	{
		assert(false);
		return;
	}

	Chair* pChair = nullptr;
	while (1)
	{
		pRestaurant->Lock();
		Chair* pChair = pRestaurant->FindEmptyChair();

		if (pChair == nullptr)
		{
			Sleep(1);
			pRestaurant->Unlock();
			continue;
		}

		Sleep(RandomRange(1, 10));
		shared_ptr<Customer> spCustomer = pRestaurant->EnterCsutomer();
		if (spCustomer == nullptr)
		{
			pRestaurant->Unlock();
			return;
		}

		if (spCustomer->GetCustomerType() == CT_RESERVATION)
		{
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		}

		pRestaurant->SitToChair(spCustomer, pChair->chairId);

		if (spCustomer->GetCustomerType() == CT_RESERVATION)
		{
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
		}
		pRestaurant->Unlock();
	}
}

size_t Restaurant::CheckEmptyChairCount()
{
	size_t emptyChairCount = 0;
	for (auto chair : mChairs)
	{
		if (chair.bCanUseState)
		{
			emptyChairCount++;
		}
	}
	return emptyChairCount;
}

void Restaurant::WriteLog(const char* szTitle)
{
	int hour = mClock / 60 % 24;
	int min = mClock % 60;
	string logText;
	logText.append("[");
	logText.append(szTitle);
	logText.append("]");
	logText.append(" TimeStamp (");
	logText.append(to_string(hour));
	logText.append("h :");
	logText.append(to_string(min));
	logText.append("m)");
	mLogger.WriteLog(logText.c_str());
}

void Restaurant::Lock()
{
	EnterCriticalSection(&mCriticalSecionDoor);
}

void Restaurant::Unlock()
{
	LeaveCriticalSection(&mCriticalSecionDoor);
}