#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/Support/MathExtras.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>


using namespace llvm;

namespace {

    class StrengthReduction final : public FunctionPass {
    public:
        static char ID;

        StrengthReduction() : FunctionPass(ID) {}

        void getAnalysisUsage(AnalysisUsage &AU) const override {}

        static bool is_power_of_2(uint64_t x) {
            return x != 0 && !(x & (x - 1));
        }

        static bool handleIntDivInstruction(BasicBlock &B, Instruction &I) {
            Value *op_s = I.getOperand(1);
            bool changed = false;
            if (auto *CI = dyn_cast<ConstantInt>(op_s)) {
                if (is_power_of_2(CI->getZExtValue())) {
                    ReplaceInstWithInst(&I, BinaryOperator::CreateAShr(I.getOperand(0), ConstantInt::get(
                            op_s->getType(), Log2_64(CI->getZExtValue()))));
                    changed = true;
                }
            }
            return changed;
        }

        static bool handleFDivInstruction(BasicBlock &B, Instruction &I) {
            Value *op_s = I.getOperand(1);
            bool changed = false;
            if (auto *CI = dyn_cast<ConstantFP>(op_s)) {
                double log_value = Log2(CI->getValueAPF().convertToDouble());
                if (log_value == floor(log_value)) {
                    ReplaceInstWithInst(&I, BinaryOperator::CreateAShr(I.getOperand(0), ConstantFP::get(
                            op_s->getType(), Log2(CI->getValueAPF().convertToDouble()))));
                    changed = true;
                }
            }
            return changed;
        }

        static bool handleIntMulInstruction(BasicBlock &B, Instruction &I) {
            Value *op_s = I.getOperand(1);
            bool changed = false;
            if (auto *CI = dyn_cast<ConstantInt>(op_s)) {
                if (is_power_of_2(CI->getZExtValue())) {
                    ReplaceInstWithInst(&I, BinaryOperator::CreateShl(I.getOperand(0), ConstantInt::get(
                            op_s->getType(), Log2_64(CI->getZExtValue()))));
                    changed = true;
                }
            }
            return changed;
        }

        static bool handleFMulInstruction(BasicBlock &B, Instruction &I) {
            Value *op_s = I.getOperand(1);
            bool changed = false;
            if (auto *CI = dyn_cast<ConstantFP>(op_s)) {
                double log_value = Log2(CI->getValueAPF().convertToDouble());
                if (log_value == floor(log_value)) {
                    ReplaceInstWithInst(&I, BinaryOperator::CreateShl(I.getOperand(0), ConstantFP::get(
                            op_s->getType(), Log2(CI->getValueAPF().convertToDouble()))));
                    changed = true;
                }
            }
            return changed;
        }

        bool runOnFunction(Function &F) override {
            bool changed = false;
            for (BasicBlock &B: F) {
                for (BasicBlock::iterator DI = B.begin(); DI != B.end();) {
                    Instruction &I = *DI;
                    ++DI;
                    if (I.getOpcode() == Instruction::FDiv) {
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
    }; // class StrengthReduction

    char StrengthReduction::ID = 0;
    RegisterPass<StrengthReduction> X("strength-reduction",
                                      "CSCD70: Strength Reduction");

} // anonymous namespace
