// File from llvm_course: https://github.com/lisitsynSA/llvm_course/blob/main/SDL/sim.c

#define SIM_X_SIZE 512
#define SIM_Y_SIZE 256

void simFlush();
void simPutPixel(int x, int y, int argb);
int simRand();