#ifndef PINGCAP_COND_H
#define PINGCAP_COND_H

#include <sys/time.h>
#include "PingCAP_mutex.h"

namespace pingcap
{
  namespace common
  {
    class PingCAPCond : public PingCAPMutex
    {
      public:
        PingCAPCond()
        {
          pthread_cond_init(&cond_, NULL);
        }
    
        ~PingCAPCond()
        {
          pthread_cond_destroy(&cond_);
        }
    
        bool wait(int milliseconds = 0)
        {
          bool ret = true;
          if (milliseconds == 0)
          { // 永久等待
              pthread_cond_wait(&cond_, &mutex_);
          }
          else
          {
            struct timeval curtime;
    
            struct timespec abstime;
            gettimeofday(&curtime, NULL);
    
            int64_t us = (static_cast<int64_t>(curtime.tv_sec) *
                          static_cast<int64_t>(1000000) +
                          static_cast<int64_t>(curtime.tv_usec) +
                          static_cast<int64_t>(milliseconds) *
                          static_cast<int64_t>(1000));
            abstime.tv_sec = static_cast<int>(us / static_cast<int64_t>(1000000));
            abstime.tv_nsec = static_cast<int>(us % static_cast<int64_t>(1000000)) * 1000;
            ret = (pthread_cond_timedwait(&cond_, &mutex_, &abstime) == 0);
          }
          return ret;
        }
    
        void signal()
        {
          pthread_cond_signal(&cond_);
        }
    
        void broadcast()
        {
          pthread_cond_broadcast(&cond_);
        }
    
    private:
        pthread_cond_t cond_;
    };
  }
}

#endif // PINGCAP_COND_H
