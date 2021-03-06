#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;

    int lineCount;
    int lineCapacity;
    int* lines; // encoded using run length encoding: [occurences, line #, ...]

    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
void writeConstant(Chunk* chunk, Value value, int line);
int addConstant(Chunk* chunk, Value value);
void encodeLine(Chunk* chunk, int line);
void printLines(Chunk* chunk);
int getLine(Chunk* chunk, int offset);

#endif