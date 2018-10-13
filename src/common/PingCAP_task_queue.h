#ifndef PINGCAP_TASK_QUEUE_H
#define PINGCAP_TASK_QUEUE_H

#include "PingCAP_cond.h"
#include "PingCAP_task_item.h"
#include "PingCAP_simple_queue.h"

using namespace std;

namespace pingcap
{
  namespace common
  {
      template <typename T>
      class TaskQueue
      {
          public:
              TaskQueue();
              ~TaskQueue();
          
          public:
              /**
               * the size of queue
               * @return size
               */
              int length();
              
              /**
               * Add a task to the task queue.
               * @param task [in] task need to add 
               * @param maxQueueLen [in] if maxQueueLen > 0, Maximum length of the queue
               * @param block [in] block==true, this thread can wait util queue_.size less than maxQueueLen
               * @return Success or failure
               */
              bool add(TaskItem<T> *task, int maxQueueLen = 0, bool block = true);
              /**
               * get a task from task queue.
               * @param task [out] task get from queue
               * @param wait [in] wait==true, If there are no extractable elements in the queue, this thread can wait util there are extractable elements.
               * @return Success or failure
               */
              bool get(TaskItem<T> *&task, bool wait);
              /**
               * Delete a completed task from the queue.
               * @param task [in] task need to delete
               * @return Success or failure
               */
              bool done(TaskItem<T> *&task);
              /**
               * shutdown this queue
               * @param waitFinish [in] waitFinish==true, this thread need wait other unfinished thread done
               * @return Success or failure
               */
              void shutdown(bool waitFinish = false);
              /**
               * Whether the task queue is closed
               * @return Success or failure
               */
              bool is_closed();
      
          private:
              bool stop_;
              SimpleQueue<T> queue_;
              PingCAPCond cond_;
              PingCAPCond pushcond_;
              bool waitFinish_;
              bool waiting_;
      };
      
      template <typename T>
      TaskQueue<T>::TaskQueue()
      {
          stop_ = false;
          waitFinish_ = false;
          waiting_ = false;
      }
      
      template <typename T>
      TaskQueue<T>::~TaskQueue()
      {
          shutdown();
      }
      
      template <typename T>
      inline int TaskQueue<T>::length()
      {
          return queue_.size();
      }
      
      template <typename T>
      bool TaskQueue<T>::add(TaskItem<T> *task, int maxQueueLen, bool block)
      {
          bool ret = false;
          if (!stop_)
          {
              if (maxQueueLen > 0 && queue_.size() >= maxQueueLen)
              {
                  pushcond_.lock();
                  waiting_ = true;
                  while (false == stop_ && queue_.size() >= maxQueueLen && block)
                  {
                      pushcond_.wait(1000);
                  }
                  waiting_ = false;
                  if ((queue_.size() >= maxQueueLen && !block) || stop_)
                  {
                      pushcond_.unlock();
                      ret = false;
                  }
                  else
                  {
                      pushcond_.unlock();
                      cond_.lock();
                      ret = queue_.push(task);
                      cond_.unlock();
                      cond_.signal();
                  }
              }
              else
              {
                  cond_.lock();
                  ret = queue_.push(task);
                  cond_.unlock();
                  cond_.signal();
              }
          }
          return ret;
      }
      
      template <typename T>
      bool TaskQueue<T>::get(TaskItem<T> *&task, bool wait)
      {
          bool ret = false;
          while (!stop_)
          {
              cond_.lock();
              if (stop_)
              {
                  cond_.unlock();
                  break;
              }
              ret = queue_.get(task);
              cond_.unlock();
              if (waiting_)
              {
                  pushcond_.lock();
                  pushcond_.signal();
                  pushcond_.unlock();
              }
              if (ret || !wait)
              {
                  break;
              }
          }
          if (waitFinish_)
          {
          }
          return ret;
      }
      
      template <typename T>
      bool TaskQueue<T>::done(TaskItem<T> *&task)
      {
          bool ret = false;
          if (TaskItem<T>::TaskState::DNOE == task->get_state())
          {
              while (!stop_)
              {
                  cond_.lock();
                  if (stop_)
                  {
                      cond_.unlock();
                      break;
                  }
                  ret = queue_.pop(task);
                  cond_.unlock();
                  if (waiting_)
                  {
                      pushcond_.lock();
                      pushcond_.signal();
                      pushcond_.unlock();
                  }
                  break;
              }
              if (waitFinish_)
              {
              }
          }
          return ret;
      }
      
      template <typename T>
      void TaskQueue<T>::shutdown(bool waitFinish)
      {
          cond_.lock();
          stop_ = true;
          waitFinish_ = waitFinish;
          cond_.broadcast();
          cond_.unlock();
      }
      
      template <typename T>
      bool TaskQueue<T>::is_closed()
      {
          return stop_;
      }
  }
}

#endif  //PINGCAP_TASK_QUEUE_H
