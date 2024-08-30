#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "archiver.h"
#include "argvprocessing.h"

const int MAXFILENAME = 100;

int main(int argc, char **argv)
{
    fvals fval[ARGVNUM] = {};
    if (argvReceive(argc, argv, fval) == BAD){
        printf("invalid cmd args\n");
        return BAD;
    }
    if(fval[H].bl || fval[HELP].bl){
        printHelp();
        return GOOD;
    }

    setBytesInblock(6);

    char infname [MAXFILENAME] = {};
    char outfname[MAXFILENAME] = {};

    strcpy(infname, fval[I].str);
    if (fval[O].str[0] != '\0')
        strcpy(outfname, fval[O].str);
    else
    {
        size_t index = strchr(fval[I].str, '.') - fval[I].str;
        strncpy(outfname, fval[I].str, index);
        if (fval[E].bl)
            strcpy(outfname + index, ".cre");
        else
            strcpy(outfname + index, ".txt");
    }

    FILE *in  = fopen(infname,  "rb");
    FILE *out = fopen(outfname, "wb");
    assert(in  != NULL);
    assert(out != NULL);

    if (fval[D].bl && fval[E].bl){
        printf("cmd args error\n");
        return BAD;
    }
    else if (fval[D].bl){
        if (decode(in, out) == -1)
        {
            printf("wrong input file format\n");
            return BAD;
        }
    }
    else if (fval[E].bl){
        encode(in, out);
    }

    fclose(in);
    fclose(out);
    return 0;
}
