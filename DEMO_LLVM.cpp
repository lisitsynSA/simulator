/*#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include <fstream>
#include <iostream>*/

#include "DEMO_interface.h"

#define GAS_SIZE 128
#define V_MAX 16
#define SLOWDOWN 4 // (V_MAX - SLOWDOWN) / V_MAX
#define SCALE 8
#define SIZE_X SCALE *DEMO_SIZE_X
#define SIZE_Y SCALE *DEMO_SIZE_Y

typedef struct {
  int32_t x;
  int32_t y;
  int32_t vx;
  int32_t vy;
} mol_t;

mol_t LLVM_gas[GAS_SIZE];

void LLVM_init_gas() {
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    LLVM_gas[i].x = DEMO_rand() % (SCALE * DEMO_SIZE_X);
    LLVM_gas[i].y = DEMO_rand() % (SCALE * DEMO_SIZE_Y);
    LLVM_gas[i].vx = (DEMO_rand() % (2 * V_MAX)) - V_MAX;
    LLVM_gas[i].vy = (DEMO_rand() % (2 * V_MAX)) - V_MAX;
  }
  for (uint32_t y = 0; y < DEMO_SIZE_Y; y++)
    for (uint32_t x = 0; x < DEMO_SIZE_X; x++) {
      DEMO_put_pixel(y, x, 0xff000000);
    }
}

uint32_t LLVM_get_mol_color(uint32_t vx, uint32_t vy, int32_t v2max) {
  uint32_t v = vx * vx + vy * vy;
  uint32_t red = 0xff * v / v2max;
  uint32_t blue = 0xff - red;
  return 0xff000000 + (red << 16) + blue;
}

void LLVM_draw_gas() {
  int32_t v2max = 0;
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    if (v2max <
        LLVM_gas[i].vx * LLVM_gas[i].vx + LLVM_gas[i].vy * LLVM_gas[i].vy) {
      v2max = LLVM_gas[i].vx * LLVM_gas[i].vx + LLVM_gas[i].vy * LLVM_gas[i].vy;
    }
  }
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    DEMO_put_pixel(LLVM_gas[i].y / SCALE, LLVM_gas[i].x / SCALE,
                   LLVM_get_mol_color(LLVM_gas[i].vx, LLVM_gas[i].vy, v2max));
  }
  DEMO_flush();
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    DEMO_put_pixel(LLVM_gas[i].y / SCALE, LLVM_gas[i].x / SCALE, 0xff000000);
  }
}

void LLVM_step_gas() {
  for (uint32_t i = 0; i < GAS_SIZE; i++) {
    LLVM_gas[i].x += LLVM_gas[i].vx;
    if (LLVM_gas[i].x < 0) {
      LLVM_gas[i].x = -LLVM_gas[i].x;
      LLVM_gas[i].vx = -LLVM_gas[i].vx;
    }
    if (LLVM_gas[i].x > SIZE_X) {
      LLVM_gas[i].x = 2 * SIZE_X - LLVM_gas[i].x;
      LLVM_gas[i].vx = -LLVM_gas[i].vx;
    }
    LLVM_gas[i].y += LLVM_gas[i].vy;
    if (LLVM_gas[i].y < 0) {
      LLVM_gas[i].y = -LLVM_gas[i].y;
      LLVM_gas[i].vy = -LLVM_gas[i].vy;
    }
    if (LLVM_gas[i].y > SIZE_Y) {
      LLVM_gas[i].y = 2 * SIZE_Y - LLVM_gas[i].y;
      LLVM_gas[i].vy = -LLVM_gas[i].vy;
    }
  }
}

void DEMO_LLVM_main() {
  LLVM_init_gas();
  while (1) {
    LLVM_draw_gas();
    LLVM_step_gas();
  }
}
/*
int DEMO_LLVM_main() {
  llvm::LLVMContext context;
  // ; ModuleID = 'top'
  // source_filename = "top"
  llvm::Module *module = new llvm::Module("top", context);
  llvm::IRBuilder<> builder(context);

  // declare void @main()
  llvm::FunctionType *funcType =
      llvm::FunctionType::get(builder.getVoidTy(), false);
  llvm::Function *mainFunc = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, "main", module);
  // entry:
  llvm::BasicBlock *entryBB =
      llvm::BasicBlock::Create(context, "entry", mainFunc);

  builder.SetInsertPoint(entryBB);
  llvm::Function *CalleeF = llvm::Function::Create(
      llvm::FunctionType::get(builder.getVoidTy(),
                              llvm::ArrayRef<llvm::Type *>(builder.getVoidTy()),
                              false),
      llvm::Function::ExternalLinkage, "LLVM_main", module);
  builder.CreateCall(CalleeF);

  std::cout << "#[LLVM IR]:\n";
  std::string s;
  llvm::raw_string_ostream os(s);
  module->print(os, nullptr);
  os.flush();
  std::cout << s;

  // Interpreter of LLVM IR
  std::cout << "Running code...\n";
  llvm::ExecutionEngine *ee =
      llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module)).create();
  ee->InstallLazyFunctionCreator([&](const std::string &fnName) -> void * {
    if (fnName == "LLVM_main") {
      return reinterpret_cast<void *>(LLVM_main);
    }
    return nullptr;
  });
  ee->finalizeObject();
  std::vector<llvm::GenericValue> noargs;
  llvm::GenericValue v = ee->runFunction(mainFunc, noargs);
  std::cout << "Code was run.\n";

  return 0;
}*/
