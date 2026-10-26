#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <cpu.h>
#include <reader_assembler.h>

int main(int argc, char *argv[]) {
    printf("\nWelcome to XT1s CPU!\n");
    printf("\n=== == = == ===\n\n");

    XT1s cpu = {0};
    cpu.running = true;

    size_t program_size = 0;
    if (argc == 2) {
        if (load_program(argv[1], cpu.RAM, MEM_SIZE, &program_size) != 0) {
            return 1;
        }
    } else if (argc == 1) {
        cpu.RAM[0] = 0x02; cpu.RAM[1] = 0x00; cpu.RAM[2] = 0x0a;
        cpu.RAM[3] = 0x02; cpu.RAM[4] = 0x01; cpu.RAM[5] = 0x04;
        cpu.RAM[6] = 0x04; cpu.RAM[7] = 0x00; cpu.RAM[8] = 0x01;
        cpu.RAM[9] = 0x06; cpu.RAM[10] = 0x00;
        cpu.RAM[11] = 0x01;
        program_size = 12;
    } else {
        fprintf(stderr, "Usage: %s [program.xts]\n", argv[0]);
        return 1;
    }

    while (cpu.running) {
        if (cpu.PC >= MEM_SIZE) {
            fprintf(stderr, "Program counter exceeded memory.\n");
            return 1;
        }
        if (cpu.PC >= program_size) {
            fprintf(stderr, "Program ended without hlt.\n");
            return 1;
        }
        int result = step(&cpu);
        if (result != 0) {
            fprintf(stderr, "Execution failed at address %u.\n",
                    (unsigned)(cpu.PC - 1));
            return 1;
        }
    }

    printf("\n=== == = == ===\n");
    printf("r0=%u | r1=%u | r2=%u | r3=%u | flg=%u\n",
           (unsigned)cpu.R[0], (unsigned)cpu.R[1],
           (unsigned)cpu.R[2], (unsigned)cpu.R[3],
           (unsigned)cpu.flag);

    return 0;
}