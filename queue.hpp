#ifndef _QUEUE_
#define _QUEUE_

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


#endif //!_QUEUE_