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

#include <iostream>
#include <vector>

#include "algebraicOpts.cpp"

using namespace llvm;

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
    
        // Print output for each function
            bool runOnFunction(Function &F) override {
                for (auto& B : F) {
                    std::vector<Instruction*> deadCode{};
                    
                        for (auto& I : B) {
                            if (dyn_cast<BinaryOperator>(&I)) {
                                // Insert at the point where the instruction `op` appears.
                                //IRBuilder<> builder(binOp);

                                // Algebraic identities: e.g., x+0 = 0+x = x.
                                // First check for addition
                                    switch(I.getOpcode()) {
                                        case Instruction::Add:{
                                            int rc = add0(I);
                                            if (rc > 0) {
                                                // One of the operands was 0 and instruction was eliminated from basic block
                                                deadCode.push_back(&I);
                                            } else if (rc < 0) {
                                                // Something went wrong
                                            }   
                                            break; //Add
                                        }
                                        case Instruction::Sub: {
                                            outs() << "Found binary sub: ";
                                            I.dump();
                                            outs() << "\n";
                                            int rc = sub0(I);
                                            if (rc > 0) {
                                                // One of the operands was 0 and instruction was eliminated from basic block
                                                deadCode.push_back(&I);
                                            } else if (rc < 0) {
                                                // Something went wrong
                                            }
                                            break;  //Subtract
                                        }
                                        case Instruction::Mul: {
                                            int rc = mul1(I);
                                            if (rc > 0) {
                                                // One of the operands was 0 and instruction was eliminated from basic block
                                                deadCode.push_back(&I);
                                            } else if (rc < 0) {
                                                // Something went wrong
                                            }
                                            break;  //Multiply
                                        }
                                        case Instruction::SDiv: {
                                            int rc = div1(I);
                                            if (rc > 0) {
                                                // One of the operands was 0 and instruction was eliminated from basic block
                                                deadCode.push_back(&I);
                                            } else if (rc < 0) {
                                                // Something went wrong
                                            }
                                            break;  //Divide
                                        }
                                    } // end Switch
                            } // end if(dyn_cast)
                        } // end Instruction loop
                    // Now remove instructions marked for deletion
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
static RegisterPass<LocalOpts> X("local-opts", "15745: Local Optimizations", false, false);