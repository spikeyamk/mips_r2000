mipsel-elf-gcc -march=r2000 -O0 -mabi=32 sample.c -S
mipsel-elf-as -O0 -EB sample.s -o sample.elf
mipsel-elf-objcopy -O binary -j .text sample.elf sample.raw
mipsel-elf-objdump -D sample.elf --disassembler-color=on --visualize-jumps=color > sample_dis.ansi
hexdump -v -e '1/1 "%02x" "\n"' sample.raw | sed "s/^/8'h/" | sed 's/$/,/' > sample.hex