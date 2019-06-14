#ifndef _EVENT_
#define _EVENT_

#ifdef _WIN32
#include <mutex>
using namespace std;
#else

#include <pthread.h>

class mutex /*: public EVENT*/
{
public:
	mutex(bool bInitialState = true){
		pthread_mutex_init(&mux, NULL);
	}
	~mutex(){
		pthread_mutex_destroy(&mux);
	}
	int try_lock(){
		return pthread_mutex_trylock(&mux);
	}
	int lock(){
		return pthread_mutex_lock(&mux);
	}
	bool unlock(){
		return pthread_mutex_unlock(&mux);
	}
private:
	pthread_mutex_t mux;
};
#endif

#endif //_EVENT_
