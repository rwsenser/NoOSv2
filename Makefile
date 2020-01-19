CC=gcc
TARGET=noosv2
C_FILES=./NoOSrt/src/memcmp.c \
	./NoOSrt/src/memcpy.c \
	./NoOSrt/src/memset.c \
	./NoOSrt/src/strcat.c \
	./NoOSrt/src/strchr.c \
	./NoOSrt/src/strcmp.c \
	./NoOSrt/src/strcpy.c \
	./NoOSrt/src/strncpy.c \
	./NoOSrt/src/strlen.c \
	./NoOSrt/src/strncmp.c \
	./NoOSrt/src/strstr.c \
	./NoOSrt/src/strutil.c \
	./NoOSrt/src/atoi.c \
	./NoOSrt/src/ctos.c \
	./NoOSrt/src/tty.c \
	./NoOSrt/src/io.c \
	./NoOSrt/src/NoOSrt.c \
	./NoOSrt/src/malloc.c \
	./NoOS/src/NoOS.c \
	./NoOS/src/NoOSfs.c \
	./NoOS/src/eefs_swap.c \
	./NoOS/src/eefs_fileapi.c \
	./NoOS/src/eefs_filesys.c \
	./kernel.c
OBJS=$(C_FILES:.c=.o)

all compile: $(TARGET)
all: finale
.PHONY: all compile clean finale

%.o:
	$(CC) -c -Os $(@:.o=.c) -o $@\
	       	-I./NoOSrt/inc -I./NoOS/inc -ffreestanding -fno-exceptions -m32 -D_ix86_ -DNOOSRT
#	$(CC) -c $(@:.o=.c) -o $@ -ffreestanding -fno-exceptions -m32 -D_ix86_

$(TARGET): $(OBJS)
	$(shell nasm -f elf start.asm -o start.o)
	$(CC) -m32 -nostdlib -nodefaultlibs -lgcc start.o $? -T linker.ld -o $(TARGET)

finale:
	$(shell cd ~/myOS/myOSbuild/)
	$(shell cp $(TARGET) ./iso/boot/$(TARGET))
	$(shell grub-mkrescue iso --output=$(TARGET).iso)

clean:
	rm -f *.o $(TARGET) $(TARGET).iso
	find . -name \*.o | xargs --no-run-if-empty rm

# skip: ./echo/version-etc.c \
#	./echo/fixRS.c \
#	./echo/progname.c \
#	./echo/version.c \
#	./echo/echo.c \
#	./eefsOS/src/eefs_swap.c \
#	./eefsOS/src/eefs_fileapi.c \
#
