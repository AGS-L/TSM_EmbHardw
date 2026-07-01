# BEGIN: You can modify the following region
PROJECT = camera
DEBUG ?= 0
CFLAGS ?=
LDFLAGS ?=
ASFLAGS ?=
# END.

TOOLCHAIN := or1k-elf
CC := $(TOOLCHAIN)-gcc
LD := $(TOOLCHAIN)-ld
AS := $(TOOLCHAIN)-as
ELF2MEM := convert_or32

CSRCS := $(wildcard src/*.c)

SUPPORT_CSRCS := $(wildcard support/src/*.c)
SUPPORT_SSRCS := $(wildcard support/src/*.s)

CFLAGS += -DPRINTF_INCLUDE_CONFIG_H -Isupport/include
LDFLAGS += -nostartfiles

ifeq ($(DEBUG), 1)
    BUILD = build-debug
    CFLAGS += -Og -g
else
    BUILD = build-release
    CFLAGS += -DNDEBUG
endif

_SUPPORT_CSRCS_ := $(notdir $(SUPPORT_CSRCS))
_SUPPORT_SSRCS_ := $(notdir $(SUPPORT_SSRCS))

OBJS := $(CSRCS:%.c=$(BUILD)/%.c.o) $(_SUPPORT_CSRCS_:%.c=$(BUILD)/support/%.c.o) $(_SUPPORT_SSRCS_:%.s=$(BUILD)/support/%.s.o)

ELF := $(addsuffix .elf,$(BUILD)/$(PROJECT))
MEM := $(addsuffix .mem,$(BUILD)/$(PROJECT))

mem: $(MEM)
elf: $(ELF)

$(MEM): $(ELF)
	@mkdir -p $(@D)
	cd $(BUILD); \
	$(ELF2MEM) $(addsuffix .elf,$(PROJECT)); \
	mv $(addsuffix .elf.mem,$(PROJECT)) $(addsuffix .mem,$(PROJECT)); \
	mv $(addsuffix .elf.cmem,$(PROJECT)) ../../../bin/$(addsuffix .cmem,$(PROJECT));

$(ELF): $(OBJS)
	@mkdir -p $(@D)
	$(CC) -v $(LDFLAGS) $^ -o $@

# User source code
$(BUILD)/src/%.c.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Support
$(BUILD)/support/%.c.o: support/src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/support/%.s.o: support/src/%.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

.PHONY: clean

clean:
	-rm -rf $(BUILD)/*
