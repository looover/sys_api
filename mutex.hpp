#ifndef _EVENT_
#define _EVENT_

#ifdef __WIN32
#include <mutex>
using namespace std;
#else
//#include <pthread.h>
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
		pthread_mutex_trylock(&mux);
	}
	int lock(){
		pthread_mutex_lock(&mux);
	}
	bool unlock(){
		pthread_mutex_unlock(&mux);
	}
private:
	pthread_mutex_t mux;
};
#endif

#endif //_EVENT_
