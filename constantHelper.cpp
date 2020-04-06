#include "llvm/Pass.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Value.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/User.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/APInt.h"

using namespace llvm;

bool isConstant(Value *op, int& constant) {
    Value *op0 = op;

    // If op is an instruction, get the argument of the instruction
    if ( Instruction* CI = dyn_cast<Instruction>(op) ) {
         op0 = CI->getOperand(0);
    }
 
    if ( ConstantInt* CI = dyn_cast<ConstantInt>(op0) ) {
        if (CI->getBitWidth() <= 32) {
            constant = CI->getSExtValue();
            return true;
        }
    }

    return false;

} // end isConstant