#ifndef PINGCAP_MUTEX_H
#define PINGCAP_MUTEX_H

#include <pthread.h>
#include <assert.h>
#include "PingCAP_define.h"

namespace pingcap
{
  namespace common
  {
    class PingCAPMutex
    {
      public:
        PingCAPMutex()
        {
          const int iRet = pthread_mutex_init(&mutex_, NULL);
          (void) iRet;
          assert( iRet == 0 );
        }
    
        ~PingCAPMutex()
        {
          pthread_mutex_destroy(&mutex_);
        }
        
        void lock ()
        {
          pthread_mutex_lock(&mutex_);
        }
    
        int trylock ()
        {
          return pthread_mutex_trylock(&mutex_);
        }
    
        void unlock()
        {
          pthread_mutex_unlock(&mutex_);
        }
    
    protected:
        pthread_mutex_t mutex_;
    };
  }
}

#endif // PINGCAP_MUTEX_H
