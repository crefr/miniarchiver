#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "archiver.h"

int main(int argc, char **argv)
{
    FILE *in  = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    assert(in  != NULL);
    assert(out != NULL);

    encode(in, out);

    return 0;
}
