#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rand64-sw.h"

/* Software implementation.  */


/* Input stream containing random bytes.  */
FILE *urandstream;

/* Initialize the software rand64 implementation.  */
void
software_rand64_init (char* filename)
{

  urandstream = fopen (filename, "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
unsigned long long
software_rand64 (void)
{ 
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

unsigned long long
software_mrand48 (void)
{
  struct drand48_data buf;
  long int result1, result2;
  long int seed = (long int) (time(NULL));

  mrand48_r(&buf, &result1);
  mrand48_r(&buf, &result2);
  srand48_r(seed, &buf);
  
  return (result1 << 32) | result2;
}

/* Finalize the software rand64 implementation.  */
void
software_rand64_fini (void)
{
  fclose (urandstream);
}
