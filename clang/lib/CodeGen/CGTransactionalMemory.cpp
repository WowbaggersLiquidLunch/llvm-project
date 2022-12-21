//===--- CGTransactionalMemory.cpp - Emit LLVM IR for Transactions --------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the code for emitting CXX Transactional Memory operations.
//
//===----------------------------------------------------------------------===//

#include "CGCall.h"
#include "CodeGenFunction.h"

using namespace clang;
using namespace CodeGen;

StringRef CodeGenFunction::TransactionBoundary::functionName() const {
  switch (backingStorage) {
  case TransactionBoundary::Start: return "__cxa_enter_transaction"; break;
  case TransactionBoundary::End:   return "__cxa_exit_transaction";  break;
  }
}

void CodeGenFunction::EmitCXXTransactionBoundary(TransactionBoundary boundary) {
  auto resultType = getContext().VoidTy;
  auto arguments = CallArgList();
  const auto &functionInfo =
      this->CGM.getTypes().arrangeBuiltinFunctionCall(resultType, arguments);
  auto *functionType = this->CGM.getTypes().GetFunctionType(functionInfo);
  auto functionAttributeBuilder = llvm::AttrBuilder(this->getLLVMContext());
  functionAttributeBuilder
      .addAttribute(llvm::Attribute::NoUnwind)
      .addAttribute(llvm::Attribute::WillReturn);
  auto functionAttributes = llvm::AttributeList::get(
      this->getLLVMContext(), llvm::AttributeList::FunctionIndex,
      functionAttributeBuilder);
  auto function = this->CGM.CreateRuntimeFunction(
      functionType, boundary.functionName(), functionAttributes);
  EmitNounwindRuntimeCall(function);
}
