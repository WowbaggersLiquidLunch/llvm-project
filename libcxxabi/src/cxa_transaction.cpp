//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "cxxabi.h"

#include <__threading_support>
#ifndef _LIBCXXABI_HAS_NO_THREADS
#if defined(__ELF__) && defined(_LIBCXXABI_LINK_PTHREAD_LIB)
#pragma comment(lib, "pthread")
#endif
#endif

namespace __cxxabiv1 {
namespace __n4923 {

// When POSIX threads are not available, make the mutex operations a nop
#ifndef _LIBCXXABI_HAS_NO_THREADS
static _LIBCPP_CONSTINIT std::__libcpp_mutex_t __cxa_transaction_mutex = _LIBCPP_MUTEX_INITIALIZER;
#else
static _LIBCPP_CONSTINIT void* heap_mutex = 0;
#endif

extern "C" {
void __cxa_enter_transaction() { std::__libcpp_mutex_lock(&__cxa_transaction_mutex); }
void __cxa_exit_transaction() { std::__libcpp_mutex_unlock(&__cxa_transaction_mutex); }
} // extern "C"

} // __n4923
} // __cxxabiv1
