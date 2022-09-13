#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>

#include "output.h"
#include "rand64-hw.h"
#include "rand64-sw.h"

bool writebytes (unsigned long long x, int nbytes)
{
  do
    {
      if (putchar (x) < 0)
        return false;
      x >>= CHAR_BIT;
      nbytes--;
    }
  while (0 < nbytes);

  return true;
}

void pr_out(int o, long long nbytes, int* output_errno,
	    int wordsize, unsigned long long (*rand64)(void))
{
  if (o > 0) {
    int count = 0, i = 0;
    while (i < o) {
      count++;
      i += wordsize;
    }
    int totalsize = count * wordsize;

    do {
      unsigned long long *mem = malloc(totalsize);
      if (mem == NULL) {
        fprintf(stderr, "memory could not be allocated");
        exit(1);
      }
      int j = 0;
      while (j < o) {
        mem[j] = rand64();
	j += wordsize;
      }
      if (o >= nbytes) {
	write(1, mem, nbytes);
	nbytes = 0;
      }
      else {
	write(1, mem, o);
	nbytes -= o;
      }

      free(mem);
	
    } while (0 < nbytes);
  }

  else {
    do {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes(x, outbytes)) {
        *output_errno = errno;
        break;
      }
      nbytes -= outbytes;
    } while (0 < nbytes);
  }
      
  if (fclose (stdout) != 0)
    *output_errno = errno;

  if (output_errno)
    {
     errno = *output_errno;
   }
}
