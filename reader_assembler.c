#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader_assembler.h"

static int report_line_error(const char *path, size_t line_number,
                             const char *message)
{
    fprintf(stderr, "%s:%zu: %s\n", path, line_number, message);
    return -1;
}

static int parse_register(const char *token, uint8_t *reg)
{
    if (token[0] != 'r' || token[1] < '0' || token[1] > '3' ||
        token[2] != '\0') {
        return -1;
    }
    *reg = (uint8_t)(token[1] - '0');
    return 0;
}

static int parse_value(const char *token, uint8_t *value)
{
    char *end = NULL;
    int base = token[0] == '0' &&
               (token[1] == 'x' || token[1] == 'X') ? 16 : 10;

    errno = 0;
    long parsed = strtol(token, &end, base);
    if (errno != 0 || end == token || *end != '\0' ||
        parsed < 0 || parsed > (long)UINT8_MAX) {
        return -1;
    }
    *value = (uint8_t)parsed;
    return 0;
}

int load_program(const char *path, uint8_t *ram, size_t ram_size,
                 size_t *program_size)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open '%s'.\n", path);
        return -1;
    }

    char line[256];
    size_t line_number = 0;
    size_t used = 0;
    int result = 0;

    while (fgets(line, sizeof line, file) != NULL) {
        ++line_number;
        if (strchr(line, '\n') == NULL && !feof(file)) {
            int character;
            while ((character = fgetc(file)) != '\n' && character != EOF) {
            }
            result = report_line_error(path, line_number, "line is too long");
            break;
        }

        char *comment = strpbrk(line, "#;");
        if (comment != NULL) {
            *comment = '\0';
        }

        char *tokens[4];
        size_t token_count = 0;
        char *token = strtok(line, " \t\r\n");
        while (token != NULL) {
            if (token_count == sizeof tokens / sizeof tokens[0]) {
                break;
            }
            tokens[token_count++] = token;
            token = strtok(NULL, " \t\r\n");
        }
        if (token_count == 0) {
            continue;
        }
        if (token != NULL) {
            result = report_line_error(path, line_number, "too many operands");
            break;
        }

        uint8_t encoded[3];
        size_t encoded_size = 0;
        uint8_t first_reg;
        uint8_t second_reg;
        uint8_t value;

        if (strcmp(tokens[0], "nop") == 0 && token_count == 1) {
            encoded[0] = 0x00;
            encoded_size = 1;
        } else if (strcmp(tokens[0], "hlt") == 0 && token_count == 1) {
            encoded[0] = 0x01;
            encoded_size = 1;
        } else if (strcmp(tokens[0], "ldi") == 0 && token_count == 3) {
            if (parse_register(tokens[1], &first_reg) != 0 ||
                parse_value(tokens[2], &value) != 0) {
                result = report_line_error(
                    path, line_number, "expected: ldi r0-r3 value (0-255)");
                break;
            }
            encoded[0] = 0x02;
            encoded[1] = first_reg;
            encoded[2] = value;
            encoded_size = 3;
        } else if ((strcmp(tokens[0], "cpy") == 0 ||
                    strcmp(tokens[0], "add") == 0 ||
                    strcmp(tokens[0], "sub") == 0) &&
                   token_count == 3) {
            if (parse_register(tokens[1], &first_reg) != 0 ||
                parse_register(tokens[2], &second_reg) != 0) {
                result = report_line_error(
                    path, line_number, "expected two registers: r0-r3");
                break;
            }
            encoded[0] = strcmp(tokens[0], "cpy") == 0 ? 0x03 :
                         strcmp(tokens[0], "add") == 0 ? 0x04 : 0x05;
            encoded[1] = first_reg;
            encoded[2] = second_reg;
            encoded_size = 3;
        } else if ((strcmp(tokens[0], "lsh") == 0 ||
                    strcmp(tokens[0], "rsh") == 0) &&
                   token_count == 2) {
            if (parse_register(tokens[1], &first_reg) != 0) {
                result = report_line_error(
                    path, line_number, "expected a register: r0-r3");
                break;
            }
            encoded[0] = strcmp(tokens[0], "lsh") == 0 ? 0x06 : 0x07;
            encoded[1] = first_reg;
            encoded_size = 2;
        } else {
            result = report_line_error(path, line_number,
                                       "unknown instruction or wrong operands");
            break;
        }

        if (used > ram_size || encoded_size > ram_size - used) {
            result = report_line_error(path, line_number,
                                       "program does not fit in memory");
            break;
        }
        memcpy(ram + used, encoded, encoded_size);
        used += encoded_size;
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading '%s'.\n", path);
        result = -1;
    }
    if (fclose(file) != 0) {
        fprintf(stderr, "Error closing '%s'.\n", path);
        result = -1;
    }
    if (result == 0 && used == 0) {
        fprintf(stderr, "'%s' contains no instructions.\n", path);
        result = -1;
    }
    if (result == 0) {
        *program_size = used;
    }
    return result;
}
