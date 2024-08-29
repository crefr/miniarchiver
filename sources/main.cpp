#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "archiver.h"

const int MAXFILENAME = 100;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("incorrect cmd args, type e to encode and d to decode\n");
        return -1;
    }

    char infname [MAXFILENAME] = {};
    char outfname[MAXFILENAME] = {};

    printf("type name of in file\n");
    scanf("%s", infname);

    printf("type name of out file\n");
    scanf("%s", outfname);

    FILE *in  = fopen(infname,  "rb");
    FILE *out = fopen(outfname, "wb");

    assert(in  != NULL);
    assert(out != NULL);

    if (argv[1][0] == 'e')
    {
        encode(in, out);
    }
    else if (argv[1][0] == 'd')
    {
        if (decode(in, out) == -1)
        {
            printf("wrong input file format\n");
            return -1;
        }
    }
    else
    {
        printf("incorrect cmd args, type e to encode and d to decode\n");
        return -1;
    }
    fclose(in);
    fclose(out);
    return 0;
}
