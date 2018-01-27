#ifndef _EVENT_
#define _EVENT_

#include <windows.h>

typedef void * MPEventID;
typedef void * MPSemaphoreID;
typedef void * MPCriticalRegionID;
typedef unsigned long OSStatus;
typedef unsigned long MPTaskID;

#ifndef INFINITE
#define INFINITE            0xFFFFFFFF  // Infinite timeout
#endif

#define MAX_EVENT_NUM 32
#define MAX_W2A_BUFFER_LEN 1024

class EVENT /*: public EVENT*/
{
public:
	EVENT(bool bInitialState = true){
		LPSECURITY_ATTRIBUTES lpEventAttributes = 0;
		BOOL bManualReset = false;
		LPCTSTR lpName = 0;

		m_SemaphoreID = CreateEvent(
			lpEventAttributes,
			bManualReset,
			bInitialState,
			lpName);
	}
	EVENT(const char * eventName, bool bInitialState = true){
		LPSECURITY_ATTRIBUTES lpEventAttributes = 0;
		BOOL bManualReset = true;
		LPCTSTR lpName = 0;

		m_SemaphoreID = CreateEvent(
			lpEventAttributes,
			bManualReset,
			bInitialState,
			eventName);
	}
	~EVENT(){
		if (m_SemaphoreID){
			CloseHandle(m_SemaphoreID);
			m_SemaphoreID = 0;
		}
	}
	bool WaitOne(int milsecTimeout = INFINITE, bool exitContext = false){
		return (WaitForSingleObject(m_SemaphoreID, milsecTimeout) == WAIT_OBJECT_0);
	}

	bool Set(){
		return (SetEvent(m_SemaphoreID) == TRUE);
	}
	bool Reset(){
		return (ResetEvent(m_SemaphoreID) == TRUE);
	}
	//void Close(){
	//	CloseHandle(m_SemaphoreID);
	//	m_SemaphoreID = 0;
	//}

private:
	MPSemaphoreID	m_SemaphoreID;
};

#endif //_EVENT_
