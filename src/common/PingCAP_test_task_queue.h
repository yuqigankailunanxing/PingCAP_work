
#ifndef PINGCAP_TEST_TASK_QUEUE_H
#define PINGCAP_TEST_TASK_QUEUE_H

#include "PingCAP_task_queue.h"

using namespace std;
using namespace pingcap::common;

/**
 * The entry of the producer thread's execution function.
 * @param arg [in] task_queue
 * @return NULL
 */
template <typename T>
void * thread_func_producer(void *arg)
{
  TaskQueue<T> *const task_queue = (TaskQueue<T> *)arg;
  if (NULL == task_queue)
  {
      
  }
  else
  {
      int i = 0;
      while (!task_queue->is_closed())
      {
          cout << "BEFORE!!! producer = " << pthread_self() << ", i = " << i << endl;
          TaskItem<T> *task = NULL;
          task = new TaskItem<T>();
          task->set_state(TaskItem<T>::TaskState::Ready);
          task->set_task_id();
          task_queue->add(task, -1, false);
          delete task;
          cout << "AFTER!!!  producer = " << pthread_self() << ", i = " << i << endl;
          ++i;
      }
  }
  return NULL;
}

/**
 * The entry of the consumer thread's execution function.
 * @param arg [in] task_queue
 * @return NULL
 */
template <typename T>
void * thread_func_consumer(void *arg)
{
  TaskQueue<T> *const task_queue = (TaskQueue<T> *)arg;
  if (NULL == task_queue)
  {
      
  }
  else
  {
      int i = 0;
      while (!task_queue->is_closed())
      {
          cout << "BEFORE!!! consumer = " << pthread_self() << ", i = " << i << endl;
          TaskItem<T> *task = NULL;
          task_queue->get(task, false);
          if (NULL != task)
          {
              task->set_state(TaskItem<T>::TaskState::DNOE);
              task_queue->done(task);
          }
          cout << "AFTER!!!  consumer = " << pthread_self() << ", i = " << i << endl;
          ++i;
      }
  }
  return NULL;
}

#endif // PINGCAP_TEST_TASK_QUEUE_H
