#ifndef SCOPEDLOCKER_HPP_
#define SCOPEDLOCKER_HPP_

#include <pthread.h>

class ScopedLocker
{
public:
  ScopedLocker(pthread_mutex_t* mutex) : mutexref(mutex){
    //lock mutex
    pthread_mutex_lock(mutexref);
  }
  ~ScopedLocker(){
    pthread_mutex_unlock(mutexref);
  }
private:
  pthread_mutex_t* mutexref;
};
#endif
