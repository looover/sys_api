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

//class EVENT
//{
//public:
//	EVENT(){
//		m_hWaitHandle = 0;
//	}
//	virtual ~EVENT(){
//
//	}
//	void * m_hWaitHandle;
//	virtual bool WaitOne(int milsecTimeout = INFINITE, bool exitContext = false){
//		if (m_hWaitHandle == 0)
//			return false;
//
//		return WaitForSingleObject(m_hWaitHandle, milsecTimeout) == WAIT_OBJECT_0;
//	}
//	int WaitMulti(EVENT *HandlePtr, int nCount, int fWaitAll = 0, int milsecTimeout = 0){
//		void * pEventArray[MAX_EVENT_NUM];
//		for (int i = 0; i< nCount; i++)
//			pEventArray[i] = HandlePtr[i].m_hWaitHandle;
//
//		DWORD result = WaitForMultipleObjects(
//			nCount,
//			pEventArray,
//			fWaitAll,
//			milsecTimeout
//			);
//		if (result == WAIT_FAILED || result == WAIT_TIMEOUT)
//			return false;
//		else
//			return (result - WAIT_OBJECT_0) + 1;
//	}
//};

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
