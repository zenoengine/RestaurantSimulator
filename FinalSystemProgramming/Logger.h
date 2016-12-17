#pragma once
#include <iostream>
#include <Windows.h>

class Logger
{
public:
	Logger();
	virtual ~Logger();

	void WriteLog(const char* title);

private:
	FILE* mpFile = nullptr;
	CRITICAL_SECTION mCS;
};

