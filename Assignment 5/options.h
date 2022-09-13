#ifndef OPTIONS
#define OPTIONS

#include <stdbool.h>

struct OptStruct {
  int i;
  int o;
  char* file_path;
  long long nbytes;
};

_Bool validInput(int argc, char** argv, long long *nbytes);

_Bool processOptions(int argc, char** argv, struct OptStruct *ptr_opts);

#endif
