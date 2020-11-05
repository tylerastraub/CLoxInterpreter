#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->capacity = 0;
    chunk->count = 0;
    chunk->code = NULL;
    chunk->lineCapacity = 0;
    chunk->lineCount = 0;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if(chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    encodeLine(chunk, line);
    ++chunk->count;
}

void writeConstant(Chunk* chunk, Value value, int line) {
    int constant = addConstant(chunk, value);
    writeChunk(chunk, OP_CONSTANT, line);
    writeChunk(chunk, constant, line);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

void encodeLine(Chunk* chunk, int line) {
    if(chunk->lineCapacity < chunk->lineCount + 2) {
        int oldCapacity = chunk->lineCapacity;
        chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->lineCapacity);
    }

    if(chunk->lineCount == 0) {
        chunk->lines[0] = line;
        chunk->lines[1] = 1;
        chunk->lineCount += 2;
    } else if(chunk->lines[chunk->lineCount - 2] == line) {
        chunk->lines[chunk->lineCount - 1]++;
    } else {
        chunk->lines[chunk->lineCount] = line;
        chunk->lines[chunk->lineCount + 1] = 1;
        chunk->lineCount += 2;
    }
}

int getLine(Chunk* chunk, int offset) {
    for(int i = 1; i < chunk->lineCount; i += 2) {
        for(int j = 0; j < chunk->lines[i]; ++j) {
            if(offset == 0) return chunk->lines[i - 1];
            --offset;
        }
    }
}