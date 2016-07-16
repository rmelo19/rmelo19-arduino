BINDIR=bin
NAME=reflex
OUT=$(BINDIR)/$(NAME)
FLOAT_FLAGS=-mfloat-abi=hard -mfpu=fpv4-sp-d16
OPENOCD=../tools/openocd/bin/openocd
CFLAGS=$(DEFS) -O2 -g $(FLOAT_FLAGS) -mthumb -mcpu=cortex-m4 -ffunction-sections -fdata-sections -MD -std=gnu99 -Wall -I$(PERIPHLIB_PATH)/inc -Istm32 -DSTM32F427_437xx -DHSE_VALUE=25000000 -Werror -I.
LDFLAGS=-g $(FLOAT_FLAGS) -mthumb -mcpu=cortex-m4
TC=arm-none-eabi-
CC=$(TC)gcc

# SRCS= \
# startup.c \
# stand_alone_i2c.c \
# leds.c \
# pin.c
SRCS=startup.c leds.c console.c stubs.c enet.c systime.c dmxl.c fan.c \
     tactile.c pin.c enc.c state.c async_poll.c delay.c error.c

# systime.c\

# main.c \

# console.c \
# stubs.c \
# enet.c \

# dmxl.c \
# fan.c \
# tactile.c \
# pin.c \
# enc.c \
# state.c \
# async_poll.c \
# delay.c \
# error.c

OBJS=$(SRCS:%.c=$(BINDIR)/%.o)
OBJSCPP=$(SRCS:%.cpp=$(BINDIR)/%.o)

default: $(BINDIR) $(OUTCPP).bin $(OUT).bin

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/%.o: %.c %.cpp
	$(TC)gcc $(CFLAGS) -c $< -o $@

$(OUT): $(OBJS)
	$(TC)gcc $(OBJS) -lc -lgcc -lm  -T stm32/stm32f427.ld -Wl,--no-gc-sections $(LDFLAGS) -o $(OUT) -Wl,-Map=$(BINDIR)/$(NAME).map,--cref
	$(TC)objdump -S -d $(OUT) > $(OUT).objdump

$(OUTCPP): $(OBJSCPP)
	$(TC)gcc $(OBJSCPP) -lc -lgcc -lm  -T stm32/stm32f427.ld -Wl,--no-gc-sections $(LDFLAGS) -o $(OUTCPP) -Wl,-Map=$(BINDIR)/$(NAME).map,--cref)
	$(TC)objdump -S -d $(OUTCPP) > $(OUTCPP).objdump

$(OUT).bin: $(OUT)
	$(TC)objcopy -O binary $(OUT) $(OUT).bin
	$(TC)size $(OUT)

$(OUTCPP).bin: $(OUTCPP)
	$(TC)objcopy -O binary $(OUTCPP) $(OUTCPP).bin
	$(TC)size $(OUTCPP)

clean:
	-rm $(BINDIR)/*
	rmdir $(BINDIR)

dfu_download: $(OUT).bin
	sudo dfu-util -d 0483:df11 --dfuse-address 0x08000000 -a 0 -D bin/reflex.bin -v

program: ${OUT}.bin
	$(OPENOCD) -f stm32/olimex-arm-usb-tiny-h.cfg -f stm32/stm32.cfg -c "init; halt; flash write_image erase ${OUT}.bin 0x8000000; verify_image ${OUT}.bin 0x8000000; sleep 100; reset run; sleep 100; shutdown"

dump_flash:
	$(OPENOCD) -f stm32/olimex-arm-usb-tiny-h.cfg -f stm32/stm32.cfg -c "init; halt; dump_image ${OUT}.bin.dump 0x8000000 0x1000; reset run; shutdown"

gdb_server: $(OUT).bin
	$(OPENOCD) -f stm32/olimex-arm-usb-tiny-h.cfg -f stm32/stm32.cfg -c "init; halt"

gdb: $(OUT).bin
	$(TC)gdb $(OUT) -x stm32/gdb_init_commands

reset:
	$(OPENOCD) -f stm32/olimex-arm-usb-tiny-h.cfg -f stm32/stm32.cfg -c "init; halt; reset run; shutdown"

install_gccarm_on_ubuntu:
	sudo add-apt-repository ppa:terry.guo/gcc-arm-embedded
	sudo apt-get update
	sudo apt-get install gcc-arm-none-eabi

