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

#include <iostream>
#include <vector>

using namespace llvm;

int add0(Instruction& I){
    outs() << "add0 Entered\n";
    Value* op0 = I.getOperand(0);  
    Value* op1 = I.getOperand(1);
    outs() << "dump of op0 = ";
    op0 -> dump();
    outs() << "dump of op1 = ";
    op1->dump();

    if (ConstantInt *CI0 = dyn_cast<ConstantInt>(op0)) {
        outs() << "Op0 is constant\n";
        if (CI0 -> isZero()){
            outs() << "op0 is 0\n"; 
            I.replaceAllUsesWith(op1);
            if (I.use_empty()) {
            return 1;
            } else {
            outs() << "***** ERROR: Add instruction still used *****\n";
            return -1;
            }
        }
    }

    if (ConstantInt *CI1 = dyn_cast<ConstantInt>(op1)) {
        outs() << "Op1 is constant\n";
        if (CI1 -> isZero()){
            outs() << "op1 is 0\n"; 
            I.replaceAllUsesWith(op0);
            if (I.use_empty()) {
            return 1;
            } else {
            outs() << "***** ERROR: Add instruction still used *****\n";
            return -1;

            }
        }
    }

    // Neither operarand 0
    return 0;
}

int sub0(Instruction& I){
    outs() << "sub0 Entered\n";
    Value* op0 = I.getOperand(0);  
    Value* op1 = I.getOperand(1);
    outs() << "dump of op0 = ";
    op0 -> dump();
    outs() << "dump of op1 = ";
    op1->dump();



    if (ConstantInt *CI1 = dyn_cast<ConstantInt>(op1)) {
        outs() << "Op1 is constant\n";
        if (CI1 -> isZero()){
            outs() << "op1 is 0\n"; 
            I.replaceAllUsesWith(op0);
            if (I.use_empty()) {
            return 1;
            } else {
            outs() << "***** ERROR: Add instruction still used *****\n";
            return -1;

            }
        }
    }

    if (ConstantInt *CI0 = dyn_cast<ConstantInt>(op0)) {
        outs() << "Op0 is constant\n";
        if (CI0 -> isZero()){
            outs() << "op0 is 0\n"; 
            // Should replace instruction with negation
            // Don't do anything now
            return 0;
        }
    }

    // Checking if subtracting equal values
    if (op0 == op1) {
        outs() << "Subtraction operands are equal\n";
        // Result is 0
        // Change instruction from "sub nsw i32 %0, %1" to "I.replaceAllUsesWith(0)"
        IntegerType* intType = IntegerType::get(I.getContext(),32);
        ConstantInt* zero = ConstantInt::get(intType, 0);
        //APInt* zero = APInt.getNullValue(32);
        I.replaceAllUsesWith(zero);
        return 1;


    }

    // Neither operarand 0
    return 0;
}

int mul1(Instruction& I){
    outs() << "mul1 Entered\n";
    Value* op0 = I.getOperand(0);  
    Value* op1 = I.getOperand(1);
    outs() << "dump of op0 = ";
    op0 -> dump();
    outs() << "dump of op1 = ";
    op1->dump();

    if (ConstantInt *CI0 = dyn_cast<ConstantInt>(op0)) {
        outs() << "Op0 is constant\n";
        if (CI0 -> isZero()){
            outs() << "op0 is 0.\n"; 
            // 1st operand is 0, so result is 0
            // Change instruction from "mul nsw i32 %0, %1" to "I.replaceAllUsesWith(0)"
            IntegerType* intType = IntegerType::get(I.getContext(),32);
            ConstantInt* zero = ConstantInt::get(intType, 0);
            I.replaceAllUsesWith(zero);
            return 1;
        }
    }

    if (ConstantInt *CI1 = dyn_cast<ConstantInt>(op1)) {
        outs() << "Op1 is constant\n";
        if (CI1 -> isZero()){
            outs() << "op1 is 0\n"; 
            // 2st operand is 0, so result is 0
            // Change instruction from "mul nsw i32 %0, %1" to "I.replaceAllUsesWith(0)"
            IntegerType* intType = IntegerType::get(I.getContext(),32);
            ConstantInt* zero = ConstantInt::get(intType, 0);
            I.replaceAllUsesWith(zero);
            return 1;

        }
    }
    if (op0 == op1) {
        // Replace sdiv with 1
        outs() << "Multiply operands are equal\n";
        // Change instruction from "sub nsw i32 %0, %1" to "I.replaceAllUsesWith(1)"
        IntegerType* intType = IntegerType::get(I.getContext(),32);
        ConstantInt* zero = ConstantInt::get(intType, 1);
        //APInt* zero = APInt.getNullValue(32);
        I.replaceAllUsesWith(zero);
        return 1;
    }

    // Neither operarand 0
    return 0;
}

int div1(Instruction& I){
    outs() << "div1 Entered\n";
    Value* op0 = I.getOperand(0);  
    Value* op1 = I.getOperand(1);
    outs() << "dump of op0 = ";
    op0 -> dump();
    outs() << "dump of op1 = ";
    op1->dump();

    if (ConstantInt *CI0 = dyn_cast<ConstantInt>(op0)) {
        outs() << "Op0 is constant\n";
        if (CI0 -> isZero()){
            outs() << "op0 is 0.\n"; 
            // 1st operand is 0, so result is 0
            // Change instruction from "sdiv nsw i32 %0, %1" to "I.replaceAllUsesWith(0)"
            IntegerType* intType = IntegerType::get(I.getContext(),32);
            ConstantInt* zero = ConstantInt::get(intType, 0);
            I.replaceAllUsesWith(zero);
            return 1;
        }
    }
    if (op0 == op1) {
        // Replace sdiv with 1
        outs() << "Subtraction operands are equal\n";
        // Change instruction from "sub nsw i32 %0, %1" to "I.replaceAllUsesWith(0)"
        IntegerType* intType = IntegerType::get(I.getContext(),32);
        ConstantInt* zero = ConstantInt::get(intType, 1);
        //APInt* zero = APInt.getNullValue(32);
        I.replaceAllUsesWith(zero);
        return 1;
    }

    // Nothing happened
    return 0;
}
