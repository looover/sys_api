#ifndef _SEMAPHORE_HPP
#define _SEMAPHORE_HPP

#ifdef _WIN32
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

class Semaphore /*: public Semaphore*/
{
public:
	Semaphore(bool bInitialState = true){
		LPSECURITY_ATTRIBUTES lpEventAttributes = 0;
		BOOL bManualReset = false;
		LPCTSTR lpName = 0;

		m_SemaphoreID = CreateEvent(
			lpEventAttributes,
			bManualReset,
			bInitialState,
			lpName);
	}
	Semaphore(const char * eventName, bool bInitialState = true){
		LPSECURITY_ATTRIBUTES lpEventAttributes = 0;
		BOOL bManualReset = true;
		LPCTSTR lpName = 0;

		m_SemaphoreID = CreateEvent(
			lpEventAttributes,
			bManualReset,
			bInitialState,
			eventName);
	}
	~Semaphore(){
		if (m_SemaphoreID){
			CloseHandle(m_SemaphoreID);
			m_SemaphoreID = 0;
		}
	}
	bool Wait(int milsecTimeout = INFINITE, bool exitContext = false){
		return (WaitForSingleObject(m_SemaphoreID, milsecTimeout) == WAIT_OBJECT_0);
	}

	bool Post(){
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

class Semaphore /*: public Semaphore*/
{
public:
	Semaphore(){
		sem_init(&sem, 0, 0);
	}
	~Semaphore(){
		sem_destroy(&sem);
	}
	int Wait(int elapsed = 0){
		int ret = -1;
		if(elapsed == 0){
			sem_wait(&sem);
		}else{
			struct timespec timeout;
			timeout.tv_nsec = (elapsed % 1000) * 1000;
			timeout.tv_sec = elapsed / 1000;

			ret = sem_timedwait(&sem, &timeout);
		}

		return ret;
	}

	int Post(){
		return sem_post(&sem);
	}
	int Reset(){
		return 0;
	}

private:
	sem_t	sem;
};
#endif

#endif //_Semaphore_
