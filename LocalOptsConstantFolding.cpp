// Cleaned up version of LocalOpts1.cpp
// 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 15-745 S18 Assignment 1: LocalOpts.cpp
// Group:
////////////////////////////////////////////////////////////////////////////////


#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Value.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/User.h"
#include "llvm/IR/IRBuilder.h"
#include "constantHelper.cpp"

#include <iostream>
#include <vector>

#include "algebraicOpts.cpp"

using namespace llvm;
using namespace std;

namespace {
    class LocalOpts : public FunctionPass {
    public:
        static char ID;
        LocalOpts() : FunctionPass(ID) { }
        ~LocalOpts() { }

        // We don't modify the program, so we preserve all analyses
        // void getAnalysisUsage(AnalysisUsage &AU) const override {
        //   AU.setPreservesAll();
        // }

        // Do some initialization
        // bool doInitialization(Module &M) override {
        //   // errs() << "15745 Local Optimization Pass\n"; // TODO: remove this.
        //   outs() << "Transformations applied:\n";
        //   return false;
        // }


    
        bool runOnFunction(Function &F) override {
            for (auto& B : F) {
                std::vector<Instruction*> deadCode{};
                
                for (auto& I : B) {
                    if (dyn_cast<BinaryOperator>(&I)) {
                        // Insert at the point where the instruction `op` appears.
                        //IRBuilder<> builder(binOp);
                        outs() << "Found binary operator: " ; I.dump(); outs() << "\n";
                        // Get the 2 operands
                        Value *op0 = I.getOperand(0);
                        Value *op1 = I.getOperand(1);
            
                        int constL = 0, constR = 0;
                        int newConst = 0;

                        if (isConstant(op0, constL)) {
                            outs() << "Left operand is constant of value " << constL << "\n";
                            if (isConstant(op1, constR)) {
                                outs() << "Right operand is constant of value " << constR << "\n";
                                // Both operands are constant; compute value
                                switch(I.getOpcode()) {
                                    case Instruction::Add:{
                                        newConst = constL + constR;
                                        break; //Add
                                    }
                                    case Instruction::Sub: {
                                        newConst = constL - constR;
                                        break;  //Subtract
                                    }
                                    case Instruction::Mul: {
                                        newConst = constL * constR;
                                        break;  //Multiply
                                    }
                                    case Instruction::SDiv: {
                                        newConst = constL / constR;
                                        break;  //Divide
                                    }
                                } // end Switch
                                // Now replace instruction with computed value
                               // Create Value object containing the new constant value
                                Type *myIntType = IntegerType::get(I.getContext(), 32);
                                 Constant *constValue = ConstantInt::getSigned(myIntType, newConst);
                                I.replaceAllUsesWith(constValue);
                                if (I.use_empty()) {
                                    outs() << "Instruction successfully replaced with " << newConst << "\n";
                                    deadCode.push_back(&I);

                                } else {
                                    outs() << "***** ERROR: Could not replace instruction with computed value *****\n";
                                }
                            } // Testing right operand
                        } // end constant test

                    } // end if(dyn_cast)
                } // end Instruction loop

                // Exitted instruction loop.  Now safe to remove instructions marked for deletion
                outs() << "deadcode.size() = " << deadCode.size() << "\n";
                while (!deadCode.empty()) {
                    deadCode.back() -> eraseFromParent();
                    deadCode.pop_back();
                }
            }// end BB loop
            // True => modified the code
            return true;
        } // end runOnFunction
    }; // end class LocalOpts
} // end namespace

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LocalOpts::ID = 0;
static RegisterPass<LocalOpts> X("local-opts", "Local Optimizations: Constant Folding", false, false);