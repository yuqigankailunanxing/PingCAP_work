#ifndef PINGCAP_SIMPLE_QUEUE_H
#define PINGCAP_SIMPLE_QUEUE_H

#include "PingCAP_define.h"
#include "PingCAP_task_item.h"

using namespace std;

namespace pingcap
{
  namespace common
  {
      template <typename T>
      class SimpleQueue
      {
          public:
              SimpleQueue();
              ~SimpleQueue();
              
          public:
              /**
               * the size of task_map_
               * @return size
               */
              int size() const;
              
              /**
               * Clear the contents of the task_map_ and the pointer that point to in the task_map_
               */
              void clear();
              /**
               * push a task to the task_map_.
               * @param task [in] task need to push 
               * @return Success or failure
               */
              bool push(TaskItem<T> *&task);
              /**
               * get a task from task_map_.
               * @param task [out] task get from task_map_
               * @return Success or failure
               */
              bool get(TaskItem<T> *&task);
              /**
               * pop a completed task from the task_map_.
               * @param task [in] task need to pop
               * @return Success or failure
               */
              bool pop(TaskItem<T> *&task);
              
          private:
              map<uint64_t, TaskItem<T> *> task_map_;
      };
      
      template <typename T>
      SimpleQueue<T>::SimpleQueue()
      {
      }
      
      template <typename T>
      SimpleQueue<T>::~SimpleQueue()
      {
          clear();
      }
      
      template <typename T>
      inline int SimpleQueue<T>::size() const
      {
        return task_map_.size();
      }
      
      template <typename T>
      void SimpleQueue<T>::clear()
      {
          typename map<uint64_t, TaskItem<T> *>::iterator iter;
          for (iter = task_map_.begin(); iter != task_map_.end(); ++iter)
          {
              free(iter->second);
          }
          task_map_.clear();
      }
      
      template <typename T>
      bool SimpleQueue<T>::push(TaskItem<T> *&task)
      {
          bool ret = true;
          if (NULL != task)
          {
              void * alloc_ptr = NULL;
              if (NULL == (alloc_ptr = malloc(sizeof(TaskItem<T>))))
              {
                  ret = false;
              }
              else
              {
                  TaskItem<T> * ret_ptr = NULL;
                  memcpy(alloc_ptr, (const void *)task, sizeof(TaskItem<T>));
                  ret_ptr = (TaskItem<T> *)alloc_ptr;
                  task_map_.insert(pair<uint64_t, TaskItem<T> *>(task->task_id_, ret_ptr));
              }
          }
          return ret;
      }
      
      template <typename T>
      bool SimpleQueue<T>::get(TaskItem<T> *&task)
      {
          bool ret = true;
          typename map<uint64_t, TaskItem<T> *>::iterator iter;
          for (iter = task_map_.begin(); iter != task_map_.end(); ++iter)
          {
              if (TaskItem<T>::TaskState::Ready == iter->second->get_state())
              {
                  iter->second->set_state(TaskItem<T>::TaskState::DONING);
                  break;
              }
          }
          if (iter == task_map_.end())
          {
              task = NULL;
              ret = false;
          }
          else
          {
              task = iter->second;
              ret = true;
          }
          return ret;
      }
      
      template <typename T>
      bool SimpleQueue<T>::pop(TaskItem<T> *&task)
      {
          bool ret = true;
          if (NULL != task)
          {
              typename map<uint64_t, TaskItem<T> *>::iterator iter;
              if (task_map_.end() == (iter = task_map_.find(task->task_id_)))
              {
                  ret = false;
              }
              else if (TaskItem<T>::TaskState::DNOE != task->state_)
              {
                  task->set_state(TaskItem<T>::TaskState::Ready);
                  //ret = false;
              }
              else
              {
                  task_map_.erase(iter);
              }
          }
          return ret;
      }
}

#endif  //PINGCAP_SIMPLE_QUEUE_H
