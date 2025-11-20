CC      = mipsel-elf-gcc
OBJCOPY = mipsel-elf-objcopy
OBJDUMP = mipsel-elf-objdump
CFLAGS  = -EB -march=r2000 -nostdlib -B/usr/mipsel-elf/bin -Wl,--verbose -O0 -G0 -T r2000.ld
OBJ     = start.o main.o

all: main.elf main_dis.ansi main_text.raw main_data.raw main_text.hex main_data.hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@
main.elf: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $@
main_dis.ansi: main.elf
	$(OBJDUMP) -D $< --disassembler-color=on --visualize-jumps=color > $@
main_text.raw: main.elf
	$(OBJCOPY) -O binary --only-section=.text $< $@
main_data.raw: main.elf
	$(OBJCOPY) -O binary --only-section=.data $< $@
main_text.hex: main_text.raw
	hexdump -v -e '1/1 "%02x" "\n"' main_text.raw | sed "s/^/0x/" | sed 's/$$/,/' > main_text.hex
main_data.hex: main_data.raw
	hexdump -v -e '1/1 "%02x" "\n"' main_data.raw | sed "s/^/0x/" | sed 's/$$/,/' > main_data.hex

clean:
	rm -f $(OBJ) main.elf main_dis.ansi main_text.raw main_data.raw main_text.hex main_data.hex