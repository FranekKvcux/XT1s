#ifndef XT1S_READER_ASSEMBLER_H
#define XT1S_READER_ASSEMBLER_H

#include <stddef.h>
#include <stdint.h>

int load_program(const char *path, uint8_t *ram, size_t ram_size,
                 size_t *program_size);

#endif
