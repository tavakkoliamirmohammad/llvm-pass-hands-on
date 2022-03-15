#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/IR/Constants.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

using namespace llvm;

namespace {

    class ConstFoldOpt final : public FunctionPass {
    public:
        static char ID;

        ConstFoldOpt() : FunctionPass(ID) {}

        void getAnalysisUsage(AnalysisUsage &AU) const override {}

        static bool handleIntMulInstruction(BasicBlock &B, Instruction &I) {
            Value *op_f = I.getOperand(0);
            Value *op_s = I.getOperand(1);
            auto CI_F = dyn_cast<ConstantInt>(op_f);
            auto CI_S = dyn_cast<ConstantInt>(op_s);
            bool changed = false;
            if (CI_F && CI_S) {
                I.replaceAllUsesWith(ConstantInt::get(CI_F->getType(), CI_F->getZExtValue() * CI_S->getZExtValue()));
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        static bool handleFMulInstruction(BasicBlock &B, Instruction &I) {
            Value *op_f = I.getOperand(0);
            Value *op_s = I.getOperand(1);
            auto CI_F = dyn_cast<ConstantFP>(op_f);
            auto CI_S = dyn_cast<ConstantFP>(op_s);
            bool changed = false;
            if (CI_F && CI_S) {
                I.replaceAllUsesWith(ConstantFP::get(CI_F->getType(), CI_F->getValueAPF().convertToDouble() *
                                                                      CI_S->getValueAPF().convertToDouble()));
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        static bool handleIntDivInstruction(BasicBlock &B, Instruction &I) {
            Value *op_f = I.getOperand(0);
            Value *op_s = I.getOperand(1);
            auto CI_F = dyn_cast<ConstantInt>(op_f);
            auto CI_S = dyn_cast<ConstantInt>(op_s);
            bool changed = false;
            if (CI_F && CI_S) {
                I.replaceAllUsesWith(ConstantInt::get(CI_F->getType(), CI_F->getZExtValue() / CI_S->getZExtValue()));
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        static bool handleFDivInstruction(BasicBlock &B, Instruction &I) {
            Value *op_f = I.getOperand(0);
            Value *op_s = I.getOperand(1);
            auto CI_F = dyn_cast<ConstantFP>(op_f);
            auto CI_S = dyn_cast<ConstantFP>(op_s);
            bool changed = false;
            if (CI_F && CI_S) {
                I.replaceAllUsesWith(ConstantFP::get(CI_F->getType(), CI_F->getValueAPF().convertToDouble() /
                                                                      CI_S->getValueAPF().convertToDouble()));
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        static bool handleIntAddInstruction(BasicBlock &B, Instruction &I) {
            Value *op_f = I.getOperand(0);
            Value *op_s = I.getOperand(1);
            auto CI_F = dyn_cast<ConstantInt>(op_f);
            auto CI_S = dyn_cast<ConstantInt>(op_s);
            bool changed = false;
            if (CI_F && CI_S) {
                I.replaceAllUsesWith(ConstantInt::get(CI_F->getType(), CI_F->getZExtValue() + CI_S->getZExtValue()));
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        static bool handleFAddInstruction(BasicBlock &B, Instruction &I) {
            Value *op_f = I.getOperand(0);
            Value *op_s = I.getOperand(1);
            auto CI_F = dyn_cast<ConstantFP>(op_f);
            auto CI_S = dyn_cast<ConstantFP>(op_s);
            bool changed = false;
            if (CI_F && CI_S) {
                I.replaceAllUsesWith(ConstantFP::get(CI_F->getType(), CI_F->getValueAPF().convertToDouble() +
                                                                      CI_S->getValueAPF().convertToDouble()));
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        static bool handleIntSubInstruction(BasicBlock &B, Instruction &I) {
            Value *op_f = I.getOperand(0);
            Value *op_s = I.getOperand(1);
            auto CI_F = dyn_cast<ConstantInt>(op_f);
            auto CI_S = dyn_cast<ConstantInt>(op_s);
            bool changed = false;
            if (CI_F && CI_S) {
                I.replaceAllUsesWith(ConstantInt::get(CI_F->getType(), CI_F->getZExtValue() - CI_S->getZExtValue()));
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }

        static bool handleFSubInstruction(BasicBlock &B, Instruction &I) {
            Value *op_f = I.getOperand(0);
            Value *op_s = I.getOperand(1);
            auto CI_F = dyn_cast<ConstantFP>(op_f);
            auto CI_S = dyn_cast<ConstantFP>(op_s);
            bool changed = false;
            if (CI_F && CI_S) {
                I.replaceAllUsesWith(ConstantFP::get(CI_F->getType(), CI_F->getValueAPF().convertToDouble() -
                                                                      CI_S->getValueAPF().convertToDouble()));
                I.eraseFromParent();
                changed = true;
            }
            return changed;
        }


        bool runOnFunction(Function &F) override {
            bool changed = false;
            for (BasicBlock &B: F) {
                for (BasicBlock::iterator DI = B.begin(); DI != B.end();) {
                    Instruction &I = *DI;
                    ++DI;
                 if (I.getOpcode() == Instruction::Add) {
                        changed |= handleIntAddInstruction(B, I);
                    } else if (I.getOpcode() == Instruction::FAdd) {
                        changed |= handleFAddInstruction(B, I);
                    } else if (I.getOpcode() == Instruction::Sub) {
                        changed |= handleIntSubInstruction(B, I);
                    } else if (I.getOpcode() == Instruction::FSub) {
                        changed |= handleFSubInstruction(B, I);
                    } else if (I.getOpcode() == Instruction::FDiv) {
                        changed |= handleFDivInstruction(B, I);
                    } else if (I.getOpcode() == Instruction::UDiv || I.getOpcode() == Instruction::SDiv) {
                        changed |= handleIntDivInstruction(B, I);
                    } else if (I.getOpcode() == Instruction::Mul) {
                        changed |= handleIntMulInstruction(B, I);
                    } else if (I.getOpcode() == Instruction::FMul) {
                        changed |= handleFMulInstruction(B, I);
                    }
                }
            }
            return changed;
        }
    }; // class ConstFoldOpt

    char ConstFoldOpt::ID = 0;
    RegisterPass<ConstFoldOpt> X("const-fold-opt",
                                 "CSCD70: Constant Folding Optimization");

} // anonymous namespace
