#include <fstream>
#include <iostream>
#include <unordered_map>

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

// clang++ `llvm-config --cppflags --ldflags --libs` ./simulator_utils/assembler.cpp -lSDL2 -I"/usr/include/SDL2/"
// ./a.out ./simulator_utils/app.s 2> app.ll

#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <time.h>

#define FRAME_TICKS 50
#define SIM_X_SIZE 512
#define SIM_Y_SIZE 256

static SDL_Renderer *Renderer = NULL;
static SDL_Window *Window = NULL;
static Uint32 Ticks = 0;

extern void app();

void simInit()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(SIM_X_SIZE, SIM_Y_SIZE, 0, &Window, &Renderer);
    SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
    SDL_RenderClear(Renderer);
    srand(time(NULL));
    std::cout << "simInit finished" << std::endl;
}

void simExit()
{
    SDL_Event event;
    std::cout << "simExit started" << std::endl;
    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
    SDL_Quit();
    std::cout << "simExit started" << std::endl;
}

void simFlush()
{
    SDL_PumpEvents();
    assert(SDL_TRUE != SDL_HasEvent(SDL_QUIT) && "User-requested quit");
    Uint32 cur_ticks = SDL_GetTicks() - Ticks;
    if (cur_ticks < FRAME_TICKS)
    {
        SDL_Delay(FRAME_TICKS - cur_ticks);
    }
    SDL_RenderPresent(Renderer);
}

void simPutPixel(int x, int y, int argb)
{
    assert(0 <= x && x < SIM_X_SIZE && "Out of range");
    assert(0 <= y && y < SIM_Y_SIZE && "Out of range");
    Uint8 a = argb >> 24;
    Uint8 r = (argb >> 16) & 0xFF;
    Uint8 g = (argb >> 8) & 0xFF;
    Uint8 b = argb & 0xFF;
    SDL_SetRenderDrawColor(Renderer, r, g, b, a);
    SDL_RenderDrawPoint(Renderer, x, y);
    Ticks = SDL_GetTicks();
}
//////////////////////////////
// All instructions
//////////////////////////////

// by @ marked instructions that exec with IR
enum InsnId_t {

  EXIT,   // void
  PUSH,   // 1r
  POP,    // 1r
  LANCO,  // 3r
  ADAN,   // 2r 1imm
  ADICBR, // 1r 3imm -> 1r 1imm 2lab
  ADICBRS,// 1r 3imm -> 1r 1imm 2lab
  ICBR,   // 1r 3imm -> 1r 1imm 2lab                @
  GETPTRA,// 3r
  GETPTRAI,// 2r 1imm
  GETPTR, // 3r
  MVI,     // 1r 1imm
  MV,     // 2r
  ADD,    // 3r                                     @
  ADDI,   // 2r 1imm                                @
  SREM,   // 3r                                     @
  SREMI,  // 2r 1imm                                @
  SHLI,   // 2r 1imm                                @
  ANDI,   // 2r 1imm                                @
  OR,     // 3r                                     @
  SELECT, // 4r                                     @
  SELECTI,// 2r 2imm                                @
  ICMPS, // 2r 1imm 
  ICMPL, // 2r 1imm 
  ICMPEQ,// 2r 1imm 
  ALLOCA, // 1r 1imm 
  LOAD,   // 2r 
  STORE,  // 1r 1imm 
  BR,     // 1imm -> 1lab                           @
  BRCOND,// 1r, 2imm  -> 1r 2lab                    @
  CALL,   // 1imm -> 1lab
  RET,    // void
  SIMFLUSH,
  SIMPUTPIXEL,
  MEMCPY,
};

//////////////////////////////
// Model for simulation
//////////////////////////////

using RegId_t = uint8_t;
using RegVal_t = uint32_t;
using Stack_t = std::vector<RegVal_t>;

const int REG_FILE_SIZE = 48;
class CPU {
public:
  RegVal_t REG_FILE[REG_FILE_SIZE] = {};
  RegVal_t PC;
  RegVal_t NEXT_PC;
  Stack_t CALL_STACK;
  Stack_t STACK;
  uint32_t RUN;
  std::vector<int> memory;
  int next_memory_ptr;

  CPU(int memory_size) : memory(memory_size) {}
  bool stack_ok() {
    if (STACK.empty()) {
      RUN = 0;
      outs() << "[RUNTIME ERROR] STACK ERROR\n";
      return false;
    }
    return true;
  }
  bool call_stack_ok() {
    if (STACK.empty()) {
      RUN = 0;
      outs() << "[RUNTIME ERROR] CALL STACK ERROR\n";
      return false;
    }
    return true;
  }
};

//////////////////////////////
// Universal Instruction
//////////////////////////////

