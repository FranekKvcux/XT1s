#define MEM_SIZE 1024

typedef struct XT1s
{
    uint8_t RAM[MEM_SIZE];
    uint16_t PC;
    uint8_t R[4];
    uint8_t flag;
    bool running;
} XT1s;

int step(XT1s *cpu) {
    uint8_t opcode = cpu->RAM[cpu->PC++];

    switch (opcode)
    {
    case 0x00: {     // nop
        printf("nop\n");
        break;
    }
    case 0x01: {     // hlt
        cpu->running = false;
        printf("hlt\n");
        break;
    }
    case 0x02: {        // ldi reg val
        uint8_t reg = cpu->RAM[cpu->PC++];
        if (reg > 3) {printf("There is 4 regs!\n"); return -1;}
        uint8_t val = cpu->RAM[cpu->PC++];
        cpu->R[reg] = val;
        printf("ldi r%d %d\n", reg, val);
        break;
    }
    case 0x03: {        // cpy reg reg
        uint8_t fst_reg = cpu->RAM[cpu->PC++];
        if (fst_reg > 3) {printf("There is 4 regs\n"); return -1;}
        uint8_t scn_reg = cpu->RAM[cpu->PC++];
        if (scn_reg > 3) {printf("There is 4 regs\n"); return -1;}
        cpu->R[fst_reg] = cpu->R[scn_reg];
        printf("cpy r%d r%d\n", fst_reg, scn_reg);
        break;
    }
    case 0x04: {        // add reg reg
        uint8_t fst_reg = cpu->RAM[cpu->PC++];
        if (fst_reg > 3) {printf("There is 4 regs\n"); return -1;}
        uint8_t scn_reg = cpu->RAM[cpu->PC++];
        if (scn_reg > 3) {printf("There is 4 regs\n"); return -1;}
        cpu->R[fst_reg] += cpu->R[scn_reg];
        printf("add r%d r%d\n", fst_reg, scn_reg);
        break;
    }
    case 0x05: {        // sub reg reg
        uint8_t fst_reg = cpu->RAM[cpu->PC++];
        if (fst_reg > 3) {printf("There is 4 regs\n"); return -1;}
        uint8_t scn_reg = cpu->RAM[cpu->PC++];
        if (scn_reg > 3) {printf("There is 4 regs\n"); return -1;}
        cpu->R[fst_reg] -= cpu->R[scn_reg];
        printf("sub r%d r%d", fst_reg, scn_reg);
        break;
    }
    case 0x06: {        // lsh reg
        uint8_t reg = cpu->RAM[cpu->PC++];
        if (reg > 3) {printf("There is 4 regs\n"); return -1;}
        cpu->R[reg] <<= 1;
        printf("lsh r%d\n", reg);
        break;
    }
    case 0x07: {        //rsh reg
        uint8_t reg = cpu->RAM[cpu->PC++];
        if (reg > 3) {printf("There is 4 regs\n"); return -1;}
        cpu->R[reg] >>= 1;
        printf("rsh r%d\n", reg);
        break;
    }
    default:
        return -1;
    }
    return 0;
}