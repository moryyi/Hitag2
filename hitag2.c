/*    
    One implementation of Hitag2 algorithm.
    This program is the part of keystream generation.
    
    Problems still remains:
        1. Storage of keystream is not ideal;
        2. I didn't figure out a good way to actually encrypt plaintext bit-by-bit;
    
    Future refactor is required:
        1. New storage method of variables may be involved.

    Author  : Song
    Date    : 2018/11/2    
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int C1 = 0xA63C;
const int C2 = 0xA770;
const int C3 = 0xD949CBB0;

typedef unsigned short byte;

///
byte r[48] = { 0 };
byte id[32] = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
byte iv[32] = { 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 };
byte key[48] = { 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0 };

byte faa(byte n);
byte fbb(byte n);
byte fcc(byte n);

byte fa(byte x1, byte x2, byte x3, byte x4);
byte fb(byte x1, byte x2, byte x3, byte x4);
byte fc(byte x1, byte x2, byte x3, byte x4, byte x5);


void shl1(byte *state, byte b);
void nextKeyStream();

byte l(byte a[]);
byte f(byte a[]);

void initialize();
byte get1KeyStream();


int
main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage\n\t%s [length]\n", argv[0]);
        return 0;
    }
    int arg = strtol(argv[1], NULL, 10);
    initialize();
    int i;
    for (i = 0; i < arg; i++) {
        printf("%d", get1KeyStream());
        nextKeyStream();
    }
    printf("\n");
    return 0;
}

/// Hitag2 algorithm initialization
void
initialize() {
    memcpy(r, id, 32);
    memcpy(r + 32, key, 16);

    int i;
    for (i = 0; i < 32; i++) {
        byte b = key[16 + i] ^ iv[i] ^ f(r);
        shl1(r, b);
    }
}

/// Hitag2 algorithm starts
byte
get1KeyStream() {
    return f(r);
}

/// Subfunctions
void
nextKeyStream() {
    byte b = l(r);
    shl1(r, b);
}

byte
l(byte a[]) {
    return (a[0] ^ a[2] ^ a[3] ^ a[6] ^ a[7] ^ a[8]
            ^ a[16] ^ a[22] ^ a[23] ^ a[26] ^ a[30]
            ^ a[41] ^ a[42] ^ a[43] ^ a[46] ^ a[47]) & 0x1;
}

byte
f(byte a[]) {
    return fc(
        fa(a[2], a[3], a[5], a[6]),
        fb(a[8], a[12], a[14], a[15]),
        fb(a[17], a[21], a[23], a[26]),
        fb(a[28], a[29], a[31], a[33]),
        fa(a[34], a[43], a[44], a[46])
    );
}

byte
faa(byte n) {
    return (C1 >> (15 - n)) & 0x1;
}

byte
fbb(byte n) {
    return (C2 >> (15 - n)) & 0x1;
}

byte
fcc(byte n) {
    return (C3 >> (31 - n)) & 0x1;
}

byte
fa(byte x1, byte x2, byte x3, byte x4) {
    return faa(((x1 & 0x1) << 3)
            + ((x2 & 0x1) << 2)
            + ((x3 & 0x1) << 1)
            + (x4 & 0x1));
}

byte
fb(byte x1, byte x2, byte x3, byte x4) {
    return fbb(((x1 & 0x1) << 3)
            + ((x2 & 0x1) << 2)
            + ((x3 & 0x1) << 1)
            + (x4 & 0x1));
}

byte
fc(byte x1, byte x2, byte x3, byte x4, byte x5) {
    return fcc(((x1 & 0x1) << 4)
            + ((x2 & 0x1) << 3)
            + ((x3 & 0x1) << 2)
            + ((x4 & 0x1) << 1)
            + (x5 & 0x1));
}

void
shl1(byte *state, byte b) {
    int i;
    for (i = 1; i < 48; i++) {
        state[i-1] = state[i];
    }
    state[47] = b;
}

