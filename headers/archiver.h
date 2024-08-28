#ifndef ARCHIVER_INCLUDED
#define ARCHIVER_INCLUDED

void encode(FILE* infile, FILE* outfile);
unsigned char codedByte(bool isrep, unsigned char count);

#endif
