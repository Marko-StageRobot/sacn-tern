# 借りた火で
# 自分の鍋を
# 煮るレシピ
all : sacn_tern.elf

PREFIX?=riscv64-unknown-elf
SDK:=../vendor/ch32v307/EVT/EXAM
RAW:=$(SDK)/ETH/MAC_RAW

INCLUDES:=-I. -I$(SDK)/SRC/Core -I$(SDK)/SRC/Debug -I$(SDK)/SRC/Peripheral/inc -I$(RAW)/ETH_Driver -I$(RAW)/User
OURS:=$(wildcard *.c)
THEIRS:=$(wildcard $(SDK)/SRC/Peripheral/src/*.c)
THEIRS+=$(SDK)/SRC/Core/core_riscv.c $(SDK)/SRC/Debug/debug.c
THEIRS+=$(RAW)/ETH_Driver/eth_driver_10M.c $(RAW)/User/system_ch32v30x.c
THEIRS+=$(SDK)/SRC/Startup/startup_ch32v30x_D8C.S

CFLAGS:=-march=rv32imac_zicsr_zifencei -mabi=ilp32 -Os -ffunction-sections -fdata-sections -Wall $(INCLUDES)
LDFLAGS:=-T $(SDK)/SRC/Ld/Link.ld -nostartfiles -Wl,--gc-sections -Wl,--print-memory-usage

sacn_tern.elf : $(OURS) $(THEIRS)
	$(PREFIX)-gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

check :
	$(PREFIX)-gcc -fsyntax-only $(CFLAGS) $(OURS)

clean :
	rm -f sacn_tern.elf
