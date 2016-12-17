#include "Logger.h"

Logger::Logger()
{
	mpFile = fopen("Log.txt", "w");
	InitializeCriticalSection(&mCS);
}

Logger::~Logger()
{
	fclose(mpFile);
	DeleteCriticalSection(&mCS);
}

void Logger::WriteLog(const char * text)
{
	EnterCriticalSection(&mCS);
	fprintf(mpFile, text);
	fprintf(mpFile, "\n");
	LeaveCriticalSection(&mCS);
}
