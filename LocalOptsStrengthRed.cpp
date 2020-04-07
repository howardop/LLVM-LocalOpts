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
#include "llvm/IR/Operator.h"

#include <iostream>
#include <vector>

#include "algebraicOpts.cpp"
#include "constantHelper.cpp"

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
        bool doInitialization(Module &M) override {
          // errs() << "15745 Local Optimization Pass\n"; // TODO: remove this.
          outs() << "Strength Reductions applied:\n";
          return false;
        }

        int shiftCount = 0; // Used in creating names for new instructions
    
        bool runOnFunction(Function &F) override {
            for (auto& B : F) {
                class ReplaceInstr {
                    public: 
                    Instruction* from;
                    Instruction* to;
                    ReplaceInstr(Instruction* oldI, Instruction* newI) : from(oldI), to(newI){};
                };
                std::vector<ReplaceInstr*> replCode{};
                
                for (auto& I : B) {
                    if (dyn_cast<BinaryOperator>(&I)) {
                        // Insert at the point where the instruction `op` appears.
                        //IRBuilder<> builder(binOp);
                        outs() << "Found binary operator: " ; I.dump(); outs() << "\n";
                        // Get the 2 operands
                        Value *op0 = I.getOperand(0);
                        Value *op1 = I.getOperand(1);
            
                        int constR = 0;
                        int log2ConstR = 0;  // Used to turn mult/div into shift

                        if (isConstant(op1, constR)) {
                            outs() << "Right operand is constant of value " << constR << "\n";
                            if (isPowerOf2(constR, log2ConstR)){
                                outs() << constR << " is 2**" << log2ConstR << "\n";
                                IntegerType* intType = IntegerType::get(I.getContext(),32);
                                ConstantInt* op1Log2 = ConstantInt::get(intType, log2ConstR);
                                switch(I.getOpcode()) {
                                    case Instruction::Mul: {
                                        Instruction* ShL = 
                                            BinaryOperator::Create(Instruction::Shl, op0, op1Log2, 
                                                    "shl_" + to_string(++shiftCount));
                                        ShL->dump();
                                        ReplaceInstr* replace = new ReplaceInstr(&I, ShL);
                                        replCode.push_back(replace);
                                        break;  //Multiply
                                    }
                                    case Instruction::SDiv: {
                                        outs() << "Division instruction found\n";
                                        Instruction* AShR = 
                                            BinaryOperator::Create(Instruction::AShr, op0, op1Log2, 
                                                    "ashr_" + to_string(++shiftCount));
                                        AShR->dump();
                                        ReplaceInstr* replace = new ReplaceInstr(&I, AShR);
                                        replCode.push_back(replace);
                                        break;  //Divide
                                    }
                                } // end Switch
                            }
                        } // end constant test
                    } // end if(dyn_cast)
                } // end Instruction loop

                // Exitted instruction loop.  Now safe to remove instructions marked for deletion
                outs() << "replCode.size() = " << replCode.size() << "\n";
                while (!replCode.empty()) {
                    ReplaceInstWithInst(replCode.back() -> from, replCode.back() -> to);
                    replCode.pop_back();
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
static RegisterPass<LocalOpts> X("local-opts", "Strength Reduction", false, false);