class Instr {
public:
  InsnId_t m_ID;
  void (*m_INSTR)(CPU *, Instr *);
  RegId_t m_rs1;
  RegId_t m_rs2;
  RegId_t m_rs3;
  RegId_t m_rs4;
  RegVal_t m_imm1;
  RegVal_t m_imm2;
  RegVal_t m_imm3;
  std::string m_name;
  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name) {}
  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegVal_t imm1)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_imm1(imm1) {}

  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegId_t rs1)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1) {}
  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegId_t rs1, RegVal_t imm1)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_imm1(imm1) {}
  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegId_t rs1, RegVal_t imm1, RegVal_t imm2)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), 
        m_imm1(imm1), m_imm2(imm2) {}
  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegId_t rs1, RegVal_t imm1, RegVal_t imm2, RegVal_t imm3)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), 
        m_imm1(imm1), m_imm2(imm2), m_imm3(imm3) {}
  
  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegId_t rs1, RegId_t rs2)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_rs2(rs2) {}
  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegId_t rs1, RegId_t rs2, RegVal_t imm1)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_rs2(rs2),
        m_imm1(imm1) {}
  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegId_t rs1, RegId_t rs2, RegVal_t imm1, RegVal_t imm2)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_rs2(rs2),
        m_imm1(imm1), m_imm2(imm2) {}

  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegId_t rs1, RegId_t rs2, RegId_t rs3)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_rs2(rs2),
        m_rs3(rs3) {}

  Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name,
        RegId_t rs1, RegId_t rs2, RegId_t rs3, RegId_t rs4)
      : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_rs2(rs2),
        m_rs3(rs3), m_rs4(rs4) {}

  void dump() { 
//    outs() << m_name << "\n"; 
  }
};

//////////////////////////////
// Interpreter function
//////////////////////////////

void do_exit(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->RUN = 0;
}
void do_push(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->STACK.push_back(instr->m_rs1);
}
void do_pop(CPU *cpu, Instr *instr) {
  instr->dump();
  if (!cpu->stack_ok()) {
    return;
  }
  cpu->REG_FILE[instr->m_rs1] = cpu->STACK.back();
  cpu->STACK.pop_back();
}
void do_lanco(CPU *cpu, Instr *instr) {
  instr->dump();
  auto ptr = cpu->REG_FILE[instr->m_rs2] + 4 * cpu->REG_FILE[instr->m_rs3];
  cpu->REG_FILE[instr->m_rs1] = (cpu->memory[ptr / 4] == 1); 
}
void do_adan(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] =
      cpu->REG_FILE[instr->m_rs2] + instr->m_imm1;
  cpu->REG_FILE[instr->m_rs1] =
      cpu->REG_FILE[instr->m_rs1] & instr->m_imm1;
}
void do_adicbr(CPU *cpu, Instr *instr) {
  instr->dump();

  if ((cpu->REG_FILE[instr->m_rs1] + 1) == instr->m_imm1) {
    cpu->NEXT_PC = instr->m_imm2;
  } else {
    cpu->NEXT_PC = instr->m_imm3;
  }

  cpu->REG_FILE[instr->m_rs1] += 1;
}
void do_adicbrs(CPU *cpu, Instr *instr) {
  instr->dump();
  if (cpu->REG_FILE[instr->m_rs1] < instr->m_imm1) {
    cpu->NEXT_PC = instr->m_imm2;
  } else {
    cpu->NEXT_PC = instr->m_imm3;
  }

  cpu->REG_FILE[instr->m_rs1] += 1;
}
void do_icbr(CPU *cpu, Instr *instr) {
  instr->dump();

  if (cpu->REG_FILE[instr->m_rs1] == instr->m_imm1) {
    cpu->NEXT_PC = instr->m_imm2;
  } else {
    cpu->NEXT_PC = instr->m_imm3;
  }
}
void do_getptr(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] + 4 * cpu->REG_FILE[instr->m_rs3];
}
void do_getptra(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] + 4 * cpu->REG_FILE[instr->m_rs3];
}
void do_getptrai(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] + 4 * instr->m_imm1;
}
void do_mvi(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = instr->m_imm1;
}
void do_mv(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2];
}
void do_add(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] =
      cpu->REG_FILE[instr->m_rs2] + cpu->REG_FILE[instr->m_rs3];
}
void do_addi(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] + instr->m_imm1;
}
void do_srem(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = 
      cpu->REG_FILE[instr->m_rs2] % cpu->REG_FILE[instr->m_rs3];
}
void do_sremi(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] % instr->m_imm1;
}
void do_shli(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] << instr->m_imm1;
}
void do_andi(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] & instr->m_imm1;
}
void do_or(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = 
      cpu->REG_FILE[instr->m_rs2] | cpu->REG_FILE[instr->m_rs3];
}
void do_select(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = 
      cpu->REG_FILE[instr->m_rs2] ? 
      cpu->REG_FILE[instr->m_rs3] : cpu->REG_FILE[instr->m_rs4];
}
void do_selecti(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = 
      cpu->REG_FILE[instr->m_rs2] ? 
      instr->m_imm1 : instr->m_imm2;
}
void do_icmps(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = 
      (cpu->REG_FILE[instr->m_rs2] > instr->m_imm1) ? 
      1 : 0;
}
void do_icmpl(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = 
      (cpu->REG_FILE[instr->m_rs2] < instr->m_imm1) ? 
      1 : 0;
}
void do_icmpeq(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = 
      (cpu->REG_FILE[instr->m_rs2] == instr->m_imm1) ? 
      1 : 0;
}
void do_alloca(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = 4 * cpu->next_memory_ptr;

  cpu->next_memory_ptr += instr->m_imm1; 
}
void do_load(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->REG_FILE[instr->m_rs1] = *(cpu->memory.begin() + cpu->REG_FILE[instr->m_rs2] / 4);
}
void do_store(CPU *cpu, Instr *instr) {
  instr->dump();
  if (cpu->REG_FILE[instr->m_rs1] / 4 > cpu->memory.size()) {
    std::cout << "ABORT" << std::endl;
    std::abort();
  }
  cpu->memory[cpu->REG_FILE[instr->m_rs1] / 4] = instr->m_imm1;
}
void do_br(CPU *cpu, Instr *instr) {
  instr->dump();
  cpu->NEXT_PC = instr->m_imm1;
}
void do_brcond(CPU *cpu, Instr *instr) {
  instr->dump();
  if (cpu->REG_FILE[instr->m_rs1]) { 
    cpu->NEXT_PC = instr->m_imm1;
  } else {
    cpu->NEXT_PC = instr->m_imm2;
  }
}
void do_call(CPU *cpu, Instr *instr) {
  instr->dump();

  cpu->CALL_STACK.push_back(cpu->NEXT_PC);
  for (int i = 2; i < REG_FILE_SIZE; i++) {
    cpu->STACK.push_back(cpu->REG_FILE[i]);
  }
  cpu->NEXT_PC = instr->m_imm1;
}
void do_ret(CPU *cpu, Instr *instr) {
  instr->dump();
  if (!cpu->call_stack_ok()) {
    return;
  }
  cpu->NEXT_PC = cpu->CALL_STACK.back();
  cpu->CALL_STACK.pop_back();
  for (int i = REG_FILE_SIZE - 1; i > 1; i--) {
    cpu->REG_FILE[i] = cpu->STACK.back();
    cpu->STACK.pop_back();
  }
}
void do_simflush(CPU *cpu, Instr *instr) {
  instr->dump();
  simFlush();
}
void do_simputpixel(CPU *cpu, Instr *instr) {
  instr->dump();
  int x0 = cpu->REG_FILE[0];
  int x1 = cpu->REG_FILE[1];
  int x2 = cpu->REG_FILE[2];

  simPutPixel(x0, x1, x2);
}
void do_memcpy(CPU *cpu, Instr *instr) {
  instr->dump();

  int dest = cpu->REG_FILE[instr->m_rs1] / 4;
  int source = cpu->REG_FILE[instr->m_rs2] / 4;
  int size = instr->m_imm1;

  for (int i = 0; i < size / 4; i++) {
    cpu->memory[dest + i] = cpu->memory[source + i];
  }
}

