#ifndef PINGCAP_DEFINE_H
#define PINGCAP_DEFINE_H

#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <map>

namespace pingcap
{
  namespace common
  {
    const int PINGCAP_SUCCESS = EXIT_SUCCESS;
    const int PINGCAP_ERROR = EXIT_FAILURE;

    const int64_t PINGCAP_MAX_RUN_TASK = 5;
    const uint64_t PINGCAP_NUMBER_OF_THREADS = 1;
    //ERROR CODE
    const int PINGCAP_INVALID_ARGUMENT = -1;
    const int PINGCAP_NOT_INIT = -2;
    const int PINGCAP_INIT_TWICE = -3;
    const int PINGCAP_ENTRY_NOT_EXIST = -4;
    const int PINGCAP_ALLOCATE_MEMORY_FAILED = -5;
    const int PINGCAP_SIZE_OVERFLOW = -6;
  }
}

#define ATTRIBUTE_ALIGN_SIZE 64
#define ATTRIBUTE_ALIGNED __attribute__((aligned(ATTRIBUTE_ALIGN_SIZE)))

#endif  //PINGCAP_DEFINE_H
