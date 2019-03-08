#ifndef _EVENT_
#define _EVENT_

#ifdef __WIN32
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
#else
//#include <pthread.h>
#include <semaphore.h>

class EVENT /*: public EVENT*/
{
public:
	EVENT(bool bInitialState = true){
		sem_init(&sem, 0, bInitialState);
	}
	EVENT(const char * eventName, bool bInitialState = true){
		sem_init(&sem, 0, bInitialState);
	}
	~EVENT(){
		sem_destroy(&sem);
	}
	int WaitOne(int milsecTimeout = 0, bool exitContext = false){
		struct timespec timeout;
		timeout.tv_nsec = (milsecTimeout % 1000) * 1000;
		timeout.tv_sec = milsecTimeout / 1000;
		
		return sem_timedwait(&sem, &timeout);
	}

	int Set(){
		return sem_post(&sem);
	}
	bool Reset(){
		while(1);
	}
	//void Close(){
	//	sem_destroy(sem_t *sem);
	//}

private:
	sem_t	sem;
};
#endif

#endif //_EVENT_
