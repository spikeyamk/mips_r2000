CC      = mipsel-elf-gcc
OBJCOPY = mipsel-elf-objcopy
OBJDUMP = mipsel-elf-objdump
CFLAGS  = -EB -march=r2000 -nostdlib -B/usr/mipsel-elf/bin -Wl,--verbose -O0 -G0 -T r2000.ld
OBJ     = start.o bubble_sort_demo.o

all: bubble_sort_demo.elf bubble_sort_demo_dis.ansi bubble_sort_demo_text.raw bubble_sort_demo_data.raw bubble_sort_demo_text.hex bubble_sort_demo_data.hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@
bubble_sort_demo.elf: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $@
bubble_sort_demo_dis.ansi: bubble_sort_demo.elf
	$(OBJDUMP) -D $< --disassembler-color=on --visualize-jumps=color > $@
bubble_sort_demo_text.raw: bubble_sort_demo.elf
	$(OBJCOPY) -O binary --only-section=.text $< $@
bubble_sort_demo_data.raw: bubble_sort_demo.elf
	$(OBJCOPY) -O binary --only-section=.data $< $@
bubble_sort_demo_text.hex: bubble_sort_demo_text.raw
	hexdump -v -e '1/1 "%02x" "\n"' bubble_sort_demo_text.raw | sed "s/^/0x/" | sed 's/$$/,/' > bubble_sort_demo_text.hex
bubble_sort_demo_data.hex: bubble_sort_demo_data.raw
	hexdump -v -e '1/1 "%02x" "\n"' bubble_sort_demo_data.raw | sed "s/^/0x/" | sed 's/$$/,/' > bubble_sort_demo_data.hex

clean:
	rm -f $(OBJ) bubble_sort_demo.elf bubble_sort_demo_dis.ansi bubble_sort_demo_text.raw bubble_sort_demo_data.raw bubble_sort_demo_text.hex bubble_sort_demo_data.hex
