#include "PingCAP_test_task_queue.h"

using namespace std;
using namespace pingcap::common;

int main()
{
    pthread_t threads_producer[PINGCAP_NUMBER_OF_THREADS];
    pthread_t threads_consumer[PINGCAP_NUMBER_OF_THREADS];
    TaskQueue<int> task_queue;
    int i = 0;
    
    for (i = 0; i < PINGCAP_NUMBER_OF_THREADS; ++i)
    {
        //create producer and consumer thread
        pthread_create(&threads_producer[i], NULL, thread_func_producer<int>, (void *)(&task_queue));
        pthread_create(&threads_consumer[i], NULL, thread_func_consumer<int>, (void *)(&task_queue));
    }
    for (i = 0; i < PINGCAP_NUMBER_OF_THREADS; ++i)
    {
        pthread_join(threads_producer[i], NULL);
        pthread_join(threads_consumer[i], NULL);
    }
    return 0;
}
