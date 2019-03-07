#ifndef __THREAD_CPP
#define __THREAD_CPP

#ifdef __WIN32
#include <windows.h>
class THREAD {
	THREAD(unsigned long (*fp)(void*), void* param){
		thread_num = 1;
		create_thread(fp, param);
	}
	THREAD(unsigned long (*fp)(void*), void* param, int n){
		thread_num = n;
		create_thread(fp, param);	
	}
	~THREAD(){
		if(thread){
			for(int i = 0; i < thread_num; i++){
				if(thread[i]){
					CloseHandle(thread[i]);
				}
			}
			delete[] thread;
		}
		if(id){
			delete[] id;
		}
	}
private:
	void create_thread(unsigned long (*fp)(void*), void* param){
		thread = new (void*)[thread_num];
		id = new (unsigned long)[thread_num];
		for(int i = 0; i < thread_num; i++){
			thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fp, &(void**)param[i], 0, &id[i]);
		}	
	}
public:
	void join(){
		WaitForMultipleObjects(thread_num, thread, TRUE, INFINITE);
	}
public:

private:
	unsigned int thread_num;
	unsigned int *id;
	HANDLE *thread;
};
#else

#include <pthread.h>
class THREAD {
public:
	THREAD(void* (*fp)(void*), void* param){
		thread_num = 1;
		create_thread(fp, param);
	}
	THREAD(void* (*fp)(void*), void* param, int n){
		thread_num = n;
		create_thread(fp, param);	
	}
	~THREAD(){
		if(thread){
			delete[] thread;
		}
		if(id){
			delete[] id;
		}
	}
private:
	void create_thread(void* (*fp)(void*), void* param){
		thread = new pthread_t[thread_num];
		id = new unsigned long[thread_num];
		for(int i = 0; i < thread_num; i++){
			//thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)fp, &(void**)param[i], 0, &id[i]);
			thread[i] = pthread_create(&id[i],  NULL, fp, ((void**)param)[i]);
		}	
	}
public:
	int cancle(){
		for(int i = 0; i < thread_num; i++){
			pthread_cancel(thread[i]);
		}

		return 0;
	}
	void join(){
			for(int i = 0; i < thread_num; i++){
				if(thread[i]){
					//CloseHandle(thread[i]);
					pthread_join(thread[i], 0);
				}
			}
		//WaitForMultipleObjects(thread_num, thread, TRUE, INFINITE);
	}
public:

private:
	unsigned int thread_num;
	unsigned long *id;
	pthread_t *thread;
};

#endif

#endif
