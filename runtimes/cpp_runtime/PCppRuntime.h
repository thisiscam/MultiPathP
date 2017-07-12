#ifndef BASIC_CPP_RUNTIME_H
#define BASIC_CPP_RUNTIME_H

#ifdef USE_VALUE_SUMMARY
#define RUNTIME_NAMESPACE vs_cpp_runtime
#else
#define RUNTIME_NAMESPACE basic_cpp_runtime
#endif

#include "Instrumentation.hpp"

#include "PTypes.h"
#include "Scheduler.hpp"
#include "ExecutionEngine.hpp"

#include "RandomScheduler.hpp"
#include "DelayBoundedScheduler.hpp"

#endif