void *lazyFunctionCreator(const std::string &fnName) {
  if (fnName == "do_exit") {
    return reinterpret_cast<void *>(do_exit);
  }
  if (fnName == "do_push") {
    return reinterpret_cast<void *>(do_push);
  }
  if (fnName == "do_pop") {
    return reinterpret_cast<void *>(do_pop);
  }
  if (fnName == "do_lanco") {
    return reinterpret_cast<void *>(do_lanco);
  }
  if (fnName == "do_adan") {
    return reinterpret_cast<void *>(do_adan);
  }
  if (fnName == "do_adicbr") {
    return reinterpret_cast<void *>(do_adicbr);
  }
  if (fnName == "do_adicbrs") {
    return reinterpret_cast<void *>(do_adicbrs);
  }
  if (fnName == "do_icbr") {
    return reinterpret_cast<void *>(do_icbr);
  }
  if (fnName == "do_getptr") {
    return reinterpret_cast<void *>(do_getptr);
  }
  if (fnName == "do_getptra") {
    return reinterpret_cast<void *>(do_getptra);
  }
  if (fnName == "do_getptrai") {
    return reinterpret_cast<void *>(do_getptrai);
  }
  if (fnName == "do_mv") {
    return reinterpret_cast<void *>(do_mv);
  }
  if (fnName == "do_mvi") {
    return reinterpret_cast<void *>(do_mvi);
  }
  if (fnName == "do_add") {
    return reinterpret_cast<void *>(do_add);
  }
  if (fnName == "do_addi") {
    return reinterpret_cast<void *>(do_addi);
  }
  if (fnName == "do_sremi") {
    return reinterpret_cast<void *>(do_sremi);
  }
  if (fnName == "do_shli") {
    return reinterpret_cast<void *>(do_shli);
  }
  if (fnName == "do_andi") {
    return reinterpret_cast<void *>(do_andi);
  }
  if (fnName == "do_srem") {
    return reinterpret_cast<void *>(do_srem);
  }
  if (fnName == "do_select") {
    return reinterpret_cast<void *>(do_select);
  }
  if (fnName == "do_selecti") {
    return reinterpret_cast<void *>(do_selecti);
  }
  if (fnName == "do_icmps") {
    return reinterpret_cast<void *>(do_icmps);
  }
  if (fnName == "do_icmpl") {
    return reinterpret_cast<void *>(do_icmpl);
  }
  if (fnName == "do_icmpeq") {
    return reinterpret_cast<void *>(do_icmpeq);
  }
  if (fnName == "do_alloca") {
    return reinterpret_cast<void *>(do_alloca);
  }
  if (fnName == "do_load") {
    return reinterpret_cast<void *>(do_load);
  }
  if (fnName == "do_store") {
    return reinterpret_cast<void *>(do_store);
  }
  if (fnName == "do_br") {
    return reinterpret_cast<void *>(do_br);
  }
  if (fnName == "do_brcond") {
    return reinterpret_cast<void *>(do_brcond);
  }
  if (fnName == "do_call") {
    return reinterpret_cast<void *>(do_call);
  }
  if (fnName == "do_ret") {
    return reinterpret_cast<void *>(do_ret);
  }
  if (fnName == "do_simflush") {
    return reinterpret_cast<void *>(do_simflush);
  }
  if (fnName == "do_simputpixel") {
    return reinterpret_cast<void *>(do_simputpixel);
  }
  if (fnName == "do_memcpy") {
    return reinterpret_cast<void *>(do_memcpy);
  }
  return nullptr;
}

