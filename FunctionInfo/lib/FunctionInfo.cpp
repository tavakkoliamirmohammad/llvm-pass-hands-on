#include <llvm/IR/Module.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Instructions.h>

using namespace llvm;

namespace {

    class FunctionInfo final : public ModulePass {
    public:
        static char ID;

        FunctionInfo() : ModulePass(ID) {}

        // We don't modify the program, so we preserve all analysis.
        virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.setPreservesAll();
        }

        virtual bool runOnModule(Module &M) override {
            for (auto iter = M.getFunctionList().begin(); iter != M.getFunctionList().end(); ++iter) {
                Function &F = *iter;
                outs() << "Function Name: " << F.getName() << "\n" << "Number of Arguments: ";
                if (F.isVarArg()) {
                    outs() << "*";
                } else {
                    outs() << F.arg_size();
                }
                outs() << "\n" <<
                       "Number OF BBs: " << F.getBasicBlockList().size() << "\n" <<
                       "Number of Instructions " << F.getInstructionCount() << "\n";
                int call_count = 0;
                for (auto U: F.users()) {
                    if (auto I = dyn_cast<Instruction>(U)) {
                        if (isa<CallInst>(I)) {
                            ++call_count;
                        }
                    }
                }
                outs() << "Number of Calls: " << call_count << "\n";
                outs() << "-----------------" << "\n";
            }
            return false;
        }
    }; // class FunctionInfo

    char FunctionInfo::ID = 0;
    RegisterPass<FunctionInfo> X("function-info", "CSCD70: Function Information");

} // anonymous namespace
