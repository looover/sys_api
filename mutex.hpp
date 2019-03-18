#ifndef _EVENT_
#define _EVENT_

#ifdef __WIN32
#include <mutex>
using namespace sdd;
#else
//#include <pthread.h>
#include <semaphore.h>

class mutex /*: public EVENT*/
{
public:
	mutex(bool bInitialState = true){
	}
	mutex(const char * eventName, bool bInitialState = true){
	}
	~mutex(){
	}
	int try_lock(){
	}

	int lock(){
	}
	bool unlock(){
	}
	//void Close(){
	//	sem_destroy(sem_t *sem);
	//}

private:
	sem_t	sem;
};
#endif

#endif //_EVENT_
