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

// Checks if num is a power of 2.  If so, returns the power so mult/div can be turned into shift
bool isPowerOf2(int val, int& log2) {
    int bitCount = 0;
    int pos = -1;
    if (val < 0) {
        val = -val;
    }
    while (val > 0) {
        pos = pos+1;
        // Test if left-most bit is set
        if ((val & 1) == 1) {
            bitCount = bitCount + 1;
            if (bitCount > 1) {
                // If number is power of 2, it will have only 1 bit set
                return false;
            }
        }
        val = val >> 1;
    }
    if (bitCount == 1) {
        log2 = pos;
        return true;
    }
    return false;

}