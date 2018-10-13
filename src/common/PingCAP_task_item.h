#ifndef PINGCAP_TASK_ITEM_H
#define PINGCAP_TASK_ITEM_H

//#include "PingCAP_define.h"

namespace pingcap
{
  namespace common
  {
      template <typename T>
      class TaskItem
      {
          public:
              enum TaskState
              {
                  Ready = 0,
                  DONING,
                  DNOE
              };
              
          public:
              TaskItem();
              ~TaskItem();
              
          public:
              void set_state(TaskState state);
              TaskState & get_state();
              bool set_task_id();
              
          public:
              uint64_t task_id_;
              TaskState state_;
              //T *volatile data_;
              volatile T data_;
      };
      
      template <typename T>
      TaskItem<T>::TaskItem()
      {
          task_id_ = 0;
          state_ = Ready;
          //data_ = NULL;
      }
      
      template <typename T>
      TaskItem<T>::~TaskItem()
      {
          task_id_ = 0;
          state_ = Ready;
          //data_ = NULL;
      }
      
      template <typename T>
      inline void TaskItem<T>::set_state(TaskState state)
      {
          state_ = state;
      }
      
      template <typename T>
      inline typename TaskItem<T>::TaskState &TaskItem<T>::get_state()
      {
          return state_;
      }
      
      template <typename T>
      bool TaskItem<T>::set_task_id()
      {
          bool ret = true;
          
          register uint64_t old_value = 0;
          register uint64_t new_value = 0;
          while (true)
          {
              old_value = task_id_;
              new_value = old_value;
              new_value++;
              if (old_value == __sync_val_compare_and_swap(&task_id_, old_value, new_value))
              {
                  break;
              }
          }
          return ret;
      }
  }
}

#endif // PINGCAP_TASK_ITEM_H
