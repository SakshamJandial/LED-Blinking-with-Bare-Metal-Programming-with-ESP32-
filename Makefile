# Final Makefile for ESP32 Bare-Metal Project

## --- Toolchain and Program Definitions ---
TOOLCHAIN_PREFIX := xtensa-esp32-elf-
CC := $(TOOLCHAIN_PREFIX)gcc
AS := $(TOOLCHAIN_PREFIX)as
LD := $(TOOLCHAIN_PREFIX)ld
ESPTOOL := python -m esptool

## --- Project Files ---
SRC_C_FILES := main.c
SRC_S_FILES := startup.S
OBJ_FILES := $(SRC_C_FILES:.c=.o) $(SRC_S_FILES:.S=.o)
LINKER_SCRIPT := esp32.ld
ELF_FILE := blink.elf
BIN_FILE := blink.bin

## --- Build Flags ---
CFLAGS := -c -nostdlib -ffreestanding
LDFLAGS := -nostdlib -T $(LINKER_SCRIPT)

## --- esptool Options ---
PORT := COM5
BAUD := 460800

.PHONY: all build flash erase clean

## --- Build Targets ---
all: build
build: $(BIN_FILE)

$(BIN_FILE): $(ELF_FILE)
	@echo "--> Creating bootable binary: $(BIN_FILE)"
	$(ESPTOOL) --chip esp32 elf2image --flash-mode dio --flash-freq 40m --flash-size 2MB -o $(BIN_FILE) $(ELF_FILE)

$(ELF_FILE): $(OBJ_FILES)
	@echo "--> Linking: $(ELF_FILE)"
	$(LD) $(LDFLAGS) -o $(ELF_FILE) $(OBJ_FILES)

# Rule to compile C files
%.o: %.c
	@echo "--> Compiling C: $<"
	$(CC) $(CFLAGS) $< -o $@

# Rule to assemble S files
%.o: %.S
	@echo "--> Assembling: $<"
	$(AS) $< -o $@

flash: all
	@echo "--> Flashing $(BIN_FILE) to $(PORT)"
	$(ESPTOOL) --chip esp32 --port $(PORT) --baud $(BAUD) write_flash 0x1000 $(BIN_FILE)

erase:
	@echo "############################################################"
	@echo "# HOLD 'BOOT', PRESS/RELEASE 'RESET', THEN RELEASE 'BOOT' #"
	@echo "############################################################"
	@pause
	@echo "--> Erasing flash on $(PORT)"
	$(ESPTOOL) --chip esp32 --port $(PORT) --baud $(BAUD) erase_flash

clean:
	@echo "--> Cleaning build files"
	del /Q $(OBJ_FILES) $(ELF_FILE) $(BIN_FILE) 2>nul || (exit 0)