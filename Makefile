all::
CROSS_COMPILE=powerpc-wrs-vxworks-
OBJCOPY=$(CROSS_COMPILE)objcopy

BIN_NAME    = FRC_UserProgram
INIT_FUNC   = $(BIN_NAME)_StartupLibraryInit
ALL_CFLAGS += -DINIT_FUNC=$(INIT_FUNC)
ALL_LDFLAGS += -Wl,-Ur -Tvxworks_elf.ld

obj-$(BIN_NAME).elf = main.o vxworks-util.o vxworks-no-munch.o test_cxx.o
TARGETS = $(BIN_NAME).elf

$(BIN_NAME).out: $(BIN_NAME).elf
	$(OBJCOPY) -G $(INIT_FUNC) -G _dtors -G _ctors \
		-R .comment \
		-g --strip-unneeded $< $@
TRASH += $(BIN_NAME).out

all:: $(BIN_NAME).out
include base.mk

export WIND_BASE:=/var/empty