//////////////////////////////
// MAIN
//////////////////////////////

int main(int argc, char *argv[]) {
  if (argc != 2) {
    outs() << "[ERROR] Need 1 argument: file with assembler\n";
    return 1;
  }
  std::ifstream input;
  input.open(argv[1]);
  if (!input.is_open()) {
    outs() << "[ERROR] Can't open " << argv[1] << "\n";
    return 1;
  }

  std::string name;
  std::string arg;
  std::unordered_map<std::string, RegVal_t> BB_PC;

  outs() << "\n#[FILE]:\nBBs:";
  RegVal_t pc = 1;
  while (input >> name) {
    // 4 args
    if (!name.compare("ADICBR") || !name.compare("ADICBRS") || 
        !name.compare("ICBR") || !name.compare("SELECT") || 
        !name.compare("SELECTI")) {
      input >> arg >> arg >> arg >> arg;
      pc++;
      continue;
    }
    // 3 args
    if (!name.compare("LANCO") || !name.compare("ADAN") || 
        !name.compare("GETPTR") || !name.compare("GETPTRA") || 
        !name.compare("GETPTRAI") || !name.compare("ADD") || 
        !name.compare("ADDI") || !name.compare("SREM") ||
        !name.compare("SREMI") || !name.compare("SHLI") ||
        !name.compare("ANDI") || !name.compare("OR") ||
        !name.compare("ICMPS") || !name.compare("ICMPL") || !name.compare("ICMPEQ") ||
        !name.compare("BRCOND") || !name.compare("MEMCPY")) {
      input >> arg >> arg >> arg;
      pc++;
      continue;
    }
    // 2 args
    if (!name.compare("MV") || !name.compare("MVI") || !name.compare("ALLOCA") || 
        !name.compare("LOAD") || !name.compare("STORE")) {
      input >> arg >> arg;
      pc++;
      continue;
    }
    // 1 arg
    if (!name.compare("PUSH") || !name.compare("POP") || 
        !name.compare("BR") || !name.compare("CALL")) {
      input >> arg;
      pc++;
      continue;
    }
    // 0 args
    if (!name.compare("EXIT") || !name.compare("RET") || 
        !name.compare("SIMFLUSH") || !name.compare("SIMPUTPIXEL")) {
      pc++;
      continue;
    }

    outs() << " " << name << "(" << pc << ")";
    BB_PC[name] = pc;
  }
  outs() << "\n";
  input.close();
  input.open(argv[1]);

  std::string arg1;
  std::string arg2;
  std::string arg3;
  std::string arg4;
  std::vector<Instr *> Instructions;
  Instructions.push_back(
      new Instr(InsnId_t::EXIT, do_exit, "[RUNTIME ERROR] Segmentation fault"));
  // Read instruction from file
  outs() << "#[FILE] BEGIN\n";

  while (input >> name) {
    outs() << name;
    // void
    if (!name.compare("EXIT")) {
      Instructions.push_back(new Instr(InsnId_t::EXIT, do_exit, name));
      outs() << "\n";
      continue;
    }
    if (!name.compare("RET")) {
      Instructions.push_back(new Instr(InsnId_t::RET, do_ret, name));
      outs() << "\n";
      continue;
    }
    if (!name.compare("SIMFLUSH")) {
      Instructions.push_back(new Instr(InsnId_t::SIMFLUSH, do_simflush, name));
      outs() << "\n";
      continue;
    }
    if (!name.compare("SIMPUTPIXEL")) {
      Instructions.push_back(new Instr(InsnId_t::SIMPUTPIXEL, do_simputpixel, name));
      outs() << "\n";
      continue;
    }

    // 1 label
    if (!name.compare("BR") || !name.compare("CALL")) {
      input >> arg1;
      outs() << " " << arg1 << "\n";
      RegVal_t imm1 = BB_PC[arg1];
      if (!name.compare("BR")) {
        Instructions.push_back(new Instr(InsnId_t::BR, do_br, name, imm1));
      }
      if (!name.compare("CALL")) {
        Instructions.push_back(new Instr(InsnId_t::CALL, do_call, name, imm1));
      }
      continue;
    }

    // 1 register
    if (!name.compare("PUSH") || !name.compare("POP")) {
      input >> arg1;
      outs() << " " << arg1 << "\n";
      RegId_t rs1 = stoi(arg1.substr(1));
      if (!name.compare("PUSH")) {
        Instructions.push_back(new Instr(InsnId_t::PUSH, do_push, name, rs1));
      }
      if (!name.compare("POP")) {
        Instructions.push_back(new Instr(InsnId_t::POP, do_pop, name, rs1));
      }
      continue;
    }

    // 1 register, 1 immediate
    if (!name.compare("MVI") || !name.compare("ALLOCA") ||
        !name.compare("STORE")) {
      input >> arg1 >> arg2;
      outs() << " " << arg1 << " " << arg2 << "\n";
      RegId_t rs1 = stoi(arg1.substr(1));
      RegVal_t imm1 = stoi(arg2);
      if (!name.compare("MVI")) {
        Instructions.push_back(new Instr(InsnId_t::MVI, do_mvi, name, rs1, imm1));
      }
      if (!name.compare("ALLOCA")) {
        Instructions.push_back(new Instr(InsnId_t::ALLOCA, do_alloca, name, rs1, imm1));
      }
      if (!name.compare("STORE")) {
        Instructions.push_back(new Instr(InsnId_t::STORE, do_store, name, rs1, imm1));
      }
      continue;
    }

    // 1 register, 2 labels
    if (!name.compare("BRCOND")) {
      input >> arg1 >> arg2 >> arg3;
      outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
      RegId_t rs1 = stoi(arg1.substr(1));
      RegVal_t imm1 = BB_PC[arg2];
      RegVal_t imm2 = BB_PC[arg3];
      Instructions.push_back(new Instr(InsnId_t::BRCOND, do_brcond, name, rs1, imm1, imm2));
      continue;
    }

    // 1 register, 1 immediate, 2 labels
    if (!name.compare("ADICBR") || !name.compare("ADICBRS") ||
        !name.compare("ICBR")) {
      input >> arg1 >> arg2 >> arg3 >> arg4;
      outs() << " " << arg1 << " " << arg2 << " " << arg3 << " " << arg4 << "\n";
      RegId_t rs1 = stoi(arg1.substr(1));
      RegVal_t imm1 = stoi(arg2);
      RegVal_t imm2 = BB_PC[arg3];
      RegVal_t imm3 = BB_PC[arg4];
      if (!name.compare("ADICBR")) {
        Instructions.push_back(new Instr(InsnId_t::ADICBR, do_adicbr, name, rs1, imm1, imm2, imm3));
      }
      if (!name.compare("ADICBRS")) {
        Instructions.push_back(new Instr(InsnId_t::ADICBRS, do_adicbrs, name, rs1, imm1, imm2, imm3));
      }
      if (!name.compare("ICBR")) {
        Instructions.push_back(new Instr(InsnId_t::ICBR, do_icbr, name, rs1, imm1, imm2, imm3));
      }
      continue;
    }

    // 2 registers
    if (!name.compare("LOAD") || !name.compare("MV")) {
      input >> arg1 >> arg2;
      outs() << " " << arg1 << " " << arg2 << "\n";
      RegId_t rs1 = stoi(arg1.substr(1));
      RegId_t rs2 = stoi(arg2.substr(1));
      if (!name.compare("MV")) {
        Instructions.push_back(new Instr(InsnId_t::MV, do_mv, name, rs1, rs2));
      }
      if (!name.compare("LOAD")) {
        Instructions.push_back(new Instr(InsnId_t::LOAD, do_load, name, rs1, rs2));
      }
      continue;
    }

    // 2 registers, 1 immediate
    if (!name.compare("ADAN") || !name.compare("GETPTRAI") ||
        !name.compare("ADDI") || !name.compare("SREMI") ||
        !name.compare("SHLI") || !name.compare("ANDI") ||
        !name.compare("ICMPS") || !name.compare("ICMPL") || !name.compare("ICMPEQ") || 
        !name.compare("MEMCPY")) {
      input >> arg1 >> arg2 >> arg3;
      outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
      RegId_t rs1 = stoi(arg1.substr(1));
      RegId_t rs2 = stoi(arg2.substr(1));
      RegVal_t imm1 = stoi(arg3);
      if (!name.compare("ADAN")) {
        Instructions.push_back(
            new Instr(InsnId_t::ADAN, do_adan, name, rs1, rs2, imm1));
      }
      if (!name.compare("GETPTRAI")) {
        Instructions.push_back(
            new Instr(InsnId_t::GETPTRAI, do_getptrai, name, rs1, rs2, imm1));
      }
      if (!name.compare("ADDI")) {
        Instructions.push_back(
            new Instr(InsnId_t::ADDI, do_addi, name, rs1, rs2, imm1));
      }
      if (!name.compare("SREMI")) {
        Instructions.push_back(
            new Instr(InsnId_t::SREMI, do_sremi, name, rs1, rs2, imm1));
      }
      if (!name.compare("SHLI")) {
        Instructions.push_back(
            new Instr(InsnId_t::SHLI, do_shli, name, rs1, rs2, imm1));
      }
      if (!name.compare("ANDI")) {
        Instructions.push_back(
            new Instr(InsnId_t::ANDI, do_andi, name, rs1, rs2, imm1));
      }
      if (!name.compare("ICMPS")) {
        Instructions.push_back(
            new Instr(InsnId_t::ICMPS, do_icmps, name, rs1, rs2, imm1));
      }
      if (!name.compare("ICMPL")) {
        Instructions.push_back(
            new Instr(InsnId_t::ICMPL, do_icmpl, name, rs1, rs2, imm1));
      }
      if (!name.compare("ICMPEQ")) {
        Instructions.push_back(
            new Instr(InsnId_t::ICMPEQ, do_icmpeq, name, rs1, rs2, imm1));
      }
      if (!name.compare("MEMCPY")) {
        Instructions.push_back(
            new Instr(InsnId_t::MEMCPY, do_memcpy, name, rs1, rs2, imm1));
      }
      continue;
    }

    // 2 registers, 2 immediate
    if (!name.compare("SELECTI")) {
      input >> arg1 >> arg2 >> arg3 >> arg4;
      outs() << " " << arg1 << " " << arg2 << " " << arg3 <<  " " << arg4 << "\n";
      RegId_t rs1 = stoi(arg1.substr(1));
      RegId_t rs2 = stoi(arg2.substr(1));
      RegVal_t imm1 = stoi(arg3);
      RegVal_t imm2 = stoi(arg4);
      Instructions.push_back(new Instr(InsnId_t::SELECTI, do_selecti, name, rs1, rs2, imm1, imm2));
      continue;
    }

    // 3 registers
    if (!name.compare("LANCO") || !name.compare("ADD") || 
        !name.compare("GETPTR") || !name.compare("GETPTRA") || 
        !name.compare("SREM") || !name.compare("OR")) {
      input >> arg1 >> arg2 >> arg3;
      outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
      RegId_t rs1 = stoi(arg1.substr(1));
      RegId_t rs2 = stoi(arg2.substr(1));
      RegId_t rs3 = stoi(arg3.substr(1));
      if (!name.compare("LANCO")) {
        Instructions.push_back(
            new Instr(InsnId_t::LANCO, do_lanco, name, rs1, rs2, rs3));
      }
      if (!name.compare("ADD")) {
        Instructions.push_back(
            new Instr(InsnId_t::ADD, do_add, name, rs1, rs2, rs3));
      }
      if (!name.compare("GETPTR")) {
        Instructions.push_back(
            new Instr(InsnId_t::GETPTR, do_getptr, name, rs1, rs2, rs3));
      }
      if (!name.compare("GETPTRA")) {
        Instructions.push_back(
            new Instr(InsnId_t::GETPTRA, do_getptra, name, rs1, rs2, rs3));
      }
      if (!name.compare("SREM")) {
        Instructions.push_back(
            new Instr(InsnId_t::SREM, do_srem, name, rs1, rs2, rs3));
      }
      if (!name.compare("OR")) {
        Instructions.push_back(
            new Instr(InsnId_t::OR, do_or, name, rs1, rs2, rs3));
      }
      continue;
    }

    // 4 registers
    if (!name.compare("SELECT")) {
      input >> arg1 >> arg2 >> arg3 >> arg4;
      outs() << " " << arg1 << " " << arg2 << " " << arg3 << " " << arg4 << "\n";
      RegId_t rs1 = stoi(arg1.substr(1));
      RegId_t rs2 = stoi(arg2.substr(1));
      RegId_t rs3 = stoi(arg3.substr(1));
      RegId_t rs4 = stoi(arg3.substr(1));
      Instructions.push_back(new Instr(InsnId_t::SELECT, do_select, name, rs1, rs2, rs3, rs4));
      continue;
    }

    if (BB_PC.find(name) == BB_PC.end()) {
      outs() << "\n[ERROR] Unknown instruction: " << name << "\n";
      Instructions.clear();
      return 1;
    }
    outs() << "\n";
  }
  outs() << "#[FILE] END\n";

  // App simulation
  CPU cpu(SIM_X_SIZE * SIM_Y_SIZE * 2);
  for (int i = 0; i < REG_FILE_SIZE; i++) {
    cpu.REG_FILE[i] = 0;
  }
  cpu.RUN = 1;
  cpu.PC = 1;
  cpu.next_memory_ptr = 0;
  outs() << "\n#[EXEC] BEGIN\n";
  simInit();
  // Loop execution
  while (cpu.RUN) {
    cpu.NEXT_PC = cpu.PC + 1;
    Instructions[cpu.PC]->m_INSTR(&cpu, Instructions[cpu.PC]);
//    std::cout << "PC: " << cpu.PC << " NEXT_PC: " << cpu.NEXT_PC << std::endl;
    cpu.PC = cpu.NEXT_PC;
  }
  simExit();
  outs() << "#[EXEC] END\n";

  // Dump registers after simulation
  for (int i = 0; i < REG_FILE_SIZE; i++) {
    outs() << "[" << i << "] " << cpu.REG_FILE[i] << "\n";
  }

  // Build IR for application
  outs() << "#[LLVM IR] BEGIN\n";
  LLVMContext context;
  // ; ModuleID = 'top'
  // source_filename = "top"
  Module *module = new Module("top", context);
  IRBuilder<> builder(context);

  // declare void @main()
  FunctionType *funcType = FunctionType::get(builder.getVoidTy(), false);
  Function *mainFunc =
      Function::Create(funcType, Function::ExternalLinkage, "main", module);
  // entry:
  BasicBlock *entryBB = BasicBlock::Create(context, "entry", mainFunc);

  builder.SetInsertPoint(entryBB);

  // createCalleeFunctions(builder, module);
  FunctionType *CalleeType = FunctionType::get(
      builder.getVoidTy(),
      ArrayRef<Type *>({builder.getInt8PtrTy(), builder.getInt8PtrTy()}),
      false);

  // Get pointer to CPU for function args
  Value *cpu_p = builder.getInt64((uint64_t)&cpu);
  ArrayType *regFileType = ArrayType::get(builder.getInt32Ty(), REG_FILE_SIZE);
  module->getOrInsertGlobal("regFile", regFileType);
  GlobalVariable *regFile = module->getNamedGlobal("regFile");

  std::unordered_map<RegVal_t, BasicBlock *> BBMap;

  for (auto &name : BB_PC) {
    BBMap[name.second] = BasicBlock::Create(context, name.first, mainFunc);
  }

  std::cout << "Instructions.size(): " << Instructions.size() << std::endl;  
  for (RegVal_t PC = 1; PC < Instructions.size(); PC++) {
    std::cout << "PC: " << PC << ", name: " << Instructions[PC]->m_name << std::endl;
    // Set IRBuilder to current BB
    if (BBMap.find(PC) != BBMap.end()) {
      builder.CreateBr(BBMap[PC]);
      builder.SetInsertPoint(BBMap[PC]);
    }
    // IR implementation for BR instruction
    if (Instructions[PC]->m_ID == BR) {
      builder.CreateBr(BBMap[Instructions[PC]->m_imm1]);
      continue;
    }
    // IR implementation for BRCOND instruction
    if (Instructions[PC]->m_ID == BRCOND) {
      // arg1
      Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs1);
      Value *cond = builder.CreateCondBr(builder.CreateLoad(builder.getInt32Ty(), arg1_p), 
                              BBMap[Instructions[PC]->m_imm1],
                              BBMap[Instructions[PC]->m_imm2]);
      continue;
    }
    // IR implementation for ICBR instruction
    if (Instructions[PC]->m_ID == ICBR) {
      // arg1
      Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs1);
      Value *arg2 = builder.getInt32(Instructions[PC]->m_imm1);
      Value *cond = builder.CreateICmpEQ(
            builder.CreateLoad(builder.getInt32Ty(), arg1_p), arg2);
      if (cond) {
        builder.CreateCondBr(cond, BBMap[Instructions[PC]->m_imm2],
                                   BBMap[Instructions[PC]->m_imm3]);
      }
      continue;
    }
    // IR implementation for ICMPS instruction
    if (Instructions[PC]->m_ID == ICMPS) {
      Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                Instructions[PC]->m_rs1);
      Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs2);
      Value *arg2 = builder.getInt32(Instructions[PC]->m_imm1);
      Value *cond = builder.CreateICmpSGT(
            builder.CreateLoad(builder.getInt32Ty(), arg1_p), arg2);
      builder.CreateStore(cond, res_p);
    }
    // IR implementation for ICMPL instruction
    if (Instructions[PC]->m_ID == ICMPL) {
      Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                Instructions[PC]->m_rs1);
      Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs2);
      Value *arg2 = builder.getInt32(Instructions[PC]->m_imm1);
      Value *cond = builder.CreateICmpSLT(
            builder.CreateLoad(builder.getInt32Ty(), arg1_p), arg2);
      builder.CreateStore(cond, res_p);
    }
    // IR implementation for ICMPEQ instruction
    if (Instructions[PC]->m_ID == ICMPEQ) {
      Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                Instructions[PC]->m_rs1);
      Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs2);
      Value *arg2 = builder.getInt32(Instructions[PC]->m_imm1);
      Value *cond = builder.CreateICmpEQ(
            builder.CreateLoad(builder.getInt32Ty(), arg1_p), arg2);
      builder.CreateStore(cond, res_p);
      continue;
    }
    // IR implementation for OP instruction
    if (Instructions[PC]->m_ID == ADD || Instructions[PC]->m_ID == SREM || 
        Instructions[PC]->m_ID == OR) {
      // res
      Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                Instructions[PC]->m_rs1);
      // arg1
      Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs2);
      // arg2
      Value *arg2_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs3);
      Value *op_arg1_arg2; 

      if (Instructions[PC]->m_ID == ADD) {
        op_arg1_arg2 = builder.CreateAdd(builder.CreateLoad(builder.getInt32Ty(), arg1_p),
                            builder.CreateLoad(builder.getInt32Ty(), arg2_p));
      }
      if (Instructions[PC]->m_ID == SREM) {
        op_arg1_arg2 = builder.CreateSRem(builder.CreateLoad(builder.getInt32Ty(), arg1_p),
                            builder.CreateLoad(builder.getInt32Ty(), arg2_p));
      }
      if (Instructions[PC]->m_ID == OR) {
        op_arg1_arg2 = builder.CreateOr(builder.CreateLoad(builder.getInt32Ty(), arg1_p),
                            builder.CreateLoad(builder.getInt32Ty(), arg2_p));
      }
      builder.CreateStore(op_arg1_arg2, res_p);
      continue;
    }
    // IR implementation for OPI instruction
    if (Instructions[PC]->m_ID == ADDI || Instructions[PC]->m_ID == SREMI ||
        Instructions[PC]->m_ID == SHLI || Instructions[PC]->m_ID == ANDI) {
      // res
      Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                Instructions[PC]->m_rs1);
      // arg1
      Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs2);
      // arg2
      Value *arg2 = builder.getInt32(Instructions[PC]->m_imm1);
      Value *op_arg1_arg2;
      if (Instructions[PC]->m_ID == ADDI) {
        op_arg1_arg2 = builder.CreateAdd(
          builder.CreateLoad(builder.getInt32Ty(), arg1_p), arg2);
      }
      if (Instructions[PC]->m_ID == SREMI) {
        op_arg1_arg2 = builder.CreateSRem(
          builder.CreateLoad(builder.getInt32Ty(), arg1_p), arg2);
      }
      if (Instructions[PC]->m_ID == SHLI) {
        op_arg1_arg2 = builder.CreateShl(
          builder.CreateLoad(builder.getInt32Ty(), arg1_p), arg2);
      }
      if (Instructions[PC]->m_ID == ANDI) {
        op_arg1_arg2 = builder.CreateAnd(
          builder.CreateLoad(builder.getInt32Ty(), arg1_p), arg2);
      }
      builder.CreateStore(op_arg1_arg2, res_p);
      continue;
    }
    // IR implementation for SELECT instruction
    if (Instructions[PC]->m_ID == SELECT) {
      Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                Instructions[PC]->m_rs1);
      Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs2);
      Value *arg2_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs3);
      Value *arg3_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs4);
      Value *op_args = builder.CreateSelect(
                            builder.CreateLoad(builder.getInt32Ty(), arg1_p),
                            builder.CreateLoad(builder.getInt32Ty(), arg2_p),
                            builder.CreateLoad(builder.getInt32Ty(), arg3_p));
      builder.CreateStore(op_args, res_p);
      continue;
    }
    // IR implementation for SELECTI instruction
    if (Instructions[PC]->m_ID == SELECTI) {
      Value *res_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                Instructions[PC]->m_rs1);
      Value *arg1_p = builder.CreateConstGEP2_32(regFileType, regFile, 0,
                                                 Instructions[PC]->m_rs2);
      Value *arg2 = builder.getInt32(Instructions[PC]->m_imm1);
      Value *arg3 = builder.getInt32(Instructions[PC]->m_imm2);
      Value *op_args = builder.CreateSelect(
                            builder.CreateLoad(builder.getInt32Ty(), arg1_p),
                              arg2, arg3);
      builder.CreateStore(op_args, res_p);
      continue;
    }
    // Get pointer to instruction for function args
    Value *instr_p = builder.getInt64((uint64_t)Instructions[PC]);
    // Call simulation function for other instructions
    std::string lower_inst = Instructions[PC]->m_name;
    std::transform(lower_inst.begin(), lower_inst.end(), lower_inst.begin(), ::tolower);
    builder.CreateCall(module->getOrInsertFunction(
                           "do_" + lower_inst, CalleeType),
                       ArrayRef<Value *>({cpu_p, instr_p}));
  }
  // ret i32 0
