#ifndef _QUEUE_
#define _QUEUE_

#ifdef __WIN32
//#include <queue>
#include <mutex>
#include <assert.h>
#define	 MAX_MSG_NUMBER		2

using namespace std;

#include "event.hpp"

class Semaphore{
private:
	std::mutex sema;
public:
	void Set(){
		sema.unlock();
	}
	void Wait(){
		sema.lock();
	}
	void Relase(){
		sema.unlock();
	}
};

class QUEUE
{
public:
	QUEUE(){
		Cnt = 0;
		Start = 0;
		End = 0;
	}
	~QUEUE(){
		assert(Cnt == 0);
	}
	void PutIntoQueue(void*msg)
	{
retry:
		Mux.lock();
		if (Cnt < MAX_MSG_NUMBER)
		{
			Msg[End++] = msg;
			if (End >= MAX_MSG_NUMBER){
				End = 0;
			}
			if (Cnt == 0){
				DataLeft.Set();
			}
			if (++Cnt == MAX_MSG_NUMBER){
				SpaceLeft.Reset();
			}
			Mux.unlock();
		}
		else{
			Mux.unlock();
			SpaceLeft.WaitOne();	//wait not full
			goto retry;
		}
	}
	void *GetNoPending(void)
	{
		void * ret = NULL;
		if (Mux.try_lock())
		{
			if (Cnt){
				ret = Msg[Start];
				if (++Start >= MAX_MSG_NUMBER){
					Start = 0;
				}
				if (Cnt == MAX_MSG_NUMBER){
					SpaceLeft.Set();
				}
				if (--Cnt == 0){
					DataLeft.Reset();
				}
			}
			Mux.unlock();
		}

		return ret;
	}
	void *GetFromQueue(void)
	{
		void * ret = NULL;
retry:
		Mux.lock();
		if (Cnt)
		{
			ret = Msg[Start];
			if (++Start >= MAX_MSG_NUMBER){
				Start = 0;
			}
			if (Cnt == MAX_MSG_NUMBER){
				SpaceLeft.Set();
			}
			if (--Cnt == 0){
				DataLeft.Reset();
			}
			Mux.unlock();
		}
		else{
			Mux.unlock();
			DataLeft.WaitOne();
			goto retry;
		}

		return ret;
	}
	int IsEmpty(){
		int ret = 0;
		Mux.lock();
		ret = Cnt;
		Mux.unlock();
		return ret == 0;
	}
private:
	std::mutex Mux;
	int Cnt;
	int Start;
	int End;
	//void* MaxCnt;
	void * Msg[MAX_MSG_NUMBER];

private:
	EVENT DataLeft;
	EVENT SpaceLeft;
};

#else

#include <pthread.h>
#include <semaphore.h>
#define	 MAX_MSG_NUMBER		2

class QUEUE
{
public:
	QUEUE(){
		Cnt = 0;
		Start = 0;
		End = 0;

		pthread_mutex_init(&Lock, NULL);
		pthread_cond_init(&NoEmpty, NULL);
		pthread_cond_init(&NoFull, NULL);
	}
	~QUEUE(){
		assert(Cnt == 0);
		pthread_cond_destroy(&NoEmpty);
		pthread_cond_destroy(&NoFull);
		pthread_mutex_destroy(&Lock);
	}
	void PutIntoQueue(void*msg)
	{
		pthread_mutex_lock(&Lock);
		if (Cnt >= MAX_MSG_NUMBER){
			pthread_cond_wait(&NoFull, &Lock);
		}
		assert(Cnt < MAX_MSG_NUMBER);
		Msg[End++] = msg;
		if (End >= MAX_MSG_NUMBER){
			End = 0;
		}
		Cnt++;
		pthread_cond_signal(&NoEmpty);
		pthread_mutex_unlock(&Lock);
	}
	void *GetNoPending(void)
	{
		void * ret = NULL;
		if(pthread_mutex_trylock(&Lock))
		{
			if (Cnt){
				ret = Msg[Start];
				if (++Start >= MAX_MSG_NUMBER){
					Start = 0;
				}
				Cnt--;
				pthread_cond_signal(&NoFull);
			}
			pthread_mutex_unlock(&Lock);
		}

		return ret;
	}
	void *GetFromQueue(const struct timespec *abstime = NULL)
	{
		void * ret = NULL;
		pthread_mutex_lock(&Lock);
		if (Cnt <= 0){
			if(abstime){
				pthread_cond_timedwait(&NoEmpty, &Lock, abstime);
			}
			else{
				pthread_cond_wait(&NoEmpty, &Lock);
			}
		}
		if(Cnt > 0){
			ret = Msg[Start];
			if (++Start >= MAX_MSG_NUMBER){
				Start = 0;
			}
			Cnt--;
			pthread_cond_signal(&NoFull);
		}
		pthread_mutex_unlock(&Lock);

		return ret;
	}
	int IsEmpty(){
		return Cnt;
	}
private:
	int Cnt;
	int Start;
	int End;
	//void* MaxCnt;
	void * Msg[MAX_MSG_NUMBER];

private:
	pthread_mutex_t		Lock;
	pthread_cond_t		NoEmpty;
	pthread_cond_t		NoFull;
};


#endif

#endif //!_QUEUE_
