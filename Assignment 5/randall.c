/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */


#include <stdbool.h>
#include <errno.h>
#include <stdio.h>

#include "output.h"
#include "options.h"
#include "rand64-hw.h"
#include "rand64-sw.h"


/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{

  struct OptStruct opts;
  struct OptStruct *ptr_opts = &opts;

  ptr_opts->i = 0;
  ptr_opts->o = -1;
  ptr_opts->file_path = "";

  bool valid = validInput(argc, argv, &ptr_opts->nbytes);
  bool validOpts = processOptions(argc, argv, ptr_opts);

  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  if (!validOpts) {
    fprintf (stderr, "invalid input");
    exit(1);
  }

  /* If there's no work to do, don't worry about which library to use.  */
  if (ptr_opts->nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */

  void (*initialize) (char* file);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  if (ptr_opts->i == 0)
    {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
  else if (ptr_opts->i == 1)
    {
      initialize = hardware_rand64_init;
      rand64 = software_mrand48;
      finalize = hardware_rand64_fini;
    }
  else if (ptr_opts->i == 2)
  {
      initialize = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
  }

  int output_errno = 0;
  int wordsize = sizeof rand64 ();
  initialize(ptr_opts->file_path);
  pr_out(ptr_opts->o, ptr_opts->nbytes, &output_errno, wordsize, (*rand64));

  finalize ();

  return !!output_errno;
}
