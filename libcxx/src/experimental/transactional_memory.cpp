// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <mutex>

// instead of extern c, this should be in experimental namespace
extern "C" {
namespace {
std::mutex mtx; // need to be re-entrant
}
void transaction_start() _NOEXCEPT {
  mtx.lock();
}
void transaction_end() _NOEXCEPT {
  mtx.unlock();
}
}
