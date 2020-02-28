#ifndef _BUFFER_H
#define _BUFFER_H

#include <pthread.h>
#include <semaphore.h>

class BUFFER
{
public:
	BUFFER(int size) : MAX_BUFFER_SIZE(size){
		Init();
	}
	~BUFFER(){
		assert(Size == 0);

		if(Data){
			delete[] Data;
			Data = 0;
		}

		pthread_cond_destroy(&NoEmpty);
		pthread_cond_destroy(&NoFull);
		pthread_mutex_destroy(&Lock);
	}
	void Init(){
		Size = 0;
		Data = new unsigned char[MAX_BUFFER_SIZE];

		memset(Data, 0xFF, MAX_BUFFER_SIZE);

		Tail = Data;
		Header = Data;
	
		pthread_mutex_init(&Lock, NULL);
		pthread_cond_init(&NoEmpty, NULL);
		pthread_cond_init(&NoFull, NULL);
	}
	void WriteBuffer(unsigned char * src, int len)
	{
		pthread_mutex_lock(&Lock);
		if (Size >= MAX_BUFFER_SIZE){
			pthread_cond_wait(&NoFull, &Lock);
		}
		assert(Size + len <= MAX_BUFFER_SIZE);//have a bug
		memcpy(Tail, src, len);
		Tail += len;
		if (Tail >= Data + MAX_BUFFER_SIZE){
			Tail = Data;
		}
		Size += len;
		pthread_cond_signal(&NoEmpty);
		pthread_mutex_unlock(&Lock);
	}
	/*
	void *GetNoPending(void)
	{
		void * ret = NULL;
		if(pthread_mutex_trylock(&Lock))
		{
			if (Size){
				ret = Data[Header];
				if (++Header >= MAX_BUFFER_SIZE){
					Header = 0;
				}
				Size--;
				pthread_cond_signal(&NoFull);
			}
			pthread_mutex_unlock(&Lock);
		}

		return ret;
	}
	*/
	int ReadBuffer(unsigned char * dst, int len)
	{
		int ret = 0;
		pthread_mutex_lock(&Lock);
		if (Size <= 0){
			//if(abstime){
			//	pthread_cond_timedwait(&NoEmpty, &Lock, abstime);
			//}
			//else{
				pthread_cond_wait(&NoEmpty, &Lock);
			//}
		}

		assert(Size >= len);
		if(Size >= len){
			memcpy(dst, Header, len);//have a bug
			//memset(dst, 0x00, len);
			Header += len;
			if (Header >= Data + MAX_BUFFER_SIZE){
				Header = Data;
			}
			Size -= len;
			ret = len;
			pthread_cond_signal(&NoFull);
		}
		pthread_mutex_unlock(&Lock);

		return ret;
	}
	int IsEmpty(){
		return Size;
	}
private:
	int Size;
	unsigned char * Header;
	unsigned char * Tail;
	//void* MaxSize;
	unsigned char * Data;

	const int MAX_BUFFER_SIZE;
private:
	pthread_mutex_t		Lock;
	pthread_cond_t		NoEmpty;
	pthread_cond_t		NoFull;
};

#endif //!_BUFFER_

