#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "options.h"
#include "rand64-hw.h"

_Bool validInput(int argc, char** argv, long long *nbytes) {
  if (argc >= 2) {
    char *endptr;
    errno = 0;
    *nbytes = strtoll(argv[argc-1], &endptr, 10);
    if (!errno)
      return true;
  }
  return false;
}


_Bool processOptions(int argc, char** argv, struct OptStruct *ptr_opts) {

  /* Check arguments.  */

  char *endptr;

  if (argc == 2)
  {
    errno = 0;
    ptr_opts->nbytes = strtoll (argv[1], &endptr, 10);
    if (errno)
      perror (argv[1]);
  }
  else
  {
    int c;
    while ((c = getopt(argc, argv, "i:o:")) != -1) {
      switch(c) {
        case 'i':
          if (strcmp(optarg, "rdrand") == 0 ) {
	    if (rdrand_supported()) {
	      ptr_opts->i = 0;
	    }
	    else {
	      fprintf(stderr, "rdrand is not available");
	      return false;
	    }
      	  }
	  
	  else if (strcmp(optarg, "mrand48_r") == 0) {
	    ptr_opts->i = 1;
	  }
	  
	  else if (optarg[0] == '/') {
	    ptr_opts->i = 2; 
	    ptr_opts->file_path = optarg;
	  }
	  
	  else
	    return false;

	  break;
	  
        case 'o':
	  if (strcmp(optarg, "stdio") == 0) {
	    ptr_opts->o = -1;
	  }
	  else {
	    ptr_opts->o = atoi(optarg);
	  }
          break;
      }
    }
    ptr_opts->nbytes = strtoll(argv[argc-1], &endptr, 10);
  }
  return true;
}
