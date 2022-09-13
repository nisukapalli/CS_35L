/* Initialize the software rand64 implementation.  */
void software_rand64_init (char* filename);

/* Return a random value, using software operations.  */
unsigned long long software_rand64 (void);
unsigned long long software_mrand48 (void);

/* Finalize the software rand64 implementation.  */
void software_rand64_fini (void);
