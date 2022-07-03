# Static Symbolic Execution on SVF using Z3
## Build

### Method 1 
` bash build.sh `
### Method 2
- required ENV: [`$LLVM_DIR`](https://releases.llvm.org/download.html) , [`$SVF_DIR`](https://github.com/SVF-tools/SVF) and [`$Z3_DIR`](https://github.com/Z3Prover/z3/releases/tag/z3-4.8.10)
- ` cmake -g -DCMAKE_BUILD_TYPE=Debug -DLLVM_DIR=/home/SVF-tools/SVF/llvm-12.0.0.obj -DSVF_DIR=/home/SVF-tools/SVF -DZ3_DIR=/home/z3`

 (Currently can use [LLVM-12](https://github.com/llvm/llvm-project/releases/tag/llvmorg-12.0.0))

## Folders
- Test demo code in svf-ex
- Symbolic execution in sse


### LLVM Series

- [LLVM Language Reference Manual](https://llvm.org/docs/LangRef.html#abstract)
- [LLVM Value reference hand book and API](https://llvm.org/doxygen/group__LLVMCCoreValueGeneral.html) 


### SVF and Programming Series


### Z3 Series 
- [SAT-Wikipedia](https://en.wikipedia.org/wiki/Satisfiability_modulo_theories)
- [Constraint programming](https://en.wikipedia.org/wiki/Constraint_programming)
- [SMT solver impetus(zhihu)](https://www.zhihu.com/people/rainoftime/posts)
- [Z3 C++ example](https://sat-smt.codes/SAT_SMT_by_example.pdf)










