#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <cpu.h>

int main() {
    printf("\nWelcome to XT1s CPU!\n");
    printf("\n=== == = == ===\n\n");

    XT1s cpu = {0};
    cpu.running = true;
    
    cpu.RAM[0] = 0x02; cpu.RAM[1] = 0x00; cpu.RAM[2] = 0x0a;    // ldi r0 10
    cpu.RAM[3] = 0x02; cpu.RAM[4] = 0x01; cpu.RAM[5] = 0x04;    // ldi r1 4
    cpu.RAM[6] = 0x04; cpu.RAM[7] = 0x00; cpu.RAM[8] = 0x01;    // add r0 r1
    cpu.RAM[9] = 0x06; cpu.RAM[10] = 0x00;                      // lsh r0
    cpu.RAM[11] = 0x01;                                         // hlt

    while (cpu.running) {
        if (cpu.PC >= MEM_SIZE) {
            printf("Program Counter's value is to big");
            break;
        }
        int result = step(&cpu);
        if (result != 0) {
            printf("Something went wrong :(");
            break;
        }
    }

    printf("\n=== == = == ===\n");
    printf("r0=%d | r1=%d | r2=%d | r3=%d | flg=%d", cpu.R[0], cpu.R[1], cpu.R[2], cpu.R[3], cpu.flag);

    return 0;
}