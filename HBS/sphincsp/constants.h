#include "params.h"

# define SEED_NBYTES 48
# define MSG_NBYTES 33
# define SK_NBYTES 64
# define SIG_NBYTES MSG_NBYTES+SPX_BYTES

void FileReading(FILE *fp, int size, unsigned char value[size]);