# Compiling
`gcc main.c -o main`

**!!! You must install gcc**

# Using
```c
cpu.RAM[0] = 0x00; // nop
cpu.RAM[1] = 0x04; cpu.RAM[2] = 0x00; cpu.RAM[3] = 0x01; // add r0 r1
cpu.RAM[4] = 0x06; cpu.RAM[5] = 0x02; // lsh r3
```

# Instructions

`0x00` - `nop` - No Operation

`0x01` - `hlt` - Stop

`0x02` - `ldi reg val` - Load Val 

`0x03` - `cpy reg1 reg2` - Copy reg2 to reg1

`0x04` - `add reg1 reg2` - Add reg2 to reg1 (store in reg1)

`0x05` - `sub reg1 reg2` - Subtract reg2 from reg1 (store in reg1)

`0x06` - `lsh reg` - Left shift reg

`0x07` - `rsh reg` - Right shift reg