#ifndef OUTPUT
#define OUTPUT

bool writebytes (unsigned long long x, int nbytes);
void pr_out(int o, long long nbytes, int* output_errno,
	    int wordsize, unsigned long long (*rand64)(void));

#endif
