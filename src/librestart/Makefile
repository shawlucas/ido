AS=mips-linux-gnu-gcc -x assembler-with-cpp

TARGET=librestart.a

AS_FLAGS := -mabi=32 -mips3
S_FILES := xstat.s
O_FILES := xstat.o

default: all

all: $(TARGET)

$(TARGET): $(O_FILES)
	mips-linux-gnu-ar rcs $(TARGET) $(O_FILES)

$(O_FILES): $(S_FILES)
	$(AS) $(C_FLAGS) -c -I$(IRIX_ROOT)/usr/include -Wall -Werror -fPIE xstat.s -o xstat.o

%.o: %.s
	$(AS) $(C_FLAGS) -c -I$(IRIX_ROOT)/usr/include -Wall -Werror -fPIE xstat.s -o xstat.o

.PHONY: clean
clean:
	rm -rvf $(O_FILES)
	rm -rvf $(TARGET)
