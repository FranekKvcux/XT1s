# Compiling
```powershell
gcc -std=c11 -Wall -Wextra -I. main.c reader_assembler.c -o xt1s
```

**!!! You must install gcc**

# Using
Pass a text program file as the first argument:

```text
xt1s code.xts
```

On Windows, run it from the current directory with:

```powershell
.\xt1s.exe code.xts
```

If no file is passed, the built-in example is run. Each instruction goes on
its own line. Registers are `r0` through `r3`, immediate values are decimal
or hexadecimal (`0x` prefix), and `#` or `;` starts a comment. A program must
end with `hlt`.

Example `code.xts`:

```text
ldi r0 10
ldi r1 4
add r0 r1
lsh r0
hlt
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
