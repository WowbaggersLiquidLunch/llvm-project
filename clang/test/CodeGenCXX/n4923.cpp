// RUN: %clang_cc1 -ftm-ts %s -emit-llvm -o - | FileCheck %s

void testAtomicDo() {
  atomic do {
    // CHECK: call void @__cxa_enter_transaction()
  }
  // CHECK-NEXT: call void @__cxa_exit_transaction()
}

void testTransactionExit(int number) {
  atomic do {
    if (number == 0)
      return;
      // CHECK: call void @__cxa_exit_transaction()
    while (number > 0)
      atomic do {
        // CHECK: call void @__cxa_enter_transaction()
        while (number > 100) {
          atomic do {
            // CHECK: call void @__cxa_enter_transaction()
            break;
          }
          // CHECK: call void @__cxa_exit_transaction()
        }
        break;
      }
      // CHECK: call void @__cxa_exit_transaction()
    auto lambda = []() {
      return 42;
    };
    auto atomicLambda = []() {
      atomic do {
        // CHECK: call void @__cxa_enter_transaction()
        return 24;
      }
      // CHECK: call void @__cxa_exit_transaction()
    };
  }
  // CHECK: call void @__cxa_exit_transaction()
}
