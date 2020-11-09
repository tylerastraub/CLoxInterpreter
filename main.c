#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

#include <stdio.h>

int main(int argc, const char* argv[]) {
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    writeConstant(&chunk, 1.2, 123);
    writeConstant(&chunk, 1.7, 123);
    writeChunk(&chunk, OP_RETURN, 127);

    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);
    freeVM();
    freeChunk(&chunk);
    
    return 0;
}

// Left of on section 15:3: A value stack manipulator