//  builder.CreateRet(builder.getInt32(0));

  outs() << "#[LLVM IR] DUMP\n";
//  std::ostream llfile = std::ostream::open("app.ll", "w");
//  module->print(llfile, nullptr);
//  std::ostream::close(llfile);
  module->print(errs(), nullptr);
  module->print(outs(), nullptr);
  outs() << "#[LLVM IR] END\n";
  for (int i = 0; i < REG_FILE_SIZE; i++) {
    cpu.REG_FILE[i] = 0;
  }

  // App simulation with execution engine
  outs() << "#[LLVM EE] RUN\n";
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
  ee->InstallLazyFunctionCreator(lazyFunctionCreator);
  ee->addGlobalMapping(regFile, (void *)cpu.REG_FILE);
  ee->finalizeObject();
  ArrayRef<GenericValue> noargs;

  cpu.RUN = 1;
  cpu.PC = 1;
  outs() << "#[LLVM EE] ALL PREPARED\n";
  simInit();
  ee->runFunction(mainFunc, noargs);
  simExit();
  outs() << "#[LLVM EE] END\n";

  // Registers dump after simulation with EE
  for (int i = 0; i < REG_FILE_SIZE; i++) {
    outs() << "[" << i << "] " << cpu.REG_FILE[i] << "\n";
  }

  Instructions.clear();
  return 0;
}