//main.cpp
#include "timer.h"

extern int userMain(int argc, char* argv[]);
int main(int argc, char* argv[]) {
 Timer::inicSystem();
 int value = userMain(argc, argv);
 Timer::restoreSystem();
 return value;
}
